using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using GbReaper.Classes;
using GbReaper.Forms;

namespace GbReaper.Controls {
    public partial class UcRomViewer : UserControl {

        protected Image mImage = null;
        protected int mZoomfactor = 1;
        protected Point mMouseHover = Point.Empty;
        protected Image mSelectedTile = null;

        public Image SelectedTile { get { return this.mSelectedTile; } }

        public delegate void RomTileSelectDelegate(Image pImage);
        public delegate void RomTileSelectMulitpleDelegate(IList<Image> pImages);
        public event RomTileSelectDelegate RomTileSelected;
        public event RomTileSelectMulitpleDelegate RomTileSelectedMultiple;
        public event RomTileSelectDelegate RomTileViewed;

        protected Rectangle SourceRectangle {
            get {
                Rectangle vSource = new Rectangle(
                    hbar.Value + Tile.WIDTH_PX * this.mZoomfactor * (this.mMouseHover.X / (Tile.WIDTH_PX * this.mZoomfactor)) - (hbar.Value % (Tile.WIDTH_PX * this.mZoomfactor)),
                    vbar.Value + Tile.HEIGHT_PX * this.mZoomfactor * (this.mMouseHover.Y / (Tile.HEIGHT_PX * this.mZoomfactor)) - (vbar.Value % (Tile.HEIGHT_PX * this.mZoomfactor)),
                    Tile.WIDTH_PX * this.mZoomfactor,
                    Tile.HEIGHT_PX * this.mZoomfactor
                    );

                return vSource;
            }
        }

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


            DrawingLogic.SetGraphicsNoInterpol(e.Graphics);

            e.Graphics.DrawImage(this.mImage, 0, -vbar.Value);//, e.ClipRectangle.Width - vbar.Width, e.ClipRectangle.Height - hbar.Height);

            if (!this.mMouseHover.IsEmpty) {
                Rectangle vRed = new Rectangle(SourceRectangle.Location, SourceRectangle.Size);
                vRed.Offset(0, -vbar.Value);
                e.Graphics.DrawRectangle(
                    Pens.Red,
                    vRed
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
            this.mMouseHover = e.Location;// this.PointToClient(Cursor.Position);


            this.Invalidate();
        }


        public void SetImage(Image pImage, int pZoomfactor) {
            this.mImage = pImage;
            this.mZoomfactor = pZoomfactor;

            this.UpdateSidebars();
            this.Invalidate();
        }

        protected void OnRomTileSelected(Image pImg) {
            if (this.RomTileSelected != null) {
                this.RomTileSelected(pImg);
            }
        }

        protected void OnRomTileViewed(Image pImg) {
            if (this.RomTileViewed != null) {
                this.RomTileViewed(pImg);
            }
        }

        protected void OnRomTileSelectedMultiple(IList<Image> pImages) {
            if (this.RomTileSelectedMultiple != null) {
                this.RomTileSelectedMultiple(pImages);
            }
        }

        protected override void OnMouseUp(MouseEventArgs e) {
            base.OnMouseUp(e);

            Bitmap vBmpTile = GetSelectedBitmap8x8();
            if (vBmpTile == null)
                return;

            this.mSelectedTile = vBmpTile;

            OnRomTileViewed(vBmpTile);
        }

        protected override void OnDoubleClick(EventArgs e) {
            base.OnDoubleClick(e);

            if (!this.mMouseHover.IsEmpty && this.mImage != null) {
                if (ModifierKeys == Keys.None) {
                    //pick ONE
                    Bitmap vBmpTile = GetSelectedBitmap8x8();

                    this.OnRomTileSelected(vBmpTile);
                }
                else if (ModifierKeys == Keys.Control) {
                    //Pick 4 in a row
                    IList<Image> vList = GetSelectedNBitmaps(4);

                    this.OnRomTileSelectedMultiple(vList);

                }
                else if (ModifierKeys == Keys.Shift) {
                    //Pick n in a row
                    using (FrmNumericInput vFrm = new FrmNumericInput()) {
                        if (DialogResult.OK == vFrm.ShowDialog(this.ParentForm)) {
                            IList<Image> vList = GetSelectedNBitmaps(vFrm.Count);
                            this.OnRomTileSelectedMultiple(vList);
                        }
                    }
                }
            }
        }


        private IList<Image> GetSelectedNBitmaps(int pCount) {
            List<Image> vList = new List<Image>();
            if (this.mImage == null)
                return null;

            Rectangle vSource = SourceRectangle;

            for (int i = 0; i < pCount; i++) {
                Bitmap vBmpTileZoomed = ((Bitmap)this.mImage).Clone(vSource, ((Bitmap)this.mImage).PixelFormat);

                if (this.mZoomfactor != 1) {
                    Bitmap vBmp8x8 = new Bitmap(Tile.WIDTH_PX, Tile.HEIGHT_PX);

                    using (Graphics vG = Graphics.FromImage(vBmp8x8)) {
                        DrawingLogic.SetGraphicsNoInterpol(vG);


                        vG.DrawImage(vBmpTileZoomed, 0, 0, vBmp8x8.Width, vBmp8x8.Height);
                    }

                    vBmpTileZoomed.Dispose();
                    vBmpTileZoomed = vBmp8x8;
                }

                vList.Add(vBmpTileZoomed);

                //now move to NEXT bitmap
                vSource.X += 8 * this.mZoomfactor;
                if (vSource.X >= this.mImage.Width) {
                    vSource.X = 0;
                    vSource.Y += 8 * this.mZoomfactor;
                }
            }

            return vList;
        }
        private Bitmap GetSelectedBitmap8x8() {
            if (this.mImage == null)
                return null;

            Rectangle vSource = SourceRectangle;
            Bitmap vBmpTileZoomed = ((Bitmap)this.mImage).Clone(vSource, ((Bitmap)this.mImage).PixelFormat);

            if (this.mZoomfactor == 1) {
                return vBmpTileZoomed;
            }
            else {
                Bitmap vBmp8x8 = new Bitmap(Tile.WIDTH_PX, Tile.HEIGHT_PX);

                using (Graphics vG = Graphics.FromImage(vBmp8x8)) {
                    DrawingLogic.SetGraphicsNoInterpol(vG);


                    vG.DrawImage(vBmpTileZoomed, 0, 0, vBmp8x8.Width, vBmp8x8.Height);
                }

                return vBmp8x8;
            }

        }

        private void UcRomViewer_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.PageDown) {
                vbar.Value += 64;
            }
            else if (e.KeyCode == Keys.PageUp) {
                vbar.Value = Math.Max(0, vbar.Value - 64);
            }

            this.Invalidate();
        }
    }
}
