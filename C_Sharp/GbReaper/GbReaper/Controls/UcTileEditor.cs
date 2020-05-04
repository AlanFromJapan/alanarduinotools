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
        public const int TILE_SIZE = 32;
        private Tile mCurrentTile = null;
        private bool mLastColorClickedWasLeft = true;

        private int mColorIndexLeft = 0;
        private int mColorIndexRight = 3;

        protected Color ColorLeft {
            get {
                if (this.mCurrentTile == null) {
                    return Palette.DEFAULT_PALETTE.mColors[mColorIndexLeft];
                }
                else {
                    return mCurrentTile.Palette.mColors[mColorIndexLeft];
                }
            }
            set {
                int vFoundIndex = -1;
                for (int i = 0;  i < this.mCurrentTile.Palette.mColors.Length; i++) {
                    vFoundIndex = (value == this.mCurrentTile.Palette.mColors[i] ? i : vFoundIndex);
                }
                if (vFoundIndex < 0) {
                    throw new Exception("Cannot find color in palette!");
                }
                else {
                    mColorIndexLeft = vFoundIndex;
                }
            }
        }

        protected Color ColorRight {
            get {
                if (this.mCurrentTile == null) {
                    return Palette.DEFAULT_PALETTE.mColors[mColorIndexRight];
                }
                else {
                    return mCurrentTile.Palette.mColors[mColorIndexRight];
                }
            }
            set {
                int vFoundIndex = -1;
                for (int i = 0; i < this.mCurrentTile.Palette.mColors.Length; i++) {
                    vFoundIndex = (value == this.mCurrentTile.Palette.mColors[i] ? i : vFoundIndex);
                }
                if (vFoundIndex < 0) {
                    throw new Exception("Cannot find color in palette!");
                }
                else {
                    mColorIndexRight = vFoundIndex;
                }
            }
        }

        public UcTileEditor() {
            InitializeComponent();
        }

        private void UcTileEditor_Load(object sender, EventArgs e) {
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);

            btnLeft.BackColor = ColorLeft;
            btnRight.BackColor = ColorRight;

            panEdit.Paint += new PaintEventHandler(panEdit_Paint);
            pan8.Paint += new PaintEventHandler(pan8_Paint);
            pan16.Paint += new PaintEventHandler(pan16_Paint);
            pan32.Paint += new PaintEventHandler(pan32_Paint);
            pan64.Paint += new PaintEventHandler(pan64_Paint);
            pan128.Paint += new PaintEventHandler(pan128_Paint);
            pan128Alt.Paint += new PaintEventHandler(pan128Alt_Paint);
            panEdit.MouseMove += new MouseEventHandler(panEdit_MouseMove);
            panEdit.MouseDown += new MouseEventHandler(panEdit_MouseDown);
        }

        void panEdit_MouseDown(object sender, MouseEventArgs e) {
            DoMousePaint(e);
        }

        void panEdit_MouseMove(object sender, MouseEventArgs e) {
            DoMousePaint(e);
        }

        private void DoMousePaint(MouseEventArgs e) {
            if (this.mCurrentTile == null)
                return;

            if (e.Button == System.Windows.Forms.MouseButtons.Left || e.Button == System.Windows.Forms.MouseButtons.Right) {
                Rectangle vSelected = new Rectangle(
                    TILE_SIZE * ((e.X) / TILE_SIZE),
                    TILE_SIZE * ((e.Y) / TILE_SIZE),
                    TILE_SIZE,
                    TILE_SIZE);


                this.mCurrentTile.SetPixel(
                    vSelected.X / TILE_SIZE,
                    vSelected.Y / TILE_SIZE,
                    (e.Button == System.Windows.Forms.MouseButtons.Left ? this.ColorLeft : this.ColorRight));
            }
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
            DrawingLogic.DrawGrid(e.Graphics, new Rectangle(new Point(0, 0), ((Control)sender).Size), Pens.DarkGray, 8, 8);
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
                //unplug previous event handler
                this.mCurrentTile.TileChanged -= new Tile.TileChangeDelegate(CurrentTile_TileChanged);
            }

            this.mCurrentTile = pS;
            this.mCurrentTile.TileChanged -= new Tile.TileChangeDelegate(CurrentTile_TileChanged);
            this.mCurrentTile.TileChanged += new Tile.TileChangeDelegate(CurrentTile_TileChanged);
            this.Refresh();
        }

        void CurrentTile_TileChanged(Tile pTile) {
            this.Invalidate();
            this.Refresh();
        }

        private void ctxColors_Opening(object sender, CancelEventArgs e) {
            Palette vPal = (this.mCurrentTile == null ? Palette.DEFAULT_PALETTE : this.mCurrentTile.Palette);

            color0ToolStripMenuItem.BackColor = vPal.mColors[0];
            color1ToolStripMenuItem.BackColor = vPal.mColors[1];
            color2ToolStripMenuItem.BackColor = vPal.mColors[2];
            color3ToolStripMenuItem.BackColor = vPal.mColors[3];
        }

        private void btnLeft_MouseUp(object sender, MouseEventArgs e) {
            mLastColorClickedWasLeft = true;
            ctxColors.Show((Control)sender, e.Location);
        }

        private void btnRight_MouseUp(object sender, MouseEventArgs e) {
            mLastColorClickedWasLeft = false;
            ctxColors.Show((Control)sender, e.Location);
        }

        private void ctxColors_ItemClicked(object sender, ToolStripItemClickedEventArgs e) {
            if (mLastColorClickedWasLeft) {
                btnLeft.BackColor = e.ClickedItem.BackColor;
                ColorLeft = e.ClickedItem.BackColor;
            }
            else {
                btnRight.BackColor = e.ClickedItem.BackColor;
                ColorRight = e.ClickedItem.BackColor;
            
            }
        }

        
    }
}
