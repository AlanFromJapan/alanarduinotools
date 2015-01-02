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

        //Blinking status memory
        private bool mRedBlinkStatus = false;

        //Communication pipe
        private NamedPipeServerStream mPipeIn = null;
        //Listener thread
        private Thread mThreadPipeListener = null;
        private bool mThreadMustRun = true;
        #endregion



        public Form1() {
            InitializeComponent();
        }


        private void Form1_Load(object sender, EventArgs e) {
            mThreadPipeListener = new Thread(new ThreadStart(ThreadPipeServer));
            mThreadPipeListener.Start();

            try {
                Console.WriteLine("Pass 1: Opening the first Blink(1) found.");
                mTricolorUsbSignal.Open(Constants.VENDOR_ID, Constants.PRODUCT_ID);
            }
            catch (Exception ex) {
                //MessageBox.Show("Error on connection: " + ex.Message);
            }
            //int versionNumber = blink1.GetVersion();
            //Console.WriteLine("Pass 1: Blink(1) device is at version: {0}.", versionNumber.ToString());


        }

        //Body of the listener thread
        //http://jonathonreinhart.blogspot.jp/2012/12/named-pipes-between-c-and-python.html
        protected void ThreadPipeServer() {
            mPipeIn = new NamedPipeServerStream(Constants.NAMED_PIPE_INPUT);
            LogLine( "Named pipe opened " + Constants.NAMED_PIPE_INPUT);

            LogLine("Waiting for connection...");
            mPipeIn.WaitForConnection();
            LogLine("Got a customer, start serving");

            using (StreamReader vSR = new StreamReader(mPipeIn)) {
                while (mThreadMustRun) {
                    try {
                        string vLine = vSR.ReadLine();
                        if (vLine == null) {
                            Thread.Sleep(100);
                            continue;
                        }

                        LogLine(vLine);
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
            for (int i = 0; i < 256; i++) {
                Console.WriteLine(string.Format("Pass 1: Send arbitrary data {0:X2} and answer is ", i) + mTricolorUsbSignal.SendRawBuffer(new byte[] { Constants.PACKET_ID, (byte)i }));
                Thread.Sleep(100);
            }

        }

        private void btnRed_Click(object sender, EventArgs e) {
            ckbRedBlink.Checked = true;
            SendChar(0x80);
        }

        private void SendChar(byte pValue) {
            Console.WriteLine(string.Format("Send arbitrary data {0:X2} and answer is ", pValue) + mTricolorUsbSignal.SendRawBuffer(new byte[] { Constants.PACKET_ID, pValue }));
        }

        private void btnYellow_Click(object sender, EventArgs e) {
            ckbRedBlink.Checked = false;
            SendChar(0x40);
        }

        private void btnNoProblem_Click(object sender, EventArgs e) {
            ckbRedBlink.Checked = false;
            SendChar(0x20);
        }

        private void ckbRedBlink_CheckedChanged(object sender, EventArgs e) {
            timBlink.Enabled = ckbRedBlink.Checked;
            SendChar(0x80);
        }

        private void timBlink_Tick(object sender, EventArgs e) {
            if (mRedBlinkStatus) {
                //off
                SendChar(0x00);
            }
            else { 
                //on
                SendChar(0x80);
            }

            mRedBlinkStatus = !mRedBlinkStatus;
        }


        private void LogLine(string pLine) {

            ThreadSafe(() => txbPipeOutput.Text += string.Format("{0:HH:mm:ss} ", DateTime.Now) + pLine + "\r\n");

        }

        private void ThreadSafe(MethodInvoker method) {
            if (InvokeRequired)
                Invoke(method);
            else
                method();
        }
    }
}
