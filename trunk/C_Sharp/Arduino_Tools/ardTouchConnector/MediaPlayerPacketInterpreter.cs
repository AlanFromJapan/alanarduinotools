using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;
using System.Windows.Forms;
using ARCPO;

namespace ardTouchConnector {
    /// <summary>
    /// Remote controls the Media Player
    /// Based on (thanks a lot to them) :
    /// http://www.codeproject.com/KB/cs/wmp_pinvoke.aspx
    /// http://www.eventghost.org/forum/viewtopic.php?f=2&t=319
    /// http://msdn.microsoft.com/en-us/library/bb417079.aspx
    /// http://msdn.microsoft.com/en-us/library/ms646275%28VS.85%29.aspx
    /// http://www.codeguru.com/cpp/w-p/system/keyboard/article.php/c5655/
    /// </summary>
    public class MediaPlayerPacketInterpreter : BasePacketInterpreter {
        #region Constants

        /// <summary>
        /// Window handle of media player
        /// </summary>
        private System.Int32 mWindowHandle = 0;

        /// <summary>
        /// The code to send for custom command
        /// </summary>
        private const int WM_APPCOMMAND = 0x0319;

        /// <summary>
        /// Windows API Constants
        /// http://www.eventghost.org/forum/viewtopic.php?f=2&t=319
        /// </summary>
        #region API CONSTS
        public const int APPCOMMAND_BROWSER_BACKWARD    =  1;
        public const int APPCOMMAND_BROWSER_FORWARD     =  2;
        public const int APPCOMMAND_BROWSER_REFRESH     =  3;
        public const int APPCOMMAND_BROWSER_STOP        =  4;
        public const int APPCOMMAND_BROWSER_SEARCH      =  5;
        public const int APPCOMMAND_BROWSER_FAVORITES   =  6;
        public const int APPCOMMAND_BROWSER_HOME        =  7;
        public const int APPCOMMAND_VOLUME_MUTE         =  8;
        public const int APPCOMMAND_VOLUME_DOWN         =  9;
        public const int APPCOMMAND_VOLUME_UP           =  10;
        public const int APPCOMMAND_MEDIA_NEXTTRACK     =  11;
        public const int APPCOMMAND_MEDIA_PREVIOUSTRACK =  12;
        public const int APPCOMMAND_MEDIA_STOP          =  13;
        public const int APPCOMMAND_MEDIA_PLAY_PAUSE    =  14;
        public const int APPCOMMAND_LAUNCH_MAIL         =  15;
        public const int APPCOMMAND_LAUNCH_MEDIA_SELECT =  16;
        public const int APPCOMMAND_LAUNCH_APP1         =  17;
        public const int APPCOMMAND_LAUNCH_APP2         =  18;
        public const int APPCOMMAND_BASS_DOWN           =  19;
        public const int APPCOMMAND_BASS_BOOST          =  20;
        public const int APPCOMMAND_BASS_UP             =  21;
        public const int APPCOMMAND_TREBLE_DOWN         =  22;
        public const int APPCOMMAND_TREBLE_UP           =  23;
        public const int APPCOMMAND_MICROPHONE_VOLUME_MUTE = 24;
        public const int APPCOMMAND_MICROPHONE_VOLUME_DOWN = 25;
        public const int APPCOMMAND_MICROPHONE_VOLUME_UP =  26;
        public const int APPCOMMAND_HELP                =  27;
        public const int APPCOMMAND_FIND                =  28;
        public const int APPCOMMAND_NEW                 =  29;
        public const int APPCOMMAND_OPEN                =  30;
        public const int APPCOMMAND_CLOSE               =  31;
        public const int APPCOMMAND_SAVE                =  32;
        public const int APPCOMMAND_PRINT               =  33;
        public const int APPCOMMAND_UNDO                =  34;
        public const int APPCOMMAND_REDO                =  35;
        public const int APPCOMMAND_COPY                =  36;
        public const int APPCOMMAND_CUT                 =  37;
        public const int APPCOMMAND_PASTE               =  38;
        public const int APPCOMMAND_REPLY_TO_MAIL       =  39;
        public const int APPCOMMAND_FORWARD_MAIL        =  40;
        public const int APPCOMMAND_SEND_MAIL           =  41;
        public const int APPCOMMAND_SPELL_CHECK         =  42;
        public const int APPCOMMAND_DICTATE_OR_COMMAND_CONTROL_TOGGLE  = 43;
        public const int APPCOMMAND_MIC_ON_OFF_TOGGLE   =  44;
        public const int APPCOMMAND_CORRECTION_LIST     =  45;
        public const int APPCOMMAND_MEDIA_PLAY          =  46;
        public const int APPCOMMAND_MEDIA_PAUSE         =  47;
        public const int APPCOMMAND_MEDIA_RECORD        =  48;
        public const int APPCOMMAND_MEDIA_FAST_FORWARD  =  49;
        public const int APPCOMMAND_MEDIA_REWIND        =  50;
        public const int APPCOMMAND_MEDIA_CHANNEL_UP    =  51;
        public const int APPCOMMAND_MEDIA_CHANNEL_DOWN  =  52;
        #endregion
        #endregion

