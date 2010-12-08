using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ARCPO;
using System.Xml;

namespace ardTouchConnector.Forms {
    public partial class FrmTokenPlaylistEditor : Form {
        protected const string EXTENDED_XMLPATH = @"//customRun[@label='(Extended custom commands for ardPlayer)']/*";

        protected const int IMGINDEX_MUSIC = 0;
        protected const int IMGINDEX_OTHER = 1;

        protected List<CommandRunExt> mCREList = new List<CommandRunExt>();
        protected CommandRunExt mCurrentCRE = null;
        protected bool mLoadingItem = false;


        public FrmTokenPlaylistEditor() {
            InitializeComponent();
        }

        private void label4_Click(object sender, EventArgs e) {

        }

        private void FrmTokenPlaylistEditor_Load(object sender, EventArgs e) {
            ARCPO_Packet vP = FrmMain.sUNIQUE_INSTANCE.LatestReceivedPacket;
            if (vP != null) {
                txbLastToken.Text = vP.ContentString;
            }

            ReadCommandsFromXML();

            MakeListItems();
        }

        /// <summary>
        /// Rebuild graphic list of playlist
        /// </summary>
        private void MakeListItems() {
            this.lvItems.Items.Clear();
            foreach (CommandRunExt vCRE in this.mCREList) {
                ListViewItem vLVI = new ListViewItem(vCRE.mLabel, (vCRE.mCommand.Equals("wmplayer.exe", StringComparison.InvariantCultureIgnoreCase) ? IMGINDEX_MUSIC : IMGINDEX_OTHER));
                vLVI.Tag = vCRE;
                this.lvItems.Items.Add(vLVI);
            }
        }

        /// <summary>
        /// Reads the data from on disk xml file
        /// </summary>
        private void ReadCommandsFromXML() {
            XmlDocument vDoc = new XmlDocument();
            vDoc.Load(BasePacketInterpreter.XmlCommandFileName);

            foreach (XmlNode vSubNode in vDoc.SelectNodes(EXTENDED_XMLPATH)) {

                CommandRunExt vCRE = new CommandRunExt();
                vCRE.mLabel = vSubNode.Attributes["label"].Value;
                vCRE.mPacketSubtype = vSubNode.Attributes["packetSubtype"].Value;
                vCRE.mPacketBody = vSubNode.Attributes["packetBody"].Value;
                vCRE.mCommand = vSubNode.Attributes["command"].Value;
                if (vSubNode.Attributes["params"].Value != string.Empty) {
                    vCRE.mParams = vSubNode.Attributes["params"].Value;
                }
                vCRE.mMustConfirm = Convert.ToBoolean(vSubNode.Attributes["mustConfirm"].Value);

                mCREList.Add(vCRE);
            }


        }

        private void lvItems_SelectedIndexChanged(object sender, EventArgs e) {
            if (this.lvItems.SelectedItems.Count == 1) {
                try {
                    mLoadingItem = true;
                    LoadItem(this.lvItems.SelectedItems[0]);
                }
                finally {
                    mLoadingItem = false;
                }
            }
        }

        /// <summary>
        /// Loads an item visually
        /// </summary>
        /// <param name="listViewItem"></param>
        private void LoadItem(ListViewItem pLVI) {
            CommandRunExt vCRE = (CommandRunExt)pLVI.Tag;

            txbLabel.Text = vCRE.mLabel;
            txbCommand.Text = vCRE.mCommand;
            txbParam.Text = vCRE.mParams;
            txbBody.Text = vCRE.mPacketBody;

            mCurrentCRE = vCRE;
        }

        private void CREValue_TextChanged(object sender, EventArgs e) {
            if (!mLoadingItem && this.mCurrentCRE != null) {
                mCurrentCRE.mLabel = txbLabel.Text;
                mCurrentCRE.mCommand = txbCommand.Text;
                mCurrentCRE.mParams = txbParam.Text;
                mCurrentCRE.mPacketBody = txbBody.Text;


                if (this.lvItems.SelectedItems.Count == 1) {
                    this.lvItems.SelectedItems[0].Text = mCurrentCRE.mLabel;
                }
            }
        }

        private void btnNew_Click(object sender, EventArgs e) {
            if (string.IsNullOrEmpty(txbLastToken.Text)) {
                MessageBox.Show("Need a identifier/RFID/message body.");
                return;
            }
            CommandRunExt vCRE = new CommandRunExt();
            vCRE.mPacketBody = txbLastToken.Text;
            vCRE.mLabel = "New token!";

            ListViewItem vLVI = new ListViewItem(vCRE.mLabel, IMGINDEX_OTHER);
            vLVI.Tag = vCRE;
            this.lvItems.Items.Add(vLVI);
            this.mCREList.Add(vCRE);

        }

        private void btnSave_Click(object sender, EventArgs e) {
            if (DialogResult.Yes != MessageBox.Show("Are you sure you want to save ? It will override your previous configuration that will be lost forever.\r\n\r\nAfter saving, close and re-open application for changes to take effect.", "Save warning", MessageBoxButtons.YesNo, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2)) {
                return;
            }

            //go for the save
            XmlDocument vDoc = new XmlDocument();
            vDoc.Load(BasePacketInterpreter.XmlCommandFileName);

            XmlNode vParentNode = vDoc.SelectSingleNode(EXTENDED_XMLPATH).ParentNode;
            while (vParentNode.HasChildNodes) {
                vParentNode.RemoveChild(vParentNode.FirstChild);
            }

            foreach (CommandRunExt vCRE in this.mCREList) {
                XmlElement vElem = vDoc.CreateElement("customRunExt");

                XmlAttribute vAttr = vDoc.CreateAttribute("label");
                vElem.Attributes.Append(vAttr);
                vElem.Attributes["label"].Value = vCRE.mLabel;

                vAttr = vDoc.CreateAttribute("packetSubtype");
                vElem.Attributes.Append(vAttr);
                vElem.Attributes["packetSubtype"].Value = vCRE.mPacketSubtype;

                vAttr = vDoc.CreateAttribute("packetBody");
                vElem.Attributes.Append(vAttr);
                vElem.Attributes["packetBody"].Value = vCRE.mPacketBody;

                vAttr = vDoc.CreateAttribute("command");
                vElem.Attributes.Append(vAttr);
                vElem.Attributes["command"].Value = vCRE.mCommand;

                vAttr = vDoc.CreateAttribute("params");
                vElem.Attributes.Append(vAttr);
                vElem.Attributes["params"].Value = vCRE.mParams;

                vAttr = vDoc.CreateAttribute("mustConfirm");
                vElem.Attributes.Append(vAttr);
                vElem.Attributes["mustConfirm"].Value = vCRE.mMustConfirm.ToString();

                vParentNode.AppendChild(vElem);
            }

            vDoc.Save(BasePacketInterpreter.XmlCommandFileName);

        }

        private void tsbDelete_Click(object sender, EventArgs e) {
            if (this.lvItems.SelectedItems.Count == 1) {
                try {
                    mLoadingItem = true;
                    this.mCREList.Remove((CommandRunExt)this.lvItems.SelectedItems[0].Tag);
                    this.lvItems.Items.Remove(this.lvItems.SelectedItems[0]);
                }
                finally {
                    mLoadingItem = false;
                }
            }
        }
    }
}