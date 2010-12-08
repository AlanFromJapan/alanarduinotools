using System;
using System.Collections.Generic;
using System.Text;
using ARCPO;
using System.Configuration;

namespace ardTouchConnector
{
    /// <summary>
    /// The class that connects the UI and the ardTouch
    /// </summary>
    public class ATConnector
    {
        #region Constants

		#endregion

		#region Attributes
        protected int mPacketCounter = 0;
        protected ARCPO_Connector mConnector = null;

        protected List<ARCPO_Packet> mSentMessageHistory = new List<ARCPO_Packet>();
        protected List<ARCPO_Packet> mReceivedMessageHistory = new List<ARCPO_Packet>();

        #endregion

        #region Properties

        #endregion

        #region Events
        /// <summary>
        /// Raised on packet reception
        /// </summary>
        public event EventHandler<ARCPO_ReceivedEventArgs> PacketReceived;
        #endregion

        #region Constructor

        public ATConnector() {

        }


        #endregion

        #region Methods

        /// <summary>
        /// Starts the connection to the ardTouch
        /// </summary>
        public void Start(string pCOMPort, bool vPollMessage)
        {
            mConnector = new ARCPO_Connector(Constants.SERIAL_COMMUNICATION_SPEED, pCOMPort);

            mConnector.PacketReceived -= new EventHandler<ARCPO_ReceivedEventArgs>(Connector_PacketReceived);
            mConnector.PacketReceived += new EventHandler<ARCPO_ReceivedEventArgs>(Connector_PacketReceived);

            //start thread for listening to messages
            mConnector.PollMessages = vPollMessage;
        }

        /// <summary>
        /// Starts the connection to the ardTouch
        /// </summary>
        public void Start(string pCOMPort) 
        {
            this.Start(pCOMPort, true);
        }

        /// <summary>
        /// Starts the connection to the ardTouch
        /// </summary>
        public void Start() {
            this.Start(ConfigurationManager.AppSettings["COMPort"]);
        }

        /// <summary>
        /// Message received from arduino
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        protected void Connector_PacketReceived(object sender, ARCPO_ReceivedEventArgs e)
        {
            this.mReceivedMessageHistory.Add(e.Packet);

            if (this.PacketReceived != null)
            {
                this.PacketReceived(this, e);
            }

        }


        /// <summary>
        /// Sends a set contrast mesage, returns true if everything ok
        /// </summary>
        /// <param name="pContrast"></param>
        /// <returns></returns>
        public bool SendSetContrast(int pContrast) {
            pContrast += 128; //to avoid sending the FATAL 0 on the serial link)

            return this.SendPacket("" + (char)pContrast, (byte)Constants.TYPE_OUT_SETCONTRAST, (byte)Constants.TYPE_OUT_SETCONTRAST);
        }

        /// <summary>
        /// Sends text message
        /// </summary>
        /// <param name="pContrast"></param>
        /// <returns></returns>
        public bool SendText(string pText)
        {
            return this.SendPacket(pText, (byte)Constants.TYPE_OUT_NEWTEXT, (byte)Constants.TYPE_OUT_NEWTEXT);
        }

        /// <summary>
        /// Sends time message
        /// </summary>
        /// <param name="pContrast"></param>
        /// <returns></returns>
        public bool SendTime() {
            return this.SendPacket(DateTime.Now.ToString("HH:mm  dd/MM/yyyy"), (byte)Constants.TYPE_OUT_SENDTIME, (byte)Constants.TYPE_OUT_SENDTIME);
        }

        /// <summary>
        /// Sends text message
        /// </summary>
        /// <param name="pContrast"></param>
        /// <returns></returns>
        public bool SendMailMessage(string pText, int pCount)
        {
            int vHigh = 1 + (byte)(pCount >> 8);
            int vLow = (byte)(pCount & 255);

            if (pCount == 0) { 
                //send 255 255 means 0
                vHigh = 255;
                vLow = 255;
            }

            return this.SendPacket("" + (char)vHigh + (char)vLow + pText, (byte)Constants.TYPE_OUT_NEWMAIL, (byte)Constants.TYPE_OUT_NEWMAIL);
        }

        #region Send packet methods

        /// <summary>
        /// Prepares a packet based on parameters
        /// </summary>
        /// <param name="pContent"></param>
        /// <param name="pType"></param>
        /// <param name="pSubType"></param>
        /// <returns></returns>
        private ARCPO_Packet MakePacket(string pContent, byte pType, byte pSubType)
        {
            ARCPO_Packet vP = new ARCPO_Packet();
            int vPacketId = mPacketCounter;
            vP.mType = pType;
            //dont allow packet id to be 0!!
            vP.mID = (byte)((mPacketCounter++ % (256 - 1)) + 1);
            vP.mSubType = pSubType;
            vP.mExpectAcknowledge = true;
            vP.ContentString = pContent;

            return vP;
        }

        /// <summary>
        /// Sends a packet on the line
        /// </summary>
        /// <param name="pContent"></param>
        /// <param name="pType"></param>
        /// <param name="pSubType"></param>
        /// <returns></returns>
        private bool SendPacket(string pContent, byte pType, byte pSubType)
        {
            ARCPO_Packet vP = this.MakePacket(pContent, pType, pSubType);

            return this.SendPacket(vP);
        }

        private bool SendPacket(ARCPO_Packet pP)
        {
            bool vSentSuccess = false;

            //for (int i = 0; !vSentSuccess && i < 3; i++)
            //{
                vSentSuccess = mConnector.SendPacket(pP);
            //}

            return vSentSuccess;
        }

        /// <summary>
        /// DEBUG purpose :Sends a packet and returns as string whatever is answered by arduino
        /// </summary>
        /// <param name="pPacket"></param>
        /// <returns></returns>
        public string DEBUG_SendPacketAndReturnLineContent(string pContent, byte pType, byte pSubType)
        {
            ARCPO_Packet vP = this.MakePacket(pContent, pType, pSubType);

            return this.mConnector.DEBUG_SendPacketAndReturnLineContent(vP);
        }
        #endregion
        #endregion
    }
}
