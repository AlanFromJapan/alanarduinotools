namespace GbReaper.Controls {
    partial class UcLibraryList {
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(UcLibraryList));
            this.lvLibrary = new System.Windows.Forms.ListView();
            this.panTop = new System.Windows.Forms.Panel();
            this.btnNew = new System.Windows.Forms.Button();
            this.panTop.SuspendLayout();
            this.SuspendLayout();
            // 
            // lvLibrary
            // 
            this.lvLibrary.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvLibrary.HideSelection = false;
            this.lvLibrary.Location = new System.Drawing.Point(0, 32);
            this.lvLibrary.Name = "lvLibrary";
            this.lvLibrary.OwnerDraw = true;
            this.lvLibrary.Size = new System.Drawing.Size(381, 224);
            this.lvLibrary.TabIndex = 1;
            this.lvLibrary.UseCompatibleStateImageBehavior = false;
            this.lvLibrary.View = System.Windows.Forms.View.Tile;
            this.lvLibrary.SelectedIndexChanged += new System.EventHandler(this.lvLibrary_SelectedIndexChanged);
            // 
            // panTop
            // 
            this.panTop.Controls.Add(this.btnNew);
            this.panTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.panTop.Location = new System.Drawing.Point(0, 0);
            this.panTop.Name = "panTop";
            this.panTop.Size = new System.Drawing.Size(381, 32);
            this.panTop.TabIndex = 2;
            // 
            // btnNew
            // 
            this.btnNew.Dock = System.Windows.Forms.DockStyle.Left;
            this.btnNew.Image = ((System.Drawing.Image)(resources.GetObject("btnNew.Image")));
            this.btnNew.Location = new System.Drawing.Point(0, 0);
            this.btnNew.Name = "btnNew";
            this.btnNew.Size = new System.Drawing.Size(32, 32);
            this.btnNew.TabIndex = 0;
            this.btnNew.UseVisualStyleBackColor = true;
            this.btnNew.Click += new System.EventHandler(this.btnNew_Click);
            // 
            // UcLibraryList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.lvLibrary);
            this.Controls.Add(this.panTop);
            this.Name = "UcLibraryList";
            this.Size = new System.Drawing.Size(381, 256);
            this.Load += new System.EventHandler(this.UcLibraryList_Load);
            this.panTop.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView lvLibrary;
        private System.Windows.Forms.Panel panTop;
        private System.Windows.Forms.Button btnNew;
    }
}
