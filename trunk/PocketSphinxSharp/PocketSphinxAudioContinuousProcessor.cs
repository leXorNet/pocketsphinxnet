using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace PocketSphinxSharp
{
  public class PocketSphinxAudioContinuousProcessor
  {
    #region Interop

    public const int CONT_AD_STATE_SIL=0;
    public const int CONT_AD_STATE_SPEECH = 1;


    /**
     * Initialize a continuous listening/silence filtering object.
     *
     * One time initialization of a continuous listening/silence filtering
     * object/module.  This can work in either "stream mode", where it
     * reads data from an audio device represented by
     * <code>ad_rec_t</code>, or in "block mode", where it filters out
     * silence regions from blocks of data passed into it.
     *
     * @param ad An audio device to read from, or 0 to operate in block mode.
     * @param adfunc The function used to read audio from <code>ad</code>,
     * or 0 to operate in block mode.  This is usually ad_read().
     * @return A pointer to a READ-ONLY structure used in other calls to
     * the object.  If any error occurs, the return value is 0.
     */
    public delegate int AdFunction(IntPtr ad_rec_handle, short[] buffer, int max);

    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr cont_ad_init(IntPtr ad_rec_handle, IntPtr adfunc);

    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr cont_ad_init(IntPtr ad_rec_handle, AdFunction adfunc);
    /**< In: adfunc = source function to be invoked
          to obtain raw A/D data.  See ad.h for the
          required prototype definition. */



    /**
     * Initializes a continuous listening object which simply passes data through (!)
     *
     * Like cont_ad_init, but put the module in raw mode; i.e., all data is passed
     * through, unfiltered.  (By special request.)
     */

    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr cont_ad_init_rawmode(IntPtr ad_rec_handle, IntPtr adfunc);

    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr cont_ad_init_rawmode(IntPtr ad_rec_handle, AdFunction adfunc);
    /**< In: adfunc = source function to be invoked
          to obtain raw A/D data.  See ad.h for the
          required prototype definition. */


    /**
     * Read raw audio data into the silence filter.
     *
     * The main read routine for reading speech/silence segmented audio data.  Audio
     * data is copied into the caller provided buffer, much like a file read routine.
     *
     * In "block mode", i.e. if 0 was passed as a read function to
     * <code>cont_ad_init</code>, the data in <code>buf</code> is taken as
     * input, and any non-silence data is written back to <code>buf</code>
     * on exit.  In this case, you must take care that <code>max</code>
     * does not overflow the internal buffer of the silence filter.  The
     * available number of samples can be obtained by calling
     * cont_ad_buffer_space().  Any excess data will be discarded.
     *
     * In normal mode, only speech segments are copied; silence segments are dropped.
     * In rawmode (cont_ad module initialized using cont_ad_init_rawmode()), all data
     * are passed through to the caller.  But, in either case, any single call to
     * cont_ad_read will never return data that crosses a speech/silence segment
     * boundary.
     * 
     * The following variables are updated for use by the caller (see cont_ad_t above):
     *   cont_ad_t.state,
     *   cont_ad_t.read_ts,
     *   cont_ad_t.seglen,
     *   cont_ad_t.siglvl.
     * 
     * Return value: Number of samples actually read, possibly 0; <0 if EOF on A/D source.
     */
    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern int cont_ad_read(IntPtr cont_ad_handle, short[] buffer, int max);
    /**< In: Object pointer returned by cont_ad_init */
    /**< In/Out: In block mode, contains input data.
          On return, buf contains A/D data returned
          by this function, if any. */
    /**< In: Maximum number of samples to be filled into buf.
               NOTE: max must be at least 256; otherwise
               the functions returns -1. */

    /**
     * Get the maximum number of samples which can be passed into cont_ad_read().
     */

    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern int cont_ad_buffer_space(IntPtr cont_ad_handle);


    /**
     * Calibrate the silence filter.
     *
     * Calibration to determine an initial silence threshold.  This function can be called
     * any number of times.  It should be called at least once immediately after cont_ad_init.
     * The silence threshold is also updated internally once in a while, so this function
     * only needs to be called in the middle if there is a definite change in the recording
     * environment.
     * The application is responsible for making sure that the raw audio source is turned on
     * before the calibration.
     * Return value: 0 if successful, <0 otherwise.
     */
    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern int cont_ad_calib(IntPtr cont_ad_handle);


    /**
     * Calibrate the silence filter without an audio device.
     *
     * If the application has not passed an audio device into the silence filter
     * at initialisation,  this routine can be used to calibrate the filter. The
     * buf (of length max samples) should contain audio data for calibration. This
     * data is assumed to be completely consumed. More than one call may be
     * necessary to fully calibrate. 
     * Return value: 0 if successful, <0 on failure, >0 if calibration not
     * complete.
     */


    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern int cont_ad_calib_loop(IntPtr cont_ad_handle, short[] buffer, int max);


    /**
     * Get the number of samples required to calibrate the silence filter.
     *
     * Since, as mentioned above, the calibration data is assumed to be
     * fully consumed, it may be desirable to "hold onto" this data in
     * case it contains useful speech.  This function returns the number
     * of samples required to calibrate the silence filter, which is
     * useful in allocating a buffer to store this data.
     *
     * @return Number of samples required for successful calibration.
     */
    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern int cont_ad_calib_size(IntPtr cont_ad_handle);

    /**
     * Set silence and speech threshold parameters.
     *
     * The silence threshold is the max power
     * level, RELATIVE to the peak background noise level, in any silence frame.  Similarly,
     * the speech threshold is the min power level, RELATIVE to the peak background noise
     * level, in any speech frame.  In general, silence threshold <= speech threshold.
     * Increasing the thresholds (say, from the default value of 2 to 3 or 4) reduces the
     * sensitivity to background noise, but may also increase the chances of clipping actual
     * speech.
     * @return: 0 if successful, <0 otherwise.
     */

    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern int cont_ad_set_thresh(IntPtr cont_ad_handle, int sil, int sp);


    /**
     * Set the changable parameters.
     *
     *   delta_sil, delta_speech, min_noise, and max_noise are in dB,
     *   winsize, speech_onset, sil_onset, leader and trailer are in frames of
     *   16 ms length (256 samples @ 16kHz sampling).
     */


    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern int cont_ad_set_params(IntPtr cont_ad_handle, int delta_sil, int delta_speech, int min_noise, int max_noise,
      int winsize, int speech_onset, int sil_onset, int leader, int trailer, float adapt_rate);



    /**
     * PWP 1/14/98 -- get the changable params.
     *
     *   delta_sil, delta_speech, min_noise, and max_noise are in dB,
     *   winsize, speech_onset, sil_onset, leader and trailer are in frames of
     *   16 ms length (256 samples @ 16kHz sampling).
     */


    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern int cont_ad_get_params(IntPtr cont_ad_handle, out int delta_sil, out int delta_speech, out int min_noise, out int max_noise,
      out int winsize, out int speech_onset, out int sil_onset, out int leader, out int trailer, out float adapt_rate);

    /**
     * Reset, discarding any accumulated speech segments.
     * @return 0 if successful, <0 otherwise.
     */

    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern int cont_ad_reset(IntPtr cont_ad_handle);

    /**
     * Close the continuous listening object.
     */

    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern int cont_ad_close(IntPtr cont_ad_handle);


    /**
     * Set the silence and speech thresholds.
     *
     * For this to remain permanently in effect, the auto_thresh field of
     * the continuous listening module should be set to FALSE or 0.
     * Otherwise the thresholds may be modified by the noise- level
     * adaptation.
     */
    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public static extern int cont_set_thresh(IntPtr cont_ad_handle, int silence, int speech);

    #endregion
    #region Code
    protected IntPtr handle = IntPtr.Zero;

    public IntPtr Handle
    {
      get { return this.handle; }
    }
    public PocketSphinxAudioContinuousProcessor()
    {
    }
    ~PocketSphinxAudioContinuousProcessor()
    {
      this.Close();
    }

    public int SetThreshold(int silence, int speech)
    {
      if (this.handle != IntPtr.Zero)
      {
        return cont_set_thresh(this.handle,silence, speech);
      }
      return -1;
    }
    public int SetAdThreshold(int silence, int speech)
    {
      if (this.handle != IntPtr.Zero)
      {
        return cont_ad_set_thresh(this.handle, silence, speech);
      }
      return -1;
    }

    public bool Init(PocketSphinxAudioRecorder recoder)
    {
      this.Close();

      if (this.handle == IntPtr.Zero && recoder!=null && recoder.Handle!=IntPtr.Zero)
      {
        return (this.handle = cont_ad_init(recoder.Handle, IntPtr.Zero)) != IntPtr.Zero;

      }
      return false;
    }
    public bool Init(PocketSphinxAudioRecorder recoder,AdFunction adfunction)
    {
      this.Close();

      if (this.handle == IntPtr.Zero && recoder != null && recoder.Handle != IntPtr.Zero)
      {
        return (this.handle = cont_ad_init(recoder.Handle, adfunction)) != IntPtr.Zero;

      }
      return false;
    }
    public bool InitRawMode(PocketSphinxAudioRecorder recoder)
    {
      this.Close();

      if (this.handle == IntPtr.Zero && recoder != null && recoder.Handle != IntPtr.Zero)
      {
        return (this.handle = cont_ad_init_rawmode(recoder.Handle, IntPtr.Zero)) != IntPtr.Zero;

      }
      return false;
    }
    public bool InitRawMode(PocketSphinxAudioRecorder recoder, AdFunction adfunction)
    {
      this.Close();

      if (this.handle == IntPtr.Zero && recoder != null && recoder.Handle != IntPtr.Zero)
      {
        return (this.handle = cont_ad_init_rawmode(recoder.Handle, adfunction)) != IntPtr.Zero;

      }
      return false;
    }
    public int Read(short[] buffer)
    {
      if (buffer!=null)
      {
        return this.Read(buffer, buffer.Length);
      }
      return -1;
    }
    public int Read(short[] buffer, int max)
    {
      if(this.handle!=IntPtr.Zero)
      {
        return cont_ad_read(this.handle,buffer,max);
      }
      return -1;
    }

    public int CalibLoop(short[] buffer)
    {
      if (buffer != null)
      {
        return this.CalibLoop(buffer, buffer.Length);
      }
      return -1;
    }
    public int CalibLoop(short[] buffer, int max)
    {
      if (this.handle != IntPtr.Zero)
      {
        return cont_ad_calib_loop(this.handle, buffer, max);
      }
      return -1;
    }

    public int GetBufferSpace()
    {
      if (this.handle != IntPtr.Zero)
      {
        return cont_ad_buffer_space(this.handle);
      }
      return -1;
    }
    public int GetCalibSize()
    {
      if (this.handle != IntPtr.Zero)
      {
        return cont_ad_calib_size(this.handle);
      }
      return -1;
    }

    public int Calib()
    {
      if (this.handle != IntPtr.Zero)
      {
        return cont_ad_calib(this.handle);
      }
      return -1;
    }

    public int SetParameters(int delta_sil, int delta_speech, int min_noise, int max_noise,
      int winsize, int speech_onset, int sil_onset, int leader, int trailer, float adapt_rate)
    {
      if (this.handle != IntPtr.Zero)
      {
        return cont_ad_set_params(this.handle,delta_sil,delta_speech,min_noise,max_noise,winsize,speech_onset,sil_onset,leader,trailer,adapt_rate);
      }
      return -1;
    }

    public int GetParameters(out int delta_sil, out int delta_speech, out int min_noise, out int max_noise,
      out int winsize, out int speech_onset, out int sil_onset, out int leader, out int trailer, out float adapt_rate)
    {
      if (this.handle != IntPtr.Zero)
      {
        return cont_ad_get_params(this.handle, out delta_sil, out delta_speech, out min_noise, out max_noise, out winsize, out speech_onset, out sil_onset, out leader, out trailer, out adapt_rate);
      }
      else
      {
        delta_sil = 0;
        delta_speech = 0;
        min_noise = 0;
        max_noise = 0;
        winsize = 0;
        speech_onset = 0;
        sil_onset = 0;
        leader = 0;
        trailer = 0;
        adapt_rate = 0.0f;
      }
      return -1;
    }


    public int Reset()
    {
      if (this.handle != IntPtr.Zero)
      {
        return cont_ad_reset(this.handle);
      }
      return -1;
    }
    public int Close()
    {
      int ret = -1;
      if (this.handle != IntPtr.Zero)
      {
         ret = cont_ad_close(this.handle);

         this.handle = IntPtr.Zero;
      }
      return ret;
    }
    #endregion
  }
}
