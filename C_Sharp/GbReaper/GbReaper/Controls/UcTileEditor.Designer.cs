namespace GbReaper.Controls {
    partial class UcTileEditor {
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
            this.panEdit = new System.Windows.Forms.Panel();
            this.pan8 = new System.Windows.Forms.Panel();
            this.pan16 = new System.Windows.Forms.Panel();
            this.pan32 = new System.Windows.Forms.Panel();
            this.pan64 = new System.Windows.Forms.Panel();
            this.pan128 = new System.Windows.Forms.Panel();
            this.pan128Alt = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // panEdit
            // 
            this.panEdit.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panEdit.Location = new System.Drawing.Point(0, 0);
            this.panEdit.Margin = new System.Windows.Forms.Padding(0);
            this.panEdit.Name = "panEdit";
            this.panEdit.Size = new System.Drawing.Size(256, 256);
            this.panEdit.TabIndex = 0;
            // 
            // pan8
            // 
            this.pan8.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan8.Location = new System.Drawing.Point(274, 14);
            this.pan8.Name = "pan8";
            this.pan8.Size = new System.Drawing.Size(8, 8);
            this.pan8.TabIndex = 1;
            // 
            // pan16
            // 
            this.pan16.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan16.Location = new System.Drawing.Point(288, 14);
            this.pan16.Name = "pan16";
            this.pan16.Size = new System.Drawing.Size(16, 16);
            this.pan16.TabIndex = 2;
            // 
            // pan32
            // 
            this.pan32.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan32.Location = new System.Drawing.Point(310, 14);
            this.pan32.Name = "pan32";
            this.pan32.Size = new System.Drawing.Size(32, 32);
            this.pan32.TabIndex = 2;
            // 
            // pan64
            // 
            this.pan64.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan64.Location = new System.Drawing.Point(348, 14);
            this.pan64.Name = "pan64";
            this.pan64.Size = new System.Drawing.Size(64, 64);
            this.pan64.TabIndex = 2;
            // 
            // pan128
            // 
            this.pan128.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan128.Location = new System.Drawing.Point(274, 87);
            this.pan128.Name = "pan128";
            this.pan128.Size = new System.Drawing.Size(128, 128);
            this.pan128.TabIndex = 2;
            // 
            // pan128Alt
            // 
            this.pan128Alt.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan128Alt.Location = new System.Drawing.Point(408, 87);
            this.pan128Alt.Name = "pan128Alt";
            this.pan128Alt.Size = new System.Drawing.Size(128, 128);
            this.pan128Alt.TabIndex = 2;
            // 
            // UcTileEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pan128Alt);
            this.Controls.Add(this.pan128);
            this.Controls.Add(this.pan64);
            this.Controls.Add(this.pan32);
            this.Controls.Add(this.pan16);
            this.Controls.Add(this.pan8);
            this.Controls.Add(this.panEdit);
            this.MinimumSize = new System.Drawing.Size(0, 256);
            this.Name = "UcTileEditor";
            this.Size = new System.Drawing.Size(646, 256);
            this.Load += new System.EventHandler(this.UcTileEditor_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panEdit;
        private System.Windows.Forms.Panel pan8;
        private System.Windows.Forms.Panel pan16;
        private System.Windows.Forms.Panel pan32;
        private System.Windows.Forms.Panel pan64;
        private System.Windows.Forms.Panel pan128;
        private System.Windows.Forms.Panel pan128Alt;
    }
}
