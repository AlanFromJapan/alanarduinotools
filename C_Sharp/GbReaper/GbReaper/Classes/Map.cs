using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace GbReaper.Classes {
    public class Map {
        class MapBucket {
            public Sprite mSprite = null;
        }

        public readonly int Width;
        public readonly int Height;
        private MapBucket[,] mMatrix = null;
        private string mName = null;

        public string Name { get { return mName; } set { mName = value; } }

        public event EventHandler MapChanged;

        public Map(int pW, int pH) {
            this.Width = pW;
            this.Height = pH;
            this.mMatrix = new MapBucket[this.Width, this.Height];
        }

        public void SetSprite(Sprite pSprite, int pX, int pY) { 
            //no check

            //unregister event handler of previous sprite
            if (this.mMatrix[pX, pY] != null) {
                MapBucket vOld = this.mMatrix[pX, pY];
                if (vOld.mSprite != null) {
                    vOld.mSprite.SpriteChanged -= new Sprite.SpriteChangeDelegate(SpriteChangedHandler);
                }
            }

            MapBucket vNew = new MapBucket();
            vNew.mSprite = pSprite;
            this.mMatrix[pX, pY] = vNew;

            RegisterSpriteChangeHandler(pSprite);

            OnMapChanged();
        }

        private void RegisterSpriteChangeHandler(Sprite pSprite) {
            //trick to avoid multiple registration on the same instance (always ok to unregister)
            pSprite.SpriteChanged -= new Sprite.SpriteChangeDelegate(SpriteChangedHandler);
            pSprite.SpriteChanged += new Sprite.SpriteChangeDelegate(SpriteChangedHandler);
        }

        protected void SpriteChangedHandler(Sprite pSprite) {
            //repaint
            this.OnMapChanged();
        }

        protected void OnMapChanged() {
            if (this.MapChanged != null) {
                this.MapChanged(this, null);
            }
        }

        public Image GetImage() {
            Bitmap vBuff = new Bitmap(this.Width * Sprite.WIDTH_PX, this.Height * Sprite.HEIGHT_PX);
            Graphics vG = Graphics.FromImage(vBuff);

            try {

                for (int y = 0; y < this.Height; y++) {
                    for (int x = 0; x < this.Width; x++) {
                        if (this.mMatrix[x, y] != null) {
                            MapBucket vOld = this.mMatrix[x, y];
                            if (vOld.mSprite != null) {
                                vG.DrawImageUnscaled(vOld.mSprite.Image, x * Sprite.WIDTH_PX, y * Sprite.HEIGHT_PX);

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
    }
}
