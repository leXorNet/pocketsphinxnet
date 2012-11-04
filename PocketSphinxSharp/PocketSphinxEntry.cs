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

    public void Work(string argsfile)
    {
      CommandLine cl = new CommandLine();

      cl.Init();

      cl.SetString("-hmm", "model/hmm/en_US/hub4wsj_sc_8k");
      cl.SetString("-lm", "model/lm/en/turtle.DMP");
      cl.SetString("-dict", "model/lm/en/turtle.dic");


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
                
  
                hyp = engine.GetHypothesis(out score,out out_uttid);

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
