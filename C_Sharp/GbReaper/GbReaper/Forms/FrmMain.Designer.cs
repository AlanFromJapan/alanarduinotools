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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmMain));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.quitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ofdRom = new System.Windows.Forms.OpenFileDialog();
            this.panMain = new System.Windows.Forms.Panel();
            this.tabMaps = new System.Windows.Forms.TabControl();
            this.panLib = new System.Windows.Forms.Panel();
            this.panLeft = new System.Windows.Forms.Panel();
            this.panLeftBottom = new System.Windows.Forms.Panel();
            this.pan128Alt = new System.Windows.Forms.Panel();
            this.pan32 = new System.Windows.Forms.Panel();
            this.sfdProject = new System.Windows.Forms.SaveFileDialog();
            this.ucTileEd = new GbReaper.Controls.UcTileEditor();
            this.ucLibView = new GbReaper.Controls.UcLibraryList();
            this.ucRomViewer1 = new GbReaper.Controls.UcRomViewer();
            this.tpMapWelcome = new System.Windows.Forms.TabPage();
            this.btnNewMap = new System.Windows.Forms.Button();
            this.menuStrip1.SuspendLayout();
            this.panMain.SuspendLayout();
            this.tabMaps.SuspendLayout();
            this.panLib.SuspendLayout();
            this.panLeft.SuspendLayout();
            this.panLeftBottom.SuspendLayout();
            this.tpMapWelcome.SuspendLayout();
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
            this.saveAsToolStripMenuItem,
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
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.loadToolStripMenuItem.Text = "&Load";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F12;
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.saveAsToolStripMenuItem.Text = "Save as ...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(164, 6);
            // 
            // quitToolStripMenuItem
            // 
            this.quitToolStripMenuItem.Name = "quitToolStripMenuItem";
            this.quitToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Q)));
            this.quitToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
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
            this.panMain.Controls.Add(this.tabMaps);
            this.panMain.Controls.Add(this.ucTileEd);
            this.panMain.Controls.Add(this.panLib);
            this.panMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panMain.Location = new System.Drawing.Point(252, 26);
            this.panMain.Name = "panMain";
            this.panMain.Size = new System.Drawing.Size(800, 747);
            this.panMain.TabIndex = 4;
            // 
            // tabMaps
            // 
            this.tabMaps.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this.tabMaps.Controls.Add(this.tpMapWelcome);
            this.tabMaps.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabMaps.Location = new System.Drawing.Point(0, 256);
            this.tabMaps.Name = "tabMaps";
            this.tabMaps.SelectedIndex = 0;
            this.tabMaps.Size = new System.Drawing.Size(600, 491);
            this.tabMaps.TabIndex = 3;
            // 
            // panLib
            // 
            this.panLib.Controls.Add(this.ucLibView);
            this.panLib.Dock = System.Windows.Forms.DockStyle.Right;
            this.panLib.Location = new System.Drawing.Point(600, 0);
            this.panLib.Name = "panLib";
            this.panLib.Size = new System.Drawing.Size(200, 747);
            this.panLib.TabIndex = 0;
            // 
            // panLeft
            // 
            this.panLeft.Controls.Add(this.ucRomViewer1);
            this.panLeft.Controls.Add(this.panLeftBottom);
            this.panLeft.Dock = System.Windows.Forms.DockStyle.Left;
            this.panLeft.Location = new System.Drawing.Point(0, 26);
            this.panLeft.Name = "panLeft";
            this.panLeft.Size = new System.Drawing.Size(252, 747);
            this.panLeft.TabIndex = 5;
            // 
            // panLeftBottom
            // 
            this.panLeftBottom.Controls.Add(this.pan128Alt);
            this.panLeftBottom.Controls.Add(this.pan32);
            this.panLeftBottom.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panLeftBottom.Location = new System.Drawing.Point(0, 616);
            this.panLeftBottom.Name = "panLeftBottom";
            this.panLeftBottom.Size = new System.Drawing.Size(252, 131);
            this.panLeftBottom.TabIndex = 4;
            // 
            // pan128Alt
            // 
            this.pan128Alt.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan128Alt.Location = new System.Drawing.Point(62, 1);
            this.pan128Alt.Name = "pan128Alt";
            this.pan128Alt.Size = new System.Drawing.Size(128, 128);
            this.pan128Alt.TabIndex = 4;
            // 
            // pan32
            // 
            this.pan32.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan32.Location = new System.Drawing.Point(12, 6);
            this.pan32.Name = "pan32";
            this.pan32.Size = new System.Drawing.Size(32, 32);
            this.pan32.TabIndex = 3;
            // 
            // sfdProject
            // 
            this.sfdProject.DefaultExt = "gbxml";
            this.sfdProject.Filter = "GB Reaper project (*.gbxml)|*.gbxml|All files|*.*";
            this.sfdProject.Title = "Where to save the project?";
            // 
            // ucTileEd
            // 
            this.ucTileEd.Dock = System.Windows.Forms.DockStyle.Top;
            this.ucTileEd.Location = new System.Drawing.Point(0, 0);
            this.ucTileEd.MinimumSize = new System.Drawing.Size(0, 256);
            this.ucTileEd.Name = "ucTileEd";
            this.ucTileEd.Size = new System.Drawing.Size(600, 256);
            this.ucTileEd.TabIndex = 1;
            // 
            // ucLibView
            // 
            this.ucLibView.Dock = System.Windows.Forms.DockStyle.Right;
            this.ucLibView.Location = new System.Drawing.Point(0, 0);
            this.ucLibView.Name = "ucLibView";
            this.ucLibView.Size = new System.Drawing.Size(200, 747);
            this.ucLibView.TabIndex = 0;
            // 
            // ucRomViewer1
            // 
            this.ucRomViewer1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ucRomViewer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ucRomViewer1.Location = new System.Drawing.Point(0, 0);
            this.ucRomViewer1.Name = "ucRomViewer1";
            this.ucRomViewer1.Size = new System.Drawing.Size(252, 616);
            this.ucRomViewer1.TabIndex = 3;
            // 
            // tpMapWelcome
            // 
            this.tpMapWelcome.Controls.Add(this.btnNewMap);
            this.tpMapWelcome.Location = new System.Drawing.Point(4, 4);
            this.tpMapWelcome.Name = "tpMapWelcome";
            this.tpMapWelcome.Size = new System.Drawing.Size(592, 465);
            this.tpMapWelcome.TabIndex = 0;
            this.tpMapWelcome.Text = "Maps";
            this.tpMapWelcome.UseVisualStyleBackColor = true;
            // 
            // btnNewMap
            // 
            this.btnNewMap.Image = ((System.Drawing.Image)(resources.GetObject("btnNewMap.Image")));
            this.btnNewMap.Location = new System.Drawing.Point(187, 187);
            this.btnNewMap.Name = "btnNewMap";
            this.btnNewMap.Size = new System.Drawing.Size(230, 110);
            this.btnNewMap.TabIndex = 0;
            this.btnNewMap.Text = "Create new map...";
            this.btnNewMap.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnNewMap.UseVisualStyleBackColor = true;
            this.btnNewMap.Click += new System.EventHandler(this.btnNewMap_Click);
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1052, 773);
            this.Controls.Add(this.panMain);
            this.Controls.Add(this.panLeft);
            this.Controls.Add(this.menuStrip1);
            this.DoubleBuffered = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "FrmMain";
            this.Text = "GB Reaper";
            this.Load += new System.EventHandler(this.FrmMain_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.panMain.ResumeLayout(false);
            this.tabMaps.ResumeLayout(false);
            this.panLib.ResumeLayout(false);
            this.panLeft.ResumeLayout(false);
            this.panLeftBottom.ResumeLayout(false);
            this.tpMapWelcome.ResumeLayout(false);
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
        private Controls.UcTileEditor ucTileEd;
        private System.Windows.Forms.Panel panLeft;
        private System.Windows.Forms.Panel panLeftBottom;
        private System.Windows.Forms.Panel pan32;
        private System.Windows.Forms.Panel pan128Alt;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog sfdProject;
        private System.Windows.Forms.TabControl tabMaps;
        private System.Windows.Forms.TabPage tpMapWelcome;
        private System.Windows.Forms.Button btnNewMap;
    }
}

