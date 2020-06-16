using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using System.Globalization;
using GbReaper.Classes;
using GbReaper.Forms;

namespace GbReaper.Controls {
    public partial class UcLibraryList : UserControl {
        private Library mCurrentLib = null;

        public delegate void SelectedTileChangedDelegate(Tile pS);
        public event SelectedTileChangedDelegate SelectedTileChanged;
        
        public delegate void TilesDeletedDelegate(IList<Tile> pDeletedTiles);
        public event TilesDeletedDelegate TilesDeleted;

        protected ListViewItem lviReorderingTile = null;
        protected ListViewItem lviReorderingTarget = null;

        public Tile SelectedTile {
            get { 
                if (lvLibrary.SelectedItems!= null && lvLibrary.SelectedItems.Count > 0){
                    return ((TileViewItem)lvLibrary.SelectedItems[0]).mTile;
                }
                return null;
            }
        }

        public UcLibraryList() {
            InitializeComponent();
        }

        private void UcLibraryList_Load(object sender, EventArgs e) {
            lvLibrary.DrawItem += new DrawListViewItemEventHandler(lvLibrary_DrawItem);
            lvLibrary.DrawSubItem += new DrawListViewSubItemEventHandler(lvLibrary_DrawSubItem);
            lvLibrary.DrawColumnHeader += new DrawListViewColumnHeaderEventHandler(lvLibrary_DrawColumnHeader);
        }

        void lvLibrary_DrawColumnHeader(object sender, DrawListViewColumnHeaderEventArgs e) {
            //https://msdn.microsoft.com/en-us/library/system.windows.forms.listview.ownerdraw%28v=vs.110%29.aspx
            using (StringFormat sf = new StringFormat()) {
                // Store the column text alignment, letting it default 
                // to Left if it has not been set to Center or Right. 
                switch (e.Header.TextAlign) {
                    case HorizontalAlignment.Center:
                        sf.Alignment = StringAlignment.Center;
                        break;
                    case HorizontalAlignment.Right:
                        sf.Alignment = StringAlignment.Far;
                        break;
                }

                // Draw the standard header background.
                e.DrawBackground();

                // Draw the header text. 
                using (Font headerFont =
                            new Font("Helvetica", 10, FontStyle.Bold)) {
                    e.Graphics.DrawString(e.Header.Text, headerFont,
                        Brushes.Black, e.Bounds, sf);
                }
            }
        }

        void lvLibrary_DrawSubItem(object sender, DrawListViewSubItemEventArgs e) {
            TextFormatFlags flags = TextFormatFlags.Left;

            using (StringFormat sf = new StringFormat()) {
                // Store the column text alignment, letting it default 
                // to Left if it has not been set to Center or Right. 
                switch (e.Header.TextAlign) {
                    case HorizontalAlignment.Center:
                        sf.Alignment = StringAlignment.Center;
                        flags = TextFormatFlags.HorizontalCenter;
                        break;
                    case HorizontalAlignment.Right:
                        sf.Alignment = StringAlignment.Far;
                        flags = TextFormatFlags.Right;
                        break;
                }

                // Draw the text and background for a subitem with a  
                // negative value.  
                double subItemValue;
                if (e.ColumnIndex > 0 && Double.TryParse(
                    e.SubItem.Text, NumberStyles.Currency,
                    NumberFormatInfo.CurrentInfo, out subItemValue) &&
                    subItemValue < 0) {
                    // Unless the item is selected, draw the standard  
                    // background to make it stand out from the gradient. 
                    if ((e.ItemState & ListViewItemStates.Selected) == 0) {
                        e.DrawBackground();
                    }

                    // Draw the subitem text in red to highlight it. 
                    e.Graphics.DrawString(e.SubItem.Text,
                        lvLibrary.Font, Brushes.Red, e.Bounds, sf);

                    return;
                }

                // Draw normal text for a subitem with a nonnegative  
                // or nonnumerical value.
                e.DrawText(flags);
            }
        }

