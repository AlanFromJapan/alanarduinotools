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
    public partial class UcRomViewer : UserControl {

        protected Image mImage = null;
        protected int mZoomfactor = 1;
        protected Point mMouseHover = Point.Empty;

        public delegate void RomSpriteSelectDelegate(Image pImage);
        public event RomSpriteSelectDelegate RomSpriteSelected;

        public UcRomViewer() {
            InitializeComponent();
        }

        private void vbar_Scroll(object sender, ScrollEventArgs e) {
            this.Invalidate();
        }
        private void hbar_Scroll(object sender, ScrollEventArgs e) {
            this.Invalidate();
        }

        private void UcRomViewer_Load(object sender, EventArgs e) {

        }

        protected override void OnPaint(PaintEventArgs e) {
            base.OnPaint(e);

            if (mImage == null)
                return;

            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            
            e.Graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            e.Graphics.DrawImage(this.mImage, -hbar.Value, -vbar.Value);//, e.ClipRectangle.Width - vbar.Width, e.ClipRectangle.Height - hbar.Height);

            if (!this.mMouseHover.IsEmpty) {
                e.Graphics.DrawRectangle(
                    Pens.Red,
                    Sprite.WIDTH_PX * this.mZoomfactor * (this.mMouseHover.X / (Sprite.WIDTH_PX * this.mZoomfactor)) - (hbar.Value % (Sprite.WIDTH_PX * this.mZoomfactor)),
                    Sprite.HEIGHT_PX * this.mZoomfactor * (this.mMouseHover.Y / (Sprite.HEIGHT_PX * this.mZoomfactor)) - (vbar.Value % (Sprite.HEIGHT_PX * this.mZoomfactor)), 
                    Sprite.WIDTH_PX * this.mZoomfactor,
                    Sprite.HEIGHT_PX * this.mZoomfactor
                    );
            }
        }

        private void UpdateSidebars() {
            if (mImage == null)
                return;

            vbar.Minimum = 0;
            if (this.mImage.Height < this.Height) {
                vbar.Maximum = 0;
                vbar.Value = 0;
            }
            else {
                vbar.Maximum = this.mImage.Height - this.Height + hbar.Height;
                if (vbar.Value > vbar.Maximum)
                    vbar.Value = vbar.Maximum;
            }

            hbar.Minimum = 0;
            hbar.Maximum = this.mImage.Width - this.Width + vbar.Width;
            if (hbar.Value > hbar.Maximum)
                hbar.Value = hbar.Maximum;

        }

        protected override void OnResize(EventArgs e) {
            base.OnResize(e);

            UpdateSidebars();

            this.Invalidate();
        }
        protected override void OnMouseMove(MouseEventArgs e) {
            base.OnMouseMove(e);
            this.mMouseHover = this.PointToClient(Cursor.Position);


            this.Invalidate();
        }


        public void SetImage(Image pImage, int pZoomfactor) {
            this.mImage = pImage;
            this.mZoomfactor = pZoomfactor;

            this.UpdateSidebars();
            this.Invalidate();
        }

        protected void OnRomSpriteSelected(Image pImg) {
            if (this.RomSpriteSelected != null) {
                this.RomSpriteSelected(pImg);
            }
        }

        protected override void OnDoubleClick(EventArgs e) {
            base.OnDoubleClick(e);

            if (!this.mMouseHover.IsEmpty && this.mImage != null) {
                Rectangle vSource= new Rectangle(
                    hbar.Value + Sprite.WIDTH_PX * this.mZoomfactor * (this.mMouseHover.X / (Sprite.WIDTH_PX * this.mZoomfactor)) - (hbar.Value % (Sprite.WIDTH_PX * this.mZoomfactor)),
                    vbar.Value + Sprite.HEIGHT_PX * this.mZoomfactor * (this.mMouseHover.Y / (Sprite.HEIGHT_PX * this.mZoomfactor)) - (vbar.Value % (Sprite.HEIGHT_PX * this.mZoomfactor)),
                    Sprite.WIDTH_PX * this.mZoomfactor,
                    Sprite.HEIGHT_PX * this.mZoomfactor
                    );
                Bitmap vBmpSprite = ((Bitmap)this.mImage).Clone(vSource, ((Bitmap)this.mImage).PixelFormat);

                this.OnRomSpriteSelected(vBmpSprite);
            }
        }
    }
}
