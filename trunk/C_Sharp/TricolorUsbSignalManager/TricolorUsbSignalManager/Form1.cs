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

namespace TricolorUsbSignalManager {
    public partial class Form1 : Form, IDisposable {

        #region attributes
        //The tricolor USB device
        protected Blink1 mTricolorUsbSignal = new Blink1();

        //Current status 
        private NagiosChecker.NagiosEventState mCurrentStatus = NagiosChecker.NagiosEventState.UNKNOWN;

        //Blinking status memory
        private bool mRedBlinkStatus = false;

        //Communication pipe
        private NamedPipeServerStream mPipeIn = null;
        //Listener thread
        private Thread mThreadPipeListener = null;
        private bool mThreadMustRun = true;
        #endregion

        #region events


        private void DoLogLine(string pLine) {
            ThreadSafe(() => { 
                if ((float)txbPipeOutput.Text.Length >= 0.9 * (float)txbPipeOutput.MaxLength) {
                    txbPipeOutput.Clear();
                }
                txbPipeOutput.Text += string.Format("{0:HH:mm:ss} ", DateTime.Now) + pLine + "\r\n";
            });
        }
        #endregion

        public Form1() {
            InitializeComponent();
        }


        private void Form1_Load(object sender, EventArgs e) {
            mThreadPipeListener = new Thread(new ThreadStart(ThreadPipeServer));
            //mThreadPipeListener.Start();

            try {
                Console.WriteLine("Pass 1: Opening the first Blink(1) found.");
                mTricolorUsbSignal.Open(Constants.VENDOR_ID, Constants.PRODUCT_ID);
            }
            catch (Exception ex) {
                //MessageBox.Show("Error on connection: " + ex.Message);
            }
            //int versionNumber = blink1.GetVersion();
            //Console.WriteLine("Pass 1: Blink(1) device is at version: {0}.", versionNumber.ToString());

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
                mTricolorUsbSignal.Close();
            }
            catch { /* don't care */ }
        }

        private void btnCircle_Click(object sender, EventArgs e) {
            //for (int i = 0; i < 256; i++) {
            //    Console.WriteLine(string.Format("Pass 1: Send arbitrary data {0:X2} and answer is ", i) + mTricolorUsbSignal.SendRawBuffer(new byte[] { Constants.PACKET_ID, (byte)i }));
            //    Thread.Sleep(100);
            //}

            int vDelay = 500;
            int vDeltaPcnt = 15;

            while (vDelay > 5) {
                SendChar(Constants.LED_GREEN);
                Thread.Sleep(vDelay);
                SendChar(Constants.LED_ORANGE);
                Thread.Sleep(vDelay);
                SendChar(Constants.LED_RED);
                Thread.Sleep(vDelay);

                vDelay = (int)((double)vDelay * (1.0 - (double)vDeltaPcnt / 100.0));
            }

            SendChar(Constants.LED_ALL_OFF);
        }

        private void btnRed_Click(object sender, EventArgs e) {
            ckbRedBlink.Checked = true;
            SendChar(Constants.LED_RED);
        }

        private void SendChar(byte pValue) {
            Console.WriteLine(string.Format("Send arbitrary data {0:X2} and answer is ", pValue) + mTricolorUsbSignal.SendRawBuffer(new byte[] { Constants.PACKET_ID, pValue }));
        }

        private void btnYellow_Click(object sender, EventArgs e) {
            ckbRedBlink.Checked = false;
            SendChar(Constants.LED_ORANGE);
        }

        private void btnNoProblem_Click(object sender, EventArgs e) {
            ckbRedBlink.Checked = false;
            SendChar(Constants.LED_GREEN);
        }

        private void ckbRedBlink_CheckedChanged(object sender, EventArgs e) {
            timBlink.Enabled = ckbRedBlink.Checked;
            SendChar(Constants.LED_RED);
        }

        private void timBlink_Tick(object sender, EventArgs e) {
            if (mRedBlinkStatus) {
                //off
                SendChar(Constants.LED_ALL_OFF);
            }
            else { 
                //on
                SendChar(Constants.LED_RED);
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

        private void BlinkStatus(byte pStatus, int pDurationMs, int pBlinkCount) { 
            //just to avoid stucking the main thread, do this by a worker thread
            ThreadPool.QueueUserWorkItem(
                delegate(object o) {
                    SendChar(pStatus);
                    for (int i = 0; i < pBlinkCount; i++) {
                        Thread.Sleep(pDurationMs);
                        SendChar(Constants.LED_ALL_OFF);
                        Thread.Sleep(pDurationMs);
                        SendChar(pStatus);
                    }
                }
             );
        }

        private void CheckStatusAndUpdateLeds() {
            try {
                NagiosChecker vChecker = new NagiosChecker(this.DoLogLine);
                NagiosChecker.NagiosEventState vStatus = vChecker.CheckStatus();

                if (mCurrentStatus != vStatus) {
                    mCurrentStatus = vStatus;

                    switch (vStatus) {
                        case NagiosChecker.NagiosEventState.OK:
                            ckbRedBlink.Checked = false;
                            BlinkStatus(Constants.LED_GREEN, 100, 5);
                            break;
                        case NagiosChecker.NagiosEventState.WARNING:
                            ckbRedBlink.Checked = false;
                            BlinkStatus(Constants.LED_ORANGE, 100, 10);
                            break;
                        case NagiosChecker.NagiosEventState.CRITICAL:
                            ckbRedBlink.Checked = true;
                            BlinkStatus(Constants.LED_RED, 100, 20);
                            break;
                    }
                }
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
    }
}
