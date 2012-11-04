using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace PocketSphinxSharp
{
  public class PocketCommandLine
  {
    /**
     * @struct arg_t
     * Argument definition structure.
     */
    public struct arg_t
    {
      [MarshalAs(UnmanagedType.LPStr)]
	    string name;   /**< Name of the command line switch */
	    Int32 type;           /**< Type of the argument in question */
      [MarshalAs(UnmanagedType.LPStr)]
      string deflt;  /**< Default value (as a character string), or NULL if none */
      [MarshalAs(UnmanagedType.LPStr)]
	    string doc;    /**< Documentation/description string */
    } 

    /**
     * @name Values for arg_t::type
     */
    /* @{ */
    /**
     * Bit indicating a required argument.
     */
    public const Int32 ARG_REQUIRED= (1<<0);
    /**
     * Integer argument (optional).
     */
    public const Int32  ARG_INTEGER = (1<<1);
    /**
     * Floating point argument (optional).
     */
    public const Int32 ARG_FLOATING= (1<<2);
    /**
     * String argument (optional).
     */
    public const Int32  ARG_STRING =  (1<<3);
    /**
     * Boolean (true/false) argument (optional).
     */
    public const Int32  ARG_BOOLEAN = (1<<4);
    /**
     * Boolean (true/false) argument (optional).
     */
    public const Int32  ARG_STRING_LIST = (1<<5);

    /**
     * Required integer argument.
     */
    public const Int32  REQARG_INTEGER= (ARG_INTEGER | ARG_REQUIRED);
    /**
     * Required floating point argument.
     */
    public const Int32  REQARG_FLOATING =(ARG_FLOATING | ARG_REQUIRED);
    /**
     * Required string argument.
     */
    public const Int32  REQARG_STRING= (ARG_STRING | ARG_REQUIRED);
    /**
     * Required boolean argument.
     */
    public const Int32  REQARG_BOOLEAN =(ARG_BOOLEAN | ARG_REQUIRED);



    public const Int32  REQARG_INT32= (ARG_INTEGER | ARG_REQUIRED);
    /**
     * @deprecated Use REQARG_FLOATING instead.
     */
    public const Int32  REQARG_FLOAT32= (ARG_FLOATING | ARG_REQUIRED);
    /**
     * @deprecated Use REQARG_FLOATING instead.
     */
    public const Int32  REQARG_FLOAT64= (ARG_FLOATING | ARG_REQUIRED);
    /* @} */




///**
// * @struct cmd_ln_t
// * Opaque structure used to hold the results of command-line parsing.
// */


///**
// * Create a cmd_ln_t from NULL-terminated list of arguments.
// *
// * This function creates a cmd_ln_t from a NULL-terminated list of
// * argument strings.  For example, to create the equivalent of passing
// * "-hmm foodir -dsratio 2 -lm bar.lm" on the command-line:
// *
// *  config = cmd_ln_init(NULL, defs, TRUE, "-hmm", "foodir", "-dsratio", "2",
// *                       "-lm", "bar.lm", NULL);
// *
// * Note that for simplicity, <strong>all</strong> arguments are passed
// * as strings, regardless of the actual underlying type.
// *
// * @param inout_cmdln Previous command-line to update, or NULL to create a new one.
// * @param defn Array of argument name definitions, or NULL to allow any arguments.
// * @param strict Whether to fail on duplicate or unknown arguments.
// * @return A cmd_ln_t* containing the results of command line parsing, or NULL on failure.
// */
//SPHINXBASE_EXPORT
//cmd_ln_t *cmd_ln_init(cmd_ln_t *inout_cmdln, arg_t const *defn, int32 strict, ...);

///**
// * Retain ownership of a command-line argument set.
// *
// * @return pointer to retained command-line argument set.
// */
//SPHINXBASE_EXPORT
//cmd_ln_t *cmd_ln_retain(cmd_ln_t *cmdln);

///**
// * Release a command-line argument set and all associated strings.
// *
// * @return new reference count (0 if freed completely)
// */
//SPHINXBASE_EXPORT
//int cmd_ln_free_r(cmd_ln_t *cmdln);

///**
// * Parse a list of strings into argumetns.
// *
// * Parse the given list of arguments (name-value pairs) according to
// * the given definitions.  Argument values can be retrieved in future
// * using cmd_ln_access().  argv[0] is assumed to be the program name
// * and skipped.  Any unknown argument name causes a fatal error.  The
// * routine also prints the prevailing argument values (to stderr)
// * after parsing.
// *
// * @note It is currently assumed that the strings in argv are
// *       allocated statically, or at least that they will be valid as
// *       long as the cmd_ln_t returned from this function.
// *       Unpredictable behaviour will result if they are freed or
// *       otherwise become invalidated.
// *
// * @return A cmd_ln_t containing the results of command line parsing,
// *         or NULL on failure.
// **/
//SPHINXBASE_EXPORT
//cmd_ln_t *cmd_ln_parse_r(cmd_ln_t *inout_cmdln, /**< In/Out: Previous command-line to update,
//                                                     or NULL to create a new one. */
//                         arg_t const *defn,	/**< In: Array of argument name definitions */
//                         int32 argc,		/**< In: Number of actual arguments */
//                         char *argv[],		/**< In: Actual arguments */
//                         int32 strict           /**< In: Fail on duplicate or unknown
//                                                   arguments, or no arguments? */
//    );

///**
// * Parse an arguments file by deliminating on " \r\t\n" and putting each tokens
// * into an argv[] for cmd_ln_parse().
// *
// * @return A cmd_ln_t containing the results of command line parsing, or NULL on failure.
// */
//SPHINXBASE_EXPORT
//cmd_ln_t *cmd_ln_parse_file_r(cmd_ln_t *inout_cmdln, /**< In/Out: Previous command-line to update,
//                                                     or NULL to create a new one. */
//                              arg_t const *defn,   /**< In: Array of argument name definitions*/
//                              char const *filename,/**< In: A file that contains all
//                                                     the arguments */ 
//                              int32 strict         /**< In: Fail on duplicate or unknown
//                                                     arguments, or no arguments? */
//    );

///**
// * Access the generic type union for a command line argument.
// */
//SPHINXBASE_EXPORT
//anytype_t *cmd_ln_access_r(cmd_ln_t *cmdln, char const *name);

///**
// * Retrieve a string from a command-line object.
// *
// * The command-line object retains ownership of this string, so you
// * should not attempt to free it manually.
// *
// * @param cmdln Command-line object.
// * @param name the command-line flag to retrieve.
// * @return the string value associated with <tt>name</tt>, or NULL if
// *         <tt>name</tt> does not exist.  You must use
// *         cmd_ln_exists_r() to distinguish between cases where a
// *         value is legitimately NULL and where the corresponding flag
// *         is unknown.
// */
//SPHINXBASE_EXPORT
//char const *cmd_ln_str_r(cmd_ln_t *cmdln, char const *name);

///**
// * Retrieve an array of strings from a command-line object.
// *
// * The command-line object retains ownership of this array, so you
// * should not attempt to free it manually.
// *
// * @param cmdln Command-line object.
// * @param name the command-line flag to retrieve.
// * @return the array of strings associated with <tt>name</tt>, or NULL if
// *         <tt>name</tt> does not exist.  You must use
// *         cmd_ln_exists_r() to distinguish between cases where a
// *         value is legitimately NULL and where the corresponding flag
// *         is unknown.
// */
//SPHINXBASE_EXPORT
//char const **cmd_ln_str_list_r(cmd_ln_t *cmdln, char const *name);

///**
// * Retrieve an integer from a command-line object.
// *
// * @param cmdln Command-line object.
// * @param name the command-line flag to retrieve.
// * @return the integer value associated with <tt>name</tt>, or 0 if
// *         <tt>name</tt> does not exist.  You must use
// *         cmd_ln_exists_r() to distinguish between cases where a
// *         value is legitimately zero and where the corresponding flag
// *         is unknown.
// */
//SPHINXBASE_EXPORT
//long cmd_ln_int_r(cmd_ln_t *cmdln, char const *name);

///**
// * Retrieve a floating-point number from a command-line object.
// *
// * @param cmdln Command-line object.
// * @param name the command-line flag to retrieve.
// * @return the float value associated with <tt>name</tt>, or 0.0 if
// *         <tt>name</tt> does not exist.  You must use
// *         cmd_ln_exists_r() to distinguish between cases where a
// *         value is legitimately zero and where the corresponding flag
// *         is unknown.
// */
//SPHINXBASE_EXPORT
//double cmd_ln_float_r(cmd_ln_t *cmdln, char const *name);

///**
// * Retrieve a boolean value from a command-line object.
// */
//#define cmd_ln_boolean_r(c,n) (cmd_ln_int_r(c,n) != 0)

///**
// * Set a string in a command-line object.
// *
// * @param cmdln Command-line object.
// * @param name The command-line flag to set.
// * @param str String value to set.  The command-line object does not
// *            retain ownership of this pointer.
// */
//SPHINXBASE_EXPORT
//void cmd_ln_set_str_r(cmd_ln_t *cmdln, char const *name, char const *str);

///**
// * Set an integer in a command-line object.
// *
// * @param cmdln Command-line object.
// * @param name The command-line flag to set.
// * @param iv Integer value to set.
// */
//SPHINXBASE_EXPORT
//void cmd_ln_set_int_r(cmd_ln_t *cmdln, char const *name, long iv);

///**
// * Set a floating-point number in a command-line object.
// *
// * @param cmdln Command-line object.
// * @param name The command-line flag to set.
// * @param fv Integer value to set.
// */
//SPHINXBASE_EXPORT
//void cmd_ln_set_float_r(cmd_ln_t *cmdln, char const *name, double fv);


///**
// * Re-entrant version of cmd_ln_exists().
// *
// * @return True if the command line argument exists (i.e. it
// * was one of the arguments defined in the call to cmd_ln_parse_r().
// */
//SPHINXBASE_EXPORT
//int cmd_ln_exists_r(cmd_ln_t *cmdln, char const *name);

///**
// * Print a help message listing the valid argument names, and the associated
// * attributes as given in defn.
// */
//SPHINXBASE_EXPORT
//void cmd_ln_print_help_r (cmd_ln_t *cmdln,
//                          FILE *fp,	   /**< In: File to which to print */
//        const arg_t *defn /**< In: Array of argument name definitions */
//  );

///**
// * Non-reentrant version of cmd_ln_parse().
// *
// * @deprecated This is deprecated in favor of the re-entrant API
// * function cmd_ln_parse_r().
// * @return 0 if successful, <0 if error.
// */
//SPHINXBASE_EXPORT
//int32 cmd_ln_parse(const arg_t *defn,  /**< In: Array of argument name definitions */
//                   int32 argc,	       /**< In: Number of actual arguments */
//                   char *argv[],       /**< In: Actual arguments */
//                   int32 strict        /**< In: Fail on duplicate or unknown
//                                          arguments, or no arguments? */
//  );

///**
// * Parse an arguments file by deliminating on " \r\t\n" and putting each tokens
// * into an argv[] for cmd_ln_parse().
// *
// * @deprecated This is deprecated in favor of the re-entrant API
// * function cmd_ln_parse_file_r().
// *
// * @return 0 if successful, <0 on error.
// */
//SPHINXBASE_EXPORT
//int32 cmd_ln_parse_file(const arg_t *defn,   /**< In: Array of argument name definitions*/
//      char const *filename,/**< In: A file that contains all the arguments */ 
//                        int32 strict         /**< In: Fail on duplicate or unknown
//                                                arguments, or no arguments? */
//  );

///**
// * Old application initialization routine for Sphinx3 code.
// *
// * @deprecated This is deprecated in favor of the re-entrant API.
// */
//SPHINXBASE_EXPORT
//void cmd_ln_appl_enter(int argc,   /**< In: Number of actual arguments */
//           char *argv[], /**< In: Number of actual arguments */
//           char const* default_argfn, /**< In: default argument file name*/
//           const arg_t *defn /**< Command-line argument definition */
//  );


///**
// * Finalization routine corresponding to cmd_ln_appl_enter().
// *
// * @deprecated This is deprecated in favor of the re-entrant API.
// */

//SPHINXBASE_EXPORT
//void cmd_ln_appl_exit(void);

///**
// * Retrieve the global cmd_ln_t object used by non-re-entrant functions.
// *
// * @deprecated This is deprecated in favor of the re-entrant API.
// * @return global cmd_ln_t object.
// */
//SPHINXBASE_EXPORT
//cmd_ln_t *cmd_ln_get(void);


///**
// * Free the global command line, if any exists.
// * @deprecated Use the re-entrant API instead.
// */
//SPHINXBASE_EXPORT
//void cmd_ln_free (void);




  }
}
