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
    public partial class UcSpriteEditor : UserControl {
        private Sprite mCurrentSprite = null;

        public UcSpriteEditor() {
            InitializeComponent();
        }

        private void UcSpriteEditor_Load(object sender, EventArgs e) {
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
                (this.mCurrentSprite == null ? null : this.mCurrentSprite.Image), 
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
            DrawingLogic.DrawGrid(e, new Rectangle(new Point(0, 0), ((Control)sender).Size));
        }



        private void ScalePaintBackground(PaintEventArgs e, Rectangle pR) {
            DrawingLogic.ScalePaintBackground(
                (this.mCurrentSprite == null ? null : this.mCurrentSprite.Image),
                e,
                pR
            );
        }

        public void SetSprite(Sprite pS) {
            if (this.mCurrentSprite != null) { 
                //nothing
            }

            this.mCurrentSprite = pS;
            this.Refresh();
        }

        
    }
}
