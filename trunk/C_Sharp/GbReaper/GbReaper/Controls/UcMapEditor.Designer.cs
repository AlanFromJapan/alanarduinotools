namespace GbReaper.Controls {
    partial class UcMapEditor {
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
            this.panMap = new System.Windows.Forms.Panel();
            this.panTools = new System.Windows.Forms.Panel();
            this.btnNew = new System.Windows.Forms.Button();
            this.panTools.SuspendLayout();
            this.SuspendLayout();
            // 
            // panMap
            // 
            this.panMap.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panMap.Location = new System.Drawing.Point(32, 0);
            this.panMap.Name = "panMap";
            this.panMap.Size = new System.Drawing.Size(373, 284);
            this.panMap.TabIndex = 0;
            // 
            // panTools
            // 
            this.panTools.BackColor = System.Drawing.SystemColors.ControlDark;
            this.panTools.Controls.Add(this.btnNew);
            this.panTools.Dock = System.Windows.Forms.DockStyle.Left;
            this.panTools.Location = new System.Drawing.Point(0, 0);
            this.panTools.Name = "panTools";
            this.panTools.Size = new System.Drawing.Size(32, 284);
            this.panTools.TabIndex = 1;
            // 
            // btnNew
            // 
            this.btnNew.Dock = System.Windows.Forms.DockStyle.Top;
            this.btnNew.Font = new System.Drawing.Font("MS UI Gothic", 7F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnNew.Location = new System.Drawing.Point(0, 0);
            this.btnNew.Name = "btnNew";
            this.btnNew.Size = new System.Drawing.Size(32, 32);
            this.btnNew.TabIndex = 0;
            this.btnNew.Text = "New";
            this.btnNew.UseVisualStyleBackColor = true;
            this.btnNew.Click += new System.EventHandler(this.btnNew_Click);
            // 
            // UcMapEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panMap);
            this.Controls.Add(this.panTools);
            this.DoubleBuffered = true;
            this.Name = "UcMapEditor";
            this.Size = new System.Drawing.Size(405, 284);
            this.panTools.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panMap;
        private System.Windows.Forms.Panel panTools;
        private System.Windows.Forms.Button btnNew;
    }
}