        void lvLibrary_DrawItem(object sender, DrawListViewItemEventArgs e) {
            if ((e.State & ListViewItemStates.Selected) != 0) {
                // Draw the background and focus rectangle for a selected item.
                //e.Graphics.FillRectangle(Brushes.YellowGreen, e.Bounds);

                //selected item is gold
                if (this.lvLibrary.SelectedItems.Contains(e.Item)) {
                    e.Graphics.FillRectangle(Brushes.Gold, e.Bounds);
                }

                //hovering effect for reorder
                if (e.Item == lviReorderingTarget) {
                    e.Graphics.FillRectangle(Brushes.LightBlue, e.Bounds);
                }

                TileViewItem vSVI = (TileViewItem)e.Item;
                e.Graphics.DrawImage(vSVI.mTile.Image, e.Bounds.Location.X, e.Bounds.Location.Y, Tile.WIDTH_PX*2, Tile.HEIGHT_PX*2);

                Rectangle vR = new Rectangle(e.Bounds.Location, e.Bounds.Size);
                vR.Offset(Tile.WIDTH_PX * 2, 0);
                e.Graphics.DrawString(
                    (string.IsNullOrEmpty(vSVI.mTile.Name) ? vSVI.mTile.UID.ToString() : vSVI.mTile.Name), 
                    lvLibrary.Font,
                    Brushes.Black,
                    vR, 
                    StringFormat.GenericDefault
                );

                e.DrawFocusRectangle();
            }
            else {
                // Draw the background for an unselected item. 
                using (LinearGradientBrush brush =
                    new LinearGradientBrush(e.Bounds, Color.Orange,
                    Color.Maroon, LinearGradientMode.Horizontal)) {
                    e.Graphics.FillRectangle(brush, e.Bounds);
                }
            }

        }

        class TileViewItem : ListViewItem {
            public Tile mTile = null;

            public TileViewItem(Tile pS) {
                this.mTile = pS;
            }

            
        }

        public void SetLibrary(Library pLib) {
            if (this.mCurrentLib != null) {
                this.mCurrentLib.TileAdded -= new Tile.TileChangeDelegate(CurrentLib_TileAdded);
                this.mCurrentLib.TileDeleted -= new Tile.TileChangeDelegate(CurrentLib_TileDeleted);
            }
            this.mCurrentLib = pLib;
            this.mCurrentLib.TileAdded += new Tile.TileChangeDelegate(CurrentLib_TileAdded);
            this.mCurrentLib.TileDeleted += new Tile.TileChangeDelegate(CurrentLib_TileDeleted);

            //todo unbind sprite handlers before release
            lvLibrary.Clear();

            foreach (Tile s in this.mCurrentLib) {
                lvLibrary.Items.Add(new TileViewItem(s));
                s.TileChanged -= new Tile.TileChangeDelegate(TileChanged);
                s.TileChanged += new Tile.TileChangeDelegate(TileChanged);
            }


            lvLibrary.Invalidate();
            lblTileCount.Text = "Tiles: " + this.lvLibrary.Items.Count;
        }

        void TileChanged(Tile pTile) {
            lvLibrary.Invalidate();
        }

        void CurrentLib_TileDeleted(Tile pTile) {
            //TODO
            lvLibrary.Invalidate();
            lblTileCount.Text = "Tiles: " + this.lvLibrary.Items.Count;

        }

        void CurrentLib_TileAdded(Tile pTile) {
            TileViewItem vTVI = new TileViewItem(pTile);
            lvLibrary.Items.Add(vTVI);
            pTile.TileChanged -= new Tile.TileChangeDelegate(TileChanged);
            pTile.TileChanged += new Tile.TileChangeDelegate(TileChanged);
            lvLibrary.Invalidate();

            //unselect all and reselect the new one
            foreach (ListViewItem vLVI in this.lvLibrary.Items) {
                vLVI.Selected = false;
            }
            vTVI.Selected = true;

            lblTileCount.Text = "Tiles: " + this.lvLibrary.Items.Count;

        }

        private void lvLibrary_SelectedIndexChanged(object sender, EventArgs e) {
            OnSelectedTileChanged();
        }

        protected void OnSelectedTileChanged() {
            if (SelectedTileChanged != null && lvLibrary.SelectedItems.Count > 0) {
                SelectedTileChanged(((TileViewItem)lvLibrary.SelectedItems[0]).mTile);
            }
        }

