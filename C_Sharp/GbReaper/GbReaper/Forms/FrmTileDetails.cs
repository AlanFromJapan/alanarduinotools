using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace GbReaper.Forms
{
    public partial class FrmTileDetails : Form {
        public FrmTileDetails() {
            InitializeComponent();
        }

        public String TileName {
            get {
                return txbName.Text;
            }
            set { 
                txbName.Text = value;
            }
        }

        private void FrmTileDetails_Load(object sender, EventArgs e) {
            txbName.Focus();
        }
    }
}
