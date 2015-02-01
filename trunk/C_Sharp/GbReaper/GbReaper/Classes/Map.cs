using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.IO;

namespace GbReaper.Classes {
    public class Map {
        class MapBucket {
            public Tile mTile = null;
        }

        public readonly int Width;
        public readonly int Height;
        private MapBucket[,] mMatrix = null;
        private string mName = null;
        protected GbProject mParentProject = null;

        public string Name { get { return mName; } set { mName = value; } }
        public GbProject ParentProject {
            get { return this.mParentProject; }
            set { this.mParentProject = value; }
        }

        public event EventHandler MapChanged;

        public Tile this[int x, int y] {
            get {
                return (this.mMatrix[x, y] == null ?
                    null :
                    this.mMatrix[x, y].mTile);
            }
        }

        public Map(int pW, int pH) {
            this.Width = pW;
            this.Height = pH;
            this.mMatrix = new MapBucket[this.Width, this.Height];
        }

        public void SetTile(Tile pTile, int pX, int pY) { 
            //no check

            //unregister event handler of previous sprite
            if (this.mMatrix[pX, pY] != null) {
                MapBucket vOld = this.mMatrix[pX, pY];
                if (vOld.mTile != null) {
                    vOld.mTile.TileChanged -= new Tile.TileChangeDelegate(TileChangedHandler);
                }
            }

            MapBucket vNew = new MapBucket();
            vNew.mTile = pTile;
            this.mMatrix[pX, pY] = vNew;

            RegisterTileChangeHandler(pTile);

            OnMapChanged();
        }

        public void ClearTileAt(int pX, int pY) {
            //unregister event handler of previous sprite
            if (this.mMatrix[pX, pY] != null) {
                MapBucket vOld = this.mMatrix[pX, pY];
                if (vOld.mTile != null) {
                    vOld.mTile.TileChanged -= new Tile.TileChangeDelegate(TileChangedHandler);
                }
            }

            this.mMatrix[pX, pY] = null;

            OnMapChanged();
        }

        private void RegisterTileChangeHandler(Tile pTile) {
            //trick to avoid multiple registration on the same instance (always ok to unregister)
            pTile.TileChanged -= new Tile.TileChangeDelegate(TileChangedHandler);
            pTile.TileChanged += new Tile.TileChangeDelegate(TileChangedHandler);
        }

        protected void TileChangedHandler(Tile pTile) {
            //repaint
            this.OnMapChanged();
        }

        protected void OnMapChanged() {
            if (this.MapChanged != null) {
                this.MapChanged(this, null);
            }
        }

        public Image GetImage() {
            Bitmap vBuff = new Bitmap(this.Width * Tile.WIDTH_PX, this.Height * Tile.HEIGHT_PX);
            Graphics vG = Graphics.FromImage(vBuff);

            try {

                for (int y = 0; y < this.Height; y++) {
                    for (int x = 0; x < this.Width; x++) {
                        if (this.mMatrix[x, y] != null) {
                            MapBucket vOld = this.mMatrix[x, y];
                            if (vOld.mTile != null) {
                                vG.DrawImageUnscaled(vOld.mTile.Image, x * Tile.WIDTH_PX, y * Tile.HEIGHT_PX);

                            }
                        }                        
                    }
                }

            }
            finally {
                vG.Dispose();
                vG = null;
            }            
            
            return vBuff;
        }


        internal void SaveToStream(StreamWriter pSW) {
            pSW.WriteLine(string.Format("\t\t<map name=\"{0}\" width=\"{1}\" height=\"{2}\">", this.Name, this.Width, this.Height));

            for (int y = 0; y < this.Height; y++) {
                for (int x = 0; x < this.Width; x++) {
                    if (mMatrix[x, y] == null || mMatrix[x, y].mTile == null) {
                        //make smaller files and skip empty entries
                        continue;
                    }

                    pSW.WriteLine(string.Format("\t\t\t<cell tileID=\"{0}\" x=\"{1}\" y=\"{2}\" />",
                        (mMatrix[x,y] == null || mMatrix[x,y].mTile == null ? null: mMatrix[x,y].mTile.UID.ToString()),
                        x,
                        y
                        ));
                }
            }
                
            pSW.WriteLine("\t\t</map>");
        }
    }
}
