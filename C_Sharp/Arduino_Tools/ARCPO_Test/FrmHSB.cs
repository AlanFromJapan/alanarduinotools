using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Arduino_Tools {
    public partial class FrmHSB : Form {
        public FrmHSB() {
            InitializeComponent();
        }

        private void FrmHSB_Load(object sender, EventArgs e) {
            Bitmap vBmp = new Bitmap(600, 600);
            Graphics vG = Graphics.FromImage(vBmp);

            //http://en.wikipedia.org/wiki/HSL_color_space

            for (int x = 0; x < vBmp.Width; x++) {
                for (int y = 0; y < vBmp.Height; y++) {
                    double vBrightenss = 0.4;
                    double vHue = ((x * 360.0) / (double)vBmp.Width) / 360.0;
                    double vSat = y / (double)vBmp.Height;

                    double q = 0.0;
                    if (vBrightenss < 0.5) {
                        q = vBrightenss * (1 + vSat);
                    }
                    else {
                        q = vBrightenss + vSat - (vBrightenss * vSat);
                    }

                    double p = 2.0 * vBrightenss - q;
                    double tr = vHue + 1.0 / 3.0;
                    double tg = vHue;
                    double tb = vHue - 1.0/3.0;

                    vBmp.SetPixel (x, y, Color.FromArgb (
                        GetCompo(p, q, tr),
                        GetCompo(p, q, tg),
                        GetCompo(p, q, tb)
                    ));

                }
            }

            vBmp.Save("hsb.jpg");
            pictureBox1.Image = vBmp;
        }

        private int GetCompo(double p, double q, double tc) {
            double vVal = 0.0;
            tc = (tc < 0 ? tc + 1.0 : (tc > 1 ? tc - 1.0 : tc));

            if (tc < 1.0 / 6.0) {
                vVal = p + ((q - p) * 6.0 * tc);
            }
            else {
                if (tc < 0.5) {
                    vVal = q;
                }
                else {
                    if (tc < 2.0 / 3.0) {
                        vVal = p + ((q - p) * 6.0 * (2.0 / 3.0 - tc));
                    }
                    else {
                        vVal = p;
                    }
                }

            }
            return (int)(vVal * 255.0);
        }
    }
}