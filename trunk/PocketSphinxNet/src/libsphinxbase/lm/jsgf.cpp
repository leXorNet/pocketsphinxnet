/* -*- c-basic-offset: 4; indent-tabs-mode: nil -*- */
/* ====================================================================
 * Copyright (c) 2007 Carnegie Mellon University.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * This work was supported in part by funding from the Defense Advanced 
 * Research Projects Agency and the National Science Foundation of the 
 * United States of America, and the CMU Sphinx Speech Consortium.
 *
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY ``AS IS'' AND 
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
 * NOR ITS EMPLOYEES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ====================================================================
 *
 */


#ifndef POCKETSPHINX_NET
#include <string.h>
#include <assert.h>
#else
#include "libct.h"
#endif

#include "sphinxbase/ckd_alloc.h"
#include "sphinxbase/strfuncs.h"
#include "sphinxbase/hash_table.h"
#include "sphinxbase/err.h"

#include "jsgf_internal.h"
#include "jsgf_parser.h"
#include "jsgf_scanner.h"
int
yyparse (void* yyscanner, jsgf_t *jsgf);

/**
 * \file jsgf.c
 *
 * This file implements the data structures for parsing JSGF grammars
 * into Sphinx finite-state grammars.
 **/

jsgf_atom_t *
jsgf_atom_new(char *name, float weight)
{
    jsgf_atom_t *atom;

    atom = (jsgf_atom_t*)ckd_calloc(1, sizeof(*atom));
    atom->name = ckd_salloc(name);
    atom->weight = weight;
    return atom;
}

int
jsgf_atom_free(jsgf_atom_t *atom)
{
    if (atom == 0)
        return 0;
    ckd_free(atom->name);
    ckd_free(atom);
    return 0;
}

jsgf_t *
jsgf_grammar_new(jsgf_t *parent)
{
    jsgf_t *grammar;

    grammar = (jsgf_t*)ckd_calloc(1, sizeof(*grammar));
    /* If this is an imported/subgrammar, then we will share a global
     * namespace with the parent grammar. */
    if (parent) {
        grammar->rules = parent->rules;
        grammar->imports = parent->imports;
        grammar->searchpath = parent->searchpath;
        grammar->parent = parent;
    }
    else {
        char *jsgf_path;

        grammar->rules = hash_table_new(64, 0);
        grammar->imports = hash_table_new(16, 0);

        /* Silvio Moioli: no getenv() in Windows CE */
        #if !defined(_WIN32_WCE)
        if ((jsgf_path = getenv("JSGF_PATH")) != 0) {
            char *word, *c;

            /* FIXME: This should be a function in libsphinxbase. */
            /* FIXME: Also nextword() is totally useless... */
            word = jsgf_path;
			//NOTICE: don't alloc again
			// = ckd_salloc(jsgf_path);
            while ((c = (char*)strchr(word, ':'))) {
                *c = '\0';
                grammar->searchpath = glist_add_ptr(grammar->searchpath, word);
                word = c + 1;
            }
            grammar->searchpath = glist_add_ptr(grammar->searchpath, word);
            grammar->searchpath = glist_reverse(grammar->searchpath);
        }
        else {
            /* Default to current directory. */
            grammar->searchpath = glist_add_ptr(grammar->searchpath, ckd_salloc("."));
        }
        #endif 
    }

    return grammar;
}

void
jsgf_grammar_free(jsgf_t *jsgf)
{
    /* FIXME: Probably should just use refcounting instead. */
    if (jsgf->parent == 0) {
        hash_iter_t *itor;
        gnode_t *gn;

        for (itor = hash_table_iter(jsgf->rules); itor;
             itor = hash_table_iter_next(itor)) {
            ckd_free((char *)itor->ent->key);
            jsgf_rule_free((jsgf_rule_t *)itor->ent->val);
        }
        hash_table_free(jsgf->rules);
        for (itor = hash_table_iter(jsgf->imports); itor;
             itor = hash_table_iter_next(itor)) {
            ckd_free((char *)itor->ent->key);
            jsgf_grammar_free((jsgf_t *)itor->ent->val);
        }
        hash_table_free(jsgf->imports);
        for (gn = jsgf->searchpath; gn; gn = gnode_next(gn))
            ckd_free(gnode_ptr(gn));
        glist_free(jsgf->searchpath);
        for (gn = jsgf->links; gn; gn = gnode_next(gn))
            ckd_free(gnode_ptr(gn));
        glist_free(jsgf->links);
    }
    ckd_free(jsgf->name);
    ckd_free(jsgf->version);
    ckd_free(jsgf->charset);
    ckd_free(jsgf->locale);
    ckd_free(jsgf);
}

