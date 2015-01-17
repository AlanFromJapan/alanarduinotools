using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ThingM.Blink1;
using System.Threading;
using System.IO.Pipes;
using System.IO;
using TricolorUsbSignalManager.Properties;
using System.Configuration;

namespace TricolorUsbSignalManager {
    public partial class Form1 : Form, IDisposable {
        #region Constants
        #endregion


        #region attributes
        //The tricolor USB device
        protected Blink1 mTricolorUsbSignal = null;

        //Current status 
        private NagiosChecker.NagiosEventState mCurrentStatus = NagiosChecker.NagiosEventState.UNKNOWN;

        //Blinking status memory
        private bool mRedBlinkStatus = false;

        //Communication pipe
        private NamedPipeServerStream mPipeIn = null;
        //Listener thread
        private Thread mThreadPipeListener = null;
        private bool mThreadMustRun = true;

        //to detect disconnection and avoid drowning user with messages
        private bool mLastSendCharWasError = false;
        private readonly object mSync = new object();

        //the icon in the taskbar
        private NotifyIcon mSystrayIcon = null;
        #endregion

        #region Properties

        /// <summary>
        /// Gets the device and init if needed (threadsafe)
        /// </summary>
        protected Blink1 TricolorUsbSignal {
            get {
                lock (mSync) {
                    if (mTricolorUsbSignal == null) {
                        mTricolorUsbSignal = new Blink1();

                        try {
                            mTricolorUsbSignal.Open(Constants.VENDOR_ID, Constants.PRODUCT_ID);

                            if (mLastSendCharWasError) {
                                DoLogLine("SUCCESS connecting to Blink");
                                this.mSystrayIcon.ShowBalloonTip(2000, "Tricolor USB Signal", "Connected to USB signal.", ToolTipIcon.Info);
                            }
                            mLastSendCharWasError = false;
                        }
                        catch (Exception ex) {
                            if (!mLastSendCharWasError) {
                                DoLogLine("ERROR while connecting to Blink!");
                                this.mSystrayIcon.ShowBalloonTip(2000, "Tricolor USB Signal", "Connection lost to USB signal!", ToolTipIcon.Error);
                            }
                            mLastSendCharWasError = true;
                            
                            Console.WriteLine("ERROR TricolorUsbSignal.get(): " + ex.Message);
                            mTricolorUsbSignal = null;
                        }
                    }
                }
                return mTricolorUsbSignal;
            }
        }

        #endregion


        /// <summary>
        /// Threadsafe logging to the text zone
        /// </summary>
        /// <param name="pLine"></param>
        private void DoLogLine(string pLine) {
            ThreadSafe(() => { 
                if ((float)txbPipeOutput.Text.Length >= 0.9 * (float)txbPipeOutput.MaxLength) {
                    txbPipeOutput.Clear();
                }
                txbPipeOutput.Text += string.Format("{0:HH:mm:ss} ", DateTime.Now) + pLine + "\r\n";
            });
        }
        

        public Form1() {
            InitializeComponent();

            mSystrayIcon = new NotifyIcon(this.components);
            mSystrayIcon.Icon = new Icon(this.GetType(), "Fatcow-Farm-Fresh-Traffic-lights.ico");
            mSystrayIcon.DoubleClick += mSystrayIcon_DoubleClick;
            mSystrayIcon.Visible = true;
            mSystrayIcon.Text = "Tricolor USB Signal (double click to show)";
            this.Resize += Form_Resize;
        }

        private void Form_Resize(object sender, EventArgs e) {
            if (this.WindowState == FormWindowState.Minimized) {
                this.ShowInTaskbar = false;
            }
        }

        void mSystrayIcon_DoubleClick(object sender, EventArgs e) {
            if (this.WindowState == FormWindowState.Minimized) {
                this.WindowState = FormWindowState.Normal;
                this.ShowInTaskbar = true;
            }
        }



        private void Form1_Load(object sender, EventArgs e) {
            mThreadPipeListener = new Thread(new ThreadStart(ThreadPipeServer));
            //mThreadPipeListener.Start();


            //int versionNumber = blink1.GetVersion();
            //Console.WriteLine("Pass 1: Blink(1) device is at version: {0}.", versionNumber.ToString());

            if (Settings.Default.IGNORE_DATE == default(DateTime)) {
                Settings.Default.IGNORE_DATE = DateTime.ParseExact("2001/01/01", "yyyy/MM/dd", null);
                Settings.Default.Save();
            }
            dtpIgnoreBefore.Value = Settings.Default.IGNORE_DATE;

            CheckStatusAndUpdateLeds();
        }

