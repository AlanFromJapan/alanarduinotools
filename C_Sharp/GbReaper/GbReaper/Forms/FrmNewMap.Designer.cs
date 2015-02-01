namespace GbReaper.Forms {
    partial class FrmNewMap {
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
            this.btnCreate = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.undWidth = new System.Windows.Forms.NumericUpDown();
            this.undHeight = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.txbMapname = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.undWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.undHeight)).BeginInit();
            this.SuspendLayout();
            // 
            // btnCreate
            // 
            this.btnCreate.Location = new System.Drawing.Point(221, 156);
            this.btnCreate.Name = "btnCreate";
            this.btnCreate.Size = new System.Drawing.Size(75, 23);
            this.btnCreate.TabIndex = 0;
            this.btnCreate.Text = "Create";
            this.btnCreate.UseVisualStyleBackColor = true;
            this.btnCreate.Click += new System.EventHandler(this.btnCreate_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(302, 156);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 1;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(82, 12);
            this.label1.TabIndex = 2;
            this.label1.Text = "Width (in tiles):";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 39);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(87, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "Height (in tiles):";
            // 
            // undWidth
            // 
            this.undWidth.Location = new System.Drawing.Point(105, 12);
            this.undWidth.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.undWidth.Name = "undWidth";
            this.undWidth.Size = new System.Drawing.Size(120, 19);
            this.undWidth.TabIndex = 4;
            this.undWidth.Value = new decimal(new int[] {
            20,
            0,
            0,
            0});
            // 
            // undHeight
            // 
            this.undHeight.Location = new System.Drawing.Point(105, 37);
            this.undHeight.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.undHeight.Name = "undHeight";
            this.undHeight.Size = new System.Drawing.Size(120, 19);
            this.undHeight.TabIndex = 5;
            this.undHeight.Value = new decimal(new int[] {
            18,
            0,
            0,
            0});
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 65);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(59, 12);
            this.label3.TabIndex = 6;
            this.label3.Text = "Map name:";
            // 
            // txbMapname
            // 
            this.txbMapname.Location = new System.Drawing.Point(105, 62);
            this.txbMapname.Name = "txbMapname";
            this.txbMapname.Size = new System.Drawing.Size(272, 19);
            this.txbMapname.TabIndex = 7;
            // 
            // FrmNewMap
            // 
            this.AcceptButton = this.btnCreate;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(389, 191);
            this.Controls.Add(this.txbMapname);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.undHeight);
            this.Controls.Add(this.undWidth);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnCreate);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FrmNewMap";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "NewMap";
            this.Load += new System.EventHandler(this.FrmNewMap_Load);
            ((System.ComponentModel.ISupportInitialize)(this.undWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.undHeight)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnCreate;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown undWidth;
        private System.Windows.Forms.NumericUpDown undHeight;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txbMapname;
    }
}