using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.IO.Ports;
using ZedGraph;

namespace ARD_AnalogView
{
    public partial class Form1 : Form
    {      
        // Starting time in milliseconds
        private int tickStart = 0;
        private RunningAverage mRunAvg = new RunningAverage(15);
        private bool mNormalize5v = true;

        private SerialPort mPort = null;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            mPort = new SerialPort("COM18");
            mPort.BaudRate = 115200;
            mPort.DataBits = 8;
            mPort.ReadTimeout = 500;

            mPort.Open();

            #region Setup zedgraph
            //http://zedgraph.org/wiki/index.php?title=Display_Dynamic_or_Real-Time_Data
            GraphPane myPane = zgGraph.GraphPane;
            myPane.Title.Text = "Analog data";
            myPane.XAxis.Title.Text = "Time, Seconds";
            myPane.YAxis.Title.Text = "Input";

            // Save 1200 points.  At 50 ms sample rate, this is one minute
            // The RollingPointPairList is an efficient storage class that always
            // keeps a rolling set of point data without needing to shift any data values
            RollingPointPairList list = new RollingPointPairList(1200);

            RollingPointPairList listAvg = new RollingPointPairList(1200);

            // Initially, a curve is added with no data points (list is empty)
            // Color is blue, and there will be no symbols
            LineItem curve = myPane.AddCurve("Input", list, Color.Blue, SymbolType.None);

            LineItem curveAvg = myPane.AddCurve("Average", listAvg, Color.Red, SymbolType.None);

            // Just manually control the X axis range so it scrolls continuously
            // instead of discrete step-sized jumps
            myPane.XAxis.Scale.Min = 0;
            myPane.XAxis.Scale.Max = 30;
            myPane.XAxis.Scale.MinorStep = 1;
            myPane.XAxis.Scale.MajorStep = 5;

            // Scale the axes
            zgGraph.AxisChange();

            // Save the beginning time for reference
            tickStart = Environment.TickCount;

            #endregion

            ThreadPool.QueueUserWorkItem(new WaitCallback(this.ReadSerial));
        }

        private void ReadSerial(object o) {
            while (true) {
                if (mPort.IsOpen && mPort.BytesToRead > 0)
                {
                    int vVal = Convert.ToInt32(mPort.ReadLine());

                    try
                    {
                        this.Invoke(new WaitCallback(this.UpdateValue), vVal);
                    }
                    catch { }
                }
                Thread.Sleep(5);
            }
        }

        private void UpdateValue(object o)
        {

            double v = (double)(int)o;
            if (mNormalize5v) {
                v = (1024.0 - v) * 5.0 / 1024.0;
            }
            lblValue.Text = v.ToString("0.##");

            int vMax = (this.mNormalize5v ? 5 : 1024);

            panIn.Height = (int)((vMax - v) * panColor.Height / vMax);

            mRunAvg.NewSample((float)v);

            #region Zedgraph display
            //http://zedgraph.org/wiki/index.php?title=Display_Dynamic_or_Real-Time_Data
            // Make sure that the curvelist has at least one curve
            if (zgGraph.GraphPane.CurveList.Count <= 0)
                return;

            // Get the first CurveItem in the graph
            LineItem curve = zgGraph.GraphPane.CurveList[0] as LineItem;
            if (curve == null)
                return;

            // Get the PointPairList
            IPointListEdit list = curve.Points as IPointListEdit;
            // If this is null, it means the reference at curve.Points does not
            // support IPointListEdit, so we won't be able to modify it
            if (list == null)
                return;

            // Time is measured in seconds
            double time = (Environment.TickCount - tickStart) / 1000.0;

            // 3 seconds per cycle
            list.Add(time, v);

            (zgGraph.GraphPane.CurveList[1].Points as IPointListEdit).Add(time, this.mRunAvg.Average);

            // Keep the X scale at a rolling 30 second interval, with one
            // major step between the max X value and the end of the axis
            Scale xScale = zgGraph.GraphPane.XAxis.Scale;
            if (time > xScale.Max - xScale.MajorStep)
            {
                xScale.Max = time + xScale.MajorStep;
                xScale.Min = xScale.Max - 30.0;
            }

            // Make sure the Y axis is rescaled to accommodate actual data
            zgGraph.AxisChange();
            // Force a redraw
            zgGraph.Invalidate();

            #endregion
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            ToolStripButton vTSB = (ToolStripButton)sender;
            vTSB.Checked = !vTSB.Checked;
            zgGraph.GraphPane.CurveList[1].IsVisible = vTSB.Checked;

        }

        private void toolStripButton2_Click(object sender, EventArgs e) {
            ToolStripButton vTSB = (ToolStripButton)sender;
            vTSB.Checked = !vTSB.Checked;
            SetNormalize5V(vTSB.Checked);
        }

        private void SetNormalize5V(bool p) {
            mNormalize5v = p;
        }
    }
}