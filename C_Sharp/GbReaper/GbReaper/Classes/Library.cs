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
        public string NameClean { get { return CleanFileName(this.mName); } }

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

        /// <summary>
        /// Moves tile B before tile A
        /// </summary>
        /// <param name="ta"></param>
        /// <param name="tb"></param>
        public void MoveTileBefore(Tile ta, Tile tb) {
            this.mTiles.Remove(tb);
            this.mTiles.Insert(this.mTiles.IndexOf(ta), tb);
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

        internal const int PNG_EXPORT_HEIGHT = 32;
        internal const int PNG_EXPORT_PADLEFT = 40;

        public void ExportToPNG(string pPath) {
            int total_h = (PNG_EXPORT_HEIGHT + 2) * this.mTiles.Count;
            Bitmap bmp = new Bitmap(400, total_h);
            Graphics g = Graphics.FromImage(bmp);
            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.None;
            g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            g.TextRenderingHint = System.Drawing.Text.TextRenderingHint.SingleBitPerPixel;

            g.FillRectangle(Brushes.White, 0,0,400,total_h);

            int vIndex = 0;
            Font font = new Font(FontFamily.GenericMonospace, 16f, FontStyle.Bold);
            Font font2 = new Font(FontFamily.GenericMonospace, 12f, FontStyle.Regular);
            foreach (Tile vT in this.mTiles) {
                g.DrawImage(vT.Image, PNG_EXPORT_PADLEFT, (2 + PNG_EXPORT_HEIGHT) * vIndex, PNG_EXPORT_HEIGHT, PNG_EXPORT_HEIGHT);
                g.DrawString(vT.Name, font, Brushes.Black, PNG_EXPORT_PADLEFT + PNG_EXPORT_HEIGHT + 4, (2 + PNG_EXPORT_HEIGHT) * vIndex + (PNG_EXPORT_HEIGHT -font.Size) / 2);

                g.DrawString(vIndex.ToString(), font2, Brushes.Gray, 2, (2 + PNG_EXPORT_HEIGHT) * vIndex + (PNG_EXPORT_HEIGHT - font.Size) / 2);

                vIndex++;
            }

            bmp.Save(Path.Combine(pPath, this.mName + ".png"), ImageFormat.Png);
        }
    

        internal void ExportToGBDK(string pPath) {
            string vLibNameC = CleanFileName(this.Name);
            string vFilename = Path.Combine(pPath, (string.IsNullOrWhiteSpace(this.Name) ? "GbReaper_lib" : vLibNameC ));

            //make .c file
            this.ExportToGBDK_C(vLibNameC, vFilename + ".c");
            //make .h file
            this.ExportToGBDK_H(vLibNameC, vFilename + ".h");
        }

        internal void ExportToGBDK_H(string vLibNameC, string vFilename) {
            int vIndex = 0;

            using (FileStream vFS = new FileStream(vFilename, FileMode.Create, FileAccess.ReadWrite, FileShare.None)) {
                using (StreamWriter vSW = new StreamWriter(vFS)) {
                    //count header
                    vSW.WriteLine(string.Format(@"
#ifndef __{1}_H__
#define __{1}_H__

#define {1}_COUNT   {0}

", this.mTiles.Count, vLibNameC));


                    //write the defines indexes by name if any
                    vIndex = 0;
                    foreach (Tile vT in this.mTiles) {
                        string s = vT.ExportTileToGBDKDefine(vIndex++);
                        if (s != null)
                            vSW.Write(s);
                    }


                    //Write the bytes contents
                    vSW.WriteLine(string.Format(@"

//declared as const to save compile and execution time/space
extern const unsigned char {0}[]; 

#endif //__{0}_H__

", vLibNameC));
                }
            }
        }


        internal void ExportToGBDK_C(string vLibNameC, string vFilename) {
            int vIndex = 0;

            using (FileStream vFS = new FileStream(vFilename, FileMode.Create, FileAccess.ReadWrite, FileShare.None)) {
                using (StreamWriter vSW = new StreamWriter(vFS)) {


                    //Write the bytes contents
                    vSW.WriteLine(string.Format(@"
#include ""{0}.h""

//declared as const to save compile and execution time/space
const unsigned char {0}[] = 
{{
", vLibNameC));

                    bool vFirst = true;
                    vIndex = 0;
                    foreach (Tile vT in this.mTiles){
                        if (!vFirst)
                            vSW.WriteLine(",");

                        vSW.Write(vT.ExportTileToGBDKString(vIndex++));

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