        #region Attributes

        private string mClassName = null;
        private string mWindowName = null;

        #endregion

        #region Properties

        /// <summary>
        /// Gets the xml selector to this interpreter specific custom commands
        /// </summary>
        protected override string MyCommandRunXMLPath {
            get {
                return "/commands/commandRunInterpreter/mediaPlayer/customRun";
            }
        }
        #endregion

        #region Events

        #endregion

        #region Constructor

        #endregion

        #region Methods

        /// <summary>
        /// Sends the APP_COMMAND command to the given windows handler. Code MUST NOT be shifted already
        /// </summary>
        /// <param name="pCommand"></param>
        private void SendWMAppCommand(int pCommand) {
            if (this.mClassName == null && this.mWindowName == null) {
                InitFindWindowParams();
            }

            this.SendWMAppCommand(pCommand, mWindowHandle != 0);
        }

        /// <summary>
        /// Reads in the config file the necessary parameters for catching the program to talk to
        /// </summary>
        private void InitFindWindowParams() {
            XmlDocument vDoc = new XmlDocument();
            vDoc.Load(Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), XML_CUSTOM_COMMANDS));
            XmlNode vNode = vDoc.SelectSingleNode("/commands/commandRunInterpreter/mediaPlayer");
            if (vNode == null) {
                throw new Exception("Can't find mediaPlayer node!");
            }

            this.mClassName = (vNode.Attributes["playerClassName"].Value == string.Empty ? null : vNode.Attributes["playerClassName"].Value);
            this.mWindowName = (vNode.Attributes["playerWindowName"].Value == string.Empty ? null : vNode.Attributes["playerWindowName"].Value);
        }

        /// <summary>
        /// Sends the APP_COMMAND command to the given windows handler. Code MUST NOT be shifted already
        /// </summary>
        /// <param name="pCommand"></param>
        private void SendWMAppCommand(int pCommand, bool pWithRetry) {
            if (mWindowHandle <= 0) {
                //get the win handle
                mWindowHandle = Win32.FindWindow(this.mClassName, this.mWindowName);
            }

            if (mWindowHandle > 0) {
                /*
                 How to call the WM_APPCOMMAND:
                 * 1- Find the window handle and send the message (usual stuff)
                 * 2- 1st parameter of WM_COMMAND (aka the 3rd in the call here under) is 0 (zero)
                 * 3- The second parameter (4th in the call) is the constant value having been shifted left by 16 bite (or * 65536, that's the same) 
                 * 4- Returns 1 when happy, 0 when not happy
                 */
                int vCallReturn = Win32.SendMessage(mWindowHandle, WM_APPCOMMAND, 0x00000000, pCommand << 16);
                if (vCallReturn != 1) {
                    //problem? reset the window handle (most likely media player was closed ... and maybe reopened?)
                    mWindowHandle = 0;
                    if (pWithRetry) {
                        //retry, but just once
                        this.SendWMAppCommand(pCommand, false);
                    }
                }
            }
        }
        #endregion

        #region IPacketInterpreter Members

        public override void Interprete(ARCPO_Packet pPacket) {

            //is it a known custom command?
            if (this.CommandRunDict.ContainsKey(this.GetPacketKey(pPacket).ToString())) {
                CommandRun vCR = this.CommandRunDict[this.GetPacketKey(pPacket).ToString()];

                //Idea: use name (dictionnary of known values) and fallback interpretes ... later.
                this.SendWMAppCommand(Convert.ToInt32(vCR.mCommand));
            }
        }

        public override List<byte> AcceptedPacket() {
            List<byte> vL = new List<byte>();

            foreach (CommandRun vCR in this.CommandRunDict.Values) {
                vL.Add((byte)vCR.mKey[0]);
            }

            return vL;
        }

        #endregion
    }
}
