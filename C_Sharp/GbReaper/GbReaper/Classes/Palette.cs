using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace GbReaper.Classes {
    public class Palette {
        public static readonly Palette DEFAULT_PALETTE = new Palette();

        public Color[] mColors;

        public Palette() : this(new Color[] {Color.White, Color.LightGray, Color.Gray, Color.DarkGray}) {
            
        }

        public Palette(Color[] pColors) {
            this.mColors = pColors;
        }
    }
}
