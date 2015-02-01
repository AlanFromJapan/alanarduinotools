using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace GbReaper.Forms {
    public partial class FrmTilizator : Form {
        public Bitmap CreateBmp { get { return (Bitmap)picPreview.Image; } }
        public int CreateWidth { get { return (int)undWidth.Value; } }
        public int CreateHeight { get { return (int)undHeight.Value; } }
        public int CreateTop { get { return (int)undTop.Value; } }
        public int CreateLeft { get { return (int)undLeft.Value; } }

        public FrmTilizator() {
            InitializeComponent();
        }

        private void btnCancel_Click(object sender, EventArgs e) {
            this.DialogResult = DialogResult.Cancel;
        }

        private void btnCreate_Click(object sender, EventArgs e) {
            this.DialogResult = DialogResult.OK;
        }

        private void btnOpenImage_Click(object sender, EventArgs e) {
            if (ofdBitmap.ShowDialog(this) == DialogResult.OK) {
                txbImage.Text = ofdBitmap.FileName;

                Bitmap vBmp = new Bitmap(txbImage.Text);
                picPreview.Image = vBmp;
            }
        }
    }
}
