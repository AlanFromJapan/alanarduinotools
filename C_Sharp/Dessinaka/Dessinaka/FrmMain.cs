using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Dessinaka {
    public partial class FrmMain : Form {
        #region attributes
        protected Point mLastPoint = Point.Empty;
        #endregion

        protected Brush BrushLeft {
            get { return Brushes.CadetBlue; }
        }
        protected Brush BrushRight {
            get { return Brushes.IndianRed; }
        }


        public FrmMain() {
            InitializeComponent();
        }

       

        private void FrmMain_Load(object sender, EventArgs e) {

            //all key strokes are first received by the form
            this.KeyPreview = true;

            this.KeyDown += FrmMain_KeyDown;

            GoFullscreen(true);

            
        }



        /// <summary>
        /// Handles key strokes to quit application
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void FrmMain_KeyDown(object sender, KeyEventArgs e) {
            if (e.Shift && e.KeyCode == Keys.F12) {
                Application.Exit();
            }

            pbxMain.ForceRepaintBackground();
        }

        private void pbxMain_MouseMove(object sender, MouseEventArgs e) {

            if (e.Button == System.Windows.Forms.MouseButtons.Left || e.Button == System.Windows.Forms.MouseButtons.Right) {
                //memorize last point clicked
                Point vPrevPoint = mLastPoint;
                mLastPoint = new Point(e.X, e.Y);

                //make a line?
                if (vPrevPoint != Point.Empty) {
                    PictureBox vPbx = (PictureBox)sender;
                    Brush vBrush = (e.Button == System.Windows.Forms.MouseButtons.Left ? BrushLeft : BrushRight);
                    using (Graphics vG = vPbx.CreateGraphics()) {
                        Pen vP = new Pen(vBrush, 10f);
                        vP.StartCap = LineCap.Round;
                        vP.EndCap = LineCap.Round;
                        vP.DashStyle = DashStyle.Solid;
                        vP.LineJoin = LineJoin.Round;
                        
                        vG.DrawLine(vP, vPrevPoint, mLastPoint);
                    }
                }
                //draws some balls ... nice but lines should be better
                //using (Graphics vG = vPbx.CreateGraphics()) {
                //    vG.FillEllipse(vBrush, e.X, e.Y, 10, 10);
                //}


            }
        }

        private void pbxMain_MouseUp(object sender, MouseEventArgs e) {
            mLastPoint = Point.Empty;
        }

        private void pbxMain_MouseLeave(object sender, EventArgs e) {
            mLastPoint = Point.Empty;
        }

        private void pbxMain_MouseDown(object sender, MouseEventArgs e) {
            mLastPoint = new Point(e.X, e.Y);
        }


        /// <summary>
        /// http://stackoverflow.com/questions/2272019/how-to-display-a-windows-form-in-full-screen-on-top-of-the-taskbar
        /// </summary>
        /// <param name="fullscreen"></param>
        private void GoFullscreen(bool fullscreen) {
            if (fullscreen) {
                this.WindowState = FormWindowState.Normal;
                this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
                this.Bounds = Screen.PrimaryScreen.Bounds;
            }
            else {
                this.WindowState = FormWindowState.Maximized;
                this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Sizable;
            }
        }




    }
}
