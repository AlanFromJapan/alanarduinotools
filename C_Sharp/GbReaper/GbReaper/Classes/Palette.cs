using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace GbReaper.Classes {
    public class Palette {
        public string mName = null;
        public static readonly Palette DEFAULT_PALETTE = new Palette();

        public Color[] mColors;

        public Palette() : this("default", new Color[] {Color.White, Color.LightGray, Color.DarkGray, Color.Gray}) {
            
        }

        public Palette(string pName, Color[] pColors) {
            this.mColors = pColors;
            this.mName = pName;
        }
    }
}
