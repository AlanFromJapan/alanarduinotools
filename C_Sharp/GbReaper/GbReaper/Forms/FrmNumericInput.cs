using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace GbReaper.Forms {
    public partial class FrmNumericInput : Form {
        public FrmNumericInput() {
            InitializeComponent();
        }

        public int Count {
            get { return (int)nup.Value; }
            set { nup.Value = value; }
        }

        private void btnOk_Click(object sender, EventArgs e) {

        }
    }
}
