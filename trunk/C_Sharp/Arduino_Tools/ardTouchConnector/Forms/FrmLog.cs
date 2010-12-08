using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ARCPO;

namespace ardTouchConnector
{
    public partial class FrmLog : Form
    {
        public FrmLog()
        {
            InitializeComponent();
        }


        /// <summary>
        /// Logs a message
        /// </summary>
        /// <param name="pTime"></param>
        /// <param name="pReceivedOrSent">true received, false sent</param>
        /// <param name="pPacket"></param>
        public void LogMessage(DateTime pTime, string pMessage)
        {
            txbLog.Text = pTime.ToLongTimeString() + " : " + pMessage + "\r\n" + txbLog.Text;
        }

        /// <summary>
        /// Logs a message
        /// </summary>
        /// <param name="pTime"></param>
        /// <param name="pReceivedOrSent">true received, false sent</param>
        /// <param name="pPacket"></param>
        public void LogMessage(DateTime pTime, bool pReceivedOrSent, ARCPO_Packet pPacket) {
            txbLog.Text = (pReceivedOrSent ? "[>I N] " : "[<OUT] ") + pTime.ToLongTimeString() + " : " + pPacket.ToString() + "\r\n" + txbLog.Text;
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            txbLog.Clear();
        }
    }
}