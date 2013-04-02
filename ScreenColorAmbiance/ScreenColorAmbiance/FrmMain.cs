using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;

namespace ScreenColorAmbiance {
    public partial class FrmMain : Form {
        public FrmMain() {
            InitializeComponent();
        }

        private void timer1_Tick(object sender, EventArgs e) {
            //http://www.geekpedia.com/tutorial181_Capturing-screenshots-using-Csharp.html
            // Set the bitmap object to the size of the screen

            Bitmap bmpScreenshot = new Bitmap(Screen.PrimaryScreen.Bounds.Width, Screen.PrimaryScreen.Bounds.Height, PixelFormat.Format32bppArgb);

            // Create a graphics object from the bitmap

            Graphics gfxScreenshot = Graphics.FromImage(bmpScreenshot);

            // Take the screenshot from the upper left corner to the right bottom corner

            gfxScreenshot.CopyFromScreen(Screen.PrimaryScreen.Bounds.X, Screen.PrimaryScreen.Bounds.Y, 0, 0, Screen.PrimaryScreen.Bounds.Size, CopyPixelOperation.SourceCopy);


            Rectangle vRect = new Rectangle(0, 0, 100, Screen.PrimaryScreen.Bounds.Height);
            lblL.BackColor = GetZoneAverageColor(bmpScreenshot, vRect);

            vRect = new Rectangle(0, 0, Screen.PrimaryScreen.Bounds.Width, 100);
            lblT.BackColor = GetZoneAverageColor(bmpScreenshot, vRect);

            vRect = new Rectangle(Screen.PrimaryScreen.Bounds.Width - 100, 0, 100, Screen.PrimaryScreen.Bounds.Height);
            lblR.BackColor = GetZoneAverageColor(bmpScreenshot, vRect);

            bmpScreenshot.Dispose();
            gfxScreenshot.Dispose();
            GC.Collect();
            
        }

        private Color GetZoneAverageColor(Bitmap pBmp, Rectangle pRect) {
            decimal vRed = decimal.Zero;
            decimal vGreen = decimal.Zero;
            decimal vBlue = decimal.Zero;
            int vStep = 5;
            decimal vDivisor = ((decimal)pRect.Width * (decimal)pRect.Height) / ((decimal)vStep * (decimal)vStep);
            for (int x = pRect.X; x < pRect.X + pRect.Width; x += vStep) {
                for (int y = pRect.Y; y < pRect.Y + pRect.Height; y += vStep) {
                    Color vCol = pBmp.GetPixel(x, y);
                    vRed += (decimal)vCol.R / vDivisor;
                    vGreen += (decimal)vCol.G / vDivisor;
                    vBlue += (decimal)vCol.B / vDivisor;
                }
            }

           return Color.FromArgb((int)vRed, (int)vGreen, (int)vBlue);
        }

        private void FrmMain_Load(object sender, EventArgs e) {

        }
    }
}
