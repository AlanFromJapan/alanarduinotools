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
            this.components = new System.ComponentModel.Container();
            this.panTools = new System.Windows.Forms.Panel();
            this.btnRight = new System.Windows.Forms.Button();
            this.btnLeft = new System.Windows.Forms.Button();
            this.ctxColors = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.color0ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.color1ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.color2ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.color3ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pan128Alt = new GbReaper.Controls.PanelX();
            this.pan128 = new GbReaper.Controls.PanelX();
            this.pan64 = new GbReaper.Controls.PanelX();
            this.pan32 = new GbReaper.Controls.PanelX();
            this.pan16 = new GbReaper.Controls.PanelX();
            this.pan8 = new GbReaper.Controls.PanelX();
            this.panEdit = new GbReaper.Controls.PanelX();
            this.panTools.SuspendLayout();
            this.ctxColors.SuspendLayout();
            this.SuspendLayout();
            // 
            // panTools
            // 
            this.panTools.BackColor = System.Drawing.SystemColors.ControlLight;
            this.panTools.Controls.Add(this.btnRight);
            this.panTools.Controls.Add(this.btnLeft);
            this.panTools.Dock = System.Windows.Forms.DockStyle.Left;
            this.panTools.Location = new System.Drawing.Point(0, 0);
            this.panTools.Name = "panTools";
            this.panTools.Size = new System.Drawing.Size(32, 277);
            this.panTools.TabIndex = 3;
            // 
            // btnRight
            // 
            this.btnRight.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnRight.Location = new System.Drawing.Point(0, 32);
            this.btnRight.Name = "btnRight";
            this.btnRight.Size = new System.Drawing.Size(32, 35);
            this.btnRight.TabIndex = 1;
            this.btnRight.Text = "R";
            this.btnRight.UseVisualStyleBackColor = true;
            this.btnRight.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnRight_MouseUp);
            // 
            // btnLeft
            // 
            this.btnLeft.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnLeft.Location = new System.Drawing.Point(0, 0);
            this.btnLeft.Name = "btnLeft";
            this.btnLeft.Size = new System.Drawing.Size(32, 35);
            this.btnLeft.TabIndex = 0;
            this.btnLeft.Text = "L";
            this.btnLeft.UseVisualStyleBackColor = true;
            this.btnLeft.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnLeft_MouseUp);
            // 
            // ctxColors
            // 
            this.ctxColors.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.color0ToolStripMenuItem,
            this.color1ToolStripMenuItem,
            this.color2ToolStripMenuItem,
            this.color3ToolStripMenuItem});
            this.ctxColors.Name = "ctxColors";
            this.ctxColors.Size = new System.Drawing.Size(120, 92);
            this.ctxColors.Opening += new System.ComponentModel.CancelEventHandler(this.ctxColors_Opening);
            this.ctxColors.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.ctxColors_ItemClicked);
            // 
            // color0ToolStripMenuItem
            // 
            this.color0ToolStripMenuItem.Name = "color0ToolStripMenuItem";
            this.color0ToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.color0ToolStripMenuItem.Text = "Color #0";
            // 
            // color1ToolStripMenuItem
            // 
            this.color1ToolStripMenuItem.Name = "color1ToolStripMenuItem";
            this.color1ToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.color1ToolStripMenuItem.Text = "Color #1";
            // 
            // color2ToolStripMenuItem
            // 
            this.color2ToolStripMenuItem.Name = "color2ToolStripMenuItem";
            this.color2ToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.color2ToolStripMenuItem.Text = "Color #2";
            // 
            // color3ToolStripMenuItem
            // 
            this.color3ToolStripMenuItem.Name = "color3ToolStripMenuItem";
            this.color3ToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.color3ToolStripMenuItem.Text = "Color #3";
            // 
            // pan128Alt
            // 
            this.pan128Alt.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan128Alt.Location = new System.Drawing.Point(441, 94);
            this.pan128Alt.Name = "pan128Alt";
            this.pan128Alt.Size = new System.Drawing.Size(128, 128);
            this.pan128Alt.TabIndex = 2;
            // 
            // pan128
            // 
            this.pan128.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan128.Location = new System.Drawing.Point(307, 94);
            this.pan128.Name = "pan128";
            this.pan128.Size = new System.Drawing.Size(128, 128);
            this.pan128.TabIndex = 2;
            // 
            // pan64
            // 
            this.pan64.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan64.Location = new System.Drawing.Point(381, 15);
            this.pan64.Name = "pan64";
            this.pan64.Size = new System.Drawing.Size(64, 64);
            this.pan64.TabIndex = 2;
            // 
            // pan32
            // 
            this.pan32.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan32.Location = new System.Drawing.Point(343, 15);
            this.pan32.Name = "pan32";
            this.pan32.Size = new System.Drawing.Size(32, 32);
            this.pan32.TabIndex = 2;
            // 
            // pan16
            // 
            this.pan16.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan16.Location = new System.Drawing.Point(321, 15);
            this.pan16.Name = "pan16";
            this.pan16.Size = new System.Drawing.Size(16, 16);
            this.pan16.TabIndex = 2;
            // 
            // pan8
            // 
            this.pan8.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan8.Location = new System.Drawing.Point(307, 15);
            this.pan8.Name = "pan8";
            this.pan8.Size = new System.Drawing.Size(8, 8);
            this.pan8.TabIndex = 1;
            // 
            // panEdit
            // 
            this.panEdit.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panEdit.Location = new System.Drawing.Point(33, 0);
            this.panEdit.Margin = new System.Windows.Forms.Padding(0);
            this.panEdit.Name = "panEdit";
            this.panEdit.Size = new System.Drawing.Size(256, 256);
            this.panEdit.TabIndex = 0;
            // 
            // UcTileEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panTools);
            this.Controls.Add(this.pan128Alt);
            this.Controls.Add(this.pan128);
            this.Controls.Add(this.pan64);
            this.Controls.Add(this.pan32);
            this.Controls.Add(this.pan16);
            this.Controls.Add(this.pan8);
            this.Controls.Add(this.panEdit);
            this.MinimumSize = new System.Drawing.Size(0, 277);
            this.Name = "UcTileEditor";
            this.Size = new System.Drawing.Size(646, 277);
            this.Load += new System.EventHandler(this.UcTileEditor_Load);
            this.panTools.ResumeLayout(false);
            this.ctxColors.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panTools;
        private System.Windows.Forms.Button btnRight;
        private System.Windows.Forms.Button btnLeft;
        private PanelX panEdit;
        private PanelX pan8;
        private PanelX pan16;
        private PanelX pan32;
        private PanelX pan64;
        private PanelX pan128;
        private PanelX pan128Alt;
        private System.Windows.Forms.ContextMenuStrip ctxColors;
        private System.Windows.Forms.ToolStripMenuItem color0ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem color1ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem color2ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem color3ToolStripMenuItem;
    }
}
