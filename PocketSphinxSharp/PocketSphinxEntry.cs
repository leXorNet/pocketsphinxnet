using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using PocketSphinxNet;

namespace PocketSphinxSharp
{
  public class PocketSphinxEntry
  {
    PocketSphinx engine = null;
    PocketSphinxAudioRecorder recoder = null;
    PocketSphinxAudioContinuousProcessor cad = null;

    public PocketSphinxEntry()
    {
      this.engine = new PocketSphinx();
      this.recoder = new PocketSphinxAudioRecorder();
      this.cad = new PocketSphinxAudioContinuousProcessor();
    }
    #region Args
    protected ArgStruct[] DefaultArgs = new ArgStruct[] 
      {

        new ArgStruct("-logspec", ArgType._ARG_BOOLEAN, "no", null)  ,
        new ArgStruct("-smoothspec", ArgType._ARG_BOOLEAN, "no", null)  ,
        new ArgStruct("-transform", ArgType._ARG_STRING, "legacy", null)  ,
        new ArgStruct("-alpha", ArgType._ARG_FLOAT32, "0.97", null)  ,
        new ArgStruct("-samprate", ArgType._ARG_FLOAT32, "16000", null)  ,
        new ArgStruct("-frate", ArgType._ARG_INT32, "100", null)  ,
        new ArgStruct("-wlen", ArgType._ARG_FLOAT32, "0.025625", null)  ,
        new ArgStruct("-nfft", ArgType._ARG_INT32, "512", null)  ,
        new ArgStruct("-nfilt", ArgType._ARG_INT32, "40", null)  ,
        new ArgStruct("-lowerf", ArgType._ARG_FLOAT32, "133.33334", null)  ,
        new ArgStruct("-upperf", ArgType._ARG_FLOAT32, "6855.4976", null)  ,
        new ArgStruct("-unit_area", ArgType._ARG_BOOLEAN, "yes", null)  ,
        new ArgStruct("-round_filters", ArgType._ARG_BOOLEAN, "yes", null)  ,
        new ArgStruct("-ncep", ArgType._ARG_INT32, "13", null)  ,
        new ArgStruct("-doublebw", ArgType._ARG_BOOLEAN, "no", null)  ,
        new ArgStruct("-lifter", ArgType._ARG_INT32, "0", null)  ,
        new ArgStruct("-input_endian", ArgType._ARG_STRING, "little", null)  ,
        new ArgStruct("-warp_type", ArgType._ARG_STRING, "inverse_linear", null)  ,
        new ArgStruct("-warp_params", ArgType._ARG_STRING, "0", null)  ,
        new ArgStruct("-dither", ArgType._ARG_BOOLEAN, "no", null)  ,
        new ArgStruct("-seed", ArgType._ARG_INT32, "-1", null)  ,
        new ArgStruct("-remove_dc", ArgType._ARG_BOOLEAN, "no", null)  ,
        new ArgStruct("-verbose", ArgType._ARG_BOOLEAN, "no", null)  ,



        new ArgStruct("-feat", ArgType._ARG_STRING, "1s_c_d_dd", null)  ,
        new ArgStruct("-ceplen", ArgType._ARG_INT32, "13", null)  ,
        new ArgStruct("-cmn", ArgType._ARG_STRING, "current", null)  ,

        new ArgStruct("-cmninit", ArgType._ARG_STRING, "8.0", null)  ,
        new ArgStruct("-varnorm", ArgType._ARG_BOOLEAN, "no", null)  ,
        new ArgStruct("-agc", ArgType._ARG_STRING, "none", null)  ,
        new ArgStruct("-agcthresh", ArgType._ARG_FLOAT32, "2.0", null)  ,
        new ArgStruct("-lda", ArgType._ARG_STRING, null, null)  ,
        new ArgStruct("-ldadim", ArgType._ARG_INT32, "0", null)  ,
        new ArgStruct("-svspec", ArgType._ARG_STRING, null, null)  ,


        new ArgStruct("-hmm", ArgType._ARG_STRING, null, null)  ,
        new ArgStruct("-featparams", ArgType._ARG_STRING, null, null)  ,
        new ArgStruct("-mdef", ArgType._ARG_STRING,null, null)  ,
        new ArgStruct("-senmgau", ArgType._ARG_STRING,null, null)  ,

        new ArgStruct("-tmat", ArgType._ARG_STRING,null, null)  ,
        new ArgStruct("-tmatfloor", ArgType._ARG_FLOAT32,"0.0001", null)  ,
        new ArgStruct("-mean", ArgType._ARG_STRING,null, null)  ,
        new ArgStruct("-var", ArgType._ARG_STRING,null, null)  ,
        new ArgStruct("-varfloor", ArgType._ARG_FLOAT32,"0.00001", null)  ,
        new ArgStruct("-mixw", ArgType._ARG_STRING,null, null)  ,

        new ArgStruct("-mixwfloor", ArgType._ARG_FLOAT32,"0.0000001", null)  ,
        new ArgStruct("-aw", ArgType._ARG_STRING,"1", null)  ,
        new ArgStruct("-sendump", ArgType._ARG_STRING,null, null)  ,
        new ArgStruct("-mllr", ArgType._ARG_STRING,null, null)  ,
        new ArgStruct("-mmap", ArgType._ARG_BOOLEAN,"yes", null)  ,

        new ArgStruct("-ds", ArgType._ARG_INT32, "1", null)  ,
        new ArgStruct("-topn", ArgType._ARG_INT32, "1", null)  ,
        new ArgStruct("-topn_beam", ArgType._ARG_STRING, null, null)  ,
        new ArgStruct("-kdtree", ArgType._ARG_STRING, null, null)  ,
        new ArgStruct("-kdmaxdepth", ArgType._ARG_INT32, "0", null)  ,
        new ArgStruct("-kdmaxbbi", ArgType._ARG_INT32, "-1", null)  ,
        new ArgStruct("-logbase", ArgType._ARG_FLOAT32,"1.0001", null)  ,


        new ArgStruct("-beam", ArgType._ARG_FLOAT64,"1e-48", null)  ,
        new ArgStruct("-wbeam", ArgType._ARG_FLOAT64,"7e-29", null)  ,
        new ArgStruct("-pbeam", ArgType._ARG_FLOAT64,"1e-48", null)  ,
        new ArgStruct("-lpbeam", ArgType._ARG_FLOAT64,"1e-40", null)  ,
        new ArgStruct("-lponlybeam", ArgType._ARG_FLOAT64,"7e-29", null)  ,
        new ArgStruct("-fwdflatbeam", ArgType._ARG_FLOAT64,"1e-64", null)  ,
        new ArgStruct("-fwdflatwbeam", ArgType._ARG_FLOAT64,"7e-29", null)  ,
        new ArgStruct("-pl_window", ArgType._ARG_INT32,"0", null)  ,
        new ArgStruct("-pl_beam", ArgType._ARG_FLOAT64,"1e-10", null)  ,
        new ArgStruct("-pl_pbeam", ArgType._ARG_FLOAT64,"1e-5", null)  ,


        new ArgStruct("-compallsen", ArgType._ARG_BOOLEAN, "no", null)  ,
        new ArgStruct("-fwdtree", ArgType._ARG_BOOLEAN, "yes", null)  ,
        new ArgStruct("-fwdflat", ArgType._ARG_BOOLEAN, "yes", null)  ,
        new ArgStruct("-bestpath", ArgType._ARG_BOOLEAN, "yes", null)  ,
        new ArgStruct("-backtrace", ArgType._ARG_BOOLEAN, "no", null)  ,
        new ArgStruct("-latsize", ArgType._ARG_INT32,"5000", null)  ,
        new ArgStruct("-maxwpf", ArgType._ARG_INT32,"-1", null)  ,
        new ArgStruct("-maxhmmpf", ArgType._ARG_INT32,"-1", null)  ,
        new ArgStruct("-min_endfr", ArgType._ARG_INT32,"0", null)  ,
        new ArgStruct("-fwdflatefwid", ArgType._ARG_INT32,"4", null)  ,
        new ArgStruct("-fwdflatsfwin", ArgType._ARG_INT32,"25", null)  ,


        new ArgStruct("-fsg", ArgType._ARG_STRING, null, null)  ,
        new ArgStruct("-jsgf", ArgType._ARG_STRING, null, null)  ,
        new ArgStruct("-toprule", ArgType._ARG_STRING, null, null)  ,

        new ArgStruct("-fsgusealtpron", ArgType._ARG_BOOLEAN, "yes", null)  ,
        new ArgStruct("-fsgusefiller", ArgType._ARG_BOOLEAN, "yes", null)  ,


        new ArgStruct("-lm", ArgType._ARG_STRING, null, null)  ,
        new ArgStruct("-lmctl", ArgType._ARG_STRING, null, null)  ,
        new ArgStruct("-lw", ArgType._ARG_FLOAT32, "6.5", null)  ,
        new ArgStruct("-fwdflatlw", ArgType._ARG_FLOAT32, "8.5", null)  ,
        new ArgStruct("-bestpathlw", ArgType._ARG_FLOAT32, "9.5", null)  ,
        new ArgStruct("-ascale", ArgType._ARG_FLOAT32, "20.0", null)  ,
        new ArgStruct("-wip", ArgType._ARG_FLOAT32, "0.65", null)  ,
        new ArgStruct("-nwpen", ArgType._ARG_FLOAT32, "1.0", null)  ,
        new ArgStruct("-pip", ArgType._ARG_FLOAT32, "1.0", null)  ,
        new ArgStruct("-uw", ArgType._ARG_FLOAT32, "1.0", null)  ,
        new ArgStruct("-silprob", ArgType._ARG_FLOAT32, "0.005", null)  ,
        new ArgStruct("-fillprob", ArgType._ARG_FLOAT32, "1e-8", null)  ,
        new ArgStruct("-bghist", ArgType._ARG_BOOLEAN, "no", null)  ,
        new ArgStruct("-lextreedump", ArgType._ARG_INT32, "0", null)  ,

        new ArgStruct("-dict", ArgType._REQARG_STRING, null, null)  ,
        new ArgStruct("-fdict", ArgType._ARG_STRING, null, null)  ,
        new ArgStruct("-dictcase", ArgType._ARG_BOOLEAN, "no", null)  ,
        new ArgStruct("-maxnewoov", ArgType._ARG_INT32, "20", null)  ,
        new ArgStruct("-usewdphones", ArgType._ARG_BOOLEAN, "no", null)  ,

        new ArgStruct("-argfile", ArgType._ARG_STRING, null, null)  ,
        new ArgStruct("-adcdev", ArgType._ARG_STRING, null, null)  ,
        new ArgStruct("-infile", ArgType._ARG_STRING, null, null)  ,
        new ArgStruct("-time", ArgType._ARG_STRING, "no", null)  ,
        new ArgStruct("-dict", ArgType._ARG_STRING, null, null)  
      
      
      };
    #endregion

    public void Work(string argsfile)
    {
      CommandLine cl = new CommandLine();

      if (cl.ParseFile(DefaultArgs, argsfile, false))
      {
        if (this.recoder.Open())
        {
          if (engine.Create(cl))
          {
            if (cad.Init(recoder))
            {
              recoder.Start();

              cad.Calib();

              bool run = true;
              while (run)
              {
                short[] buffer = new short[4096];

                int k = 0;
                while ((k = this.cad.Read(buffer)) == 0)
                {
                }

                if (engine.StartUtt(null))
                {
                  engine.ProcessRaw(buffer, false, false);

                  while ((k = this.cad.Read(buffer)) >= 0)
                  {

                    this.engine.ProcessRaw(buffer, false, false);

                  }

                  this.recoder.Stop();
                  this.cad.Reset();
                  this.engine.EndUtt();


                  string out_uttid = null;

                  string hyp = null;
                  int score = 0;


                  hyp = engine.GetHypothesis(out score, out out_uttid);

                  if (hyp == "goodbye")
                  {
                    break;
                  }
                }
              }

              this.recoder.Stop();
              this.cad.Close();
            }
            this.engine.Clone();
          }
          this.recoder.Close();
        }
      }
    }
  }
}
