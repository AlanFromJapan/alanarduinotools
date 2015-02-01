using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using GbReaper.Classes;

namespace GbReaper.Forms {
    public partial class FrmTilizator : Form {
        public Bitmap CreateBmp { get { return (Bitmap)picPreviewA.Image; } }
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
                GeneratePreview();

            }
        }

        private void GeneratePreview() {

            Bitmap vBmp = new Bitmap(txbImage.Text);
            picPreviewA.Image = vBmp;

            Bitmap vBmpResized = DrawingLogic.CopyAndResize(vBmp, Tile.WIDTH_PX * this.CreateWidth, Tile.HEIGHT_PX * this.CreateHeight);
            picPreviewB.Image = vBmpResized;

            Bitmap vBmpColorized = DrawingLogic.MapBitmapColorsToPalette((Bitmap)vBmpResized.Clone(), Palette.DEFAULT_PALETTE);
            picPreviewC.Image = vBmpColorized;
        }

        private void FrmTilizator_Load(object sender, EventArgs e) {
            ucPaletteEditor1.CurrentPalette = Palette.DEFAULT_PALETTE;
        }

        private void undWidth_ValueChanged(object sender, EventArgs e) {
            GeneratePreview();
        }

        private void undHeight_ValueChanged(object sender, EventArgs e) {
            GeneratePreview();
        }
    }
}
