using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Configuration;
using System.Threading;
using ardTouchConnector.Forms;

namespace ardTouchConnector
{
    public partial class FrmMain : Form {
        #region Attributes
        public static FrmMain sUNIQUE_INSTANCE = null;

        /// <summary>
        /// The connector
        /// </summary>
        protected ATConnector mConnector = new ATConnector();

        /// <summary>
        /// Connects to Outlook, get mails count...
        /// </summary>
        protected OutlookConnector mOutlook = new OutlookConnector();

        /// <summary>
        /// The log form
        /// </summary>
        protected FrmLog mLogForm = null;


        /// <summary>
        /// All the known command intepreters
        /// </summary>
        protected Dictionary<byte, IPacketInterpreter> mRegisteredIntepreters = new Dictionary<byte, IPacketInterpreter>();

        /// <summary>
        /// Latest packet received
        /// </summary>
        protected ARCPO.ARCPO_Packet mLatestReceivedPacket = null;

        #endregion

        #region Properties

        /// <summary>
        /// Gets if On startup start directly to listen ?
        /// </summary>
        public bool ListenOnStartup {
            get { return Convert.ToBoolean(ConfigurationManager.AppSettings["ListenOnStartup"]); }
        }

        /// <summary>
        /// Gets if Shall the window start minimized and hide in toolbar
        /// </summary>
        public bool MinimizeOnStartup {
            get { return Convert.ToBoolean(ConfigurationManager.AppSettings["MinimizeOnStartup"]); }
        }

        /// <summary>
        /// Gets the latest packet received (null if none)
        /// </summary>
        public ARCPO.ARCPO_Packet LatestReceivedPacket {
            get { return this.mLatestReceivedPacket; }
        }


        /// <summary>
        /// Gets if the show log form should be shown
        /// </summary>
        public bool ShowLog {
            get { return this.mLogForm != null && !this.mLogForm.IsDisposed && this.mLogForm.Visible && ckbShowLog.Checked; }
        }
        #endregion

        public FrmMain()
        {
            sUNIQUE_INSTANCE = this;

            InitializeComponent();
        }

        private void FrmMain_Load(object sender, EventArgs e)
        {
            string vText ="ardTouch Connector - http://kalshagar.wikispaces.com/ardTouch [v " + this.GetType().Assembly.GetName().Version + "]";
            this.Text = vText;

            InitIntepreters();

            for (int i = 0; i <= 99; i++) {
                ddlCOMPort.Items.Add("COM" + i);
            }

            ddlCOMPort.SelectedIndex = ddlCOMPort.Items.IndexOf(ConfigurationManager.AppSettings["COMPort"]);

            mConnector.PacketReceived += new EventHandler<ARCPO.ARCPO_ReceivedEventArgs>(Connector_PacketReceived);

            if (this.ListenOnStartup) {
                StartListening();
            }

            if (this.MinimizeOnStartup) {
                this.ShowInTaskbar = false;
                this.WindowState = FormWindowState.Minimized;
                this.Hide();
            }
        }

        /// <summary>
        /// Registers all the known interpreters
        /// </summary>
        private void InitIntepreters()
        {
            BasePacketInterpreter vI = null;

            vI = new CommandRunPacketInterpreter();
            foreach (byte vB in vI.AcceptedPacket()) {
                this.mRegisteredIntepreters[vB] = vI;
            }


            vI = new MediaPlayerPacketInterpreter();
            foreach (byte vB in vI.AcceptedPacket()) {
                this.mRegisteredIntepreters[vB] = vI;
            }

        }

        /// <summary>
        /// Handles the reception of the mesage. BEWARE IT'S NOT ON THE UI Thread
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Connector_PacketReceived(object sender, ARCPO.ARCPO_ReceivedEventArgs e)
        {
            this.Invoke(new EventHandler<ARCPO.ARCPO_ReceivedEventArgs>(this.Connector_PacketReceived_CROSSTHREADED), sender, e);
        }

        /// <summary>
        /// Manages the reception of message ON THE UI THREAD
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Connector_PacketReceived_CROSSTHREADED(object sender, ARCPO.ARCPO_ReceivedEventArgs e)
        {
            this.mLatestReceivedPacket = e.Packet;

            //Log ?
            if (ShowLog) {
                this.mLogForm.LogMessage(e.ReceivedTime, true, e.Packet);
            }

            //do something ?
            if (e.Packet != null) { 
                //is a ardTouch packet ?
                if (e.Packet.mType == Constants.ARDTOUCH_TYPE && e.Packet.mSubType == Constants.ARDTOUCH_SUBTYPE) {
                    //ardTouch packet : the key is the first char of the body
                    if (ShowLog) {
                        this.mLogForm.LogMessage(e.ReceivedTime, "Received ardTouch packet.");
                    }

                    if (e.Packet.ContentString != null && e.Packet.ContentString.Length > 0) {
                        if (this.mRegisteredIntepreters.ContainsKey((byte)e.Packet.ContentString[0])) {
                            this.mRegisteredIntepreters[(byte)e.Packet.ContentString[0]].Interprete(e.Packet);
                            if (ShowLog) {
                                this.mLogForm.LogMessage(e.ReceivedTime, "Interpreter called.");
                            }
                        }
                        else {
                            if (ShowLog) {
                                this.mLogForm.LogMessage(e.ReceivedTime, "no interpreter for that packet");
                            }
                        }
                    }
                }
                else { 
                    //generic packet from another tool
                    if (ShowLog) {
                        this.mLogForm.LogMessage(e.ReceivedTime, "Received generic packet.");
                    }

                    if (this.mRegisteredIntepreters.ContainsKey((byte)e.Packet.mType)) {
                        this.mRegisteredIntepreters[(byte)e.Packet.mType].Interprete(e.Packet);
                        if (ShowLog) {
                            this.mLogForm.LogMessage(e.ReceivedTime, "Interpreter called.");
                        }
                    }
                    else {
                        if (ShowLog) {
                            this.mLogForm.LogMessage(e.ReceivedTime, "no interpreter for that packet");
                        }
                    }
                }
            }
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            StartListening();
        }

