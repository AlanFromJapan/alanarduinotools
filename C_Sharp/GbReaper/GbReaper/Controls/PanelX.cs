using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace GbReaper.Controls {
    public class PanelX : Panel{
        public PanelX () :base(){
            
        }

        protected override void OnCreateControl() {
            base.OnCreateControl();

            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            DoubleBuffered = true;
        }
    }
}