static void
jsgf_rhs_free(jsgf_rhs_t *rhs)
{
    gnode_t *gn;

    if (rhs == 0)
        return;

    jsgf_rhs_free(rhs->alt);
    for (gn = rhs->atoms; gn; gn = gnode_next(gn))
        jsgf_atom_free((jsgf_atom_t*)gnode_ptr(gn));
    glist_free(rhs->atoms);
    ckd_free(rhs);
}

jsgf_atom_t *
jsgf_kleene_new(jsgf_t *jsgf, jsgf_atom_t *atom, int plus)
{
    jsgf_rule_t *rule;
    jsgf_atom_t *rule_atom;
    jsgf_rhs_t *rhs;

    /* Generate an "internal" rule of the form (<0> | <name> <g0006>) */
    /* Or if plus is true, (<name> | <name> <g0006>) */
    rhs = (jsgf_rhs_t*)ckd_calloc(1, sizeof(*rhs));
    if (plus)
        rhs->atoms = glist_add_ptr(0, jsgf_atom_new(atom->name, 1.0));
    else
        rhs->atoms = glist_add_ptr(0, jsgf_atom_new("<0>", 1.0));
    rule = jsgf_define_rule(jsgf, 0, rhs, 0);
    rule_atom = jsgf_atom_new(rule->name, 1.0);
    rhs = (jsgf_rhs_t*)ckd_calloc(1, sizeof(*rhs));
    rhs->atoms = glist_add_ptr(0, rule_atom);
    rhs->atoms = glist_add_ptr(rhs->atoms, atom);
    rule->rhs->alt = rhs;

    return jsgf_atom_new(rule->name, 1.0);
}

jsgf_rule_t *
jsgf_optional_new(jsgf_t *jsgf, jsgf_rhs_t *exp)
{
    jsgf_rhs_t *rhs = (jsgf_rhs_t*)ckd_calloc(1, sizeof(*rhs));
    jsgf_atom_t *atom = jsgf_atom_new("<0>", 1.0);
    rhs->alt = exp;
    rhs->atoms = glist_add_ptr(0, atom);
    return jsgf_define_rule(jsgf, 0, rhs, 0);
}

void
jsgf_add_link(jsgf_t *grammar, jsgf_atom_t *atom, int from, int to)
{
    jsgf_link_t *link;

    link = (jsgf_link_t*)ckd_calloc(1, sizeof(*link));
    link->from = from;
    link->to = to;
    link->atom = atom;
    grammar->links = glist_add_ptr(grammar->links, link);
}

static char *
extract_grammar_name(char *rule_name)
{
    char* dot_pos;
    char* grammar_name = ckd_salloc(rule_name+1);
    if ((dot_pos = (char*)strrchr(grammar_name + 1, '.')) == 0) {
        ckd_free(grammar_name);
        return 0;
    }
    *dot_pos='\0';
    return grammar_name;
}

char const *
jsgf_grammar_name(jsgf_t *jsgf)
{
    return jsgf->name;
}

static char *
jsgf_fullname(jsgf_t *jsgf, const char *name)
{
    char *fullname;

    /* Check if it is already qualified */
    if (strchr(name + 1, '.'))
        return ckd_salloc(name);

    /* Skip leading < in name */
    fullname = (char*)ckd_malloc(strlen(jsgf->name) + strlen(name) + 4);
#ifndef POCKETSPHINX_NET
    sprintf(fullname, "<%s.%s", jsgf->name, name + 1);
#else
	strcpy(fullname,"<");
	strcat(fullname,jsgf->name);
	strcat(fullname,".");
	strcat(fullname,name+1);
#endif
    return fullname;
}

