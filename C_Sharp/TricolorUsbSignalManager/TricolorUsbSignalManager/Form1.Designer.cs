namespace TricolorUsbSignalManager {
    partial class Form1 {
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.btnRed = new System.Windows.Forms.Button();
            this.btnYellow = new System.Windows.Forms.Button();
            this.btnNoProblem = new System.Windows.Forms.Button();
            this.btnCircle = new System.Windows.Forms.Button();
            this.ckbRedBlink = new System.Windows.Forms.CheckBox();
            this.timBlink = new System.Windows.Forms.Timer(this.components);
            this.txbPipeOutput = new System.Windows.Forms.TextBox();
            this.timChecker = new System.Windows.Forms.Timer(this.components);
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.panLeft = new System.Windows.Forms.Panel();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.dtpIgnoreBefore = new System.Windows.Forms.DateTimePicker();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.button3 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.panLeft.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnRed
            // 
            this.btnRed.BackColor = System.Drawing.Color.Maroon;
            this.btnRed.ForeColor = System.Drawing.Color.White;
            this.btnRed.Location = new System.Drawing.Point(10, 18);
            this.btnRed.Name = "btnRed";
            this.btnRed.Size = new System.Drawing.Size(91, 42);
            this.btnRed.TabIndex = 0;
            this.btnRed.Text = "ERROR";
            this.btnRed.UseVisualStyleBackColor = false;
            this.btnRed.Click += new System.EventHandler(this.btnRed_Click);
            // 
            // btnYellow
            // 
            this.btnYellow.BackColor = System.Drawing.Color.Chocolate;
            this.btnYellow.Location = new System.Drawing.Point(10, 66);
            this.btnYellow.Name = "btnYellow";
            this.btnYellow.Size = new System.Drawing.Size(91, 42);
            this.btnYellow.TabIndex = 1;
            this.btnYellow.Text = "Warn";
            this.btnYellow.UseVisualStyleBackColor = false;
            this.btnYellow.Click += new System.EventHandler(this.btnYellow_Click);
            // 
            // btnNoProblem
            // 
            this.btnNoProblem.BackColor = System.Drawing.Color.YellowGreen;
            this.btnNoProblem.Location = new System.Drawing.Point(10, 114);
            this.btnNoProblem.Name = "btnNoProblem";
            this.btnNoProblem.Size = new System.Drawing.Size(91, 42);
            this.btnNoProblem.TabIndex = 2;
            this.btnNoProblem.Text = "No problem";
            this.btnNoProblem.UseVisualStyleBackColor = false;
            this.btnNoProblem.Click += new System.EventHandler(this.btnNoProblem_Click);
            // 
            // btnCircle
            // 
            this.btnCircle.Location = new System.Drawing.Point(107, 114);
            this.btnCircle.Name = "btnCircle";
            this.btnCircle.Size = new System.Drawing.Size(90, 42);
            this.btnCircle.TabIndex = 3;
            this.btnCircle.Text = "Circle values";
            this.btnCircle.UseVisualStyleBackColor = true;
            this.btnCircle.Click += new System.EventHandler(this.btnCircle_Click);
            // 
            // ckbRedBlink
            // 
            this.ckbRedBlink.AutoSize = true;
            this.ckbRedBlink.Location = new System.Drawing.Point(107, 32);
            this.ckbRedBlink.Name = "ckbRedBlink";
            this.ckbRedBlink.Size = new System.Drawing.Size(50, 16);
            this.ckbRedBlink.TabIndex = 4;
            this.ckbRedBlink.Text = "Blink";
            this.ckbRedBlink.UseVisualStyleBackColor = true;
            this.ckbRedBlink.CheckedChanged += new System.EventHandler(this.ckbRedBlink_CheckedChanged);
            // 
            // timBlink
            // 
            this.timBlink.Interval = 666;
            this.timBlink.Tick += new System.EventHandler(this.timBlink_Tick);
            // 
            // txbPipeOutput
            // 
            this.txbPipeOutput.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txbPipeOutput.Font = new System.Drawing.Font("Courier New", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.txbPipeOutput.Location = new System.Drawing.Point(223, 0);
            this.txbPipeOutput.Multiline = true;
            this.txbPipeOutput.Name = "txbPipeOutput";
            this.txbPipeOutput.ReadOnly = true;
            this.txbPipeOutput.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txbPipeOutput.Size = new System.Drawing.Size(584, 338);
            this.txbPipeOutput.TabIndex = 5;
            this.txbPipeOutput.WordWrap = false;
            // 
            // timChecker
            // 
            this.timChecker.Enabled = true;
            this.timChecker.Interval = 30000;
            this.timChecker.Tick += new System.EventHandler(this.timChecker_Tick);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnRed);
            this.groupBox1.Controls.Add(this.btnCircle);
            this.groupBox1.Controls.Add(this.btnYellow);
            this.groupBox1.Controls.Add(this.ckbRedBlink);
            this.groupBox1.Controls.Add(this.btnNoProblem);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(209, 172);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Set status";
            // 
            // panLeft
            // 
            this.panLeft.Controls.Add(this.groupBox3);
            this.panLeft.Controls.Add(this.groupBox2);
            this.panLeft.Controls.Add(this.groupBox1);
            this.panLeft.Dock = System.Windows.Forms.DockStyle.Left;
            this.panLeft.Location = new System.Drawing.Point(0, 0);
            this.panLeft.Name = "panLeft";
            this.panLeft.Size = new System.Drawing.Size(223, 338);
            this.panLeft.TabIndex = 5;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Controls.Add(this.dtpIgnoreBefore);
            this.groupBox3.Location = new System.Drawing.Point(0, 255);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(212, 80);
            this.groupBox3.TabIndex = 8;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Ignore before ...";
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("MS UI Gothic", 8F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.label1.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.label1.Location = new System.Drawing.Point(8, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(194, 33);
            this.label1.TabIndex = 1;
            this.label1.Text = "If you have an unsolvable event, events before this limit will be ignored.";
            // 
            // dtpIgnoreBefore
            // 
            this.dtpIgnoreBefore.CustomFormat = "yyyy/MM/dd HH:mm:ss";
            this.dtpIgnoreBefore.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dtpIgnoreBefore.Location = new System.Drawing.Point(12, 52);
            this.dtpIgnoreBefore.Name = "dtpIgnoreBefore";
            this.dtpIgnoreBefore.Size = new System.Drawing.Size(191, 19);
            this.dtpIgnoreBefore.TabIndex = 0;
            this.dtpIgnoreBefore.ValueChanged += new System.EventHandler(this.dtpIgnoreBefore_ValueChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.button3);
            this.groupBox2.Controls.Add(this.button2);
            this.groupBox2.Controls.Add(this.button1);
            this.groupBox2.Location = new System.Drawing.Point(3, 181);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(209, 68);
            this.groupBox2.TabIndex = 7;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Change status alert";
            // 
            // button3
            // 
            this.button3.BackColor = System.Drawing.Color.Maroon;
            this.button3.Location = new System.Drawing.Point(92, 19);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(35, 30);
            this.button3.TabIndex = 0;
            this.button3.UseVisualStyleBackColor = false;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button2
            // 
            this.button2.BackColor = System.Drawing.Color.Chocolate;
            this.button2.Location = new System.Drawing.Point(51, 19);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(35, 30);
            this.button2.TabIndex = 0;
            this.button2.UseVisualStyleBackColor = false;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.YellowGreen;
            this.button1.Location = new System.Drawing.Point(10, 19);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(35, 30);
            this.button1.TabIndex = 0;
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(807, 338);
            this.Controls.Add(this.txbPipeOutput);
            this.Controls.Add(this.panLeft);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.ShowInTaskbar = false;
            this.Text = "Tricolor USB Signal [http://kalshagar.wikispaces.com/Tricolor+USB+alert]";
            this.WindowState = System.Windows.Forms.FormWindowState.Minimized;
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.panLeft.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnRed;
        private System.Windows.Forms.Button btnYellow;
        private System.Windows.Forms.Button btnNoProblem;
        private System.Windows.Forms.Button btnCircle;
        private System.Windows.Forms.CheckBox ckbRedBlink;
        private System.Windows.Forms.Timer timBlink;
        private System.Windows.Forms.TextBox txbPipeOutput;
        private System.Windows.Forms.Timer timChecker;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Panel panLeft;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.DateTimePicker dtpIgnoreBefore;

    }
}

