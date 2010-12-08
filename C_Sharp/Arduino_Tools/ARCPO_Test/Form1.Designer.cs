namespace Arduino_Tools
{
    partial class Form1
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
            this.button1 = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.ckbMailAuto = new System.Windows.Forms.CheckBox();
            this.btnMail = new System.Windows.Forms.Button();
            this.ckbMessageWarn = new System.Windows.Forms.CheckBox();
            this.ckbMessagePink = new System.Windows.Forms.CheckBox();
            this.ckbMessageBlink = new System.Windows.Forms.CheckBox();
            this.ckbTime = new System.Windows.Forms.CheckBox();
            this.button5 = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.button4 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.ckbAuto = new System.Windows.Forms.CheckBox();
            this.ckbAck = new System.Windows.Forms.CheckBox();
            this.btnClear = new System.Windows.Forms.Button();
            this.ckbUpdate = new System.Windows.Forms.CheckBox();
            this.button2 = new System.Windows.Forms.Button();
            this.tim30sec = new System.Windows.Forms.Timer(this.components);
            this.txbTextLine1 = new System.Windows.Forms.TextBox();
            this.txbTextLine2 = new System.Windows.Forms.TextBox();
            this.button6 = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.Gold;
            this.button1.Location = new System.Drawing.Point(89, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(148, 47);
            this.button1.TabIndex = 0;
            this.button1.Text = "Send test packet";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // textBox1
            // 
            this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox1.Location = new System.Drawing.Point(0, 217);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox1.Size = new System.Drawing.Size(506, 399);
            this.textBox1.TabIndex = 1;
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 30000;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button6);
            this.groupBox1.Controls.Add(this.txbTextLine2);
            this.groupBox1.Controls.Add(this.txbTextLine1);
            this.groupBox1.Controls.Add(this.ckbMailAuto);
            this.groupBox1.Controls.Add(this.btnMail);
            this.groupBox1.Controls.Add(this.ckbMessageWarn);
            this.groupBox1.Controls.Add(this.ckbMessagePink);
            this.groupBox1.Controls.Add(this.ckbMessageBlink);
            this.groupBox1.Controls.Add(this.ckbTime);
            this.groupBox1.Controls.Add(this.button5);
            this.groupBox1.Controls.Add(this.groupBox2);
            this.groupBox1.Controls.Add(this.ckbAuto);
            this.groupBox1.Controls.Add(this.ckbAck);
            this.groupBox1.Controls.Add(this.btnClear);
            this.groupBox1.Controls.Add(this.ckbUpdate);
            this.groupBox1.Controls.Add(this.button2);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(506, 217);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Commands";
            // 
            // ckbMailAuto
            // 
            this.ckbMailAuto.AutoSize = true;
            this.ckbMailAuto.Location = new System.Drawing.Point(447, 96);
            this.ckbMailAuto.Name = "ckbMailAuto";
            this.ckbMailAuto.Size = new System.Drawing.Size(46, 16);
            this.ckbMailAuto.TabIndex = 11;
            this.ckbMailAuto.Text = "auto";
            this.ckbMailAuto.UseVisualStyleBackColor = true;
            this.ckbMailAuto.CheckedChanged += new System.EventHandler(this.ckbMailAuto_CheckedChanged);
            // 
            // btnMail
            // 
            this.btnMail.Location = new System.Drawing.Point(435, 73);
            this.btnMail.Name = "btnMail";
            this.btnMail.Size = new System.Drawing.Size(65, 21);
            this.btnMail.TabIndex = 10;
            this.btnMail.Text = "Mail ?";
            this.btnMail.UseVisualStyleBackColor = true;
            this.btnMail.Click += new System.EventHandler(this.btnMail_Click);
            // 
            // ckbMessageWarn
            // 
            this.ckbMessageWarn.AutoSize = true;
            this.ckbMessageWarn.Checked = true;
            this.ckbMessageWarn.CheckState = System.Windows.Forms.CheckState.Checked;
            this.ckbMessageWarn.Location = new System.Drawing.Point(121, 172);
            this.ckbMessageWarn.Name = "ckbMessageWarn";
            this.ckbMessageWarn.Size = new System.Drawing.Size(53, 16);
            this.ckbMessageWarn.TabIndex = 9;
            this.ckbMessageWarn.Text = "warn?";
            this.ckbMessageWarn.UseVisualStyleBackColor = true;
            // 
            // ckbMessagePink
            // 
            this.ckbMessagePink.AutoSize = true;
            this.ckbMessagePink.Checked = true;
            this.ckbMessagePink.CheckState = System.Windows.Forms.CheckState.Checked;
            this.ckbMessagePink.Location = new System.Drawing.Point(121, 159);
            this.ckbMessagePink.Name = "ckbMessagePink";
            this.ckbMessagePink.Size = new System.Drawing.Size(50, 16);
            this.ckbMessagePink.TabIndex = 9;
            this.ckbMessagePink.Text = "pink?";
            this.ckbMessagePink.UseVisualStyleBackColor = true;
            // 
            // ckbMessageBlink
            // 
            this.ckbMessageBlink.AutoSize = true;
            this.ckbMessageBlink.Checked = true;
            this.ckbMessageBlink.CheckState = System.Windows.Forms.CheckState.Checked;
            this.ckbMessageBlink.Location = new System.Drawing.Point(121, 146);
            this.ckbMessageBlink.Name = "ckbMessageBlink";
            this.ckbMessageBlink.Size = new System.Drawing.Size(53, 16);
            this.ckbMessageBlink.TabIndex = 9;
            this.ckbMessageBlink.Text = "blink?";
            this.ckbMessageBlink.UseVisualStyleBackColor = true;
            // 
            // ckbTime
            // 
            this.ckbTime.AutoSize = true;
            this.ckbTime.Location = new System.Drawing.Point(34, 185);
            this.ckbTime.Name = "ckbTime";
            this.ckbTime.Size = new System.Drawing.Size(49, 16);
            this.ckbTime.TabIndex = 8;
            this.ckbTime.Text = "Time";
            this.ckbTime.UseVisualStyleBackColor = true;
            this.ckbTime.CheckedChanged += new System.EventHandler(this.ckbTime_CheckedChanged);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(3, 146);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(112, 38);
            this.button5.TabIndex = 7;
            this.button5.Text = "Test for notifier";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.button4);
            this.groupBox2.Controls.Add(this.button3);
            this.groupBox2.Location = new System.Drawing.Point(89, 81);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(266, 39);
            this.groupBox2.TabIndex = 6;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Led";
            // 
            // button4
            // 
            this.button4.BackColor = System.Drawing.Color.Coral;
            this.button4.Location = new System.Drawing.Point(87, 10);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 23);
            this.button4.TabIndex = 7;
            this.button4.Text = "Led 2";
            this.button4.UseVisualStyleBackColor = false;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button3
            // 
            this.button3.BackColor = System.Drawing.Color.Chartreuse;
            this.button3.Location = new System.Drawing.Point(6, 9);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 24);
            this.button3.TabIndex = 7;
            this.button3.Text = "Led 1";
            this.button3.UseVisualStyleBackColor = false;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // ckbAuto
            // 
            this.ckbAuto.AutoSize = true;
            this.ckbAuto.Location = new System.Drawing.Point(7, 19);
            this.ckbAuto.Name = "ckbAuto";
            this.ckbAuto.Size = new System.Drawing.Size(46, 16);
            this.ckbAuto.TabIndex = 5;
            this.ckbAuto.Text = "auto";
            this.ckbAuto.UseVisualStyleBackColor = true;
            // 
            // ckbAck
            // 
            this.ckbAck.AutoSize = true;
            this.ckbAck.Checked = true;
            this.ckbAck.CheckState = System.Windows.Forms.CheckState.Checked;
            this.ckbAck.Location = new System.Drawing.Point(115, 59);
            this.ckbAck.Name = "ckbAck";
            this.ckbAck.Size = new System.Drawing.Size(96, 16);
            this.ckbAck.TabIndex = 4;
            this.ckbAck.Text = "Acknowledge?";
            this.ckbAck.UseVisualStyleBackColor = true;
            // 
            // btnClear
            // 
            this.btnClear.Location = new System.Drawing.Point(436, 41);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(64, 23);
            this.btnClear.TabIndex = 3;
            this.btnClear.Text = "clear";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // ckbUpdate
            // 
            this.ckbUpdate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.ckbUpdate.AutoSize = true;
            this.ckbUpdate.Checked = true;
            this.ckbUpdate.CheckState = System.Windows.Forms.CheckState.Checked;
            this.ckbUpdate.Location = new System.Drawing.Point(436, 18);
            this.ckbUpdate.Name = "ckbUpdate";
            this.ckbUpdate.Size = new System.Drawing.Size(58, 16);
            this.ckbUpdate.TabIndex = 2;
            this.ckbUpdate.Text = "update";
            this.ckbUpdate.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.BackColor = System.Drawing.Color.MediumSeaGreen;
            this.button2.Location = new System.Drawing.Point(243, 12);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(112, 47);
            this.button2.TabIndex = 1;
            this.button2.Text = "Send junk";
            this.button2.UseVisualStyleBackColor = false;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // tim30sec
            // 
            this.tim30sec.Enabled = true;
            this.tim30sec.Interval = 30000;
            this.tim30sec.Tick += new System.EventHandler(this.tim30sec_Tick);
            // 
            // txbTextLine1
            // 
            this.txbTextLine1.Font = new System.Drawing.Font("Courier New", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.txbTextLine1.Location = new System.Drawing.Point(180, 143);
            this.txbTextLine1.MaxLength = 16;
            this.txbTextLine1.Name = "txbTextLine1";
            this.txbTextLine1.Size = new System.Drawing.Size(144, 23);
            this.txbTextLine1.TabIndex = 12;
            this.txbTextLine1.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txbTextLine2
            // 
            this.txbTextLine2.Font = new System.Drawing.Font("Courier New", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.txbTextLine2.Location = new System.Drawing.Point(180, 169);
            this.txbTextLine2.MaxLength = 16;
            this.txbTextLine2.Name = "txbTextLine2";
            this.txbTextLine2.Size = new System.Drawing.Size(144, 23);
            this.txbTextLine2.TabIndex = 12;
            this.txbTextLine2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(330, 146);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(75, 42);
            this.button6.TabIndex = 13;
            this.button6.Text = "Send";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(506, 616);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.CheckBox ckbUpdate;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.CheckBox ckbAck;
        private System.Windows.Forms.CheckBox ckbAuto;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.CheckBox ckbTime;
        private System.Windows.Forms.CheckBox ckbMessageBlink;
		private System.Windows.Forms.Button btnMail;
		private System.Windows.Forms.CheckBox ckbMailAuto;
		private System.Windows.Forms.Timer tim30sec;
		private System.Windows.Forms.CheckBox ckbMessageWarn;
		private System.Windows.Forms.CheckBox ckbMessagePink;
        private System.Windows.Forms.TextBox txbTextLine1;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.TextBox txbTextLine2;
    }
}

