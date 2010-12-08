using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Arduino_Tools
{
    public partial class FrmColorMixerTest : Form
    {
        public FrmColorMixerTest()
        {
            InitializeComponent();
        }

        private void FrmColorMixerTest_Load(object sender, EventArgs e)
        {
            Bitmap vBmp = new Bitmap(300, 300);
            Graphics vG = Graphics.FromImage(vBmp);

            Point vPointRed = new Point(0, 0);
            Point vPointBlue = new Point(0, vBmp.Height);
            Point vPointGreen = new Point(vBmp.Width, 0);

            double vMaxDist = vBmp.Width * 0.9;// Math.Sqrt(vBmp.Width * vBmp.Width + vBmp.Height * vBmp.Height);

            for (int x = 0; x < vBmp.Width; x++) {
                for (int y = 0; y < vBmp.Height; y++)
                {
                    int vRed =      255 - (int)Math.Min(255.0, 255.0 * Math.Sqrt(Math.Pow(Math.Abs(x - vPointRed.X), 2) + Math.Pow(Math.Abs(y - vPointRed.Y), 2)) / vMaxDist);
                    int vGreen =    255 - (int)Math.Min(255.0, 255.0 * Math.Sqrt(Math.Pow(Math.Abs(x - vPointGreen.X), 2) + Math.Pow(Math.Abs(y - vPointGreen.Y), 2)) / vMaxDist);
                    int vBlue =     255 - (int)Math.Min(255.0, 255.0 * Math.Sqrt(Math.Pow(Math.Abs(x - vPointBlue.X), 2) + Math.Pow(Math.Abs(y - vPointBlue.Y), 2)) / vMaxDist);

                    vBmp.SetPixel(x, y, Color.FromArgb(vRed, vGreen, vBlue));
                }
            
            }

            pictureBox1.Image = vBmp;
        }

        private void FrmColorMixerTest_MouseMove(object sender, MouseEventArgs e)
        {

        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            Point vP = e.Location;// pictureBox1.PointToClient(e.Location);

            Color vC = (pictureBox1.Image as Bitmap).GetPixel(vP.X, vP.Y);

            this.Text = string.Format("({0}, {1}, {2})", vC.R, vC.G, vC.B);
        }
    }
}