namespace GbReaper.Controls {
    partial class UcPaletteEditor {
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
            this.panColors = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // panColors
            // 
            this.panColors.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panColors.Location = new System.Drawing.Point(0, 0);
            this.panColors.Name = "panColors";
            this.panColors.Size = new System.Drawing.Size(185, 48);
            this.panColors.TabIndex = 0;
            // 
            // UcPaletteEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panColors);
            this.Name = "UcPaletteEditor";
            this.Size = new System.Drawing.Size(185, 48);
            this.Load += new System.EventHandler(this.UcPaletteEditor_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panColors;
    }
}
