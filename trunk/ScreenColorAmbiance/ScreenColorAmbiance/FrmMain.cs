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



            decimal vRed = decimal.Zero;
            decimal vGreen = decimal.Zero;
            decimal vBlue = decimal.Zero;
            int vStep = 5;
            decimal vDivisor = ((decimal)bmpScreenshot.Width * (decimal)bmpScreenshot.Height) / ((decimal)vStep * (decimal)vStep);
            for (int x = 0; x < bmpScreenshot.Width; x += vStep) {
                for (int y = 0; y < bmpScreenshot.Height; y += vStep) {
                    Color vCol = bmpScreenshot.GetPixel(x, y);
                    vRed += (decimal)vCol.R / vDivisor;
                    vGreen += (decimal)vCol.G / vDivisor;
                    vBlue += (decimal)vCol.B / vDivisor;
                }
            }

            this.BackColor = Color.FromArgb((int)vRed, (int)vGreen, (int)vBlue);
        }
    }
}
