using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Dessinaka {
    public class MyPictureBox : PictureBox{
        public MyPictureBox() : base() { }

        protected override void OnPaintBackground(PaintEventArgs pevent) {
            base.OnPaintBackground(pevent);

            Graphics vG = pevent.Graphics;
            LinearGradientBrush vB = new LinearGradientBrush(this.Bounds, Color.Yellow, Color.Violet, 90f);
                vG.FillRectangle(vB, this.Bounds);

                vG.DrawString("Press Shift+F12 to quit. Any other key clears screen.", SystemFonts.DefaultFont, Brushes.Gray, new PointF(10f, 10f));
                 
        }

        public void ForceRepaintBackground() {
            this.Invalidate();
        }
    }
}
