namespace ardTouchConnector
{
    partial class FrmLog
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmLog));
            this.grpCommands = new System.Windows.Forms.GroupBox();
            this.btnClear = new System.Windows.Forms.Button();
            this.txbLog = new System.Windows.Forms.TextBox();
            this.grpCommands.SuspendLayout();
            this.SuspendLayout();
            // 
            // grpCommands
            // 
            this.grpCommands.Controls.Add(this.btnClear);
            this.grpCommands.Dock = System.Windows.Forms.DockStyle.Top;
            this.grpCommands.Location = new System.Drawing.Point(0, 0);
            this.grpCommands.Name = "grpCommands";
            this.grpCommands.Size = new System.Drawing.Size(476, 67);
            this.grpCommands.TabIndex = 0;
            this.grpCommands.TabStop = false;
            this.grpCommands.Text = "Commands";
            // 
            // btnClear
            // 
            this.btnClear.Image = ((System.Drawing.Image)(resources.GetObject("btnClear.Image")));
            this.btnClear.Location = new System.Drawing.Point(12, 18);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(75, 23);
            this.btnClear.TabIndex = 0;
            this.btnClear.Text = "Clear";
            this.btnClear.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // txbLog
            // 
            this.txbLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txbLog.Location = new System.Drawing.Point(0, 67);
            this.txbLog.Multiline = true;
            this.txbLog.Name = "txbLog";
            this.txbLog.ReadOnly = true;
            this.txbLog.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txbLog.Size = new System.Drawing.Size(476, 509);
            this.txbLog.TabIndex = 1;
            // 
            // FrmLog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(476, 576);
            this.Controls.Add(this.txbLog);
            this.Controls.Add(this.grpCommands);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "FrmLog";
            this.Opacity = 0.85;
            this.Text = "ardTouch - Log";
            this.grpCommands.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox grpCommands;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.TextBox txbLog;
    }
}