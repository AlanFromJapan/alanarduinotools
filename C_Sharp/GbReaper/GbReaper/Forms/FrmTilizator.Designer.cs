namespace GbReaper.Forms {
    partial class FrmTilizator {
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
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnCreate = new System.Windows.Forms.Button();
            this.undHeight = new System.Windows.Forms.NumericUpDown();
            this.undWidth = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.txbImage = new System.Windows.Forms.TextBox();
            this.btnOpenImage = new System.Windows.Forms.Button();
            this.ofdBitmap = new System.Windows.Forms.OpenFileDialog();
            this.picPreview = new System.Windows.Forms.PictureBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.undLeft = new System.Windows.Forms.NumericUpDown();
            this.undTop = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.undHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.undWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.picPreview)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.undLeft)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.undTop)).BeginInit();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(330, 178);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 3;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnCreate
            // 
            this.btnCreate.Location = new System.Drawing.Point(249, 178);
            this.btnCreate.Name = "btnCreate";
            this.btnCreate.Size = new System.Drawing.Size(75, 23);
            this.btnCreate.TabIndex = 2;
            this.btnCreate.Text = "Create";
            this.btnCreate.UseVisualStyleBackColor = true;
            this.btnCreate.Click += new System.EventHandler(this.btnCreate_Click);
            // 
            // undHeight
            // 
            this.undHeight.Location = new System.Drawing.Point(105, 62);
            this.undHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.undHeight.Name = "undHeight";
            this.undHeight.Size = new System.Drawing.Size(120, 19);
            this.undHeight.TabIndex = 9;
            this.undHeight.Value = new decimal(new int[] {
            18,
            0,
            0,
            0});
            // 
            // undWidth
            // 
            this.undWidth.Location = new System.Drawing.Point(105, 37);
            this.undWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.undWidth.Name = "undWidth";
            this.undWidth.Size = new System.Drawing.Size(120, 19);
            this.undWidth.TabIndex = 8;
            this.undWidth.Value = new decimal(new int[] {
            20,
            0,
            0,
            0});
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 64);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(87, 12);
            this.label2.TabIndex = 7;
            this.label2.Text = "Height (in tiles):";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 39);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(82, 12);
            this.label1.TabIndex = 6;
            this.label1.Text = "Width (in tiles):";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 15);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(37, 12);
            this.label3.TabIndex = 10;
            this.label3.Text = "Image:";
            // 
            // txbImage
            // 
            this.txbImage.Location = new System.Drawing.Point(105, 12);
            this.txbImage.Name = "txbImage";
            this.txbImage.Size = new System.Drawing.Size(264, 19);
            this.txbImage.TabIndex = 11;
            // 
            // btnOpenImage
            // 
            this.btnOpenImage.Location = new System.Drawing.Point(375, 10);
            this.btnOpenImage.Name = "btnOpenImage";
            this.btnOpenImage.Size = new System.Drawing.Size(30, 23);
            this.btnOpenImage.TabIndex = 12;
            this.btnOpenImage.Text = "...";
            this.btnOpenImage.UseVisualStyleBackColor = true;
            this.btnOpenImage.Click += new System.EventHandler(this.btnOpenImage_Click);
            // 
            // ofdBitmap
            // 
            this.ofdBitmap.Filter = "Bitmap|*.png;*.bmp";
            this.ofdBitmap.Title = "Choose image to import";
            // 
            // picPreview
            // 
            this.picPreview.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.picPreview.Location = new System.Drawing.Point(249, 37);
            this.picPreview.Name = "picPreview";
            this.picPreview.Size = new System.Drawing.Size(155, 125);
            this.picPreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.picPreview.TabIndex = 13;
            this.picPreview.TabStop = false;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 116);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(27, 12);
            this.label4.TabIndex = 6;
            this.label4.Text = "Left:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 141);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(26, 12);
            this.label5.TabIndex = 7;
            this.label5.Text = "Top:";
            // 
            // undLeft
            // 
            this.undLeft.Location = new System.Drawing.Point(105, 114);
            this.undLeft.Name = "undLeft";
            this.undLeft.Size = new System.Drawing.Size(120, 19);
            this.undLeft.TabIndex = 8;
            // 
            // undTop
            // 
            this.undTop.Location = new System.Drawing.Point(105, 139);
            this.undTop.Name = "undTop";
            this.undTop.Size = new System.Drawing.Size(120, 19);
            this.undTop.TabIndex = 9;
            // 
            // FrmTilizator
            // 
            this.AcceptButton = this.btnCreate;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(415, 208);
            this.Controls.Add(this.picPreview);
            this.Controls.Add(this.btnOpenImage);
            this.Controls.Add(this.txbImage);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.undTop);
            this.Controls.Add(this.undHeight);
            this.Controls.Add(this.undLeft);
            this.Controls.Add(this.undWidth);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnCreate);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FrmTilizator";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Tilizator";
            ((System.ComponentModel.ISupportInitialize)(this.undHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.undWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.picPreview)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.undLeft)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.undTop)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnCreate;
        private System.Windows.Forms.NumericUpDown undHeight;
        private System.Windows.Forms.NumericUpDown undWidth;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txbImage;
        private System.Windows.Forms.Button btnOpenImage;
        private System.Windows.Forms.OpenFileDialog ofdBitmap;
        private System.Windows.Forms.PictureBox picPreview;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.NumericUpDown undLeft;
        private System.Windows.Forms.NumericUpDown undTop;
    }
}