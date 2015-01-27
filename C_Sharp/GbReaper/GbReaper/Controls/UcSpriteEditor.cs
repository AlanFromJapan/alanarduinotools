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
            if (this.mCurrentSprite == null) {
                e.Graphics.FillRectangle(Brushes.White, pR);
                return;
            }

            e.Graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            e.Graphics.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.Half;
            e.Graphics.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;

            int vSide = e.ClipRectangle.Width / pRepeat;
            for (int x = 0; x < pRepeat; x++) {
                for (int y = 0; y < pRepeat; y++) {
                    e.Graphics.DrawImage(this.mCurrentSprite.Image, x * vSide, y * vSide, vSide, vSide);
                }
            }
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
            DrawGrid(e, new Rectangle(new Point(0, 0), ((Control)sender).Size));
        }

        private void DrawGrid(PaintEventArgs e, Rectangle pR) {
            
            for (int x = 0; x < 7; x++) {
                e.Graphics.DrawLine(Pens.Black, (x + 1) * pR.Width / 8, 0, (x + 1) * pR.Width / 8, pR.Height);
            }

            for (int y = 0; y < 7; y++) {
                e.Graphics.DrawLine(Pens.Black, 0, (y + 1) * pR.Height / 8, pR.Width, (y + 1) * pR.Height / 8);
            }
        }

        private void ScalePaintBackground(PaintEventArgs e, Rectangle pR) {
            if (this.mCurrentSprite == null) {
                e.Graphics.FillRectangle(Brushes.White, pR);
                return;
            }
            
            e.Graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            e.Graphics.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.Half;
            e.Graphics.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;

            e.Graphics.DrawImage(this.mCurrentSprite.Image, pR);
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
