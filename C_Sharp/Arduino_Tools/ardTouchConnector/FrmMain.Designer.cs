namespace ardTouchConnector
{
    partial class FrmMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmMain));
            this.btnStart = new System.Windows.Forms.Button();
            this.ckbShowLog = new System.Windows.Forms.CheckBox();
            this.pbLogo = new System.Windows.Forms.PictureBox();
            this.trbContrast = new System.Windows.Forms.TrackBar();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.lblContrastVal = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.ddlCOMPort = new System.Windows.Forms.ComboBox();
            this.ckbAcceptMsg = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.txbSendText = new System.Windows.Forms.TextBox();
            this.btnSendText = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.ckbSendTime = new System.Windows.Forms.CheckBox();
            this.btnTestMonitorMail = new System.Windows.Forms.Button();
            this.ckbMailMonitor = new System.Windows.Forms.CheckBox();
            this.timOutlook = new System.Windows.Forms.Timer(this.components);
            this.timTime = new System.Windows.Forms.Timer(this.components);
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btnPlaylistEditor = new System.Windows.Forms.Button();
            this.nic = new System.Windows.Forms.NotifyIcon(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.pbLogo)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trbContrast)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnStart
            // 
            this.btnStart.Image = ((System.Drawing.Image)(resources.GetObject("btnStart.Image")));
            this.btnStart.Location = new System.Drawing.Point(12, 12);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(75, 23);
            this.btnStart.TabIndex = 1;
            this.btnStart.Text = "Start";
            this.btnStart.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // ckbShowLog
            // 
            this.ckbShowLog.AutoSize = true;
            this.ckbShowLog.Location = new System.Drawing.Point(12, 41);
            this.ckbShowLog.Name = "ckbShowLog";
            this.ckbShowLog.Size = new System.Drawing.Size(70, 16);
            this.ckbShowLog.TabIndex = 2;
            this.ckbShowLog.Text = "Show log";
            this.ckbShowLog.UseVisualStyleBackColor = true;
            this.ckbShowLog.CheckedChanged += new System.EventHandler(this.ckbShowLog_CheckedChanged);
            // 
            // pbLogo
            // 
            this.pbLogo.BackColor = System.Drawing.Color.Black;
            this.pbLogo.Image = ((System.Drawing.Image)(resources.GetObject("pbLogo.Image")));
            this.pbLogo.Location = new System.Drawing.Point(371, 173);
            this.pbLogo.Name = "pbLogo";
            this.pbLogo.Size = new System.Drawing.Size(324, 158);
            this.pbLogo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pbLogo.TabIndex = 3;
            this.pbLogo.TabStop = false;
            // 
            // trbContrast
            // 
            this.trbContrast.LargeChange = 1;
            this.trbContrast.Location = new System.Drawing.Point(63, 18);
            this.trbContrast.Maximum = 5;
            this.trbContrast.Name = "trbContrast";
            this.trbContrast.Size = new System.Drawing.Size(104, 45);
            this.trbContrast.TabIndex = 4;
            this.trbContrast.Scroll += new System.EventHandler(this.trbContrast_Scroll);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.lblContrastVal);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.trbContrast);
            this.groupBox1.Location = new System.Drawing.Point(371, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(324, 155);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Settings";
            // 
            // lblContrastVal
            // 
            this.lblContrastVal.AutoSize = true;
            this.lblContrastVal.Location = new System.Drawing.Point(173, 29);
            this.lblContrastVal.Name = "lblContrastVal";
            this.lblContrastVal.Size = new System.Drawing.Size(18, 12);
            this.lblContrastVal.TabIndex = 6;
            this.lblContrastVal.Text = "(?)";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 29);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(51, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "Contrast:";
            // 
            // ddlCOMPort
            // 
            this.ddlCOMPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ddlCOMPort.FormattingEnabled = true;
            this.ddlCOMPort.Location = new System.Drawing.Point(93, 14);
            this.ddlCOMPort.Name = "ddlCOMPort";
            this.ddlCOMPort.Size = new System.Drawing.Size(81, 20);
            this.ddlCOMPort.TabIndex = 6;
            // 
            // ckbAcceptMsg
            // 
            this.ckbAcceptMsg.AutoSize = true;
            this.ckbAcceptMsg.Checked = true;
            this.ckbAcceptMsg.CheckState = System.Windows.Forms.CheckState.Checked;
            this.ckbAcceptMsg.Location = new System.Drawing.Point(180, 16);
            this.ckbAcceptMsg.Name = "ckbAcceptMsg";
            this.ckbAcceptMsg.Size = new System.Drawing.Size(115, 16);
            this.ckbAcceptMsg.TabIndex = 7;
            this.ckbAcceptMsg.Text = "Accept messages";
            this.ckbAcceptMsg.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 21);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(56, 12);
            this.label2.TabIndex = 8;
            this.label2.Text = "Send text:";
            // 
            // txbSendText
            // 
            this.txbSendText.ImeMode = System.Windows.Forms.ImeMode.Off;
            this.txbSendText.Location = new System.Drawing.Point(68, 18);
            this.txbSendText.MaxLength = 56;
            this.txbSendText.Name = "txbSendText";
            this.txbSendText.Size = new System.Drawing.Size(215, 19);
            this.txbSendText.TabIndex = 9;
            // 
            // btnSendText
            // 
            this.btnSendText.Location = new System.Drawing.Point(289, 16);
            this.btnSendText.Name = "btnSendText";
            this.btnSendText.Size = new System.Drawing.Size(58, 23);
            this.btnSendText.TabIndex = 10;
            this.btnSendText.Text = "Send";
            this.btnSendText.UseVisualStyleBackColor = true;
            this.btnSendText.Click += new System.EventHandler(this.btnSendText_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.ckbSendTime);
            this.groupBox2.Controls.Add(this.btnTestMonitorMail);
            this.groupBox2.Controls.Add(this.ckbMailMonitor);
            this.groupBox2.Controls.Add(this.txbSendText);
            this.groupBox2.Controls.Add(this.btnSendText);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Location = new System.Drawing.Point(12, 63);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(353, 186);
            this.groupBox2.TabIndex = 11;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Commands";
            // 
            // ckbSendTime
            // 
            this.ckbSendTime.AutoSize = true;
            this.ckbSendTime.Location = new System.Drawing.Point(7, 88);
            this.ckbSendTime.Name = "ckbSendTime";
            this.ckbSendTime.Size = new System.Drawing.Size(137, 16);
            this.ckbSendTime.TabIndex = 13;
            this.ckbSendTime.Text = "Send time (every min)";
            this.ckbSendTime.UseVisualStyleBackColor = true;
            this.ckbSendTime.CheckedChanged += new System.EventHandler(this.ckbSendTime_CheckedChanged);
            // 
            // btnTestMonitorMail
            // 
            this.btnTestMonitorMail.Location = new System.Drawing.Point(100, 57);
            this.btnTestMonitorMail.Name = "btnTestMonitorMail";
            this.btnTestMonitorMail.Size = new System.Drawing.Size(75, 23);
            this.btnTestMonitorMail.TabIndex = 12;
            this.btnTestMonitorMail.Text = "Test";
            this.btnTestMonitorMail.UseVisualStyleBackColor = true;
            this.btnTestMonitorMail.Click += new System.EventHandler(this.btnTestMonitorMail_Click);
            // 
            // ckbMailMonitor
            // 
            this.ckbMailMonitor.AutoSize = true;
            this.ckbMailMonitor.Location = new System.Drawing.Point(7, 61);
            this.ckbMailMonitor.Name = "ckbMailMonitor";
            this.ckbMailMonitor.Size = new System.Drawing.Size(87, 16);
            this.ckbMailMonitor.TabIndex = 11;
            this.ckbMailMonitor.Text = "Monitor mail";
            this.ckbMailMonitor.UseVisualStyleBackColor = true;
            this.ckbMailMonitor.CheckedChanged += new System.EventHandler(this.ckbMailMonitor_CheckedChanged);
            // 
            // timOutlook
            // 
            this.timOutlook.Interval = 15000;
            this.timOutlook.Tick += new System.EventHandler(this.timOutlook_Tick);
            // 
            // timTime
            // 
            this.timTime.Interval = 60000;
            this.timTime.Tick += new System.EventHandler(this.timTime_Tick);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.btnPlaylistEditor);
            this.groupBox3.Location = new System.Drawing.Point(12, 255);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(353, 76);
            this.groupBox3.TabIndex = 12;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Tools";
            // 
            // btnPlaylistEditor
            // 
            this.btnPlaylistEditor.Image = ((System.Drawing.Image)(resources.GetObject("btnPlaylistEditor.Image")));
            this.btnPlaylistEditor.Location = new System.Drawing.Point(8, 19);
            this.btnPlaylistEditor.Name = "btnPlaylistEditor";
            this.btnPlaylistEditor.Size = new System.Drawing.Size(339, 23);
            this.btnPlaylistEditor.TabIndex = 0;
            this.btnPlaylistEditor.Text = "Playlist editor (ardPlayer)";
            this.btnPlaylistEditor.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnPlaylistEditor.UseVisualStyleBackColor = true;
            this.btnPlaylistEditor.Click += new System.EventHandler(this.btnPlaylistEditor_Click);
            // 
            // nic
            // 
            this.nic.Icon = ((System.Drawing.Icon)(resources.GetObject("nic.Icon")));
            this.nic.Text = "ardTouch Connector";
            this.nic.Visible = true;
            this.nic.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.nic_MouseDoubleClick);
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(707, 343);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.ckbAcceptMsg);
            this.Controls.Add(this.ddlCOMPort);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.pbLogo);
            this.Controls.Add(this.ckbShowLog);
            this.Controls.Add(this.btnStart);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "FrmMain";
            this.Opacity = 0.9;
            this.Text = "arcTouch Connector - http://kalshagar.wikispaces.com/ardTouch";
            this.Load += new System.EventHandler(this.FrmMain_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pbLogo)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trbContrast)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.CheckBox ckbShowLog;
        private System.Windows.Forms.PictureBox pbLogo;
        private System.Windows.Forms.TrackBar trbContrast;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label lblContrastVal;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox ddlCOMPort;
        private System.Windows.Forms.CheckBox ckbAcceptMsg;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txbSendText;
        private System.Windows.Forms.Button btnSendText;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btnTestMonitorMail;
        private System.Windows.Forms.CheckBox ckbMailMonitor;
        private System.Windows.Forms.Timer timOutlook;
        private System.Windows.Forms.CheckBox ckbSendTime;
        private System.Windows.Forms.Timer timTime;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button btnPlaylistEditor;
        private System.Windows.Forms.NotifyIcon nic;
    }
}

