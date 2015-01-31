using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using GbReaper.Classes;

namespace GbReaper.Controls {
    public partial class UcTileEditor : UserControl {
        private Tile mCurrentTile = null;

        public UcTileEditor() {
            InitializeComponent();
        }

        private void UcTileEditor_Load(object sender, EventArgs e) {
            panEdit.Paint += new PaintEventHandler(panEdit_Paint);
            pan8.Paint += new PaintEventHandler(pan8_Paint);
            pan16.Paint += new PaintEventHandler(pan16_Paint);
            pan32.Paint += new PaintEventHandler(pan32_Paint);
            pan64.Paint += new PaintEventHandler(pan64_Paint);
            pan128.Paint += new PaintEventHandler(pan128_Paint);
            pan128Alt.Paint += new PaintEventHandler(pan128Alt_Paint);
        }

        void pan128Alt_Paint(object sender, PaintEventArgs e) {
            ScaledAndTiledPaintBackground(e, new Rectangle(new Point(0, 0), ((Control)sender).Size), 8);
        }

        void pan128_Paint(object sender, PaintEventArgs e) {
            ScaledAndTiledPaintBackground(e, new Rectangle(new Point(0, 0), ((Control)sender).Size), 4);
        }

        private void ScaledAndTiledPaintBackground(PaintEventArgs e, Rectangle pR, int pRepeat) {
            DrawingLogic.ScaledAndTiledPaintBackground(
                (this.mCurrentTile == null ? null : this.mCurrentTile.Image), 
                e, 
                pR, 
                pRepeat
            );
        }

        void pan64_Paint(object sender, PaintEventArgs e) {
            ScalePaintBackground(e, new Rectangle(new Point(0, 0), ((Control)sender).Size));
        }

        void pan32_Paint(object sender, PaintEventArgs e) {
            ScalePaintBackground(e, new Rectangle(new Point(0, 0), ((Control)sender).Size));
        }

        void pan16_Paint(object sender, PaintEventArgs e) {
            ScalePaintBackground(e, new Rectangle(new Point(0, 0), ((Control)sender).Size));
        }

        void pan8_Paint(object sender, PaintEventArgs e) {
            ScalePaintBackground(e, new Rectangle(new Point(0, 0), ((Control)sender).Size));
        }

        void panEdit_Paint(object sender, PaintEventArgs e) {
            ScalePaintBackground(e, new Rectangle(new Point(0,0),((Control)sender).Size));
            DrawingLogic.DrawGrid(e, new Rectangle(new Point(0, 0), ((Control)sender).Size), Pens.DarkGray, 8, 8);
        }



        private void ScalePaintBackground(PaintEventArgs e, Rectangle pR) {
            DrawingLogic.ScalePaintBackground(
                (this.mCurrentTile == null ? null : this.mCurrentTile.Image),
                e,
                pR
            );
        }

        public void SetTile(Tile pS) {
            if (this.mCurrentTile != null) { 
                //nothing
            }

            this.mCurrentTile = pS;
            this.Refresh();
        }

        
    }
}
