using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;
using System.Windows.Forms;
using ARCPO;

namespace ardTouchConnector {
    public abstract class BasePacketInterpreter : IPacketInterpreter {
        #region Constants
        public const string XML_CUSTOM_COMMANDS = "CustomCommands.xml";
        #endregion

        #region Attributes

        protected Dictionary<string, CommandRun> mCommandRunDict = null;

        #endregion

        #region Properties
        /// <summary>
        /// Gets the path to the command xml file
        /// </summary>
        public static string XmlCommandFileName {
            get {
                return Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), XML_CUSTOM_COMMANDS);
            }
        }

        /// <summary>
        /// Gets the xml selector to this interpreter specific custom commands
        /// </summary>
        protected abstract string MyCommandRunXMLPath {
            get;
        }

        /// <summary>
        /// Gets the list of commandrun
        /// </summary>
        protected Dictionary<string, CommandRun> CommandRunDict {
            get {
                if (this.mCommandRunDict == null) {
                    //initialize
                    this.mCommandRunDict = new Dictionary<string, CommandRun>();

                    XmlDocument vDoc = new XmlDocument();
                    vDoc.Load(BasePacketInterpreter.XmlCommandFileName);

                    foreach (XmlNode vN in vDoc.SelectNodes(this.MyCommandRunXMLPath)) {
                        CommandRun vCR = new CommandRun();

                        vCR.mLabel = vN.Attributes["label"].Value;
                        vCR.mKey = vN.Attributes["key"].Value;
                        vCR.mCommand = vN.Attributes["command"].Value;
                        if (vN.Attributes["params"].Value != string.Empty) {
                            vCR.mParams = vN.Attributes["params"].Value;
                        }
                        vCR.mMustConfirm = Convert.ToBoolean(vN.Attributes["mustConfirm"].Value);

                        //is there some extended commands to load?
                        if (vN.ChildNodes.Count > 0) {
                            foreach (XmlNode vSubNode in vN.ChildNodes) {
                                CommandRunExt vCRE = new CommandRunExt();
                                vCRE.mLabel = vSubNode.Attributes["label"].Value;
                                vCRE.mPacketSubtype = vSubNode.Attributes["packetSubtype"].Value;
                                vCRE.mPacketBody = vSubNode.Attributes["packetBody"].Value;
                                vCRE.mCommand = vSubNode.Attributes["command"].Value;
                                if (vSubNode.Attributes["params"].Value != string.Empty) {
                                    vCRE.mParams = vSubNode.Attributes["params"].Value;
                                }
                                vCRE.mMustConfirm = Convert.ToBoolean(vSubNode.Attributes["mustConfirm"].Value);

                                vCR.mSubCommands.Add(vCRE);
                            }
                        }

                        this.mCommandRunDict.Add(vCR.mKey, vCR);
                    }
                }

                return this.mCommandRunDict;
            }
        }
        #endregion


        #region IPacketInterpreter Members

        public abstract void Interprete(ARCPO.ARCPO_Packet pPacket);

        public abstract List<byte> AcceptedPacket();

        #endregion

        /// <summary>
        /// Returns the key to use to identify the intepreter
        /// </summary>
        /// <param name="pPacket"></param>
        /// <returns></returns>
        protected char GetPacketKey(ARCPO_Packet pPacket) { 
            //is a ardTouch packet ?
            if (pPacket.mType == Constants.ARDTOUCH_TYPE && pPacket.mSubType == Constants.ARDTOUCH_SUBTYPE) {
                //ardTouch packet : the key is the first char of the body
                return (pPacket.mContent!= null && pPacket.mContent.Length > 0 ? pPacket.ContentString[0] : (char)0);
            }
            else { 
                //generic, use type
                return (char)pPacket.mType;
            }
        }
    }
}