        //Body of the listener thread
        //http://jonathonreinhart.blogspot.jp/2012/12/named-pipes-between-c-and-python.html
        protected void ThreadPipeServer() {
            mPipeIn = new NamedPipeServerStream(Constants.NAMED_PIPE_INPUT);
            DoLogLine( "Named pipe opened " + Constants.NAMED_PIPE_INPUT);

            DoLogLine("Waiting for connection...");
            mPipeIn.WaitForConnection();
            DoLogLine("Got a customer, start serving");

            using (StreamReader vSR = new StreamReader(mPipeIn)) {
                while (mThreadMustRun) {
                    try {
                        string vLine = vSR.ReadLine();
                        if (vLine == null) {
                            Thread.Sleep(100);
                            continue;
                        }

                        DoLogLine(vLine);
                    }
                    catch (EndOfStreamException) { 
                        //user deconnection
                        MessageBox.Show("Lost user");
                    }
                }
            }
        }


        void IDisposable.Dispose() {
            mThreadMustRun = false;

            mPipeIn.Close();
            mPipeIn.Dispose();

            Console.WriteLine("Pass 1: Closing Blink(1) connection.");
            try {
                TricolorUsbSignal.Close();
            }
            catch { /* don't care */ }

            this.mSystrayIcon.Visible = false;
            this.mSystrayIcon.Dispose();
            this.mSystrayIcon = null;
        }

        private void btnCircle_Click(object sender, EventArgs e) {
            //for (int i = 0; i < 256; i++) {
            //    Console.WriteLine(string.Format("Pass 1: Send arbitrary data {0:X2} and answer is ", i) + mTricolorUsbSignal.SendRawBuffer(new byte[] { Constants.PACKET_ID, (byte)i }));
            //    Thread.Sleep(100);
            //}

            int vDelay = 500;
            int vDeltaPcnt = 15;

            while (vDelay > 5) {
                SetStatus(Constants.LED_GREEN);
                Thread.Sleep(vDelay);
                SetStatus(Constants.LED_ORANGE);
                Thread.Sleep(vDelay);
                SetStatus(Constants.LED_RED);
                Thread.Sleep(vDelay);

                vDelay = (int)((double)vDelay * (1.0 - (double)vDeltaPcnt / 100.0));
            }

            SetStatus(Constants.LED_ALL_OFF);
        }

        private void btnRed_Click(object sender, EventArgs e) {
            ckbRedBlink.Checked = true;
            SetStatus(Constants.LED_RED);
        }



        private void btnYellow_Click(object sender, EventArgs e) {
            ckbRedBlink.Checked = false;
            SetStatus(Constants.LED_ORANGE);
        }

        private void btnNoProblem_Click(object sender, EventArgs e) {
            ckbRedBlink.Checked = false;
            SetStatus(Constants.LED_GREEN);
        }

        private void ckbRedBlink_CheckedChanged(object sender, EventArgs e) {
            timBlink.Enabled = ckbRedBlink.Checked;
            SetStatus(Constants.LED_RED);
        }

        private void timBlink_Tick(object sender, EventArgs e) {
            if (mRedBlinkStatus) {
                //off
                SetStatus(Constants.LED_ALL_OFF);
            }
            else { 
                //on
                SetStatus(Constants.LED_RED);
            }

            mRedBlinkStatus = !mRedBlinkStatus;
        }




        private void ThreadSafe(MethodInvoker method) {
            if (InvokeRequired)
                Invoke(method);
            else
                method();
        }

        private void timChecker_Tick(object sender, EventArgs e) {
            CheckStatusAndUpdateLeds();
        }

