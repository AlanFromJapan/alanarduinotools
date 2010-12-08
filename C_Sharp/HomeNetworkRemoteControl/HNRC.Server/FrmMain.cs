using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using HNRC.Remoting.Impl;
using System.Configuration;
using HNRC.Remoting.Common;

namespace HNRC.Server {
    public partial class FrmMain : Form {
        public FrmMain() {
            InitializeComponent();
        }


        protected IRemoteControlCapable mServer = null;

        private void FrmMain_Load(object sender, EventArgs e) {
            RemoteControlImpl.ExposeByRemoting(Convert.ToInt32(ConfigurationManager.AppSettings["port"]));
            //mServer = RemoteHelper.GetReferenceToServer("tcp://localhost:" + ConfigurationManager.AppSettings["port"] + "/RemoteControlEndpoint");

            this.WindowState = FormWindowState.Minimized;
            this.Hide();
        }

        private void nic_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            Show();
            WindowState = FormWindowState.Normal;

        }

        private void FrmMain_Resize(object sender, EventArgs e) {
            if (FormWindowState.Minimized == this.WindowState)
                Hide();

        }

        private void button1_Click(object sender, EventArgs e) {
            this.mServer.Mute();
        }
    }
}