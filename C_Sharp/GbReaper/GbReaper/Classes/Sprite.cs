using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

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
        public Palette Palette { get { return Palette; } }


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
    }
}
