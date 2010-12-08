using ZedGraph;
namespace ARD_AnalogView
{
    partial class Form1
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.panColor = new System.Windows.Forms.Panel();
            this.panIn = new System.Windows.Forms.Panel();
            this.lblValue = new System.Windows.Forms.Label();
            this.zgGraph = new ZedGraph.ZedGraphControl();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton2 = new System.Windows.Forms.ToolStripButton();
            this.panColor.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panColor
            // 
            this.panColor.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panColor.BackColor = System.Drawing.Color.Black;
            this.panColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panColor.Controls.Add(this.panIn);
            this.panColor.Location = new System.Drawing.Point(12, 27);
            this.panColor.MinimumSize = new System.Drawing.Size(38, 0);
            this.panColor.Name = "panColor";
            this.panColor.Size = new System.Drawing.Size(38, 379);
            this.panColor.TabIndex = 0;
            // 
            // panIn
            // 
            this.panIn.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panIn.BackColor = System.Drawing.SystemColors.Control;
            this.panIn.Location = new System.Drawing.Point(3, 3);
            this.panIn.MinimumSize = new System.Drawing.Size(30, 0);
            this.panIn.Name = "panIn";
            this.panIn.Size = new System.Drawing.Size(30, 85);
            this.panIn.TabIndex = 2;
            // 
            // lblValue
            // 
            this.lblValue.AutoSize = true;
            this.lblValue.Font = new System.Drawing.Font("MS UI Gothic", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.lblValue.Location = new System.Drawing.Point(56, 27);
            this.lblValue.Name = "lblValue";
            this.lblValue.Size = new System.Drawing.Size(60, 33);
            this.lblValue.TabIndex = 1;
            this.lblValue.Text = "xxx";
            // 
            // zgGraph
            // 
            this.zgGraph.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.zgGraph.Location = new System.Drawing.Point(122, 27);
            this.zgGraph.Name = "zgGraph";
            this.zgGraph.ScrollGrace = 0;
            this.zgGraph.ScrollMaxX = 0;
            this.zgGraph.ScrollMaxY = 0;
            this.zgGraph.ScrollMaxY2 = 0;
            this.zgGraph.ScrollMinX = 0;
            this.zgGraph.ScrollMinY = 0;
            this.zgGraph.ScrollMinY2 = 0;
            this.zgGraph.Size = new System.Drawing.Size(785, 379);
            this.zgGraph.TabIndex = 2;
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButton1,
            this.toolStripButton2});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(919, 25);
            this.toolStrip1.TabIndex = 4;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.Checked = true;
            this.toolStripButton1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
            this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Size = new System.Drawing.Size(96, 22);
            this.toolStripButton1.Text = "Show average";
            this.toolStripButton1.Click += new System.EventHandler(this.toolStripButton1_Click);
            // 
            // toolStripButton2
            // 
            this.toolStripButton2.Checked = true;
            this.toolStripButton2.CheckState = System.Windows.Forms.CheckState.Checked;
            this.toolStripButton2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton2.Image")));
            this.toolStripButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton2.Name = "toolStripButton2";
            this.toolStripButton2.Size = new System.Drawing.Size(120, 22);
            this.toolStripButton2.Text = "Convert to voltage";
            this.toolStripButton2.Click += new System.EventHandler(this.toolStripButton2_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(919, 418);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.zgGraph);
            this.Controls.Add(this.lblValue);
            this.Controls.Add(this.panColor);
            this.Name = "Form1";
            this.Text = "Arduino - Analogic viewer";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.panColor.ResumeLayout(false);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panColor;
        private System.Windows.Forms.Label lblValue;
        private System.Windows.Forms.Panel panIn;
        private ZedGraphControl zgGraph;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private System.Windows.Forms.ToolStripButton toolStripButton2;
    }
}

