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

        /// <summary>
        /// Duplicates the contents of the param map into self. Must be same size.
        /// </summary>
        /// <param name="pSource"></param>
        public void Duplicate(Map pSource) {
            if (pSource.Width != this.Width || pSource.Height != this.Height) {
                throw new ArgumentException("Duplicate map source must have same size as target");
            }

            for (int x = 0; x < this.Width; x++)
                for (int y = 0; y < this.Height; y++) {
                    //this.mMatrix[x, y] = pSource.mMatrix[x, y];
                    this.SetTile(pSource[x, y], x, y);
                }
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

        public Image GetImage(bool pGridOnMaps) {
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

                if (pGridOnMaps) {
                    DrawingLogic.DrawGrid(vG, new Rectangle(0, 0, vBuff.Width, vBuff.Height), Pens.LightGray, this.Width, this.Height);
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
                        (mMatrix[x, y] == null || mMatrix[x, y].mTile == null ? null : mMatrix[x, y].mTile.UID.ToString()),
                        x,
                        y
                        ));
                }
            }

            pSW.WriteLine("\t\t</map>");
        }


        private static string CleanFileName(string fileName) {
            string s = Path.GetInvalidFileNameChars().Aggregate(fileName, (current, c) => current.Replace(c.ToString(), string.Empty));
            s = s.Replace(" ", "_");
            return s;
        }


        internal void ExportToPNG(string pPath, bool pGridOnMaps) {
            string vMapNameC = CleanFileName(this.Name);
            string vFilenameC = Path.Combine(pPath, vMapNameC + ".png");

            Image img = this.GetImage(pGridOnMaps);
            img.Save(vFilenameC);

        }

        internal void ExportToGBDK(string pPath) {
            string vMapNameC = CleanFileName(this.Name);
            string vFilenameC = Path.Combine(pPath, (string.IsNullOrWhiteSpace(this.Name) ? "GbReaper_map.c" : vMapNameC + ".c"));

            //export the C file
            this.ExportToGBDK_C(vMapNameC, vFilenameC);

            string vFilenameH = Path.Combine(pPath, (string.IsNullOrWhiteSpace(this.Name) ? "GbReaper_map.c" : vMapNameC + ".h"));
            //export the H file
            this.ExportToGBDK_H(vMapNameC, vFilenameH);
        }

        /// <summary>
        /// Makes the H file
        /// </summary>
        /// <param name="vMapNameC"></param>
        /// <param name="vFilename"></param>        
        protected void ExportToGBDK_H(string vMapNameC, string vFilename) {
            using (FileStream vFS = new FileStream(vFilename, FileMode.Create, FileAccess.ReadWrite, FileShare.None)) {
                using (StreamWriter vSW = new StreamWriter(vFS)) {
                    vSW.WriteLine(string.Format(@"
#ifndef __{2}_H__
#define __{2}_H__

#define {2}_WIDTH   {0}
#define {2}_HEIGHT  {1}

extern const unsigned char {2}[];

#endif  //__{2}_H__

", this.Width, this.Height, vMapNameC));
                }
            }
        }

            /// <summary>
            /// Makes the C file
            /// </summary>
            /// <param name="vMapNameC"></param>
            /// <param name="vFilename"></param>        
            protected void ExportToGBDK_C(string vMapNameC, string vFilename) {

            using (FileStream vFS = new FileStream(vFilename, FileMode.Create, FileAccess.ReadWrite, FileShare.None)) {
                using (StreamWriter vSW = new StreamWriter(vFS)) {
                    vSW.WriteLine(string.Format(@"

#include ""{2}.h""

", this.Width, this.Height, vMapNameC));

                    vSW.WriteLine(@"const unsigned char " + vMapNameC + @"[] =
{
");
                    bool vFirst = true;
                    for (int y = 0; y < this.Height; y++) {
                        for (int x=0; x < this.Width; x++){
                        
                            if (!vFirst)
                                vSW.Write(",");

                            if (this.mMatrix[x, y] == null || this.mMatrix[x, y].mTile == null) {
                                vSW.Write("0x00");
                            }
                            else {
                                int i = this.mParentProject.mLibraries[0].IndexOf(this.mMatrix[x, y].mTile);

                                vSW.Write(string.Format("0x{0:X02}", i));
                            }

                            vFirst = false;
                        }
                        vSW.WriteLine("");
                    }

                    vSW.WriteLine(@"
};

");
                }
            }
        }
    }
}