        private void btnNew_Click(object sender, EventArgs e) {
            Bitmap vBmp = new Bitmap(Tile.WIDTH_PX, Tile.HEIGHT_PX, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            using (Graphics g = Graphics.FromImage(vBmp)) {
                g.FillRectangle(new SolidBrush(Palette.DEFAULT_PALETTE.mColors[0]), 0, 0, Tile.WIDTH_PX, Tile.HEIGHT_PX);
            }

            Tile vT = new Tile(vBmp, Palette.DEFAULT_PALETTE);
            this.mCurrentLib.AddTile(vT);

            RenameTilePopup(vT);

            lblTileCount.Text = "Tiles: " + this.lvLibrary.Items.Count;
        }

        private void lvLibrary_DoubleClick(object sender, EventArgs e) {
            RenameTilePopup(this.SelectedTile);
        }

        private void RenameTilePopup(Tile pTile) {
            if (pTile == null)
                return;

            using (FrmTileDetails f = new FrmTileDetails()) {
                f.TileName = pTile.Name;
                if (f.ShowDialog(this.ParentForm) == DialogResult.OK) {
                    pTile.Name = f.TileName;
                }
            }

            this.Invalidate();
        }

        private void btnDel_Click(object sender, EventArgs e) {
            List<Tile> vDeletedTiles = new List<Tile>();
            foreach (ListViewItem v in lvLibrary.SelectedItems) {
                Tile vT = ((TileViewItem)v).mTile;
                vDeletedTiles.Add(vT);
                lvLibrary.Items.Remove(v);

                mCurrentLib.DeleteTile(vT);
            }

            lblTileCount.Text = "Tiles: " + this.lvLibrary.Items.Count;

            this.OnTilesDeleted(vDeletedTiles);
        }

        protected void OnTilesDeleted(IList<Tile> tiles) {
            if (this.TilesDeleted != null) {
                this.TilesDeleted(tiles);
            }
        }

        public Tile GetNthNextTile(int pNth) {

            bool vStartCounting = false;
            foreach (ListViewItem vLVI in this.lvLibrary.Items) {
                if (vStartCounting) {
                    pNth--;
                    if (pNth == 0) {
                        return ((TileViewItem)vLVI).mTile;
                    }
                }
                if (vLVI == lvLibrary.SelectedItems[0])
                    vStartCounting = true;
            }

            return null;
        }

        private void btnRename_Click(object sender, EventArgs e) {
            if (lvLibrary.SelectedItems.Count == 0)
                return;

            string vTilePrefix = null;
            int i = 1;
            using (FrmTileDetails f = new FrmTileDetails()) {
                if (f.ShowDialog(this.ParentForm) == DialogResult.OK) {
                    vTilePrefix = f.TileName;
                }
            }

            if (vTilePrefix == null)
                return;

            foreach (ListViewItem v in lvLibrary.SelectedItems) {
                Tile vT = ((TileViewItem)v).mTile;
                vT.Name = string.Format("{0} {1}", vTilePrefix, i++);
            }

            this.Invalidate();
        }

        private void btnPNG_Click(object sender, EventArgs e) {
            if (this.mCurrentLib == null)
                return;

            this.mCurrentLib.ExportToPNG(Environment.GetFolderPath(Environment.SpecialFolder.MyPictures));
        }

        private void btnDuplicate_Click(object sender, EventArgs e) {
            if (this.mCurrentLib == null)
                return;

            if (this.SelectedTile == null)
                return;

            Bitmap vBmp = new Bitmap(Tile.WIDTH_PX, Tile.HEIGHT_PX, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            using (Graphics g = Graphics.FromImage(vBmp)) {
                g.DrawImageUnscaled(this.SelectedTile.Image, 0, 0);
            }

            Tile vT = new Tile(vBmp, Palette.DEFAULT_PALETTE);
            this.mCurrentLib.AddTile(vT);

            RenameTilePopup(vT);

            lblTileCount.Text = "Tiles: " + this.lvLibrary.Items.Count;
        }

        private void lvLibrary_MouseDown(object sender, MouseEventArgs e) {
            lviReorderingTile = lvLibrary.GetItemAt(e.X, e.Y);
        }

        private void lvLibrary_MouseLeave(object sender, EventArgs e) {
            lviReorderingTile = null;
            lviReorderingTarget = null;
        }

        private void lvLibrary_MouseUp(object sender, MouseEventArgs e) {
            if (lviReorderingTile == null)
                return;

            ListViewItem lviTarget = lvLibrary.GetItemAt(e.X, e.Y);
            if (lviTarget != null && lviTarget != lviReorderingTile) {
                //drop onto an item take its place
                lvLibrary.Items.Remove(lviReorderingTile);
                lvLibrary.Items.Insert(lvLibrary.Items.IndexOf(lviTarget), lviReorderingTile);

                this.mCurrentLib.MoveTileBefore((lviTarget as TileViewItem).mTile, (lviReorderingTile as TileViewItem).mTile);

                lvLibrary.RedrawItems(0, lvLibrary.Items.Count-1, false);
                lvLibrary.Refresh();
                lvLibrary.Invalidate();
            }

            lviReorderingTarget = null;
        }

        private void lvLibrary_MouseHover(object sender, EventArgs e) {

        }

        private void lvLibrary_MouseMove(object sender, MouseEventArgs e) {
            if (lviReorderingTile != null) {
                lviReorderingTarget = lvLibrary.GetItemAt(e.X, e.Y);
            }
        }
    }
}