        /// <summary>
        /// Starts listening to message
        /// </summary>
        private void StartListening() {
            mConnector.Start(ddlCOMPort.SelectedItem.ToString(), ckbAcceptMsg.Checked);
            btnStart.Enabled = false;
        }

        private void ckbShowLog_CheckedChanged(object sender, EventArgs e)
        {
            if ((sender as CheckBox).Checked)
            {
                mLogForm = new FrmLog();
                this.mLogForm.FormClosed += new FormClosedEventHandler(LogForm_FormClosed);
                this.mLogForm.Show(this);

            }
            else {
                if (this.mLogForm != null && !this.mLogForm.IsDisposed && this.mLogForm.Visible) {
                    this.mLogForm.Close();
                    this.mLogForm.Dispose();
                    this.mLogForm = null;
                }
            }
        }

        private void LogForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            ckbShowLog.Checked = false;
        }

        private void trbContrast_Scroll(object sender, EventArgs e)
        {
            if (this.mConnector.SendSetContrast(trbContrast.Value))
            {
                lblContrastVal.Text = "(" + trbContrast.Value + ")";
            }

            //int vC = trbContrast.Value+ 128; //to avoid sending the FATAL 0 on the serial link)

            ////string vL = this.mConnector.DEBUG_SendPacketAndReturnLineContent("" + (char)vC, (byte)Constants.TYPE_OUT_SETCONTRAST, (byte)Constants.TYPE_OUT_SETCONTRAST);
            //string vL = this.mConnector.DEBUG_SendPacketAndReturnLineContent("" + (char)vC + vC.ToString() +"bonjour", (byte)Constants.TYPE_OUT_SETCONTRAST, (byte)Constants.TYPE_OUT_SETCONTRAST);
            //if (ShowLog)
            //{
            //    this.mLogForm.LogMessage(DateTime.Now, vL);
            //}
        }

        private void btnSendText_Click(object sender, EventArgs e)
        {
            this.mConnector.SendText(txbSendText.Text);
        }

        private void btnTestMonitorMail_Click(object sender, EventArgs e)
        {
            this.mConnector.SendMailMessage("zero mail", 0);
            Thread.Sleep(1500);
            this.mConnector.SendMailMessage("cent mails", 100);
            Thread.Sleep(1500);
            this.mConnector.SendMailMessage("deux cent cinquante six mails", 256);
            Thread.Sleep(1500);
            this.mConnector.SendMailMessage("beaucoup de mails", 1234);
            Thread.Sleep(1500);
        }

        private void timOutlook_Tick(object sender, EventArgs e)
        {

            try
            {
                int vMessCount = this.mOutlook.GetUnreadMessagesLateBound();
                if (vMessCount != this.mOutlook.mLastMessageCount)
                {
                    this.mOutlook.mLastMessageCount = vMessCount;
                    string vMessage = this.mOutlook.GetLastMessageTitleLateBound();
                    this.mConnector.SendMailMessage(vMessage.Substring(0, Math.Min(ARCPO.ARCPO_Packet.CONTENT_LENGTH - 2, vMessage.Length)), vMessCount);
                }
            }
            catch (Exception ex)
            {
                if (ShowLog)
                {
                    this.mLogForm.LogMessage(DateTime.Now, "ERROR CHECKING OUTLOOK: " + ex.Message);
                }
            }
        }

        private void ckbMailMonitor_CheckedChanged(object sender, EventArgs e)
        {
            this.timOutlook.Enabled = (sender as CheckBox).Checked;
        }

        private void timTime_Tick(object sender, EventArgs e) {
            this.mConnector.SendTime();
        }

        private void ckbSendTime_CheckedChanged(object sender, EventArgs e) {
            this.timTime.Enabled = !this.timTime.Enabled;
        }

        private void btnPlaylistEditor_Click(object sender, EventArgs e) {
            using (FrmTokenPlaylistEditor vFrm = new FrmTokenPlaylistEditor()) {
                vFrm.ShowDialog(this);
            }
        }

        private void nic_MouseDoubleClick(object sender, MouseEventArgs e) {
            this.Show();
            this.WindowState = FormWindowState.Normal;
            this.ShowInTaskbar = true;
        }
    }
}