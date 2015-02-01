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
    public partial class UcMapEditor : UserControl {
        public const int TILE_SIZE = Tile.HEIGHT_PX * 3;

        protected bool mShowGrid = true;

        protected Map mCurrentMap = null;
        public Map CurrentMap { get { return this.mCurrentMap; } }
        protected Tile mCurrentTile = null;
        public Tile CurrentTile {
            get { return this.mCurrentTile; }
            set { this.mCurrentTile = value; }
        }
        protected Rectangle GridBorders {
            get {
                return new Rectangle(
                ((panMap.Width - TILE_SIZE * this.mCurrentMap.Width) / 2),
                ((panMap.Height - TILE_SIZE * this.mCurrentMap.Height) / 2),
                TILE_SIZE * this.mCurrentMap.Width,
                TILE_SIZE * this.mCurrentMap.Height);
            }
        }


        public UcMapEditor() {
            InitializeComponent();

            panMap.Paint += new PaintEventHandler(panMap_Paint);
            panMap.MouseDown += new MouseEventHandler(panMap_MouseDown);
            panMap.MouseMove += new MouseEventHandler(panMap_MouseMove);
        }

        void panMap_MouseMove(object sender, MouseEventArgs e) {
            MousePaintCell(e);
                        
        }

        private void MousePaintCell(MouseEventArgs e) {
            if (this.mCurrentTile == null || this.mCurrentMap == null)
                return;

            if (e.Button == System.Windows.Forms.MouseButtons.Left || e.Button == System.Windows.Forms.MouseButtons.Right) {
                Rectangle vBorders = GridBorders;

                Point vP = new Point(
                    (e.X - vBorders.X) / TILE_SIZE,
                    (e.Y - vBorders.Y) / TILE_SIZE
                    );

                this.mCurrentMap.SetTile(this.mCurrentTile, vP.X, vP.Y);
            }
        }

        void panMap_MouseDown(object sender, MouseEventArgs e) {
            MousePaintCell(e);
        }

        void panMap_Paint(object sender, PaintEventArgs e) {
            if (this.mCurrentMap == null) {
                e.Graphics.DrawString("no map selected", this.panMap.Font, Brushes.Red, e.ClipRectangle, StringFormat.GenericDefault);
                return;
            }

            //make the grid
            int vMaxX = e.ClipRectangle.Width / TILE_SIZE;
            int vMaxY = e.ClipRectangle.Height / TILE_SIZE;

            //Get the limits
            Rectangle vBorders = GridBorders;

            //draw grid
            if (mShowGrid) {
                DrawingLogic.DrawGrid(e, vBorders, Pens.LightGray, this.mCurrentMap.Width, this.mCurrentMap.Height);
            }

            //draw the external borders
            e.Graphics.DrawRectangle(Pens.Turquoise, vBorders);

            //if (mSelectedPoint != Point.Empty && vBorders.Contains(mSelectedPoint)) {
            //    Rectangle vSelected = new Rectangle(
            //        vBorders.X + TILE_SIZE * ((mSelectedPoint.X - vBorders.X) / TILE_SIZE) ,
            //        vBorders.Y + TILE_SIZE * ((mSelectedPoint.Y - vBorders.Y) / TILE_SIZE),
            //        TILE_SIZE,
            //        TILE_SIZE);
            //    e.Graphics.DrawRectangle(Pens.Red, vSelected);

            //}


            //Draw content at last
            DrawingLogic.SetGraphicsNoInterpol(e.Graphics);
            for (int x = 0; x < this.mCurrentMap.Width; x++) { 
                for (int y = 0; y < this.mCurrentMap.Height; y++) {
                    Tile vT = this.mCurrentMap[x, y];

                    if (vT != null) {
                        e.Graphics.DrawImage(vT.Image,
                            vBorders.X + x * TILE_SIZE,
                            vBorders.Y + y * TILE_SIZE,
                            TILE_SIZE,
                            TILE_SIZE
                            );
                    }
                } 
            }
        }

        private void btnNew_Click(object sender, EventArgs e) {
            using (FrmNewMap vFrm = new FrmNewMap()) {
                if (DialogResult.OK == vFrm.ShowDialog(this)) {
                    if (this.mCurrentMap != null) {
                        this.mCurrentMap.MapChanged -= new EventHandler(CurrentMap_MapChanged);
                    }

                    this.mCurrentMap = new Map(vFrm.CreateWidth, vFrm.CreateHeight);
                    this.mCurrentMap.MapChanged -= new EventHandler(CurrentMap_MapChanged);
                    this.mCurrentMap.MapChanged += new EventHandler(CurrentMap_MapChanged);
                    
                    this.Invalidate();
                    //this.Refresh();
                }
            }
        }

        void CurrentMap_MapChanged(object sender, EventArgs e) {
            this.Invalidate();
            //this.Refresh();
        }

        private void btnGrid_Click(object sender, EventArgs e) {
            mShowGrid = !mShowGrid;
            this.panMap.Invalidate();
        }

    }
}