static char *
jsgf_fullname_from_rule(jsgf_rule_t *rule, const char *name)
{
    char *fullname, *grammar_name;

    /* Check if it is already qualified */
    if (strchr(name + 1, '.'))
        return ckd_salloc(name);

    /* Skip leading < in name */
    if ((grammar_name = extract_grammar_name(rule->name)) == 0)
        return ckd_salloc(name);
    fullname =  (char*)ckd_malloc(strlen(grammar_name) + strlen(name) + 4);
#ifndef POCKETSPHINX_NET
    sprintf(fullname, "<%s.%s", grammar_name, name + 1);
#else
	strcpy(fullname,"<");
	strcat(fullname,grammar_name);
	strcat(fullname,".");
	strcat(fullname,name+1);
#endif
    ckd_free(grammar_name);

    return fullname;
}

/* Extract as rulename everything after the secondlast dot, if existent. 
 * Because everything before the secondlast dot is the path-specification. */
static char *
importname2rulename(char *importname)
{
    char *rulename = ckd_salloc(importname);
    char *last_dotpos;
    char *secondlast_dotpos;

    if ((last_dotpos = (char*)strrchr(rulename+1, '.')) != 0) {
        *last_dotpos='\0';
        if ((secondlast_dotpos = (char*)strrchr(rulename+1, '.')) != 0) {
            *last_dotpos='.';
            *secondlast_dotpos='<';
            secondlast_dotpos = ckd_salloc(secondlast_dotpos);
            ckd_free(rulename);
            return secondlast_dotpos;
        }
        else {
            *last_dotpos='.';
            return rulename;
        }
    }
    else {
        return rulename;
    }
}

static int expand_rule(jsgf_t *grammar, jsgf_rule_t *rule);
static int
expand_rhs(jsgf_t *grammar, jsgf_rule_t *rule, jsgf_rhs_t *rhs)
{
    gnode_t *gn;
    int lastnode;

    /* Last node expanded in this sequence. */
    lastnode = rule->entry;

    /* Iterate over atoms in rhs and generate links/nodes */
    for (gn = rhs->atoms; gn; gn = gnode_next(gn)) {
        jsgf_atom_t *atom = (jsgf_atom_t*)gnode_ptr(gn);
        if (jsgf_atom_is_rule(atom)) {
            jsgf_rule_t *subrule;
            char *fullname;
            gnode_t *subnode;
            void *val;

            /* Special case for <0> and <VOID> pseudo-rules */
	    if (0 == strcmp(atom->name, "<0>")) {
                /* Emit a 0 transition */
                jsgf_add_link(grammar, atom,
                              lastnode, grammar->nstate);
                lastnode = grammar->nstate;
                ++grammar->nstate;
                continue;
	    }
            else if (0 == strcmp(atom->name, "<VOID>")) {
                /* Make this entire RHS unspeakable */
                return -1;
            }

            fullname = jsgf_fullname_from_rule(rule, atom->name);
            if (hash_table_lookup(grammar->rules, fullname, &val) == -1) {
                //E_ERROR("Undefined rule in RHS: %s\n", fullname);
                ckd_free(fullname);
                return -1;
            }
            ckd_free(fullname);
            subrule =(jsgf_rule_t*) val;
            /* Look for this in the stack of expanded rules */
            for (subnode = grammar->rulestack; subnode; subnode = gnode_next(subnode))
                if (gnode_ptr(subnode) == (void *)subrule)
                    break;
            if (subnode != 0) {
                /* Allow right-recursion only. */
                if (gnode_next(gn) != 0) {
                    E_ERROR("Only right-recursion is permitted (in %s.%s)\n",
                            grammar->name, rule->name);
                    return -1;
                }
                /* Add a link back to the beginning of this rule instance */
                E_INFO("Right recursion %s %d => %d\n", atom->name, lastnode, subrule->entry);
                jsgf_add_link(grammar, atom, lastnode, subrule->entry);
            }
            else {
                /* Expand the subrule */
                if (expand_rule(grammar, subrule) == -1)
                    return -1;
                /* Add a link into the subrule. */
                jsgf_add_link(grammar, atom,
                         lastnode, subrule->entry);
                lastnode = subrule->exit;
            }
        }
        else {
            /* Add a link for this token and create a new exit node. */
            jsgf_add_link(grammar, atom,
                     lastnode, grammar->nstate);
            lastnode = grammar->nstate;
            ++grammar->nstate;
        }
    }

    return lastnode;
}

