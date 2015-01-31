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


        public static void DrawGrid(PaintEventArgs e, Rectangle pR, Pen pPen, int pDivideH, int pDivideV) {

            for (int x = 0; x < pDivideH-1; x++) {
                e.Graphics.DrawLine(pPen, pR.Left + (x + 1) * pR.Width / pDivideH, pR.Top + 0, pR.Left + (x + 1) * pR.Width / pDivideH, pR.Top + pR.Height);
            }

            for (int y = 0; y < pDivideV-1; y++) {
                e.Graphics.DrawLine(pPen, pR.Left + 0, pR.Top + (y + 1) * pR.Height / pDivideV, pR.Left + pR.Width, pR.Top + (y + 1) * pR.Height / pDivideV);
            }
        }
    }
}
