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
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.loadProjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.quitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mapsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createMapsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.horizontalToVerticalWhenROMImportBy4ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.askTileNameOnImportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.allowImportOfDuplicateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportForGBDKToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.generateStubMaincToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportGridOnTheMapsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeUnusedTilesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutGbReaperToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cbxPalette = new System.Windows.Forms.ToolStripComboBox();
            this.ofdRom = new System.Windows.Forms.OpenFileDialog();
            this.panMain = new System.Windows.Forms.Panel();
            this.tabMaps = new System.Windows.Forms.TabControl();
            this.tpMapWelcome = new System.Windows.Forms.TabPage();
            this.btnNewMap = new System.Windows.Forms.Button();
            this.ucTileEd = new GbReaper.Controls.UcTileEditor();
            this.panLib = new System.Windows.Forms.Panel();
            this.ucLibView = new GbReaper.Controls.UcLibraryList();
            this.panLeft = new System.Windows.Forms.Panel();
            this.ucRomViewer1 = new GbReaper.Controls.UcRomViewer();
            this.lblRomMemo = new System.Windows.Forms.Label();
            this.panLeftBottom = new System.Windows.Forms.Panel();
            this.pan128Alt = new System.Windows.Forms.Panel();
            this.pan32 = new System.Windows.Forms.Panel();
            this.sfdProject = new System.Windows.Forms.SaveFileDialog();
            this.strip = new System.Windows.Forms.StatusStrip();
            this.stripLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.ofdProject = new System.Windows.Forms.OpenFileDialog();
            this.fbdExport = new System.Windows.Forms.FolderBrowserDialog();
            this.reloadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.panMain.SuspendLayout();
            this.tabMaps.SuspendLayout();
            this.tpMapWelcome.SuspendLayout();
            this.panLib.SuspendLayout();
            this.panLeft.SuspendLayout();
            this.panLeftBottom.SuspendLayout();
            this.strip.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.mapsToolStripMenuItem,
            this.importToolStripMenuItem,
            this.exportToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.helpToolStripMenuItem,
            this.cbxPalette});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1054, 27);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.toolStripMenuItem2,
            this.loadProjectToolStripMenuItem,
            this.reloadToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.toolStripMenuItem1,
            this.quitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 23);
            this.fileToolStripMenuItem.Text = "&File";
            this.fileToolStripMenuItem.Click += new System.EventHandler(this.fileToolStripMenuItem_Click);
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F4;
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
            this.loadToolStripMenuItem.Text = "&Load ROM";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(186, 6);
            // 
            // loadProjectToolStripMenuItem
            // 
            this.loadProjectToolStripMenuItem.Name = "loadProjectToolStripMenuItem";
            this.loadProjectToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F8;
            this.loadProjectToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
            this.loadProjectToolStripMenuItem.Text = "Load project";
            this.loadProjectToolStripMenuItem.Click += new System.EventHandler(this.loadProjectToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F11;
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
            this.saveToolStripMenuItem.Text = "Save project";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F12;
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
            this.saveAsToolStripMenuItem.Text = "Save project as ...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(186, 6);
            // 
            // quitToolStripMenuItem
            // 
            this.quitToolStripMenuItem.Name = "quitToolStripMenuItem";
            this.quitToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Q)));
            this.quitToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
            this.quitToolStripMenuItem.Text = "&Quit";
            this.quitToolStripMenuItem.Click += new System.EventHandler(this.quitToolStripMenuItem_Click);
            // 
            // mapsToolStripMenuItem
            // 
            this.mapsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.createMapsToolStripMenuItem});
            this.mapsToolStripMenuItem.Name = "mapsToolStripMenuItem";
            this.mapsToolStripMenuItem.Size = new System.Drawing.Size(48, 23);
            this.mapsToolStripMenuItem.Text = "&Maps";
            // 
            // createMapsToolStripMenuItem
            // 
            this.createMapsToolStripMenuItem.Name = "createMapsToolStripMenuItem";
            this.createMapsToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.M)));
            this.createMapsToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.createMapsToolStripMenuItem.Text = "&Create map";
            this.createMapsToolStripMenuItem.Click += new System.EventHandler(this.createMapsToolStripMenuItem_Click);
            // 
            // importToolStripMenuItem
            // 
            this.importToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.horizontalToVerticalWhenROMImportBy4ToolStripMenuItem,
            this.askTileNameOnImportToolStripMenuItem,
            this.allowImportOfDuplicateToolStripMenuItem});
            this.importToolStripMenuItem.Name = "importToolStripMenuItem";
            this.importToolStripMenuItem.Size = new System.Drawing.Size(55, 23);
            this.importToolStripMenuItem.Text = "&Import";
            // 
            // horizontalToVerticalWhenROMImportBy4ToolStripMenuItem
            // 
            this.horizontalToVerticalWhenROMImportBy4ToolStripMenuItem.Checked = true;
            this.horizontalToVerticalWhenROMImportBy4ToolStripMenuItem.CheckOnClick = true;
            this.horizontalToVerticalWhenROMImportBy4ToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.horizontalToVerticalWhenROMImportBy4ToolStripMenuItem.Name = "horizontalToVerticalWhenROMImportBy4ToolStripMenuItem";
            this.horizontalToVerticalWhenROMImportBy4ToolStripMenuItem.Size = new System.Drawing.Size(310, 22);
            this.horizontalToVerticalWhenROMImportBy4ToolStripMenuItem.Text = "Horizontal to Vertical when ROM import by 4";
            // 
            // askTileNameOnImportToolStripMenuItem
            // 
            this.askTileNameOnImportToolStripMenuItem.Checked = true;
            this.askTileNameOnImportToolStripMenuItem.CheckOnClick = true;
            this.askTileNameOnImportToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.askTileNameOnImportToolStripMenuItem.Name = "askTileNameOnImportToolStripMenuItem";
            this.askTileNameOnImportToolStripMenuItem.Size = new System.Drawing.Size(310, 22);
            this.askTileNameOnImportToolStripMenuItem.Text = "Ask tile name on import";
            // 
            // allowImportOfDuplicateToolStripMenuItem
            // 
            this.allowImportOfDuplicateToolStripMenuItem.Checked = true;
            this.allowImportOfDuplicateToolStripMenuItem.CheckOnClick = true;
            this.allowImportOfDuplicateToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.allowImportOfDuplicateToolStripMenuItem.Name = "allowImportOfDuplicateToolStripMenuItem";
            this.allowImportOfDuplicateToolStripMenuItem.Size = new System.Drawing.Size(310, 22);
            this.allowImportOfDuplicateToolStripMenuItem.Text = "Allow import of duplicate";
            // 
            // exportToolStripMenuItem
            // 
            this.exportToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exportForGBDKToolStripMenuItem,
            this.toolStripSeparator1,
            this.generateStubMaincToolStripMenuItem,
            this.exportGridOnTheMapsToolStripMenuItem});
            this.exportToolStripMenuItem.Name = "exportToolStripMenuItem";
            this.exportToolStripMenuItem.Size = new System.Drawing.Size(53, 23);
            this.exportToolStripMenuItem.Text = "&Export";
            // 
            // exportForGBDKToolStripMenuItem
            // 
            this.exportForGBDKToolStripMenuItem.Name = "exportForGBDKToolStripMenuItem";
            this.exportForGBDKToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F5;
            this.exportForGBDKToolStripMenuItem.Size = new System.Drawing.Size(201, 22);
            this.exportForGBDKToolStripMenuItem.Text = "Export for GBDK";
            this.exportForGBDKToolStripMenuItem.Click += new System.EventHandler(this.exportForGBDKToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(198, 6);
            // 
            // generateStubMaincToolStripMenuItem
            // 
            this.generateStubMaincToolStripMenuItem.CheckOnClick = true;
            this.generateStubMaincToolStripMenuItem.Name = "generateStubMaincToolStripMenuItem";
            this.generateStubMaincToolStripMenuItem.Size = new System.Drawing.Size(201, 22);
            this.generateStubMaincToolStripMenuItem.Text = "Generate stub main.c";
            // 
            // exportGridOnTheMapsToolStripMenuItem
            // 
            this.exportGridOnTheMapsToolStripMenuItem.Checked = true;
            this.exportGridOnTheMapsToolStripMenuItem.CheckOnClick = true;
            this.exportGridOnTheMapsToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.exportGridOnTheMapsToolStripMenuItem.Name = "exportGridOnTheMapsToolStripMenuItem";
            this.exportGridOnTheMapsToolStripMenuItem.Size = new System.Drawing.Size(201, 22);
            this.exportGridOnTheMapsToolStripMenuItem.Text = "Export grid on the maps";
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.removeUnusedTilesToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(46, 23);
            this.toolsToolStripMenuItem.Text = "&Tools";
            // 
            // removeUnusedTilesToolStripMenuItem
            // 
            this.removeUnusedTilesToolStripMenuItem.Name = "removeUnusedTilesToolStripMenuItem";
            this.removeUnusedTilesToolStripMenuItem.Size = new System.Drawing.Size(183, 22);
            this.removeUnusedTilesToolStripMenuItem.Text = "&Remove unused tiles";
            this.removeUnusedTilesToolStripMenuItem.Click += new System.EventHandler(this.removeUnusedTilesToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutGbReaperToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 23);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // aboutGbReaperToolStripMenuItem
            // 
            this.aboutGbReaperToolStripMenuItem.Name = "aboutGbReaperToolStripMenuItem";
            this.aboutGbReaperToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.aboutGbReaperToolStripMenuItem.Text = "&About GbReaper...";
            this.aboutGbReaperToolStripMenuItem.Click += new System.EventHandler(this.aboutGbReaperToolStripMenuItem_Click);
            // 
            // cbxPalette
            // 
            this.cbxPalette.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbxPalette.Enabled = false;
            this.cbxPalette.Name = "cbxPalette";
            this.cbxPalette.Size = new System.Drawing.Size(121, 23);
            this.cbxPalette.SelectedIndexChanged += new System.EventHandler(this.cbxPalette_SelectedIndexChanged);
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
            this.panMain.Location = new System.Drawing.Point(252, 27);
            this.panMain.Name = "panMain";
            this.panMain.Size = new System.Drawing.Size(802, 809);
            this.panMain.TabIndex = 4;
            // 
            // tabMaps
            // 
            this.tabMaps.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this.tabMaps.Controls.Add(this.tpMapWelcome);
            this.tabMaps.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabMaps.Location = new System.Drawing.Point(0, 277);
            this.tabMaps.Name = "tabMaps";
            this.tabMaps.SelectedIndex = 0;
            this.tabMaps.Size = new System.Drawing.Size(602, 532);
            this.tabMaps.TabIndex = 3;
            // 
            // tpMapWelcome
            // 
            this.tpMapWelcome.Controls.Add(this.btnNewMap);
            this.tpMapWelcome.Location = new System.Drawing.Point(4, 4);
            this.tpMapWelcome.Name = "tpMapWelcome";
            this.tpMapWelcome.Size = new System.Drawing.Size(594, 506);
            this.tpMapWelcome.TabIndex = 0;
            this.tpMapWelcome.Text = "Maps";
            this.tpMapWelcome.UseVisualStyleBackColor = true;
            // 
            // btnNewMap
            // 
            this.btnNewMap.Image = ((System.Drawing.Image)(resources.GetObject("btnNewMap.Image")));
            this.btnNewMap.Location = new System.Drawing.Point(187, 203);
            this.btnNewMap.Name = "btnNewMap";
            this.btnNewMap.Size = new System.Drawing.Size(230, 119);
            this.btnNewMap.TabIndex = 0;
            this.btnNewMap.Text = "Create new map...";
            this.btnNewMap.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnNewMap.UseVisualStyleBackColor = true;
            this.btnNewMap.Click += new System.EventHandler(this.btnNewMap_Click);
            // 
            // ucTileEd
            // 
            this.ucTileEd.Dock = System.Windows.Forms.DockStyle.Top;
            this.ucTileEd.Location = new System.Drawing.Point(0, 0);
            this.ucTileEd.MinimumSize = new System.Drawing.Size(0, 277);
            this.ucTileEd.Name = "ucTileEd";
            this.ucTileEd.Size = new System.Drawing.Size(602, 277);
            this.ucTileEd.TabIndex = 1;
            // 
            // panLib
            // 
            this.panLib.Controls.Add(this.ucLibView);
            this.panLib.Dock = System.Windows.Forms.DockStyle.Right;
            this.panLib.Location = new System.Drawing.Point(602, 0);
            this.panLib.Name = "panLib";
            this.panLib.Size = new System.Drawing.Size(200, 809);
            this.panLib.TabIndex = 0;
            // 
            // ucLibView
            // 
            this.ucLibView.Dock = System.Windows.Forms.DockStyle.Right;
            this.ucLibView.Location = new System.Drawing.Point(0, 0);
            this.ucLibView.Name = "ucLibView";
            this.ucLibView.Size = new System.Drawing.Size(200, 809);
            this.ucLibView.TabIndex = 0;
            // 
            // panLeft
            // 
            this.panLeft.Controls.Add(this.ucRomViewer1);
            this.panLeft.Controls.Add(this.lblRomMemo);
            this.panLeft.Controls.Add(this.panLeftBottom);
            this.panLeft.Dock = System.Windows.Forms.DockStyle.Left;
            this.panLeft.Location = new System.Drawing.Point(0, 27);
            this.panLeft.Name = "panLeft";
            this.panLeft.Size = new System.Drawing.Size(252, 809);
            this.panLeft.TabIndex = 5;
            // 
            // ucRomViewer1
            // 
            this.ucRomViewer1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ucRomViewer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ucRomViewer1.Location = new System.Drawing.Point(0, 0);
            this.ucRomViewer1.Name = "ucRomViewer1";
            this.ucRomViewer1.Size = new System.Drawing.Size(252, 625);
            this.ucRomViewer1.TabIndex = 3;
            // 
            // lblRomMemo
            // 
            this.lblRomMemo.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.lblRomMemo.Font = new System.Drawing.Font("Segoe UI Black", 7F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblRomMemo.Location = new System.Drawing.Point(0, 625);
            this.lblRomMemo.Name = "lblRomMemo";
            this.lblRomMemo.Size = new System.Drawing.Size(252, 42);
            this.lblRomMemo.TabIndex = 5;
            this.lblRomMemo.Text = "Double click to add 1 to library, [Ctrl] to import 4 in crossed order, [Shift] to" +
    " import up to 255";
            this.lblRomMemo.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // panLeftBottom
            // 
            this.panLeftBottom.Controls.Add(this.pan128Alt);
            this.panLeftBottom.Controls.Add(this.pan32);
            this.panLeftBottom.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panLeftBottom.Location = new System.Drawing.Point(0, 667);
            this.panLeftBottom.Name = "panLeftBottom";
            this.panLeftBottom.Size = new System.Drawing.Size(252, 142);
            this.panLeftBottom.TabIndex = 4;
            // 
            // pan128Alt
            // 
            this.pan128Alt.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan128Alt.Location = new System.Drawing.Point(62, 1);
            this.pan128Alt.Name = "pan128Alt";
            this.pan128Alt.Size = new System.Drawing.Size(128, 139);
            this.pan128Alt.TabIndex = 4;
            // 
            // pan32
            // 
            this.pan32.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pan32.Location = new System.Drawing.Point(12, 7);
            this.pan32.Name = "pan32";
            this.pan32.Size = new System.Drawing.Size(32, 35);
            this.pan32.TabIndex = 3;
            // 
            // sfdProject
            // 
            this.sfdProject.DefaultExt = "gbxml";
            this.sfdProject.Filter = "GB Reaper project (*.gbxml)|*.gbxml|All files|*.*";
            this.sfdProject.Title = "Where to save the project?";
            // 
            // strip
            // 
            this.strip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.stripLabel});
            this.strip.Location = new System.Drawing.Point(0, 836);
            this.strip.Name = "strip";
            this.strip.Size = new System.Drawing.Size(1054, 22);
            this.strip.TabIndex = 6;
            // 
            // stripLabel
            // 
            this.stripLabel.AutoToolTip = true;
            this.stripLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F);
            this.stripLabel.Name = "stripLabel";
            this.stripLabel.Size = new System.Drawing.Size(31, 17);
            this.stripLabel.Text = "Hello";
            // 
            // ofdProject
            // 
            this.ofdProject.Filter = "GbReaper projects|*.gbxml";
            this.ofdProject.Title = "Open project ...";
            // 
            // fbdExport
            // 
            this.fbdExport.Description = "Choose path to export";
            // 
            // reloadToolStripMenuItem
            // 
            this.reloadToolStripMenuItem.Name = "reloadToolStripMenuItem";
            this.reloadToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F7;
            this.reloadToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
            this.reloadToolStripMenuItem.Text = "Reload";
            this.reloadToolStripMenuItem.Click += new System.EventHandler(this.reloadToolStripMenuItem_Click);
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1054, 858);
            this.Controls.Add(this.panMain);
            this.Controls.Add(this.panLeft);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.strip);
            this.DoubleBuffered = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "FrmMain";
            this.Text = "GB Reaper";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.Load += new System.EventHandler(this.FrmMain_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.panMain.ResumeLayout(false);
            this.tabMaps.ResumeLayout(false);
            this.tpMapWelcome.ResumeLayout(false);
            this.panLib.ResumeLayout(false);
            this.panLeft.ResumeLayout(false);
            this.panLeftBottom.ResumeLayout(false);
            this.strip.ResumeLayout(false);
            this.strip.PerformLayout();
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
        private System.Windows.Forms.StatusStrip strip;
        private System.Windows.Forms.ToolStripStatusLabel stripLabel;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem loadProjectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutGbReaperToolStripMenuItem;
        private System.Windows.Forms.OpenFileDialog ofdProject;
        private System.Windows.Forms.ToolStripMenuItem exportToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportForGBDKToolStripMenuItem;
        private System.Windows.Forms.FolderBrowserDialog fbdExport;
        private System.Windows.Forms.ToolStripComboBox cbxPalette;
        private System.Windows.Forms.ToolStripMenuItem importToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem horizontalToVerticalWhenROMImportBy4ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem askTileNameOnImportToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem generateStubMaincToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem allowImportOfDuplicateToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeUnusedTilesToolStripMenuItem;
        private System.Windows.Forms.Label lblRomMemo;
        private System.Windows.Forms.ToolStripMenuItem mapsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createMapsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportGridOnTheMapsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem reloadToolStripMenuItem;
    }
}

