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
            GbReaper.Classes.Palette palette1 = new GbReaper.Classes.Palette();
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
            this.picPreviewA = new System.Windows.Forms.PictureBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.undLeft = new System.Windows.Forms.NumericUpDown();
            this.undTop = new System.Windows.Forms.NumericUpDown();
            this.picPreviewB = new System.Windows.Forms.PictureBox();
            this.picPreviewC = new System.Windows.Forms.PictureBox();
            this.label6 = new System.Windows.Forms.Label();
            this.cbxMethod = new System.Windows.Forms.ComboBox();
            this.ucPaletteEditor1 = new GbReaper.Controls.UcPaletteEditor();
            this.lblTileCount = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.undHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.undWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.picPreviewA)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.undLeft)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.undTop)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.picPreviewB)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.picPreviewC)).BeginInit();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(330, 289);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 25);
            this.btnCancel.TabIndex = 3;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnCreate
            // 
            this.btnCreate.Location = new System.Drawing.Point(249, 289);
            this.btnCreate.Name = "btnCreate";
            this.btnCreate.Size = new System.Drawing.Size(75, 25);
            this.btnCreate.TabIndex = 2;
            this.btnCreate.Text = "Create";
            this.btnCreate.UseVisualStyleBackColor = true;
            this.btnCreate.Click += new System.EventHandler(this.btnCreate_Click);
            // 
            // undHeight
            // 
            this.undHeight.Location = new System.Drawing.Point(105, 67);
            this.undHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.undHeight.Name = "undHeight";
            this.undHeight.Size = new System.Drawing.Size(92, 20);
            this.undHeight.TabIndex = 9;
            this.undHeight.Value = new decimal(new int[] {
            18,
            0,
            0,
            0});
            this.undHeight.ValueChanged += new System.EventHandler(this.undHeight_ValueChanged);
            // 
            // undWidth
            // 
            this.undWidth.Location = new System.Drawing.Point(105, 40);
            this.undWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.undWidth.Name = "undWidth";
            this.undWidth.Size = new System.Drawing.Size(92, 20);
            this.undWidth.TabIndex = 8;
            this.undWidth.Value = new decimal(new int[] {
            20,
            0,
            0,
            0});
            this.undWidth.ValueChanged += new System.EventHandler(this.undWidth_ValueChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 69);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(79, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "Height (in tiles):";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 42);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(76, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Width (in tiles):";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 16);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(39, 13);
            this.label3.TabIndex = 10;
            this.label3.Text = "Image:";
            // 
            // txbImage
            // 
            this.txbImage.Location = new System.Drawing.Point(105, 13);
            this.txbImage.Name = "txbImage";
            this.txbImage.Size = new System.Drawing.Size(264, 20);
            this.txbImage.TabIndex = 11;
            // 
            // btnOpenImage
            // 
            this.btnOpenImage.Location = new System.Drawing.Point(375, 11);
            this.btnOpenImage.Name = "btnOpenImage";
            this.btnOpenImage.Size = new System.Drawing.Size(30, 25);
            this.btnOpenImage.TabIndex = 12;
            this.btnOpenImage.Text = "...";
            this.btnOpenImage.UseVisualStyleBackColor = true;
            this.btnOpenImage.Click += new System.EventHandler(this.btnOpenImage_Click);
            // 
            // ofdBitmap
            // 
            this.ofdBitmap.Filter = "Bitmap|*.png;*.bmp;*.jpg;*.gif";
            this.ofdBitmap.Title = "Choose image to import";
            // 
            // picPreviewA
            // 
            this.picPreviewA.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.picPreviewA.Location = new System.Drawing.Point(12, 126);
            this.picPreviewA.Name = "picPreviewA";
            this.picPreviewA.Size = new System.Drawing.Size(128, 139);
            this.picPreviewA.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.picPreviewA.TabIndex = 13;
            this.picPreviewA.TabStop = false;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(218, 42);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(28, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "Left:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(218, 69);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(29, 13);
            this.label5.TabIndex = 7;
            this.label5.Text = "Top:";
            // 
            // undLeft
            // 
            this.undLeft.Location = new System.Drawing.Point(251, 40);
            this.undLeft.Name = "undLeft";
            this.undLeft.Size = new System.Drawing.Size(118, 20);
            this.undLeft.TabIndex = 8;
            // 
            // undTop
            // 
            this.undTop.Location = new System.Drawing.Point(251, 67);
            this.undTop.Name = "undTop";
            this.undTop.Size = new System.Drawing.Size(118, 20);
            this.undTop.TabIndex = 9;
            // 
            // picPreviewB
            // 
            this.picPreviewB.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.picPreviewB.Location = new System.Drawing.Point(146, 126);
            this.picPreviewB.Name = "picPreviewB";
            this.picPreviewB.Size = new System.Drawing.Size(128, 139);
            this.picPreviewB.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.picPreviewB.TabIndex = 13;
            this.picPreviewB.TabStop = false;
            // 
            // picPreviewC
            // 
            this.picPreviewC.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.picPreviewC.Location = new System.Drawing.Point(280, 126);
            this.picPreviewC.Name = "picPreviewC";
            this.picPreviewC.Size = new System.Drawing.Size(128, 139);
            this.picPreviewC.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.picPreviewC.TabIndex = 13;
            this.picPreviewC.TabStop = false;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(50, 98);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(46, 13);
            this.label6.TabIndex = 15;
            this.label6.Text = "Method:";
            // 
            // cbxMethod
            // 
            this.cbxMethod.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbxMethod.FormattingEnabled = true;
            this.cbxMethod.Location = new System.Drawing.Point(105, 93);
            this.cbxMethod.Name = "cbxMethod";
            this.cbxMethod.Size = new System.Drawing.Size(267, 21);
            this.cbxMethod.TabIndex = 16;
            this.cbxMethod.SelectedIndexChanged += new System.EventHandler(this.cbxMethod_SelectedIndexChanged);
            // 
            // ucPaletteEditor1
            // 
            this.ucPaletteEditor1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ucPaletteEditor1.CurrentPalette = palette1;
            this.ucPaletteEditor1.Location = new System.Drawing.Point(12, 289);
            this.ucPaletteEditor1.Name = "ucPaletteEditor1";
            this.ucPaletteEditor1.Size = new System.Drawing.Size(128, 25);
            this.ucPaletteEditor1.TabIndex = 14;
            // 
            // lblTileCount
            // 
            this.lblTileCount.AutoSize = true;
            this.lblTileCount.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.lblTileCount.Location = new System.Drawing.Point(146, 295);
            this.lblTileCount.Name = "lblTileCount";
            this.lblTileCount.Size = new System.Drawing.Size(72, 13);
            this.lblTileCount.TabIndex = 17;
            this.lblTileCount.Text = "360 tiles used";
            // 
            // FrmTilizator
            // 
            this.AcceptButton = this.btnCreate;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(418, 327);
            this.Controls.Add(this.lblTileCount);
            this.Controls.Add(this.cbxMethod);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.ucPaletteEditor1);
            this.Controls.Add(this.picPreviewC);
            this.Controls.Add(this.picPreviewB);
            this.Controls.Add(this.picPreviewA);
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
            this.Load += new System.EventHandler(this.FrmTilizator_Load);
            ((System.ComponentModel.ISupportInitialize)(this.undHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.undWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.picPreviewA)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.undLeft)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.undTop)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.picPreviewB)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.picPreviewC)).EndInit();
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
        private System.Windows.Forms.PictureBox picPreviewA;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.NumericUpDown undLeft;
        private System.Windows.Forms.NumericUpDown undTop;
        private System.Windows.Forms.PictureBox picPreviewB;
        private System.Windows.Forms.PictureBox picPreviewC;
        private Controls.UcPaletteEditor ucPaletteEditor1;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox cbxMethod;
        private System.Windows.Forms.Label lblTileCount;
    }
}