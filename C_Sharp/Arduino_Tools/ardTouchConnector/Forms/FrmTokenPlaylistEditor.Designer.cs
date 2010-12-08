namespace ardTouchConnector.Forms {
    partial class FrmTokenPlaylistEditor {
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmTokenPlaylistEditor));
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnSave = new System.Windows.Forms.Button();
            this.lvItems = new System.Windows.Forms.ListView();
            this.imgList = new System.Windows.Forms.ImageList(this.components);
            this.panDetails = new System.Windows.Forms.Panel();
            this.txbBody = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.txbParam = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.txbCommand = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.txbLabel = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.tsbDelete = new System.Windows.Forms.ToolStripButton();
            this.label1 = new System.Windows.Forms.Label();
            this.txbLastToken = new System.Windows.Forms.TextBox();
            this.btnNew = new System.Windows.Forms.Button();
            this.panDetails.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(515, 398);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 0;
            this.btnCancel.Text = "Ca&ncel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // btnSave
            // 
            this.btnSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSave.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnSave.Location = new System.Drawing.Point(434, 398);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(75, 23);
            this.btnSave.TabIndex = 1;
            this.btnSave.Text = "&Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // lvItems
            // 
            this.lvItems.LargeImageList = this.imgList;
            this.lvItems.Location = new System.Drawing.Point(12, 39);
            this.lvItems.MultiSelect = false;
            this.lvItems.Name = "lvItems";
            this.lvItems.ShowItemToolTips = true;
            this.lvItems.Size = new System.Drawing.Size(234, 346);
            this.lvItems.SmallImageList = this.imgList;
            this.lvItems.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.lvItems.StateImageList = this.imgList;
            this.lvItems.TabIndex = 2;
            this.lvItems.UseCompatibleStateImageBehavior = false;
            this.lvItems.SelectedIndexChanged += new System.EventHandler(this.lvItems_SelectedIndexChanged);
            // 
            // imgList
            // 
            this.imgList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imgList.ImageStream")));
            this.imgList.TransparentColor = System.Drawing.Color.Transparent;
            this.imgList.Images.SetKeyName(0, "GenericMusicDoc.ico");
            this.imgList.Images.SetKeyName(1, "app.ico");
            // 
            // panDetails
            // 
            this.panDetails.Controls.Add(this.txbBody);
            this.panDetails.Controls.Add(this.label5);
            this.panDetails.Controls.Add(this.txbParam);
            this.panDetails.Controls.Add(this.label4);
            this.panDetails.Controls.Add(this.txbCommand);
            this.panDetails.Controls.Add(this.label3);
            this.panDetails.Controls.Add(this.txbLabel);
            this.panDetails.Controls.Add(this.label2);
            this.panDetails.Controls.Add(this.toolStrip1);
            this.panDetails.Location = new System.Drawing.Point(252, 39);
            this.panDetails.Name = "panDetails";
            this.panDetails.Size = new System.Drawing.Size(338, 346);
            this.panDetails.TabIndex = 3;
            // 
            // txbBody
            // 
            this.txbBody.Location = new System.Drawing.Point(43, 93);
            this.txbBody.Name = "txbBody";
            this.txbBody.Size = new System.Drawing.Size(278, 19);
            this.txbBody.TabIndex = 8;
            this.txbBody.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.txbBody.TextChanged += new System.EventHandler(this.CREValue_TextChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 80);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(122, 12);
            this.label5.TabIndex = 7;
            this.label5.Text = "RFID/Body unique tag:";
            // 
            // txbParam
            // 
            this.txbParam.Location = new System.Drawing.Point(43, 172);
            this.txbParam.Name = "txbParam";
            this.txbParam.Size = new System.Drawing.Size(278, 19);
            this.txbParam.TabIndex = 6;
            this.txbParam.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.txbParam.TextChanged += new System.EventHandler(this.CREValue_TextChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 115);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(61, 12);
            this.label4.TabIndex = 5;
            this.label4.Text = "Command :";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // txbCommand
            // 
            this.txbCommand.Location = new System.Drawing.Point(43, 130);
            this.txbCommand.Name = "txbCommand";
            this.txbCommand.Size = new System.Drawing.Size(278, 19);
            this.txbCommand.TabIndex = 4;
            this.txbCommand.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.txbCommand.TextChanged += new System.EventHandler(this.CREValue_TextChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 157);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(63, 12);
            this.label3.TabIndex = 3;
            this.label3.Text = "Parameter :";
            // 
            // txbLabel
            // 
            this.txbLabel.Location = new System.Drawing.Point(43, 56);
            this.txbLabel.Name = "txbLabel";
            this.txbLabel.Size = new System.Drawing.Size(278, 19);
            this.txbLabel.TabIndex = 2;
            this.txbLabel.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.txbLabel.TextChanged += new System.EventHandler(this.CREValue_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 41);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "Label :";
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsbDelete});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(338, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // tsbDelete
            // 
            this.tsbDelete.Image = ((System.Drawing.Image)(resources.GetObject("tsbDelete.Image")));
            this.tsbDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbDelete.Name = "tsbDelete";
            this.tsbDelete.Size = new System.Drawing.Size(58, 22);
            this.tsbDelete.Text = "Delete";
            this.tsbDelete.Click += new System.EventHandler(this.tsbDelete_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(101, 12);
            this.label1.TabIndex = 4;
            this.label1.Text = "Latest token read :";
            // 
            // txbLastToken
            // 
            this.txbLastToken.Location = new System.Drawing.Point(119, 6);
            this.txbLastToken.Name = "txbLastToken";
            this.txbLastToken.Size = new System.Drawing.Size(309, 19);
            this.txbLastToken.TabIndex = 5;
            // 
            // btnNew
            // 
            this.btnNew.Image = ((System.Drawing.Image)(resources.GetObject("btnNew.Image")));
            this.btnNew.Location = new System.Drawing.Point(434, 4);
            this.btnNew.Name = "btnNew";
            this.btnNew.Size = new System.Drawing.Size(156, 23);
            this.btnNew.TabIndex = 6;
            this.btnNew.Text = "New from this token";
            this.btnNew.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnNew.UseVisualStyleBackColor = true;
            this.btnNew.Click += new System.EventHandler(this.btnNew_Click);
            // 
            // FrmTokenPlaylistEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(602, 433);
            this.Controls.Add(this.btnNew);
            this.Controls.Add(this.txbLastToken);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.panDetails);
            this.Controls.Add(this.lvItems);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.btnCancel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FrmTokenPlaylistEditor";
            this.Text = "Playlist editor for ardPlayer and other token";
            this.Load += new System.EventHandler(this.FrmTokenPlaylistEditor_Load);
            this.panDetails.ResumeLayout(false);
            this.panDetails.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.ListView lvItems;
        private System.Windows.Forms.Panel panDetails;
        private System.Windows.Forms.ImageList imgList;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton tsbDelete;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txbLastToken;
        private System.Windows.Forms.Button btnNew;
        private System.Windows.Forms.TextBox txbParam;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txbCommand;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txbLabel;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txbBody;
        private System.Windows.Forms.Label label5;
    }
}