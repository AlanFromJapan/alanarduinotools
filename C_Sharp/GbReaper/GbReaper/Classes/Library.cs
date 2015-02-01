using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

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

        public void AddTile(Tile pTile) {
            this.mTiles.Add(pTile);

            OnTileAdded(pTile);
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
    }
}
