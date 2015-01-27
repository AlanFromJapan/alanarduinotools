using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.IO;

namespace GbReaper.Classes {
    static class RomReader {

        const int READ_BUFFER_LENGTH = 8 * 2;

        public static Image GetRomAsImage(string pFilename, int pZoomFactor) {
            int vFileLength = (int)(new FileInfo(pFilename).Length);
            int vTileH = 40;

            using (BinaryReader vReader = new BinaryReader(new FileStream(pFilename, FileMode.Open, FileAccess.Read))) {
                byte[] vBuff = new byte[READ_BUFFER_LENGTH];
                int vReadBytes = 0;

                Bitmap vBitOutput = new Bitmap(8 * pZoomFactor * vTileH, 8 * pZoomFactor * (1 + ((vFileLength / (2 * 8)) * 8 * pZoomFactor) / (8 * pZoomFactor * vTileH)));
                Graphics vGraphx = Graphics.FromImage(vBitOutput);
                vGraphx.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
                vGraphx.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.Half;
                vGraphx.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
                vGraphx.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.None;


                Bitmap vBitBuff = new Bitmap(8, 8);

                int vCounter = 0;
                while ((vReadBytes = vReader.Read(vBuff, 0, READ_BUFFER_LENGTH)) > 0) {
                    for (int line = 0; line < 8; line++) {
                        for (int pix = 0; pix < 8; pix++) {
                            int vVal = 0;
                            vVal = vVal | (0x01 & (vBuff[2 * line] >> (8 - 1 - pix)));
                            vVal = vVal | (0x02 & (vBuff[2 * line + 1] >> (8 - 1 - pix)));

                            Color vColor = GetColorFromPalette(vVal);
                            vBitBuff.SetPixel(pix, line, vColor);
                        }
                    }

                    if (((vCounter * 8 * pZoomFactor) / vBitOutput.Width) * 8 * pZoomFactor >= vBitOutput.Height) {
                        break;
                    }

                    int x = (vCounter * 8 * pZoomFactor) % vBitOutput.Width;
                    int y = 8 * pZoomFactor * ((vCounter * 8 * pZoomFactor) / vBitOutput.Width);

                    vGraphx.DrawImage(vBitBuff, x, y, 8 * pZoomFactor, 8 * pZoomFactor);


                    vCounter++;
                }

                vGraphx.Dispose();

                return vBitOutput;
            }
        }

        public static Image GetRomAsImageUnscaled(string pFilename) {
            int vFileLength = (int)(new FileInfo(pFilename).Length);
            int vTileH = 40;

            using (BinaryReader vReader = new BinaryReader(new FileStream(pFilename, FileMode.Open, FileAccess.Read))) {
                byte[] vBuff = new byte[READ_BUFFER_LENGTH];
                int vReadBytes = 0;

                Bitmap vBitOutput = new Bitmap(8 *  vTileH, 8 *  (1 + ((vFileLength / (2 * 8)) * 8 ) / (8 * vTileH)));
                Graphics vGraphx = Graphics.FromImage(vBitOutput);
                vGraphx.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
                vGraphx.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.Half;
                vGraphx.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
                vGraphx.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.None;


                Bitmap vBitBuff = new Bitmap(8, 8);

                int vCounter = 0;
                while ((vReadBytes = vReader.Read(vBuff, 0, READ_BUFFER_LENGTH)) > 0) {
                    for (int line = 0; line < 8; line++) {
                        for (int pix = 0; pix < 8; pix++) {
                            int vVal = 0;
                            vVal = vVal | (0x01 & (vBuff[2 * line] >> (8-1 - pix)));
                            vVal = vVal | (0x02 & ((vBuff[2 * line+1] >> (8-1  - pix))) << 1);

                            Color vColor = GetColorFromPalette(vVal);
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

        private static Color GetColorFromPalette(int pVal) {
            switch (pVal) {
                case 1:
                    return Color.LightGray;
                case 2:
                    return Color.Gray;
                case 3:
                    return Color.DarkGray;
                default:
                    break;
            }

            return Color.White;
        }
    }
}
