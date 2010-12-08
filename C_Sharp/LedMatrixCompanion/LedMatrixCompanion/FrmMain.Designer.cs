namespace LedMatrixCompanion {
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
            this.btnAutoTest = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.btnTest3 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnAutoTest
            // 
            this.btnAutoTest.Location = new System.Drawing.Point(250, 12);
            this.btnAutoTest.Name = "btnAutoTest";
            this.btnAutoTest.Size = new System.Drawing.Size(153, 23);
            this.btnAutoTest.TabIndex = 0;
            this.btnAutoTest.Text = "AutoTest 1 : count up";
            this.btnAutoTest.UseVisualStyleBackColor = true;
            this.btnAutoTest.Click += new System.EventHandler(this.btnAutoTest_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(250, 42);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(153, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "AutoTest 2 : wave";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // btnTest3
            // 
            this.btnTest3.Location = new System.Drawing.Point(250, 71);
            this.btnTest3.Name = "btnTest3";
            this.btnTest3.Size = new System.Drawing.Size(153, 24);
            this.btnTest3.TabIndex = 2;
            this.btnTest3.Text = "AutoTest 3 : bouncing ball";
            this.btnTest3.UseVisualStyleBackColor = true;
            this.btnTest3.Click += new System.EventHandler(this.btnTest3_Click);
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(415, 267);
            this.Controls.Add(this.btnTest3);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btnAutoTest);
            this.Name = "FrmMain";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnAutoTest;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button btnTest3;
    }
}

