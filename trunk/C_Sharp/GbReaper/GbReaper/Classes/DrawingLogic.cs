using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace GbReaper.Classes {
    /// <summary>
    /// Collection of drawing logic used everywhere, so in case I need to fix it, it's all in one place.
    /// </summary>
    static class DrawingLogic {


        public static void ScaledAndTiledPaintBackground(Image pImg, PaintEventArgs e, Rectangle pR, int pRepeat) {
            if (pImg == null) {
                e.Graphics.FillRectangle(Brushes.White, pR);
                return;
            }

            e.Graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            e.Graphics.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.Half;
            e.Graphics.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;

            int vSide = e.ClipRectangle.Width / pRepeat;
            for (int x = 0; x < pRepeat; x++) {
                for (int y = 0; y < pRepeat; y++) {
                    e.Graphics.DrawImage(pImg, x * vSide, y * vSide, vSide, vSide);
                }
            }
        }

        public static void ScalePaintBackground(Image pImg, PaintEventArgs e, Rectangle pR) {
            if (pImg == null) {
                e.Graphics.FillRectangle(Brushes.White, pR);
                return;
            }

            e.Graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            e.Graphics.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.Half;
            e.Graphics.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;

            e.Graphics.DrawImage(pImg, pR);
        }


        public static void DrawGrid(PaintEventArgs e, Rectangle pR) {

            for (int x = 0; x < 7; x++) {
                e.Graphics.DrawLine(Pens.Black, (x + 1) * pR.Width / 8, 0, (x + 1) * pR.Width / 8, pR.Height);
            }

            for (int y = 0; y < 7; y++) {
                e.Graphics.DrawLine(Pens.Black, 0, (y + 1) * pR.Height / 8, pR.Width, (y + 1) * pR.Height / 8);
            }
        }
    }
}
