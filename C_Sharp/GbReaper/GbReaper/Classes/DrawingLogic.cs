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

        public static void SetGraphicsNoInterpol(Graphics pG) {
            pG.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            pG.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.Half;
            pG.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
            pG.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.None;
        }

        public static void ScaledAndTiledPaintBackground(Image pImg, PaintEventArgs e, Rectangle pR, int pRepeat) {
            if (pImg == null) {
                e.Graphics.FillRectangle(Brushes.White, pR);
                return;
            }

            SetGraphicsNoInterpol(e.Graphics);

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

            SetGraphicsNoInterpol(e.Graphics);

            e.Graphics.DrawImage(pImg, pR);
        }


        public static void DrawGrid(Graphics pG, Rectangle pR, Pen pPen, int pDivideH, int pDivideV) {

            for (int x = 0; x < pDivideH-1; x++) {
                pG.DrawLine(pPen, pR.Left + (x + 1) * pR.Width / pDivideH, pR.Top + 0, pR.Left + (x + 1) * pR.Width / pDivideH, pR.Top + pR.Height);
            }

            for (int y = 0; y < pDivideV-1; y++) {
                pG.DrawLine(pPen, pR.Left + 0, pR.Top + (y + 1) * pR.Height / pDivideV, pR.Left + pR.Width, pR.Top + (y + 1) * pR.Height / pDivideV);
            }
        }


        public static Bitmap MapBitmapColorsToPalette(Bitmap pTarget, Palette pPal) {
            for (int x = 0; x < pTarget.Width; x++) {
                for (int y = 0; y < pTarget.Height; y++) {
                    pTarget.SetPixel(x, y, pPal.GetNearestColor(pTarget.GetPixel(x, y)));
                }
            }

            return pTarget;
        }

        public static Bitmap CopyAndResize(Bitmap pSource, int pTargetWidth, int pTargetHeight) {
            Bitmap vTarget = new Bitmap(pTargetWidth, pTargetHeight);
            using (Graphics vG = Graphics.FromImage(vTarget)) {
                DrawingLogic.SetGraphicsNoInterpol(vG);
                vG.DrawImage(pSource, 0, 0, vTarget.Width, vTarget.Height);
            }
            return vTarget;
        }
    }
}