static int
expand_rule(jsgf_t *grammar, jsgf_rule_t *rule)
{
    jsgf_rhs_t *rhs;
    float norm;

    /* Push this rule onto the stack */
    grammar->rulestack = glist_add_ptr(grammar->rulestack, rule);

    /* Normalize weights for all alternatives exiting rule->entry */
    norm = 0;
    for (rhs = rule->rhs; rhs; rhs = rhs->alt) {
        if (rhs->atoms) {
            jsgf_atom_t *atom = (jsgf_atom_t*)gnode_ptr(rhs->atoms);
            norm += atom->weight;
        }
    }

    rule->entry = grammar->nstate++;
    rule->exit = grammar->nstate++;
    if (norm == 0) norm = 1;
    for (rhs = rule->rhs; rhs; rhs = rhs->alt) {
        int lastnode;

        if (rhs->atoms) {
            jsgf_atom_t *atom = (jsgf_atom_t*)gnode_ptr(rhs->atoms);
	    atom->weight /= norm;
        }
        lastnode = expand_rhs(grammar, rule, rhs);
        if (lastnode == -1) {
            return -1;
        }
        else {
            jsgf_add_link(grammar, 0, lastnode, rule->exit);
        }
    }

    /* Pop this rule from the rule stack */
    grammar->rulestack = gnode_free(grammar->rulestack, 0);
    return rule->exit;
}

jsgf_rule_iter_t *
jsgf_rule_iter(jsgf_t *grammar)
{
    return hash_table_iter(grammar->rules);
}

jsgf_rule_t *
jsgf_get_rule(jsgf_t *grammar, char const *name)
{
    void *val;

    if (hash_table_lookup(grammar->rules, name, &val) < 0)
        return 0;
    return (jsgf_rule_t *)val;
}

char const *
jsgf_rule_name(jsgf_rule_t *rule)
{
    return rule->name;
}

int
jsgf_rule_public(jsgf_rule_t *rule)
{
    return rule->_public;
}

static fsg_model_t *
jsgf_build_fsg_internal(jsgf_t *grammar, jsgf_rule_t *rule,
                        logmath_t *lmath, float32 lw, int do_closure)
{
    fsg_model_t *fsg;
    glist_t nulls;
    gnode_t *gn;

    /* Clear previous links */
    for (gn = grammar->links; gn; gn = gnode_next(gn)) {
        ckd_free(gnode_ptr(gn));
    }
    glist_free(grammar->links);
    grammar->links = 0;
    rule->entry = rule->exit = 0;
    grammar->nstate = 0;
    expand_rule(grammar, rule);

    fsg = fsg_model_init(rule->name, lmath, lw, grammar->nstate);
    fsg->start_state = rule->entry;
    fsg->final_state = rule->exit;
    grammar->links = glist_reverse(grammar->links);
    for (gn = grammar->links; gn; gn = gnode_next(gn)) {
        jsgf_link_t *link = (jsgf_link_t*)gnode_ptr(gn);

        if (link->atom) {
            if (jsgf_atom_is_rule(link->atom)) {
                fsg_model_null_trans_add(fsg, link->from, link->to,
                                        logmath_log(lmath, link->atom->weight));
            }
            else {
                int wid = fsg_model_word_add(fsg, link->atom->name);
                fsg_model_trans_add(fsg, link->from, link->to,
                                   logmath_log(lmath, link->atom->weight), wid);
            }
        }
        else {
            fsg_model_null_trans_add(fsg, link->from, link->to, 0);
        }            
    }
    if (do_closure) {
        nulls = fsg_model_null_trans_closure(fsg, 0);
        glist_free(nulls);
    }

    return fsg;
}

fsg_model_t *
jsgf_build_fsg(jsgf_t *grammar, jsgf_rule_t *rule,
               logmath_t *lmath, float32 lw)
{
    return jsgf_build_fsg_internal(grammar, rule, lmath, lw, TRUE);
}

fsg_model_t *
jsgf_build_fsg_raw(jsgf_t *grammar, jsgf_rule_t *rule,
                   logmath_t *lmath, float32 lw)
{
    return jsgf_build_fsg_internal(grammar, rule, lmath, lw, FALSE);
}

