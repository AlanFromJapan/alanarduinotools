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

        protected Map mCurrentMap = null;
        public Map CurrentMap { get { return this.mCurrentMap; } }
        protected Point mSelectedPoint = Point.Empty;

        public UcMapEditor() {
            InitializeComponent();

            panMap.Paint += new PaintEventHandler(panMap_Paint);
            panMap.MouseDown += new MouseEventHandler(panMap_MouseDown);
        }

        void panMap_MouseDown(object sender, MouseEventArgs e) {
            
            mSelectedPoint = new Point(e.X, e.Y);
            panMap.Invalidate();
        }

        void panMap_Paint(object sender, PaintEventArgs e) {
            if (this.mCurrentMap == null) {
                e.Graphics.DrawString("no map selected", this.panMap.Font, Brushes.Red, e.ClipRectangle, StringFormat.GenericDefault);
                return;
            }

            //make the grid
            int vMaxX = e.ClipRectangle.Width / TILE_SIZE;
            int vMaxY = e.ClipRectangle.Height / TILE_SIZE;

            //draw the borders
            Rectangle vBorders = new Rectangle(
                ((e.ClipRectangle.Width - TILE_SIZE * this.mCurrentMap.Width) / 2),
                ((e.ClipRectangle.Height - TILE_SIZE * this.mCurrentMap.Height) / 2),
                TILE_SIZE * this.mCurrentMap.Width,
                TILE_SIZE * this.mCurrentMap.Height);
            e.Graphics.DrawRectangle(Pens.BlueViolet, vBorders);

            //draw grid
            DrawingLogic.DrawGrid(e, vBorders, Pens.GreenYellow, this.mCurrentMap.Width, this.mCurrentMap.Height);

            if (mSelectedPoint != Point.Empty && vBorders.Contains(mSelectedPoint)) {
                Rectangle vSelected = new Rectangle(
                    vBorders.X + TILE_SIZE * ((mSelectedPoint.X - vBorders.X) / TILE_SIZE) ,
                    vBorders.Y + TILE_SIZE * ((mSelectedPoint.Y - vBorders.Y) / TILE_SIZE),
                    TILE_SIZE,
                    TILE_SIZE);
                e.Graphics.DrawRectangle(Pens.Red, vSelected);

            }
        }

        private void btnNew_Click(object sender, EventArgs e) {
            using (FrmNewMap vFrm = new FrmNewMap()) {
                if (DialogResult.OK == vFrm.ShowDialog(this)) {
                    this.mCurrentMap = new Map(vFrm.CreateWidth, vFrm.CreateHeight);
                    this.Invalidate();
                    this.Refresh();
                }
            }
        }

    }
}
