namespace GbReaper.Controls {
    partial class UcRomViewer {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.vbar = new System.Windows.Forms.VScrollBar();
            this.hbar = new System.Windows.Forms.HScrollBar();
            this.SuspendLayout();
            // 
            // vbar
            // 
            this.vbar.Dock = System.Windows.Forms.DockStyle.Right;
            this.vbar.LargeChange = 80;
            this.vbar.Location = new System.Drawing.Point(308, 0);
            this.vbar.Name = "vbar";
            this.vbar.Size = new System.Drawing.Size(17, 321);
            this.vbar.SmallChange = 8;
            this.vbar.TabIndex = 0;
            this.vbar.Scroll += new System.Windows.Forms.ScrollEventHandler(this.vbar_Scroll);
            // 
            // hbar
            // 
            this.hbar.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.hbar.LargeChange = 64;
            this.hbar.Location = new System.Drawing.Point(0, 304);
            this.hbar.Name = "hbar";
            this.hbar.Size = new System.Drawing.Size(308, 17);
            this.hbar.SmallChange = 8;
            this.hbar.TabIndex = 1;
            this.hbar.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hbar_Scroll);
            // 
            // UcRomViewer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.hbar);
            this.Controls.Add(this.vbar);
            this.DoubleBuffered = true;
            this.Name = "UcRomViewer";
            this.Size = new System.Drawing.Size(325, 321);
            this.Load += new System.EventHandler(this.UcRomViewer_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.UcRomViewer_KeyDown);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.VScrollBar vbar;
        private System.Windows.Forms.HScrollBar hbar;
    }
}