        /// <summary>
        /// Sets the led status (just set, no blink)
        /// </summary>
        /// <param name="pValue"></param>
        private void SetStatus(byte pValue) {
            try {
                Console.WriteLine(string.Format("Send arbitrary data {0:X2} and answer is ", pValue) + TricolorUsbSignal.SendRawBuffer(new byte[] { Constants.PACKET_ID, pValue }));

                if (mLastSendCharWasError) {
                    DoLogLine("SUCCESS while sending data to Blink. Reconnected?");
                    this.mSystrayIcon.ShowBalloonTip(2000, "Tricolor USB Signal", "Connected to USB signal.", ToolTipIcon.Info);
                }
                mLastSendCharWasError = false;
            }
            catch (Exception ex) {
                if (!mLastSendCharWasError) {
                    DoLogLine("ERROR while sending data to Blink. Disconnected?");
                    this.mSystrayIcon.ShowBalloonTip(2000, "Tricolor USB Signal", "Connection lost to USB signal!", ToolTipIcon.Error);

                    try {
                        TricolorUsbSignal.Close();
                    }
                    catch (Exception ex2) {
                        DoLogLine("ERROR while closing connection to Blink. Ignore and release.");
                        Console.WriteLine("ERROR SendChar() - Close(): " + ex.Message);
                    }
                    finally {
                        mTricolorUsbSignal.Dispose();
                        mTricolorUsbSignal = null;
                    }
                }
                mLastSendCharWasError = true;

                Console.WriteLine("ERROR SendChar(): " + ex.Message);
            }
        }

        /// <summary>
        /// Sets the status and do some bling bling
        /// </summary>
        /// <param name="pStatus"></param>
        /// <param name="pDurationMs"></param>
        /// <param name="pBlinkCount"></param>
        private void BlinkStatus(byte pStatus, int pDurationMs, int pBlinkCount) { 
            //just to avoid stucking the main thread, do this by a worker thread
            ThreadPool.QueueUserWorkItem(
                delegate(object o) {
                    SetStatus(pStatus);
                    for (int i = 0; i < pBlinkCount; i++) {
                        Thread.Sleep(pDurationMs);
                        SetStatus(Constants.LED_ALL_OFF);
                        Thread.Sleep(pDurationMs);
                        SetStatus(pStatus);
                    }
                }
             );
        }

        /// <summary>
        /// Sets or blinks the status
        /// </summary>
        /// <param name="pSet"></param>
        /// <param name="pStatus"></param>
        /// <param name="pDurationMs"></param>
        /// <param name="pBlinkCount"></param>
        private void SetOrBlinkStatus(bool pSet, byte pStatus, int pDurationMs, int pBlinkCount) {
            if (pSet) {
                SetStatus(pStatus);
            }
            else {
                BlinkStatus(pStatus, pDurationMs, pBlinkCount);
            }

        }

        private void CheckStatusAndUpdateLeds() {
            try {
                NagiosChecker vChecker = new NagiosChecker(
                    this.DoLogLine, 
                    Settings.Default.IGNORE_DATE,
                    ConfigurationManager.AppSettings["NagiosURL"],
                    ConfigurationManager.AppSettings["NagiosLogin"],
                    ConfigurationManager.AppSettings["NagiosPassword"]);

                NagiosChecker.NagiosEventState vStatus = vChecker.CheckStatus();

                switch (vStatus) {
                    case NagiosChecker.NagiosEventState.OK:
                        ckbRedBlink.Checked = false;
                        SetOrBlinkStatus(vStatus == mCurrentStatus, Constants.LED_GREEN, 100, 5);
                        break;
                    case NagiosChecker.NagiosEventState.WARNING:
                        ckbRedBlink.Checked = false;
                        SetOrBlinkStatus(vStatus == mCurrentStatus, Constants.LED_ORANGE, 100, 10);
                        break;
                    case NagiosChecker.NagiosEventState.CRITICAL:
                        ckbRedBlink.Checked = true;
                        SetOrBlinkStatus(vStatus == mCurrentStatus, Constants.LED_RED, 100, 20);
                        break;
                }


                mCurrentStatus = vStatus;
                
            }
            catch (Exception ex) {
                //mute for now
            }
        }

        private void button1_Click(object sender, EventArgs e) {
            BlinkStatus(Constants.LED_GREEN, 100, 5);
        }

        private void button2_Click(object sender, EventArgs e) {
            BlinkStatus(Constants.LED_ORANGE, 100, 10);
        }

        private void button3_Click(object sender, EventArgs e) {
            BlinkStatus(Constants.LED_RED, 100, 20);
        }

        private void dtpIgnoreBefore_ValueChanged(object sender, EventArgs e) {
            if (Settings.Default.IGNORE_DATE != dtpIgnoreBefore.Value) {
                Settings.Default.IGNORE_DATE = dtpIgnoreBefore.Value;

                //save
                Settings.Default.Save();
                Settings.Default.Upgrade();

                DoLogLine(string.Format("Changed 'ignore before' time to {0:yyyy/MM/dd HH:mm:ss}.", Settings.Default.IGNORE_DATE));
            }
        }

    }
}
