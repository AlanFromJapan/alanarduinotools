using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using ARCPO;
using System.Reflection;

namespace Arduino_Tools
{
    public partial class Form1 : Form
    {
        public const byte PACKET_TYPE_TEXT = 100;
        public const byte PACKET_TYPE_TIME = 101;

		public const byte PACKET_SUBTYPE_DEFAULT = 1;
		public const byte PACKET_SUBTYPE_BLINK = 2;
		public const byte PACKET_SUBTYPE_WARN = 4;
		public const byte PACKET_SUBTYPE_PINK = 8;

        private ARCPO_Connector mConnector = null;
        private int mPacketCounter = 0;
		private int mLastMessCount = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            mConnector = new ARCPO_Connector(9600, "COM8");

			this.mConnector.PacketReceived += new EventHandler<ARCPO_ReceivedEventArgs>(Connector_PacketReceived);
			this.mConnector.PollMessages = true;
        }

		private void Connector_PacketReceived(object sender, ARCPO_ReceivedEventArgs e) {
			textBox1.Text += ">>RECEIVED unexpected message " + e.Packet.mID + " : ";
			textBox1.Text += e.Packet.ContentString;
			textBox1.Text += "\r\n";
		}

        private void button1_Click(object sender, EventArgs e)
        {
            SendPacket();

        }

        private void SendPacketLed(int pLed)
        {
            ARCPO_Packet vP = new ARCPO_Packet();
            int vPacketId = mPacketCounter;
            vP.mType = 199;
            vP.mID = (byte)(mPacketCounter++ % 256);
            vP.mSubType = (byte)pLed;
            vP.mExpectAcknowledge = ckbAck.Checked; ;
            vP.ContentString = "Led : " + pLed;

            this.SendPacket(vP);
        }

        private void SendPacket(string pContent, byte pType, byte pSubType)
        {
            ARCPO_Packet vP = new ARCPO_Packet();
            int vPacketId = mPacketCounter;
            vP.mType = pType;
            vP.mID = (byte)(mPacketCounter++ % 256);
            vP.mSubType = pSubType;
            vP.mExpectAcknowledge = ckbAck.Checked; ;
            vP.ContentString = pContent;

            this.SendPacket(vP);
        }

        private void SendPacket()
        {
            SendPacket("Chat : " + DateTime.Now.ToLongTimeString(), 1, 45);
        }

        private void SendPacket(ARCPO_Packet pP)
        {
			bool vSentSuccess = false;

			for (int i = 0; !vSentSuccess && i < 3; i++) {
				textBox1.Text += ">>Sent message " + pP.mID + " : ";

				vSentSuccess = mConnector.SendPacket(pP);

				textBox1.Text += vSentSuccess + "\r\n";
			}
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (ckbAuto.Checked) {
                SendPacket();
            }
            if (ckbTime.Checked)
            {
                SendPacket("    " + DateTime.Now.ToLongTimeString(), PACKET_TYPE_TIME, PACKET_SUBTYPE_DEFAULT);
            }
            Application.DoEvents();
            /*
            if (!ckbUpdate.Checked) {
                return;
            }
            StringBuilder vSB = new StringBuilder();
            while (mPort.BytesToRead > 0)
            {
                //vSB.AppendLine(mPort.ReadExisting());
                vSB.Append((char)mPort.ReadByte());
            }
            textBox1.Text += vSB.ToString();
             */
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Random vRnd = new Random();
            int vLen = vRnd.Next(1, 100);

            for (int i = 0; i < vLen; i++) {
                this.mConnector.Port.Write(new byte[] { (byte)vRnd.Next(0, 255) }, 0, 1);
            }

            textBox1.Text += "Wrote junk chars : " + vLen;
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            textBox1.Clear();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            SendPacketLed(1);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            SendPacketLed(2);
        }

        private void button5_Click(object sender, EventArgs e)
        {
			byte vSub = PACKET_SUBTYPE_DEFAULT;
			//vSub = (byte)(vSub | PACKET_SUBTYPE_PINK | PACKET_SUBTYPE_WARN);
			if (ckbMessageBlink.Checked) {
				vSub = (byte)(vSub | PACKET_SUBTYPE_BLINK);
			}
			if (ckbMessagePink.Checked) {
				vSub = (byte)(vSub | PACKET_SUBTYPE_PINK);
			}
			if (ckbMessageWarn.Checked) {
				vSub = (byte)(vSub | PACKET_SUBTYPE_WARN);
			}

			SendPacket(
				"Ligne 1...      " +
				"  ... et ligne 2",
				PACKET_TYPE_TEXT,
				vSub
				);
        }

        private void ckbTime_CheckedChanged(object sender, EventArgs e)
        {

        }

