using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using GbReaper.Classes;

namespace GbReaper.Controls {
    public partial class UcPaletteEditor : UserControl {
        protected Palette mCurrentPalette = Palette.DEFAULT_PALETTE;

        public Palette CurrentPalette {
            get { return mCurrentPalette; }
            set { 
                mCurrentPalette = value;
                panColors.Invalidate();
            }
        }
        public UcPaletteEditor() {
            InitializeComponent();
        }

        private void UcPaletteEditor_Load(object sender, EventArgs e) {
            panColors.Paint += new PaintEventHandler(panColors_Paint);
        }

        void panColors_Paint(object sender, PaintEventArgs e) {
            if (DesignMode)
                return;

            for (int i = 0; i < CurrentPalette.mColors.Length; i++) {
                using (Brush vB = new SolidBrush(CurrentPalette.mColors[i])) {
                    e.Graphics.FillRectangle(vB, i * panColors.Width / CurrentPalette.mColors.Length, 0, panColors.Width, panColors.Height);
                }
            }
        }
    }
}
