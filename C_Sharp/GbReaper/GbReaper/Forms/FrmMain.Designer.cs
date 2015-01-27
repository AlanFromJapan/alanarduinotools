namespace GbReaper {
    partial class FrmMain {
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.quitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ofdRom = new System.Windows.Forms.OpenFileDialog();
            this.panMain = new System.Windows.Forms.Panel();
            this.ucSpriteEd = new GbReaper.Controls.UcSpriteEditor();
            this.panLib = new System.Windows.Forms.Panel();
            this.ucLibView = new GbReaper.Controls.UcLibraryList();
            this.ucRomViewer1 = new GbReaper.Controls.UcRomViewer();
            this.menuStrip1.SuspendLayout();
            this.panMain.SuspendLayout();
            this.panLib.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1052, 26);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.toolStripMenuItem1,
            this.quitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(40, 22);
            this.fileToolStripMenuItem.Text = "&File";
            this.fileToolStripMenuItem.Click += new System.EventHandler(this.fileToolStripMenuItem_Click);
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F4;
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.loadToolStripMenuItem.Text = "&Load";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(145, 6);
            // 
            // quitToolStripMenuItem
            // 
            this.quitToolStripMenuItem.Name = "quitToolStripMenuItem";
            this.quitToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Q)));
            this.quitToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.quitToolStripMenuItem.Text = "&Quit";
            this.quitToolStripMenuItem.Click += new System.EventHandler(this.quitToolStripMenuItem_Click);
            // 
            // ofdRom
            // 
            this.ofdRom.Filter = "Game boy roms (*.gb)|*.gb|All files|*.*";
            this.ofdRom.Title = "Load a ROM";
            // 
            // panMain
            // 
            this.panMain.Controls.Add(this.ucSpriteEd);
            this.panMain.Controls.Add(this.panLib);
            this.panMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panMain.Location = new System.Drawing.Point(258, 26);
            this.panMain.Name = "panMain";
            this.panMain.Size = new System.Drawing.Size(794, 622);
            this.panMain.TabIndex = 4;
            // 
            // ucSpriteEd
            // 
            this.ucSpriteEd.Dock = System.Windows.Forms.DockStyle.Top;
            this.ucSpriteEd.Location = new System.Drawing.Point(0, 0);
            this.ucSpriteEd.MinimumSize = new System.Drawing.Size(0, 256);
            this.ucSpriteEd.Name = "ucSpriteEd";
            this.ucSpriteEd.Size = new System.Drawing.Size(594, 256);
            this.ucSpriteEd.TabIndex = 1;
            // 
            // panLib
            // 
            this.panLib.Controls.Add(this.ucLibView);
            this.panLib.Dock = System.Windows.Forms.DockStyle.Right;
            this.panLib.Location = new System.Drawing.Point(594, 0);
            this.panLib.Name = "panLib";
            this.panLib.Size = new System.Drawing.Size(200, 622);
            this.panLib.TabIndex = 0;
            // 
            // ucLibView
            // 
            this.ucLibView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ucLibView.Location = new System.Drawing.Point(0, 0);
            this.ucLibView.Name = "ucLibView";
            this.ucLibView.Size = new System.Drawing.Size(200, 622);
            this.ucLibView.TabIndex = 0;
            // 
            // ucRomViewer1
            // 
            this.ucRomViewer1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ucRomViewer1.Dock = System.Windows.Forms.DockStyle.Left;
            this.ucRomViewer1.Location = new System.Drawing.Point(0, 26);
            this.ucRomViewer1.Name = "ucRomViewer1";
            this.ucRomViewer1.Size = new System.Drawing.Size(258, 622);
            this.ucRomViewer1.TabIndex = 3;
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1052, 648);
            this.Controls.Add(this.panMain);
            this.Controls.Add(this.ucRomViewer1);
            this.Controls.Add(this.menuStrip1);
            this.DoubleBuffered = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "FrmMain";
            this.Text = "GB Reaper";
            this.Load += new System.EventHandler(this.FrmMain_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.panMain.ResumeLayout(false);
            this.panLib.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem quitToolStripMenuItem;
        private System.Windows.Forms.OpenFileDialog ofdRom;
        private Controls.UcRomViewer ucRomViewer1;
        private System.Windows.Forms.Panel panMain;
        private System.Windows.Forms.Panel panLib;
        private Controls.UcLibraryList ucLibView;
        private Controls.UcSpriteEditor ucSpriteEd;
    }
}

