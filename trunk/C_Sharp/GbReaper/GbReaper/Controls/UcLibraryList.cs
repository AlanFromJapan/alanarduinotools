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

namespace GbReaper.Controls {
    public partial class UcLibraryList : UserControl {
        private Library mCurrentLib = null;

        public delegate void SelectedTileChangedDelegate(Tile pS);
        public event SelectedTileChangedDelegate SelectedTileChanged;

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

                if (this.lvLibrary.SelectedItems.Contains(e.Item)) {
                    e.Graphics.FillRectangle(Brushes.Gold, e.Bounds);
                }

                TileViewItem vSVI = (TileViewItem)e.Item;
                e.Graphics.DrawImage(vSVI.mTile.Image, e.Bounds.Location.X, e.Bounds.Location.Y, Tile.WIDTH_PX*2, Tile.HEIGHT_PX*2);

                Rectangle vR = new Rectangle(e.Bounds.Location, e.Bounds.Size);
                vR.Offset(Tile.WIDTH_PX * 2, 0);
                e.Graphics.DrawString(
                    vSVI.mTile.UID.ToString(), 
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
        }

        void TileChanged(Tile pTile) {
            lvLibrary.Invalidate();
        }

        void CurrentLib_TileDeleted(Tile pTile) {
            //TODO
            lvLibrary.Invalidate();
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
            
        }

        private void lvLibrary_SelectedIndexChanged(object sender, EventArgs e) {
            OnSelectedTileChanged();
        }

        protected void OnSelectedTileChanged() {
            if (SelectedTileChanged != null && lvLibrary.SelectedItems.Count > 0) {
                SelectedTileChanged(((TileViewItem)lvLibrary.SelectedItems[0]).mTile);
            }
        }
    }
}
