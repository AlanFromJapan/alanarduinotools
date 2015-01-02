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

namespace TricolorUsbSignalManager {
    public partial class Form1 : Form, IDisposable {

        #region attributes
        protected Blink1 mTricolorUsbSignal = new Blink1();
        #endregion

        public Form1() {
            InitializeComponent();
        }


        private void Form1_Load(object sender, EventArgs e) {
            Console.WriteLine("Pass 1: Opening the first Blink(1) found.");
            mTricolorUsbSignal.Open(Constants.VENDOR_ID, Constants.PRODUCT_ID);

            //int versionNumber = blink1.GetVersion();
            //Console.WriteLine("Pass 1: Blink(1) device is at version: {0}.", versionNumber.ToString());



        }



        void IDisposable.Dispose() {
            Console.WriteLine("Pass 1: Closing Blink(1) connection.");
            mTricolorUsbSignal.Close();  
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

        private bool mRedBlinkStatus = false;
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
    }
}
