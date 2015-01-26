using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace GbReaper {
    public partial class FrmMain : Form {
        public FrmMain() {
            InitializeComponent();
        }

        const int READ_BUFFER_LENGTH = 8*2;


        private void FrmMain_Load(object sender, EventArgs e) {
            int zoomFactor = 4;
            Image vM = GetRomAsImage(@"D:\Gameboy.dev\ROMS\tetris_(v1.1)\Tetris.gb", zoomFactor);
            ucRomViewer1.Image = vM;
        }

        private Image GetRomAsImage(string pFilename, int pZoomFactor) {
            int vFileLength = (int)(new FileInfo(pFilename).Length);
            int vTileH = 40;

            using (BinaryReader vReader = new BinaryReader(new FileStream(pFilename, FileMode.Open, FileAccess.Read))) {
                byte[] vBuff = new byte[READ_BUFFER_LENGTH];
                int vReadBytes = 0;

                Bitmap vBitOutput = new Bitmap(8 * pZoomFactor * vTileH, 8 * pZoomFactor * (1 + ((vFileLength /(2 *8)) * 8 * pZoomFactor) / (8 * pZoomFactor * vTileH)));
                Graphics vGraphx = Graphics.FromImage(vBitOutput);
                vGraphx.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;

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

        private Color GetColorFromPalette(int pVal) {
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

        private void quitToolStripMenuItem_Click(object sender, EventArgs e) {
            Application.Exit();
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e) {
            if (DialogResult.OK == ofdRom.ShowDialog(this)) {
                Image vM = GetRomAsImage(ofdRom.FileName, 4);
                ucRomViewer1.Image = vM;
            }
        }

        private void fileToolStripMenuItem_Click(object sender, EventArgs e) {

        }
    }
}
