using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace Arduino_ReadSerial
{
    public partial class Form1 : Form
    {
        private SerialPort mPort = null;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            mPort = new SerialPort("COM18");
            mPort.BaudRate = 9600;
            mPort.DataBits = 8;

            mPort.Open();

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (mPort.BytesToRead > 0)
            {
                label1.Text = mPort.ReadExisting();
            }
        }
    }
}