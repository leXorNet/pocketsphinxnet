using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using PocketSphinxNet;

namespace PocketSphinxSharp
{
  public class PocketSphinxEntry
  {
    PocketSphinx engine = null;
    PocketSphinxAudioRecorder recoder = null;
    PocketSphinxAudioContinuousProcessor cont = null;

    public PocketSphinxEntry()
    {
      this.engine = new PocketSphinx();
      this.recoder = new PocketSphinxAudioRecorder();
      this.cont = new PocketSphinxAudioContinuousProcessor();
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

        new ArgStruct("-fdict", ArgType._ARG_STRING, null, null)  ,
        new ArgStruct("-dictcase", ArgType._ARG_BOOLEAN, "no", null)  ,
        new ArgStruct("-maxnewoov", ArgType._ARG_INT32, "20", null)  ,
        new ArgStruct("-usewdphones", ArgType._ARG_BOOLEAN, "no", null)  ,

      
        new ArgStruct("-lm", ArgType._ARG_STRING, "model/lm/en/turtle.DMP", null)  ,
        new ArgStruct("-hmm", ArgType._ARG_STRING, "model/hmm/en_US/hub4wsj_sc_8k", null)  ,
      
        new ArgStruct("-dict", ArgType._REQARG_STRING, "model/lm/en/turtle.dic", null)  ,
      };
    #endregion

    public void Work(string argsfile)
    {
      Console.WriteLine("PocketSphinxNet");

      CommandLine cl = new CommandLine();

      if (cl.Init(false, 
        "-hmm", "model/hmm/en_US/hub4wsj_sc_8k",
        "-lm","model/lm/en/turtle.DMP",
        "-dict","model/lm/en/turtle.dic"
        ))
      {
        Console.WriteLine("Command line parsed.");

        if (this.recoder.Open())
        {
          Console.WriteLine("Recoder is open.");

          if (engine.Create(cl))
          {
            Console.WriteLine("Engine is ready.");

            if (cont.Init(recoder))
            {
              Console.WriteLine("Recoder is started");

              recoder.Start();

              Console.WriteLine("Calibaration started.");

              cont.Calib();

              Console.WriteLine("Calibration finished");

              while (true)
              {
                short[] buffer = new short[4096];

                int k = 0;
                int rem = 0;

                /* Wait data for next utterance */

                while ((k = this.cont.Read(buffer)) == 0)
                {
                  Thread.Sleep(100);
                }
                if (k < 0)
                {
                  //Input audio disconnected
                  break;
                }
                if (engine.StartUtt(null))
                {

                  engine.ProcessRaw(buffer, false, false);

                  Console.WriteLine("Listening...");
                  
                  
                  DateTime st = DateTime.Now;
                  DateTime ct = DateTime.Now;

                  while (true)
                  {

                    k = this.cont.Read(buffer);

                     ct = DateTime.Now;

                    if (k == 0)
                    {
                      if (ct - st > TimeSpan.FromSeconds(1))
                      {
                        break;
                      }
                    }


                    rem = this.engine.ProcessRaw(buffer, false, false);

                    if (k == 0 && rem == 0)
                    {
                      Thread.Sleep(20);
                    }
                  }

                  this.recoder.Stop();

                  while ((k = this.recoder.Read(buffer)) >= 0) ;

                  Console.WriteLine("Searching for hypothesis...");

                  this.cont.Reset();
                  this.engine.EndUtt();

                  string out_uttid = null;
                  string hyp = null;

                  int score = 0;

                  hyp = engine.GetHypothesis(out score, out out_uttid);

                  Console.WriteLine("Hypothesis: \"{0}\",Uttid:\"{1}\",Score:\"{2}\"", hyp, out_uttid, score);

                  if (hyp != null)
                  {
                    string[] parts = hyp.Split(' ');
                    if (parts.Length > 0 && parts[0] == "goodbye")
                    {
                      Console.WriteLine("Bye bye!");

                      break;

                    }
                  }

                  if (this.recoder.Start() < 0)
                  {
                  }

                }
              }

              this.recoder.Stop();
              this.cont.Close();
            }

            this.engine.Close();
            Console.WriteLine("Engine closed.");
          }
          this.recoder.Close();
          Console.WriteLine("Recorder closed.");
        }
      }
    }
  }
}
