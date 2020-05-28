using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Xml;
using System.IO;
using System.Drawing.Imaging;

namespace GbReaper.Classes {
    public class Tile {
        public const int WIDTH_PX = 8;
        public const int HEIGHT_PX = 8;

        private Guid mUID = Guid.Empty;
        private Bitmap mImage = null;
        private string mName = null;
        private Palette mPalette = Palette.DEFAULT_PALETTE;

        public Guid UID { get { return mUID; } }
        public Image Image { get { return (Image)mImage.Clone(); } }//set { mImage = value; OnTileChanged(); } }
        public string Name { get { return mName; } set { mName = value; } }
        public Palette Palette { get { return mPalette; } }


        public delegate void TileChangeDelegate(Tile pTile);
        public event TileChangeDelegate TileChanged;

        public Tile(Guid pUID, string pName, Image pImage, Palette pPalette) {
            this.mUID = pUID;
            this.mImage = (pImage is Bitmap ? (Bitmap)pImage : new Bitmap(pImage));
            this.mName = pName;
            this.mPalette = pPalette;
        }

        public Tile(string pName, Image pImage, Palette pPalette)
        : this(
            Guid.NewGuid(),
            pName,
            pImage,
            pPalette) {
                }

        public Tile(Image pImage, Palette pPalette)
            : this(
            Guid.NewGuid(),
            string.Empty,
            pImage,
            pPalette)
        {
        }

        public void ChangePalette(Palette pPalette) { 
            //TODO
        }

        /// <summary>
        /// Raises the TileChanged event
        /// </summary>
        protected void OnTileChanged() {
            if (this.TileChanged != null) {
                this.TileChanged(this);
            }
        }

        public override bool Equals(object obj) {
            return obj != null && obj is Tile && this.UID == ((Tile)obj).UID;
        }
        public override int GetHashCode() {
            return this.mUID.GetHashCode();
        }


        internal void SetPixel(int x, int y, Color pColor) {
            if (x < 0 || x > 7 || y < 0 || y > 7)
                return;

            this.mImage.SetPixel(x, y, pColor);
            OnTileChanged();
        }

        internal string ExportTileToGBDKDefine(int pIndex) {
            if (string.IsNullOrEmpty(this.Name))
                return null;

            string vN = this.Name.ToUpper().Trim().Replace(" ", "_");

            return string.Format("#define TILE_{0}   {1}\r\n", vN, pIndex);
        }


        internal string ExportTileToGBDKString(int pIndex = -1) {
            byte b1, b2;
            StringBuilder vSB = new StringBuilder(100);

            for (int y = 0; y < HEIGHT_PX; y++) {
                b1 = 0;
                b2 = 0;

                for (int x = 0; x < WIDTH_PX; x++) {
                    b1 = (byte)(b1 << 1);
                    b2 = (byte)(b2 << 1);

                    byte vIdx = (byte)this.Palette.GetIndexInPalette(this.mImage.GetPixel(x, y));
                    if (vIdx < 0)
                        throw new Exception("Error while exporting : cannot find tile color in palette!");

                    b1 = (byte)(b1 | (byte)(0x01 & vIdx));
                    b2 = (byte)(b2 | (byte)((0x02 & vIdx) >> 1));

                }

                if (vSB.Length != 0)
                    vSB.Append(",");
                vSB.AppendFormat("0x{0:X02},0x{1:X02}", b1, b2);
            }

            return string.Format("//{3} {0} [{1}]\r\n{2}", this.Name, this.UID, vSB.ToString(), (pIndex >= 0? pIndex + ":": ""));
        }

        internal void SaveToStream(System.IO.StreamWriter pSW) {
            pSW.Write("\t\t\t<tile id=\""+this.UID+"\" name=\"" + this.Name + "\" palette=\"" + this.Palette.mName + "\" >");

            Bitmap vBmp = (Bitmap)this.mImage;

            for (int y= 0; y < this.mImage.Height; y++){
                for (int x = 0; x < this.mImage.Width; x++) {
                    Color vCol = vBmp.GetPixel(x, y);
                    //string vPix = string.Format("{0:X02}{1:X02}{2:X02}", vCol.R, vCol.G, vCol.B);
                    //pSW.Write(vPix);
                    pSW.Write("{0:X02}{1:X02}{2:X02}", vCol.R, vCol.G, vCol.B);
                }
            }
            
            pSW.WriteLine("</tile>");
        }

        internal static Tile LoadFromXml (XmlNode pNode, Palette pPalette){
            try {
                Bitmap vBmp = new Bitmap(Tile.WIDTH_PX, Tile.HEIGHT_PX);

                int x = 0;
                int y = 0;
                string vImageString = pNode.InnerText;
                char[] vBuff = new char[6];
                using (StringReader vSR = new StringReader(vImageString)) {
                    while (vSR.Read(vBuff, 0, vBuff.Length) == vBuff.Length) {
                        string vHtmlColor = "#" + new string(vBuff);
                        Color vCol = ColorTranslator.FromHtml(vHtmlColor);
                        vBmp.SetPixel(x, y, vCol);

                        x++;
                        if (x >= Tile.WIDTH_PX) {
                            x = 0;
                            y++;
                            if (y >= Tile.HEIGHT_PX) {
                                break;
                            }
                        }
                    }
                }

                return new Tile(Guid.Parse(pNode.Attributes["id"].Value),
                    pNode.Attributes["name"].Value,
                    vBmp,
                    pPalette
                    );
            }
            catch (Exception) {

                throw;
            }
        }
    }
}
