using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Xml;
using System.IO;
using System.Drawing.Imaging;

namespace GbReaper.Classes {
    public class Sprite {
        public const int WIDTH_PX = 8;
        public const int HEIGHT_PX = 8;

        private Guid mUID = Guid.Empty;
        private Image mImage = null;
        private string mName = null;
        private Palette mPalette = Palette.DEFAULT_PALETTE;

        public Guid UID { get { return mUID; } }
        public Image Image { get { return mImage; } set { mImage = value; OnSpriteChanged(); } }
        public string Name { get { return mName; } set { mName = value; } }
        public Palette Palette { get { return mPalette; } }


        public delegate void SpriteChangeDelegate(Sprite pSprite);
        public event SpriteChangeDelegate SpriteChanged;

        public Sprite(Guid pUID, string pName, Image pImage, Palette pPalette) {
            this.mUID = pUID;
            this.mImage = pImage;
            this.mName = pName;
            this.mPalette = pPalette;
        }

        public Sprite(Image pImage, Palette pPalette) : this (
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
        /// Raises the SpriteChanged event
        /// </summary>
        protected void OnSpriteChanged() {
            if (this.SpriteChanged != null) {
                this.SpriteChanged(this);
            }
        }

        public override bool Equals(object obj) {
            return obj != null && obj is Sprite && this.UID == ((Sprite)obj).UID;
        }
        public override int GetHashCode() {
            return this.mUID.GetHashCode();
        }

        internal void SaveToStream(System.IO.StreamWriter pSW) {
            pSW.Write("\t\t\t<sprite id=\""+this.UID+"\" name=\"" + this.Name + "\" palette=\""+this.Palette.mName+"\">");

            Bitmap vBmp = (Bitmap)this.mImage;

            for (int y= 0; y < this.mImage.Height; y++){
                for (int x = 0; x < this.mImage.Width; x++) {
                    Color vCol = vBmp.GetPixel(x, y);
                    //string vPix = string.Format("{0:X02}{1:X02}{2:X02}", vCol.R, vCol.G, vCol.B);
                    //pSW.Write(vPix);
                    pSW.Write("{0:X02}{1:X02}{2:X02}", vCol.R, vCol.G, vCol.B);
                }
            }
            
            pSW.WriteLine("</sprite>");
        }

        internal static Sprite LoadFromXml (XmlNode pNode){
            try {
                Bitmap vBmp = new Bitmap(Sprite.WIDTH_PX, Sprite.HEIGHT_PX);

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
                        if (x >= Sprite.WIDTH_PX) {
                            x = 0;
                            y++;
                            if (y >= Sprite.HEIGHT_PX) {
                                break;
                            }
                        }
                    }
                }

                return new Sprite(Guid.Parse(pNode.Attributes["id"].Value),
                    pNode.Attributes["name"].Value,
                    vBmp,
                    Palette.DEFAULT_PALETTE //todo fix this one day
                    );
            }
            catch (Exception ex) {

                throw;
            }
        }
    }
}
