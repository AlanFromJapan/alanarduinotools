using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Configuration;

namespace HomeNetworkRemoteControl {
    public partial class FrmLogon : Form {
        public FrmLogon() {
            InitializeComponent();
        }

        public string mDomain = null;
        public string mLogin = null;
        public string mPass = null;

        private void button1_Click(object sender, EventArgs e) {
            this.mDomain = txbMachine.Text;
            this.mLogin = txbLogin.Text;
            this.mPass = txbpass.Text;
        }

        private void FrmLogon_Load(object sender, EventArgs e) {
            this.txbMachine.Text = ConfigurationManager.AppSettings["Domain"];
            this.txbLogin.Text = ConfigurationManager.AppSettings["Login"];
            this.txbpass.Text = ConfigurationManager.AppSettings["Password"];
        }
    }
}