		//http://www.eggheadcafe.com/articles/20060215.asp
		private int GetUnreadMessagesLateBound() {

			Type outlook;
			object oApp;

			outlook = Type.GetTypeFromProgID("Outlook.Application");

			oApp = Activator.CreateInstance(outlook);

			object oNameSpace = oApp.GetType().InvokeMember(
				"GetNamespace",
				BindingFlags.GetProperty,
				null,
				oApp,
				new object[1] { "MAPI" });

			object oFolder = oNameSpace.GetType().InvokeMember(
				"GetDefaultFolder",
				BindingFlags.GetProperty,
				null,
				oNameSpace,
				new object[] { 6 }); // ("6" is inbox)

			object oItems = oFolder.GetType().InvokeMember(
				"UnreadItemCount",
				BindingFlags.GetProperty,
				null,
				oFolder,
				null);

			int vCount = (int)oItems;

			return vCount;
		}


		//http://www.eggheadcafe.com/articles/20060215.asp
		//+
		//http://stackoverflow.com/questions/301882/reading-email-without-outlook-app-open
		private string GetLastMessageTitleLateBound() {

			Type outlook;
			object oApp;

			outlook = Type.GetTypeFromProgID("Outlook.Application");

			oApp = Activator.CreateInstance(outlook);

			object oNameSpace = oApp.GetType().InvokeMember(
				"GetNamespace",
				BindingFlags.GetProperty,
				null,
				oApp,
				new object[1] { "MAPI" });

			object oFolder = oNameSpace.GetType().InvokeMember(
				"GetDefaultFolder",
				BindingFlags.GetProperty,
				null,
				oNameSpace,
				new object[] { 6 }); // ("6" is inbox)

			object oFolderItems = oFolder.GetType().InvokeMember(
				"Items",
				BindingFlags.GetProperty,
				null,
				oFolder,
				null);

			object oFirstMail = oFolderItems.GetType().InvokeMember(
				"GetFirst",
				BindingFlags.InvokeMethod,
				null,
				oFolderItems,
				null);

			string vSubject = "" + oFirstMail.GetType().InvokeMember(
				"Subject",
				BindingFlags.GetProperty,
				null,
				oFirstMail,
				null);

			return vSubject;
		}

		private void btnMail_Click(object sender, EventArgs e) {
			string vMessage = GetLastMessageTitleLateBound().ToString();
			SendPacket(
				"Inbox: " + GetUnreadMessagesLateBound().ToString("0000") + "     " +
                vMessage.ToString().Substring(0, Math.Min(16, vMessage.Length)),
                PACKET_TYPE_TEXT,
                (ckbMessageBlink.Checked ? PACKET_SUBTYPE_BLINK: PACKET_SUBTYPE_DEFAULT)
                );
			
		}

		private void ckbMailAuto_CheckedChanged(object sender, EventArgs e) {

		}

		private void tim30sec_Tick(object sender, EventArgs e) {
			if (ckbMailAuto.Checked) {
				int vMessCount = GetUnreadMessagesLateBound();
				if (mLastMessCount != vMessCount) {
					mLastMessCount = vMessCount;

					string vMessage = GetLastMessageTitleLateBound().ToString();
					SendPacket(
						"Inbox: " + vMessCount.ToString("0000") + "     " +
						vMessage.ToString().Substring(0, Math.Min(16, vMessage.Length)),
						PACKET_TYPE_TEXT,
						(byte)(PACKET_SUBTYPE_DEFAULT | PACKET_SUBTYPE_WARN)
						);
				}
			}
		}

        private void button6_Click(object sender, EventArgs e)
        {
            byte vSub = PACKET_SUBTYPE_DEFAULT;
            //vSub = (byte)(vSub | PACKET_SUBTYPE_PINK | PACKET_SUBTYPE_WARN);
            if (ckbMessageBlink.Checked)
            {
                vSub = (byte)(vSub | PACKET_SUBTYPE_BLINK);
            }
            if (ckbMessagePink.Checked)
            {
                vSub = (byte)(vSub | PACKET_SUBTYPE_PINK);
            }
            if (ckbMessageWarn.Checked)
            {
                vSub = (byte)(vSub | PACKET_SUBTYPE_WARN);
            }

            SendPacket(
                CenterText(txbTextLine1.Text, 16) +
                CenterText(txbTextLine2.Text, 16),
                PACKET_TYPE_TEXT,
                vSub
                );
        }

        private string CenterText(string pString, int pLen) { 
            int vHalf = (pLen - pString.Length) / 2; 
            return
                string.Empty.PadLeft(vHalf, ' ') + pString.PadRight(pLen - vHalf, ' ');
        }
    }
}