namespace ScreenColorAmbiance {
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
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.lblL = new System.Windows.Forms.Label();
            this.lblR = new System.Windows.Forms.Label();
            this.lblT = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 333;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // lblL
            // 
            this.lblL.Location = new System.Drawing.Point(1, 28);
            this.lblL.Name = "lblL";
            this.lblL.Size = new System.Drawing.Size(53, 194);
            this.lblL.TabIndex = 0;
            this.lblL.Text = "L";
            this.lblL.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblR
            // 
            this.lblR.Location = new System.Drawing.Point(198, 28);
            this.lblR.Name = "lblR";
            this.lblR.Size = new System.Drawing.Size(53, 194);
            this.lblR.TabIndex = 1;
            this.lblR.Text = "R";
            this.lblR.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblT
            // 
            this.lblT.Location = new System.Drawing.Point(1, 0);
            this.lblT.Name = "lblT";
            this.lblT.Size = new System.Drawing.Size(250, 28);
            this.lblT.TabIndex = 2;
            this.lblT.Text = "T";
            this.lblT.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(250, 221);
            this.Controls.Add(this.lblT);
            this.Controls.Add(this.lblR);
            this.Controls.Add(this.lblL);
            this.Name = "FrmMain";
            this.Text = "Form1";
            this.TopMost = true;
            this.Load += new System.EventHandler(this.FrmMain_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Label lblL;
        private System.Windows.Forms.Label lblR;
        private System.Windows.Forms.Label lblT;
    }
}

