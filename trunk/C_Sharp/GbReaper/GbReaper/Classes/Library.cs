using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Drawing.Imaging;

namespace GbReaper.Classes {
    public class Library : IEnumerable<Tile> {
        private List<Tile> mTiles = new List<Tile>();
        private string mName = null;

        public string Name { get { return mName; } set { mName = value; } }

        public Tile this[int i] {
            get {
                return this.mTiles[i];
            }
        }


        
        public event GbReaper.Classes.Tile.TileChangeDelegate TileDeleted;
        public event GbReaper.Classes.Tile.TileChangeDelegate TileAdded;

        public Library(string pName) { 
            mName = pName; 
        }

        public IEnumerator<Tile> GetEnumerator() {
            return this.mTiles.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() {
            return this.mTiles.GetEnumerator();
        }

        public int IndexOf(Tile pT) {
            return this.mTiles.IndexOf(pT);
        }

        public void DeleteTile(Tile pTile) {
            this.mTiles.Remove(pTile);

            OnTileDeleted(pTile);
        }

        protected void OnTileDeleted(Tile pTile) {
            if (this.TileDeleted != null) {
                this.TileDeleted(pTile);
            }
        }
        protected void OnTileAdded(Tile pTile) {
            if (this.TileAdded != null) {
                this.TileAdded(pTile);
            }
        }

        internal void SaveToStream(StreamWriter pSW) {
            pSW.WriteLine("\t\t<library name=\"" + this.Name + "\">");
            foreach (Tile vS in this.mTiles) {
                vS.SaveToStream(pSW);
            }
            pSW.WriteLine("\t\t</library>");
        }

        public Tile GetTileByID(Guid pUID) {
            foreach (Tile vT in this.mTiles) {
                if (vT.UID == pUID) {
                    return vT;
                }
            }
            return null;
        }


        public void AddTile(Tile pTile) {
            this.mTiles.Add(pTile);

            OnTileAdded(pTile);
        }


        /// <summary>
        /// Adds a tile to the library and return it if new, or returns the equivalent one already existing
        /// </summary>
        /// <param name="pTile"></param>
        /// <param name="pAlreadyExisted"></param>
        /// <returns></returns>
        public Tile AddTileWithoutDuplicate(Tile pTile, out bool pAlreadyExisted) {
            foreach (Tile vT in this.mTiles) {
                if (CompareMemCmp((Bitmap)pTile.Image, (Bitmap)vT.Image)) {
                    pAlreadyExisted = true;
                    return vT;
                }
            }

            pAlreadyExisted = false;
            this.mTiles.Add(pTile);
            
            OnTileAdded(pTile);

            return pTile;
        }

        //http://stackoverflow.com/questions/2031217/what-is-the-fastest-way-i-can-compare-two-equal-size-bitmaps-to-determine-whethe
        [DllImport("msvcrt.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int memcmp(IntPtr b1, IntPtr b2, long count);

        private static bool CompareMemCmp(Bitmap b1, Bitmap b2) {
            if ((b1 == null) != (b2 == null)) return false;
            if (b1.Size != b2.Size) return false;

            var bd1 = b1.LockBits(new Rectangle(new Point(0, 0), b1.Size), ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);
            var bd2 = b2.LockBits(new Rectangle(new Point(0, 0), b2.Size), ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);

            try {
                IntPtr bd1scan0 = bd1.Scan0;
                IntPtr bd2scan0 = bd2.Scan0;

                int stride = bd1.Stride;
                int len = stride * b1.Height;

                return memcmp(bd1scan0, bd2scan0, len) == 0;
            }
            finally {
                b1.UnlockBits(bd1);
                b2.UnlockBits(bd2);
            }
        }


        private static string CleanFileName(string fileName) {
            string s = Path.GetInvalidFileNameChars().Aggregate(fileName, (current, c) => current.Replace(c.ToString(), string.Empty));
            s = s.Replace(" ", "_");
            return s;
        }

        internal void ExportToGBDK(string pPath) {
            string vLibNameC = CleanFileName(this.Name);
            string vFilename = Path.Combine(pPath, (string.IsNullOrWhiteSpace(this.Name) ? "GbReaper_lib.c" : vLibNameC + ".c"));

            using (FileStream vFS = new FileStream(vFilename, FileMode.Create, FileAccess.ReadWrite, FileShare.None)) {
                using (StreamWriter vSW = new StreamWriter(vFS)) {
                    vSW.WriteLine(string.Format(@"
#define {1}_COUNT   {0}

", this.mTiles.Count,  vLibNameC));

                    vSW.WriteLine(@"unsigned char " + vLibNameC + @"[] =
{
");
                    bool vFirst = true;
                    foreach (Tile vT in this.mTiles){
                        if (!vFirst)
                            vSW.WriteLine(",");

                        vSW.Write(vT.ExportTileToGBDKString());

                        vFirst = false;
                    }

                    vSW.WriteLine(@"
};
");
                }
            }
        }
    }
}
