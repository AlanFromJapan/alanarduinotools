using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.Remoting.Channels.Tcp;
using System.Runtime.Remoting.Channels;
using HNRC.Remoting.Common;
using System.Configuration;

namespace HomeNetworkRemoteControl {
    public partial class FrmMain : Form {

        protected IRemoteControlCapable mServer = null;

        /// <summary>
        /// Gets the server url
        /// </summary>
        protected string ServerLocation {
            get { 
                return ConfigurationManager.AppSettings["remoteServer"];
            }
        }

        public FrmMain() {
            InitializeComponent();
        }

        private void FrmMain_Load(object sender, EventArgs e) {
            using (FrmLogon vFrm = new FrmLogon()){
                vFrm.ShowDialog(this);
                this.mServer = RemoteHelper.GetReferenceToServer(this.ServerLocation, vFrm.mDomain, vFrm.mLogin, vFrm.mPass);
            }
            if (mServer == null) {
                ShowMessage("Not found (-_-)\r\n" + ServerLocation);
            }
            else {
                ShowMessage("Found (^o^)/\r\n" + ServerLocation);

            }
        }

        protected void ShowMessage(string pMessage) {
            lblInfo.Text = pMessage;
        }

        private void btnVolUp_Click(object sender, EventArgs e) {
            try {
                this.mServer.Volume(true);
                ShowMessage("Volume up (^o^)/");
            }
            catch (Exception ex) {
                ShowMessage("Error : " + ex.Message);
            }
        }

        private void btnVolLow_Click(object sender, EventArgs e) {
            try {
                this.mServer.Volume(false);
                ShowMessage("Volume down (-.-)");
            }
            catch (Exception ex) {
                ShowMessage("Error : " + ex.Message);
            }
        }

        private void btnVolMute_Click(object sender, EventArgs e) {
            try {
                this.mServer.Mute();
                ShowMessage("Mute (OxO)");
            }
            catch (Exception ex) {
                ShowMessage("Error : " + ex.Message);
            }
        }

        private void btnPause_Click(object sender, EventArgs e) {
            try {
                this.mServer.PlayPause();
                ShowMessage("Play pause X(~o~)X");
            }
            catch (Exception ex) {
                ShowMessage("Error : " + ex.Message);
            }
        }

        private void btnPrev_Click(object sender, EventArgs e) {
            try {
                this.mServer.Prev();
                ShowMessage("Previous << ~\\(~o~)b");
            }
            catch (Exception ex) {
                ShowMessage("Error : " + ex.Message);
            }
        }

        private void btnNext_Click(object sender, EventArgs e)
        {
            try {
                this.mServer.Next();
                ShowMessage("Next >> d(~o~)/~");
            }
            catch (Exception ex) {
                ShowMessage("Error : " + ex.Message);
            }        
        }

        private void nic_MouseDoubleClick(object sender, MouseEventArgs e) {
            ShowMainForm();
        }

        private void ShowMainForm() {
            Show();
            WindowState = FormWindowState.Normal;
        }

        private void FrmMain_Resize(object sender, EventArgs e) {
            if (FormWindowState.Minimized == WindowState)
                Hide();

        }

        private void playPauseToolStripMenuItem_Click(object sender, EventArgs e) {
            btnPause_Click(null, null);
        }

        private void showToolStripMenuItem_Click(object sender, EventArgs e) {
            ShowMainForm();
        }

        private void tsbShut_Click(object sender, EventArgs e) {
            if (DialogResult.Yes == MessageBox.Show("Are you SURE you want to shutdown remote PC ?", "Please confirm", MessageBoxButtons.YesNo, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2)) {
                try {
                    this.mServer.ShutdownPC();
                    ShowMessage("Good night PC ...");
                }
                catch (Exception ex) {
                    ShowMessage("Error : " + ex.Message);
                }     
            }
        }
    }
}