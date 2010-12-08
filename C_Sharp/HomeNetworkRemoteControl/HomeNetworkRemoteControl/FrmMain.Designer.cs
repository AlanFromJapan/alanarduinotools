namespace HomeNetworkRemoteControl {
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmMain));
            this.panInfo = new System.Windows.Forms.Panel();
            this.lblInfo = new System.Windows.Forms.Label();
            this.btnVolUp = new System.Windows.Forms.Button();
            this.btnVolLow = new System.Windows.Forms.Button();
            this.btnVolMute = new System.Windows.Forms.Button();
            this.btnPause = new System.Windows.Forms.Button();
            this.btnPrev = new System.Windows.Forms.Button();
            this.btnNext = new System.Windows.Forms.Button();
            this.nic = new System.Windows.Forms.NotifyIcon(this.components);
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.playPauseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.showToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.tsbShut = new System.Windows.Forms.ToolStripButton();
            this.panInfo.SuspendLayout();
            this.contextMenuStrip1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panInfo
            // 
            this.panInfo.BackColor = System.Drawing.Color.Gold;
            this.panInfo.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panInfo.Controls.Add(this.lblInfo);
            this.panInfo.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panInfo.ForeColor = System.Drawing.Color.DarkRed;
            this.panInfo.Location = new System.Drawing.Point(0, 205);
            this.panInfo.Name = "panInfo";
            this.panInfo.Size = new System.Drawing.Size(442, 59);
            this.panInfo.TabIndex = 0;
            // 
            // lblInfo
            // 
            this.lblInfo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblInfo.Font = new System.Drawing.Font("MS UI Gothic", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.lblInfo.Location = new System.Drawing.Point(0, 0);
            this.lblInfo.Name = "lblInfo";
            this.lblInfo.Size = new System.Drawing.Size(440, 57);
            this.lblInfo.TabIndex = 0;
            this.lblInfo.Text = "---";
            this.lblInfo.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // btnVolUp
            // 
            this.btnVolUp.BackColor = System.Drawing.Color.Violet;
            this.btnVolUp.Location = new System.Drawing.Point(343, 28);
            this.btnVolUp.Name = "btnVolUp";
            this.btnVolUp.Size = new System.Drawing.Size(87, 66);
            this.btnVolUp.TabIndex = 1;
            this.btnVolUp.Text = "Vol +";
            this.btnVolUp.UseVisualStyleBackColor = false;
            this.btnVolUp.Click += new System.EventHandler(this.btnVolUp_Click);
            // 
            // btnVolLow
            // 
            this.btnVolLow.BackColor = System.Drawing.Color.GreenYellow;
            this.btnVolLow.Location = new System.Drawing.Point(343, 101);
            this.btnVolLow.Name = "btnVolLow";
            this.btnVolLow.Size = new System.Drawing.Size(87, 66);
            this.btnVolLow.TabIndex = 2;
            this.btnVolLow.Text = "Vol -";
            this.btnVolLow.UseVisualStyleBackColor = false;
            this.btnVolLow.Click += new System.EventHandler(this.btnVolLow_Click);
            // 
            // btnVolMute
            // 
            this.btnVolMute.BackColor = System.Drawing.Color.DarkGray;
            this.btnVolMute.Location = new System.Drawing.Point(343, 173);
            this.btnVolMute.Name = "btnVolMute";
            this.btnVolMute.Size = new System.Drawing.Size(87, 25);
            this.btnVolMute.TabIndex = 3;
            this.btnVolMute.Text = "Mute";
            this.btnVolMute.UseVisualStyleBackColor = false;
            this.btnVolMute.Click += new System.EventHandler(this.btnVolMute_Click);
            // 
            // btnPause
            // 
            this.btnPause.BackColor = System.Drawing.Color.PowderBlue;
            this.btnPause.Image = ((System.Drawing.Image)(resources.GetObject("btnPause.Image")));
            this.btnPause.Location = new System.Drawing.Point(114, 80);
            this.btnPause.Name = "btnPause";
            this.btnPause.Size = new System.Drawing.Size(87, 78);
            this.btnPause.TabIndex = 4;
            this.btnPause.UseVisualStyleBackColor = false;
            this.btnPause.Click += new System.EventHandler(this.btnPause_Click);
            // 
            // btnPrev
            // 
            this.btnPrev.BackColor = System.Drawing.Color.AliceBlue;
            this.btnPrev.Image = ((System.Drawing.Image)(resources.GetObject("btnPrev.Image")));
            this.btnPrev.Location = new System.Drawing.Point(24, 55);
            this.btnPrev.Name = "btnPrev";
            this.btnPrev.Size = new System.Drawing.Size(83, 126);
            this.btnPrev.TabIndex = 5;
            this.btnPrev.UseVisualStyleBackColor = false;
            this.btnPrev.Click += new System.EventHandler(this.btnPrev_Click);
            // 
            // btnNext
            // 
            this.btnNext.BackColor = System.Drawing.Color.AliceBlue;
            this.btnNext.Image = ((System.Drawing.Image)(resources.GetObject("btnNext.Image")));
            this.btnNext.Location = new System.Drawing.Point(209, 55);
            this.btnNext.Name = "btnNext";
            this.btnNext.Size = new System.Drawing.Size(83, 126);
            this.btnNext.TabIndex = 5;
            this.btnNext.UseVisualStyleBackColor = false;
            this.btnNext.Click += new System.EventHandler(this.btnNext_Click);
            // 
            // nic
            // 
            this.nic.ContextMenuStrip = this.contextMenuStrip1;
            this.nic.Icon = ((System.Drawing.Icon)(resources.GetObject("nic.Icon")));
            this.nic.Text = "Home Network Remote Client";
            this.nic.Visible = true;
            this.nic.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.nic_MouseDoubleClick);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.playPauseToolStripMenuItem,
            this.toolStripMenuItem1,
            this.showToolStripMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(130, 54);
            // 
            // playPauseToolStripMenuItem
            // 
            this.playPauseToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("playPauseToolStripMenuItem.Image")));
            this.playPauseToolStripMenuItem.Name = "playPauseToolStripMenuItem";
            this.playPauseToolStripMenuItem.Size = new System.Drawing.Size(129, 22);
            this.playPauseToolStripMenuItem.Text = "Play/Pause";
            this.playPauseToolStripMenuItem.Click += new System.EventHandler(this.playPauseToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(126, 6);
            // 
            // showToolStripMenuItem
            // 
            this.showToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("showToolStripMenuItem.Image")));
            this.showToolStripMenuItem.Name = "showToolStripMenuItem";
            this.showToolStripMenuItem.Size = new System.Drawing.Size(129, 22);
            this.showToolStripMenuItem.Text = "Show";
            this.showToolStripMenuItem.Click += new System.EventHandler(this.showToolStripMenuItem_Click);
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsbShut});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(442, 25);
            this.toolStrip1.TabIndex = 6;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // tsbShut
            // 
            this.tsbShut.Image = ((System.Drawing.Image)(resources.GetObject("tsbShut.Image")));
            this.tsbShut.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbShut.Name = "tsbShut";
            this.tsbShut.Size = new System.Drawing.Size(74, 22);
            this.tsbShut.Text = "Shutdown";
            this.tsbShut.Click += new System.EventHandler(this.tsbShut_Click);
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(442, 264);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.btnNext);
            this.Controls.Add(this.btnPrev);
            this.Controls.Add(this.btnPause);
            this.Controls.Add(this.btnVolMute);
            this.Controls.Add(this.btnVolLow);
            this.Controls.Add(this.btnVolUp);
            this.Controls.Add(this.panInfo);
            this.Font = new System.Drawing.Font("MS UI Gothic", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "FrmMain";
            this.Opacity = 0.8;
            this.Text = "Home Network Remote Control - C.L.I.E.N.T.";
            this.Load += new System.EventHandler(this.FrmMain_Load);
            this.Resize += new System.EventHandler(this.FrmMain_Resize);
            this.panInfo.ResumeLayout(false);
            this.contextMenuStrip1.ResumeLayout(false);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panInfo;
        private System.Windows.Forms.Button btnVolUp;
        private System.Windows.Forms.Button btnVolLow;
        private System.Windows.Forms.Button btnVolMute;
        private System.Windows.Forms.Label lblInfo;
        private System.Windows.Forms.Button btnPause;
        private System.Windows.Forms.Button btnPrev;
        private System.Windows.Forms.Button btnNext;
        private System.Windows.Forms.NotifyIcon nic;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem playPauseToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem showToolStripMenuItem;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton tsbShut;
    }
}