fsg_model_t *
jsgf_read_file(const char *file, logmath_t * lmath, float32 lw)
{
    fsg_model_t *fsg;
    jsgf_rule_t *rule;
    jsgf_t *jsgf;
    jsgf_rule_iter_t *itor;

    if ((jsgf = jsgf_parse_file(file, 0)) == 0) {
        //E_ERROR("Error parsing file: %s\n", file);
        return 0;
    }

    rule = 0;
    for (itor = jsgf_rule_iter(jsgf); itor;
         itor = jsgf_rule_iter_next(itor)) {
        rule = jsgf_rule_iter_rule(itor);
        if (jsgf_rule_public(rule)) {
    	    jsgf_rule_iter_free(itor);
            break;
        }
    }
    if (rule == 0) {
        //E_ERROR("No public rules found in %s\n", file);
        return 0;
    }
    fsg = jsgf_build_fsg(jsgf, rule, lmath, lw);
    jsgf_grammar_free(jsgf);
    return fsg;
}

int
jsgf_write_fsg(jsgf_t *grammar, jsgf_rule_t *rule, FILE *outfh)
{
    fsg_model_t *fsg;
    logmath_t *lmath = logmath_init(1.0001, 0, 0);

    if ((fsg = jsgf_build_fsg_raw(grammar, rule, lmath, 1.0)) == 0)
        goto error_out;

    fsg_model_write(fsg, outfh);
    logmath_free(lmath);
    return 0;

error_out:
    logmath_free(lmath);
    return -1;
}
jsgf_rule_t *
jsgf_define_rule(jsgf_t *jsgf, char *name, jsgf_rhs_t *rhs, int _public)
{
    jsgf_rule_t *rule;
    void *val;

    if (name == 0) {
        name = (char*)ckd_malloc(strlen(jsgf->name) + 16);

#ifndef POCKETSPHINX_NET
        sprintf(name, "<%s.g%05d>", jsgf->name, hash_table_inuse(jsgf->rules));
#else
//need check
		strcpy(name,"<");
		strcat(name,jsgf->name);
		strcat(name,".g");

		char* pn = (char*)Marshal::StringToHGlobalAnsi(String::Format("{0:05}", hash_table_inuse(jsgf->rules))).ToPointer();
		if(pn!=0)
		{
			strcat(name,pn);

			Marshal::FreeHGlobal(IntPtr((void*)pn));
		}
		strcpy(name,">");

#endif
    }
    else {
        char *newname;

        newname = jsgf_fullname(jsgf, name);
        name = newname;
    }

    rule =(jsgf_rule_t*) ckd_calloc(1, sizeof(*rule));
    rule->refcnt = 1;
    rule->name = ckd_salloc(name);
    rule->rhs = rhs;
    rule->_public = _public;

    E_INFO("Defined rule: %s%s\n",
           rule->_public ? "PUBLIC " : "",
           rule->name);
    val = hash_table_enter(jsgf->rules, name, rule);
    if (val != (void *)rule) {
        E_WARN("Multiply defined symbol: %s\n", name);
    }
    return rule;
}

jsgf_rule_t *
jsgf_rule_retain(jsgf_rule_t *rule)
{
    ++rule->refcnt;
    return rule;
}

int
jsgf_rule_free(jsgf_rule_t *rule)
{
    if (rule == 0)
        return 0;
    if (--rule->refcnt > 0)
        return rule->refcnt;
    jsgf_rhs_free(rule->rhs);
    ckd_free(rule->name);
    ckd_free(rule);
    return 0;
}


/* FIXME: This should go in libsphinxutil */
static char *
path_list_search(glist_t paths, char *path)
{
    gnode_t *gn;

    for (gn = paths; gn; gn = gnode_next(gn)) {
        char *fullpath;
        FILE *tmp;

        fullpath = string_join((const char*)gnode_ptr(gn), "/", path, 0,0);
        tmp = fopen(fullpath, "r");
        if (tmp != 0) {
            fclose(tmp);
            return fullpath;
        }
        else
            ckd_free(fullpath);
    }
    return 0;
}

