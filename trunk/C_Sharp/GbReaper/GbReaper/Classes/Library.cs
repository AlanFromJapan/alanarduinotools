using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace GbReaper.Classes {
    public class Library : IEnumerable<Sprite> {
        private List<Sprite> mSprites = new List<Sprite>();
        private string mName = null;

        public string Name { get { return mName; } set { mName = value; } }

        public Sprite this[int i] {
            get {
                return this.mSprites[i];
            }
        }


        
        public event GbReaper.Classes.Sprite.SpriteChangeDelegate SpriteDeleted;
        public event GbReaper.Classes.Sprite.SpriteChangeDelegate SpriteAdded;

        public Library(string pName) { 
            mName = pName; 
        }

        public IEnumerator<Sprite> GetEnumerator() {
            return this.mSprites.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() {
            return this.mSprites.GetEnumerator();
        }

        public void AddSprite(Sprite pSprite) {
            this.mSprites.Add(pSprite);

            OnSpriteAdded(pSprite);
        }

        public void DeleteSprite(Sprite pSprite) {
            this.mSprites.Remove(pSprite);

            OnSpriteDeleted(pSprite);
        }

        protected void OnSpriteDeleted(Sprite pSprite) {
            if (this.SpriteDeleted != null) {
                this.SpriteDeleted(pSprite);
            }
        }
        protected void OnSpriteAdded(Sprite pSprite) {
            if (this.SpriteAdded != null) {
                this.SpriteAdded(pSprite);
            }
        }

        internal void SaveToStream(StreamWriter pSW) {
            pSW.WriteLine("\t\t<library name=\"" + this.Name + "\">");
            foreach (Sprite vS in this.mSprites) {
                vS.SaveToStream(pSW);
            }
            pSW.WriteLine("\t\t</library>");
        }
    }
}
