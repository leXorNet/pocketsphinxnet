using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace PocketSphinxSharp
{
  public abstract class PocketSphinxAudioBase
  {
    public const int DEFAULT_SAMPLES_PER_SEC = 16000;

    public const int AD_SAMPLE_SIZE = sizeof(Int16);

    public const int AD_OK = 0;
    public const int AD_EOF = -1;
    public const int AD_ERR_GEN = -1;
    public const int AD_ERR_NOT_OPEN = -2;
    public const int AD_ERR_WAVE = -3;
    public const int AD_NULL_POINTER = -4;

    [DllImport("PocketSphinxAudio",CallingConvention=CallingConvention.Cdecl)]
    public extern static void ad_mu2li(
      [MarshalAs(UnmanagedType.LPArray)]
      Int16[] outbuf,		//Out: PCM data placed here (allocated by user)
      [MarshalAs(UnmanagedType.LPArray)]
      Byte[] inbuf,	// In: Input buffer with mulaw data 
      Int32 n_samp);		// In: #Samples in inbuf


  }
  public class PocketSphinxAudioRecorder : PocketSphinxAudioBase
  {
    #region Introp


    /// <summary>
    /// Open a specific audio device for recording.
    ///
    /// The device is opened in non-blocking mode and placed in idle state.
    ///
    /// @return pointer to read-only ad_rec_t structure if successful, NULL
    /// otherwise.  The return value to be used as the first argument to
    /// other recording functions.
    ///
    /// </summary>

    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public extern static IntPtr ad_open_dev(
      [MarshalAs(UnmanagedType.LPStr)]
	    string dev, //< Device name (platform-specific)
      Int32 samples_per_sec //Samples per second 
      );

    /// <summary>
    /// Open the default audio device with a given sampling rate.
    /// </summary>
    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public extern static IntPtr ad_open_sps(
           Int32 samples_per_sec // Samples per second 
           );


    /// <summary>
    /// Open the default audio device.
    /// </summary>
    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public extern static IntPtr ad_open();


    /// <summary>
    /// Like ad_open_sps but specifies buffering required within driver.  This function is
    /// useful if the default (5000 msec worth) is too small and results in loss of data.
    /// </summary>
    /// <param name="samples_per_sec"></param>
    /// <param name="bufsize_msec"></param>
    /// <returns></returns>
    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public extern static IntPtr ad_open_sps_bufsize(Int32 samples_per_sec, Int32 bufsize_msec);

    /// <summary>
    /// Start audio recording.  Return value: 0 if successful, <0 otherwise 
    /// </summary>
    /// <param name="handle"></param>
    /// <returns></returns>
    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public extern static Int32 ad_start_rec(IntPtr handle);


    /// <summary>
    /// Stop audio recording.  Return value: 0 if successful, <0 otherwise 
    /// </summary>
    /// <param name="handle"></param>
    /// <returns></returns>
    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public extern static Int32 ad_stop_rec(IntPtr handle);

    /// <summary>
    /// Close the recording device.  Return value: 0 if successful, <0 otherwise
    /// </summary>
    /// <param name="handle"></param>
    /// <returns></returns>
    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public extern static Int32 ad_close(IntPtr handle);



    /// <summary>
    /// Read next block of audio samples while recording; read upto max samples into buf.
    /// Return value: # samples actually read (could be 0 since non-blocking); -1 if not
    /// recording and no more samples remaining to be read from most recent recording.
    /// </summary>
    /// <param name="handle"></param>
    /// <returns></returns>
    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public extern static Int32 ad_read(
      IntPtr handle,
      [MarshalAs(UnmanagedType.LPArray)]
      Int16[] buf,
      Int32 max);

    #endregion
    #region Code
    protected IntPtr handle = IntPtr.Zero;

    public IntPtr Handle { get { return this.handle; } }

    public PocketSphinxAudioRecorder()
    {
    }
    ~PocketSphinxAudioRecorder()
    {
      this.Close();
    }

    public bool Open()
    {
      this.Close();
      return ((this.handle = ad_open()) != IntPtr.Zero);
    }
    public bool Open(string device, int samples_per_second)
    {
      this.Close();
      return ((this.handle = ad_open_dev(device, samples_per_second)) != IntPtr.Zero);
    }
    public bool Open(int samples_per_second, int bufsize_msec)
    {
      this.Close();
      return ((this.handle = ad_open_sps_bufsize(samples_per_second, bufsize_msec)) != IntPtr.Zero);
    }
    public bool Open(int samples_per_second)
    {
      this.Close();
      return ((this.handle = ad_open_sps(samples_per_second)) != IntPtr.Zero);
    }

    public int Start()
    {
      if (this.handle != IntPtr.Zero)
      {
        return ad_start_rec(this.handle);
      }
      return AD_ERR_NOT_OPEN;
    }
    public int Stop()
    {
      if (this.handle != IntPtr.Zero)
      {
        return ad_stop_rec(this.handle);
      }
      return AD_ERR_NOT_OPEN;
    }

    public int Read(short[] buffer)
    {
      if (buffer != null)
      {
        return ad_read(this.handle, buffer, buffer.Length);
      }
      return AD_NULL_POINTER;
    }

    public int Read(short[] buffer, int length)
    {
      if (this.handle != IntPtr.Zero)
      {
        return ad_read(this.handle, buffer, length);
      }
      return AD_ERR_NOT_OPEN;
    }

    public int Close()
    {
      int done = AD_ERR_NOT_OPEN;

      if (this.handle != IntPtr.Zero)
      {
        done = ad_close(this.handle);
        this.handle = IntPtr.Zero;
      }
      return AD_ERR_NOT_OPEN;
    }
    #endregion
  }

  public class PocketSphinxAudioPlayer : PocketSphinxAudioBase
  {

    #region Interop

    /// <summary>
    /// Open play device
    /// 
    /// </summary>
    /// <param name="samples_per_sec"></param>
    /// <returns>The play handle (not the record handle)</returns>
    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public extern static IntPtr ad_open_play_sps(Int32 samples_per_sec);

    /// <summary>
    /// Open play device
    /// 
    /// </summary>
    /// <param name="samples_per_sec"></param>
    /// <returns>The play handle (not the record handle)</returns>
    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public extern static IntPtr ad_open_play();

    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public extern static Int32 ad_start_play(IntPtr handle);

    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public extern static Int32 ad_stop_play(IntPtr handle);


    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public extern static Int32 ad_close_play(IntPtr handle);



    ///<summary>
    ///Queue a block of audio samples for playback.
    ///Write the next block of [len] samples from rawbuf to the A/D device for playback.
    ///The device may queue less than len samples, possibly 0, since it is non-blocking.
    ///The application should resubmit the remaining data to be played.
    ///Return value: # samples accepted for playback; -1 if error.
    ///</summary>
    [DllImport("PocketSphinxAudio", CallingConvention = CallingConvention.Cdecl)]
    public extern static Int32 ad_write(IntPtr handle,
      [MarshalAs(UnmanagedType.LPArray)]
      Int16[] buf,
      Int32 len
      );

    #endregion

    #region Code
    protected IntPtr handle = IntPtr.Zero;

    public IntPtr Handle { get { return this.handle; } set { this.handle = value; } }

    public PocketSphinxAudioPlayer()
    {
    }

    public bool Open()
    {
      this.Close();
      return ((this.handle = ad_open_play()) != IntPtr.Zero);
    }
    public bool Open(int samples_per_second)
    {
      this.Close();
      return ((this.handle = ad_open_play_sps(samples_per_second)) != IntPtr.Zero);
    }
    public int Start()
    {
      if (this.handle != IntPtr.Zero)
      {
        return ad_start_play(this.handle);
      }
      return AD_ERR_NOT_OPEN;
    }
    public int Stop()
    {
      if (this.handle != IntPtr.Zero)
      {
        return ad_stop_play(this.handle);
      }
      return AD_ERR_NOT_OPEN;
    }
    public int Write(short[] buffer)
    {
      if (buffer != null)
      {
        return this.Write(buffer, buffer.Length);
      }
      return AD_NULL_POINTER;
    }
    public int Write(short[] buffer, int length)
    {
      if (this.handle != IntPtr.Zero)
      {
        return ad_write(this.handle, buffer, length);
      }
      return AD_ERR_NOT_OPEN;
    }
    public int Close()
    {
      int done = AD_ERR_NOT_OPEN;

      if (this.handle != IntPtr.Zero)
      {
        done = ad_close_play(this.handle);
        this.handle = IntPtr.Zero;
      }
      return done;
    }
    #endregion
  }
}

