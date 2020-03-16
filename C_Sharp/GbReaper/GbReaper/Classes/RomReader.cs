using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.IO;

namespace GbReaper.Classes {
    static class RomReader {

        const int READ_BUFFER_LENGTH = 8 * 2;

        public static Image GetRomAsImage(string pFilename, int pZoomFactor, Palette pPalette) {
            Bitmap vBmp = (Bitmap)GetRomAsImageUnscaled(pFilename, pPalette);
            if (pZoomFactor == 1)
                return vBmp;

            Bitmap vResult = new Bitmap(vBmp.Width * pZoomFactor, vBmp.Height * pZoomFactor);
            Graphics vGraphx = Graphics.FromImage(vResult);
            DrawingLogic.SetGraphicsNoInterpol(vGraphx);

            vGraphx.DrawImage(vBmp, 0, 0, vResult.Width, vResult.Height);

            vGraphx.Dispose();
            vBmp.Dispose();

            return vResult;
        }

        public static Image GetRomAsImageUnscaled(string pFilename, Palette pPalette) {
            int vFileLength = (int)(new FileInfo(pFilename).Length);
            int vTileH = 14;

            using (BinaryReader vReader = new BinaryReader(new FileStream(pFilename, FileMode.Open, FileAccess.Read))) {
                byte[] vBuff = new byte[READ_BUFFER_LENGTH];
                int vReadBytes = 0;

                Bitmap vBitOutput = new Bitmap(8 *  vTileH, 8 *  (1 + ((vFileLength / (2 * 8)) * 8 ) / (8 * vTileH)));
                Graphics vGraphx = Graphics.FromImage(vBitOutput);
                DrawingLogic.SetGraphicsNoInterpol(vGraphx);


                Bitmap vBitBuff = new Bitmap(8, 8);

                int vCounter = 0;
                while ((vReadBytes = vReader.Read(vBuff, 0, READ_BUFFER_LENGTH)) > 0) {
                    for (int line = 0; line < 8; line++) {
                        for (int pix = 0; pix < 8; pix++) {
                            int vVal = 0;
                            vVal = vVal | (0x01 & (vBuff[2 * line] >> (8-1 - pix)));
                            vVal = vVal | (0x02 & ((vBuff[2 * line+1] >> (8-1  - pix))) << 1);

                            Color vColor = GetColorFromPalette(vVal, pPalette);
                            vBitBuff.SetPixel(pix, line, vColor);
                        }
                    }

                    if (((vCounter * 8) / vBitOutput.Width) * 8  >= vBitOutput.Height) {
                        break;
                    }

                    int x = (vCounter * 8) % vBitOutput.Width;
                    int y = 8  * ((vCounter * 8) / vBitOutput.Width);

                    vGraphx.DrawImageUnscaled(vBitBuff, x, y);


                    vCounter++;
                }

                vGraphx.Dispose();

                return vBitOutput;
            }
        }

        private static Color GetColorFromPalette(int pVal, Palette pPalette) {
            return pPalette.mColors[pVal];
        }
    }
}
