using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace GbReaper.Controls {
    public partial class UcRomViewer : UserControl {

        protected Image mImage = null;
        protected Point mMouseHover = Point.Empty;

        public Image Image {
            set { 
                this.mImage = value;
                this.UpdateSidebars();
                this.Invalidate();
            }
        }


        public UcRomViewer() {
            InitializeComponent();
        }

        private void vbar_Scroll(object sender, ScrollEventArgs e) {
            this.Invalidate();
        }
        private void hbar_Scroll(object sender, ScrollEventArgs e) {
            this.Invalidate();
        }

        private void UcRomViewer_Load(object sender, EventArgs e) {

        }

        protected override void OnPaint(PaintEventArgs e) {
            base.OnPaint(e);

            if (mImage == null)
                return;

            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            
            e.Graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            e.Graphics.DrawImage(this.mImage, -hbar.Value, -vbar.Value);//, e.ClipRectangle.Width - vbar.Width, e.ClipRectangle.Height - hbar.Height);

            if (!this.mMouseHover.IsEmpty) {
                e.Graphics.DrawRectangle(Pens.Red, this.mMouseHover.X, this.mMouseHover.Y, 8, 8);
            }
        }

        private void UpdateSidebars() {
            if (mImage == null)
                return;

            vbar.Minimum = 0;
            vbar.Maximum = this.mImage.Height - this.Height + hbar.Height;
            if (vbar.Value > vbar.Maximum)
                vbar.Value = vbar.Maximum;

            hbar.Minimum = 0;
            hbar.Maximum = this.mImage.Width - this.Width + vbar.Width;
            if (hbar.Value > hbar.Maximum)
                hbar.Value = hbar.Maximum;

        }

        protected override void OnResize(EventArgs e) {
            base.OnResize(e);

            UpdateSidebars();

            this.Invalidate();
        }
        protected override void OnMouseMove(MouseEventArgs e) {
            base.OnMouseMove(e);
            this.mMouseHover = this.PointToClient(Cursor.Position);


            this.Invalidate();
        }

    }
}
