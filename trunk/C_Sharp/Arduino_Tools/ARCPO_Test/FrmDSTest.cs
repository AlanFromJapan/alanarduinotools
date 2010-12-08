using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Text.RegularExpressions;
using System.Threading;

namespace Arduino_Tools
{
    public partial class FrmDSTest : Form
    {
        public FrmDSTest()
        {
            InitializeComponent();
        }

        private Point mLastPoint = new Point(0, 0);
        private Point mBottomRight = new Point(220, 180);

        private Rectangle mRAWRectangle = new Rectangle();

        private Bitmap vBmp = new Bitmap(1100, 1100);
        private Graphics mG = null;
        private SerialPort mPort = null;
        private bool mCalibrating = false;
        private bool mCalibrated = false;

        private Regex mRegex = new Regex(@"(?<x>[^,]+),(?<y>\d+)", RegexOptions.IgnoreCase);

        private void FrmDSTest_Load(object sender, EventArgs e)
        {
            
            mG = Graphics.FromImage(vBmp);
            mG.FillRectangle(Brushes.White, 0, 0, vBmp.Width, vBmp.Height);
            pictureBox1.Image = vBmp;



            mPort = new SerialPort("COM16");
            mPort.BaudRate = 9600;
            mPort.DataBits = 8;
            mPort.DiscardNull = false;
            mPort.ReadTimeout = 1000;

            mPort.Open();

            ThreadPool.QueueUserWorkItem(new WaitCallback(ReadSerial));
        }

        private void ReadSerial(object pO){
            try
            {
                while (true)
                {
                    if (!mCalibrating)
                    {
                        while (mPort.BytesToRead > 0)
                        {
                            string vs = mPort.ReadLine();

                            if (vs != string.Empty)
                            {
                                this.Invoke(new WaitCallback(this.ProcessSerialData), vs);
                            }
                        }
                    }

                    Thread.Sleep(100);
                }
            }
            catch { }

        }


        private void ProcessSerialData(object pO)
        {

                string vs = pO.ToString();

                if (vs != string.Empty)
                {
                    txbLog.Text = vs + "\r\n" + txbLog.Text;

                    if (mRegex.IsMatch(vs))
                    {
                        int vX = Convert.ToInt32(mRegex.Match(vs).Result("${x}"));
                        int vY = Convert.ToInt32(mRegex.Match(vs).Result("${y}"));

                        if (vX != 0 && vY != 0)
                        {
                            //vBmp.SetPixel(vX, vY, Color.Black);


                            if (this.mCalibrated)
                            {
                                Point vP = Calibrate(new Point(vX, vY));

                                mG.FillEllipse(Brushes.Red, vP.X, vP.Y, 4, 4);
                            }
                            else
                            {
                                Point vNewPoint = new Point(vX, vY);
                                //raw output
                                mG.FillEllipse(Brushes.Black, vX, vY, 4, 4);

                                if (mLastPoint.X != 0 && mLastPoint.Y != 0) {
                                    //mG.DrawLine(Pens.Black, mLastPoint, vNewPoint);
                                    using (Pen vP = new Pen(Color.Black, 4)) {
                                        mG.DrawLine(vP, mLastPoint, vNewPoint);
                                    }
                                }

                                mLastPoint = vNewPoint;
                            }

                            pictureBox1.Refresh();
                        }
                        else {
                            mLastPoint = new Point(0, 0);
                        }
                    }



                }


        }

        private Point Calibrate(Point pOriginal)
        {
            Point vP = new Point();

            double vDistMax =
                Math.Sqrt(Math.Pow(Math.Abs(mBottomRight.X), 2)
                    + Math.Pow(Math.Abs(mBottomRight.Y), 2));

            double vDistBR =
                Math.Sqrt(Math.Pow(Math.Abs(pOriginal.X - mBottomRight.X), 2)
                    + Math.Pow(Math.Abs(pOriginal.Y - mBottomRight.Y), 2));

            double vDilatationFactor = 2.0;
            vP.X = (int)Math.Round(vDilatationFactor * (double)pOriginal.X * ((vDistMax - vDistBR) / vDistMax));
            vP.Y = (int)Math.Round(vDilatationFactor * (double)pOriginal.Y * ((vDistMax - vDistBR) / vDistMax));

            return vP;
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            
            this.mRAWRectangle.X = 0;
            this.mRAWRectangle.Y = 0;
            this.mRAWRectangle.Width = 300;
            this.mRAWRectangle.Height= 300;
            mCalibrated = !mCalibrated;
            toolStripButton1.Checked = !toolStripButton1.Checked;
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            mG.FillRectangle(Brushes.White, 0, 0, vBmp.Width, vBmp.Height);
            pictureBox1.Refresh();
        }

    }
}