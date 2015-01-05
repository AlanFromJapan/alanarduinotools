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
            this.btnRed = new System.Windows.Forms.Button();
            this.btnYellow = new System.Windows.Forms.Button();
            this.btnNoProblem = new System.Windows.Forms.Button();
            this.btnCircle = new System.Windows.Forms.Button();
            this.ckbRedBlink = new System.Windows.Forms.CheckBox();
            this.timBlink = new System.Windows.Forms.Timer(this.components);
            this.txbPipeOutput = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // btnRed
            // 
            this.btnRed.BackColor = System.Drawing.Color.Maroon;
            this.btnRed.ForeColor = System.Drawing.Color.White;
            this.btnRed.Location = new System.Drawing.Point(31, 39);
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
            this.btnYellow.Location = new System.Drawing.Point(31, 87);
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
            this.btnNoProblem.Location = new System.Drawing.Point(31, 135);
            this.btnNoProblem.Name = "btnNoProblem";
            this.btnNoProblem.Size = new System.Drawing.Size(91, 42);
            this.btnNoProblem.TabIndex = 2;
            this.btnNoProblem.Text = "No problem";
            this.btnNoProblem.UseVisualStyleBackColor = false;
            this.btnNoProblem.Click += new System.EventHandler(this.btnNoProblem_Click);
            // 
            // btnCircle
            // 
            this.btnCircle.Location = new System.Drawing.Point(31, 269);
            this.btnCircle.Name = "btnCircle";
            this.btnCircle.Size = new System.Drawing.Size(90, 46);
            this.btnCircle.TabIndex = 3;
            this.btnCircle.Text = "Circle values";
            this.btnCircle.UseVisualStyleBackColor = true;
            this.btnCircle.Click += new System.EventHandler(this.btnCircle_Click);
            // 
            // ckbRedBlink
            // 
            this.ckbRedBlink.AutoSize = true;
            this.ckbRedBlink.Location = new System.Drawing.Point(128, 53);
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
            this.txbPipeOutput.Dock = System.Windows.Forms.DockStyle.Right;
            this.txbPipeOutput.Font = new System.Drawing.Font("Courier New", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.txbPipeOutput.Location = new System.Drawing.Point(184, 0);
            this.txbPipeOutput.Multiline = true;
            this.txbPipeOutput.Name = "txbPipeOutput";
            this.txbPipeOutput.ReadOnly = true;
            this.txbPipeOutput.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txbPipeOutput.Size = new System.Drawing.Size(337, 338);
            this.txbPipeOutput.TabIndex = 5;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(521, 338);
            this.Controls.Add(this.txbPipeOutput);
            this.Controls.Add(this.ckbRedBlink);
            this.Controls.Add(this.btnCircle);
            this.Controls.Add(this.btnNoProblem);
            this.Controls.Add(this.btnYellow);
            this.Controls.Add(this.btnRed);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Tricolor USB Signal";
            this.Load += new System.EventHandler(this.Form1_Load);
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

    }
}

