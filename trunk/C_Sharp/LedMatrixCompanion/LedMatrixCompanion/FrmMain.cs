using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;

namespace LedMatrixCompanion {
    public partial class FrmMain : Form {
        private SerialPort mPort = null;

        public FrmMain() {
            InitializeComponent();
        }

        /// <summary>
        /// Connects to the serial port
        /// </summary>
        /// <param name="pSpeed"></param>
        /// <param name="pCOMPort"></param>
        private bool ConnectSerial(int pSpeed, string pCOMPort) {
            try {
                mPort = new SerialPort(pCOMPort);
                mPort.BaudRate = pSpeed;
                mPort.DataBits = 8;
                mPort.DiscardNull = false;
                mPort.ReadTimeout = 1000;

                mPort.Open();

                return true;
            }
            catch {
                this.DisconnectSerial();
                return false;
            }
        }

        /// <summary>
        /// Disconnects the current port
        /// </summary>
        private void DisconnectSerial() {
            if (this.mPort != null && this.mPort.IsOpen) {
                this.mPort.Close();
            }
            if (this.mPort != null) {
                this.mPort.Dispose();
            }
            this.mPort = null;
        }

        private void btnAutoTest_Click(object sender, EventArgs e) {
            mRunAutoTest = !mRunAutoTest;
            if (mRunAutoTest) {
                ThreadPool.QueueUserWorkItem(new WaitCallback(RunAutoTest1));
            }
        }

        bool mRunAutoTest = false;
        private void RunAutoTest1(object stateInfo) {
            try {
                ConnectSerial(19200, "COM8");
                while (mRunAutoTest) {
                    for (int i = 0;mRunAutoTest && i < 256; i++) {
                        for (int col = 0; mRunAutoTest && col < 8; col++) {
                            string vVal = i.ToString("X2");
                            mPort.Write(col.ToString());
                            mPort.Write(vVal);

                            Thread.Sleep(10);
                        }
                        Application.DoEvents();
                    }
                }
            }
            catch {
                DisconnectSerial();
            }
        }

        private void button1_Click(object sender, EventArgs e) {
            mRunAutoTest = !mRunAutoTest;
            if (mRunAutoTest) {
                ThreadPool.QueueUserWorkItem(new WaitCallback(RunAutoTest2));
            }
        }

        private void RunAutoTest2(object stateInfo) {
            try {
                ConnectSerial(19200, "COM8");
                while (mRunAutoTest) {
                    for (int col = 0; mRunAutoTest && col < 8; col++) {
                        mPort.Write(col.ToString());
                        mPort.Write("FF");
                        Thread.Sleep(10);

                        mPort.Write(((col - 1) % 8).ToString());
                        mPort.Write("00");
                        Thread.Sleep(10);

                        Thread.Sleep(100);
                    }
                    Application.DoEvents();
                    
                }
            }
            catch {
                DisconnectSerial();
            }
        }

        private void btnTest3_Click(object sender, EventArgs e) {
            mRunAutoTest = !mRunAutoTest;
            if (mRunAutoTest) {
                ThreadPool.QueueUserWorkItem(new WaitCallback(RunAutoTest3));
            }
        }

        private void RunAutoTest3(object stateInfo) {
            try {
                ConnectSerial(19200, "COM8");
                Random vRnd = new Random();
                int x = vRnd.Next(7);
                int y = vRnd.Next(7);
                int vVectX = -1, vVectY = -1;

                while (mRunAutoTest) {
                    mPort.Write(x.ToString());
                    mPort.Write("00");
                    Thread.Sleep(10);

                    if (x == 0 || x == 7) {
                        vVectX = -vVectX;
                    }
                    if (y == 0 || y == 7) {
                        vVectY = -vVectY;
                    }

                    x += vVectX;
                    y += vVectY;

                    mPort.Write(x.ToString());
                    mPort.Write((1 << y).ToString("X2"));
                    Thread.Sleep(10);

                    Thread.Sleep(50);
                    Application.DoEvents();

                }
            }
            catch {
                DisconnectSerial();
            }
        }
    }
}