jsgf_rule_t *
jsgf_import_rule(jsgf_t *jsgf, char *name)
{
    char *c, *path, *newpath;
    size_t namelen, packlen;
    void *val;
    jsgf_t *imp;
    int import_all;

    /* Trim the leading and trailing <> */
    namelen = strlen(name);
    path = (char*)ckd_malloc(namelen - 2 + 6); /* room for a trailing .gram */
    strcpy(path, name + 1);
    /* Split off the first part of the name */
    c = (char*)strrchr(path, '.');
    if (c == 0) {
        //E_ERROR("Imported rule is not qualified: %s\n", name);
        ckd_free(path);
        return 0;
    }
    packlen = c - path;
    *c = '\0';

    /* Look for import foo.* */
    import_all = (strlen(name) > 2 && 0 == strcmp(name + namelen - 3, ".*>"));

    /* Construct a filename. */
    for (c = path; *c; ++c)
        if (*c == '.') *c = '/';
    strcat(path, ".gram");
    newpath = path_list_search(jsgf->searchpath, path);
    ckd_free(path);
    if (newpath == 0)
        return 0;

    path = newpath;
    E_INFO("Importing %s from %s to %s\n", name, path, jsgf->name);

    /* FIXME: Also, we need to make sure that path is fully qualified
     * here, by adding any prefixes from jsgf->name to it. */
    /* See if we have parsed it already */
    if (hash_table_lookup(jsgf->imports, path, &val) == 0) {
        E_INFO("Already imported %s\n", path);
        imp =  (jsgf_t*)val;
        ckd_free(path);
    }
    else {
        /* If not, parse it. */
        imp = jsgf_parse_file(path, jsgf);
        val = hash_table_enter(jsgf->imports, path, imp);
        if (val != (void *)imp) {
            E_WARN("Multiply imported file: %s\n", path);
        }
    }
    if (imp != 0) {
        hash_iter_t *itor;
        /* Look for public rules matching rulename. */
        for (itor = hash_table_iter(imp->rules); itor;
             itor = hash_table_iter_next(itor)) {
            hash_entry_t *he = itor->ent;
            jsgf_rule_t *rule = (jsgf_rule_t*)hash_entry_val(he);
            int rule_matches;
            char *rule_name = importname2rulename(name);

            if (import_all) {
                /* Match package name (symbol table is shared) */
                rule_matches = !strncmp(rule_name, rule->name, packlen + 1);
            }
            else {
                /* Exact match */
                rule_matches = !strcmp(rule_name, rule->name);
            }
            ckd_free(rule_name);
            if (rule->_public && rule_matches) {
                void *val;
                char *newname;

                /* Link this rule into the current namespace. */
                c = (char*)strrchr(rule->name, '.');
                assert(c != 0);
                newname = jsgf_fullname(jsgf, c);

                E_INFO("Imported %s\n", newname);
                val = hash_table_enter(jsgf->rules, newname,
                                       jsgf_rule_retain(rule));
                if (val != (void *)rule) {
                    E_WARN("Multiply defined symbol: %s\n", newname);
                }
                if (!import_all) {
                    hash_table_iter_free(itor);
                    return rule;
                }
            }
        }
    }

    return 0;
}

jsgf_t *
jsgf_parse_file(const char *filename, jsgf_t *parent)
{
    yyscan_t yyscanner;
    jsgf_t *jsgf;
    int yyrv;
    FILE *in = 0;

    yylex_init(&yyscanner);
    if (filename == 0) {
#ifndef POCKETSPHINX_NET
		yyset_in(stdin, yyscanner);
#endif
    }
    else {
        in = fopen(filename, "r");
        if (in == 0) {
            E_ERROR_SYSTEM("Failed to open %s for parsing", filename);
            return 0;
        }
        yyset_in(in, yyscanner);
    }

    jsgf = jsgf_grammar_new(parent);
    yyrv = yyparse(yyscanner, jsgf);
    if (yyrv != 0) {
        //E_ERROR("Failed to parse JSGF grammar from '%s'\n", filename ? filename : "(stdin)");
        jsgf_grammar_free(jsgf);
        yylex_destroy(yyscanner);
        return 0;
    }
    if (in)
        fclose(in);
    yylex_destroy(yyscanner);

    return jsgf;
}
