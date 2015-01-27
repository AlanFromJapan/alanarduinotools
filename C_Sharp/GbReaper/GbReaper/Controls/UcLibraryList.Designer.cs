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
            this.lvLibrary = new System.Windows.Forms.ListView();
            this.SuspendLayout();
            // 
            // lvLibrary
            // 
            this.lvLibrary.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvLibrary.HideSelection = false;
            this.lvLibrary.Location = new System.Drawing.Point(0, 0);
            this.lvLibrary.Name = "lvLibrary";
            this.lvLibrary.OwnerDraw = true;
            this.lvLibrary.Size = new System.Drawing.Size(381, 256);
            this.lvLibrary.TabIndex = 1;
            this.lvLibrary.UseCompatibleStateImageBehavior = false;
            this.lvLibrary.View = System.Windows.Forms.View.Tile;
            this.lvLibrary.SelectedIndexChanged += new System.EventHandler(this.lvLibrary_SelectedIndexChanged);
            // 
            // UcLibraryList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.lvLibrary);
            this.Name = "UcLibraryList";
            this.Size = new System.Drawing.Size(381, 256);
            this.Load += new System.EventHandler(this.UcLibraryList_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView lvLibrary;
    }
}
