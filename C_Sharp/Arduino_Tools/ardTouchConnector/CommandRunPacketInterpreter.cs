using System;
using System.Collections.Generic;
using System.Text;
using ARCPO;
using System.Diagnostics;
using System.Windows.Forms;
using System.Xml;
using System.IO;

namespace ardTouchConnector {
    public class CommandRunPacketInterpreter : BasePacketInterpreter {
        #region Inner classes

        #endregion

        #region Attributes

        #endregion

        #region Properties

        /// <summary>
        /// Gets the xml selector to this interpreter specific custom commands
        /// </summary>
        protected override string MyCommandRunXMLPath {
            get {
                return "/commands/commandRunInterpreter/customRuns/customRun";
            }
        }
        #endregion



        #region IPacketInterpreter Members

        public override List<byte> AcceptedPacket() {
            List<byte> vL = new List<byte>(new byte[] { 
                (byte)'N',
                (byte)'X',
                (byte)'C',
                (byte)'Z',
            });

            foreach (CommandRun vCR in this.CommandRunDict.Values) {
                vL.Add((byte)vCR.mKey[0]);
            }

            return vL;
        }



        public override void Interprete(ARCPO_Packet pPacket) {
            char vKey = this.GetPacketKey(pPacket);

            switch (vKey) {
                case 'N':
                    RunNotepad();
                    break;
                case 'X':
                    RunCalculatrice();
                    break;
                case 'C':
                    RunCmd();
                    break;
                case 'Z':
                    RunCharmap();
                    break;
                default:
                    //is it a known custom command?
                    if (this.CommandRunDict.ContainsKey(vKey.ToString())) {
                        RunCustom(pPacket);
                    }
                    break;
            }
        }



        /// <summary>
        /// Runs a custom command (dispatch to submethods according packet type)
        /// </summary>
        /// <param name="commandRun"></param>
        private void RunCustom(ARCPO_Packet pPacket) {
            CommandRun vCR = this.CommandRunDict[this.GetPacketKey(pPacket).ToString()];

            if (vCR.IsExtended) {
                this.RunExtendedCommand(vCR, pPacket);
            }
            else {
                this.RunCustom(vCR);
            }
        }


        /// <summary>
        /// Runs a custom command
        /// </summary>
        /// <param name="commandRun"></param>
        private void RunCustom(ICommandRunnable pCommandRun) {
            if (pCommandRun.GetMustConfirm() &&
                DialogResult.Yes != MessageBox.Show("Are you sure you want to run " + pCommandRun.GetLabel() + " now ?", "Please confirm", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1)) {
                return;
            }

            Process vP = new Process();
            vP.StartInfo.FileName = pCommandRun.GetCommand();

            if (pCommandRun.GetParams() != null) {
                vP.StartInfo.Arguments = pCommandRun.GetParams();
            }

            vP.Start();
        }


        /// <summary>
        /// Runs an extended command, where the action is not defined only by the command type, but by also other data within the packet
        /// </summary>
        /// <param name="pPacket"></param>
        private void RunExtendedCommand(CommandRun pCR, ARCPO_Packet pPacket) {
            //search for the matching commandExt
            CommandRunExt vCRE = null;
            foreach (CommandRunExt vIter in pCR.mSubCommands) {
                if (
                    (string.IsNullOrEmpty(vIter.mPacketSubtype) || vIter.mPacketSubtype[0] == (char)pPacket.mSubType)
                    &&
                    (string.IsNullOrEmpty(vIter.mPacketBody) || vIter.mPacketBody == pPacket.ContentString)
                    ) {
                    //found
                    vCRE = vIter;
                    break;
                }
            }

            if (vCRE != null) {
                RunCustom(vCRE);
            }
        }

        private void RunNotepad() {
            Process vP = new Process();
            vP.StartInfo.FileName = "notepad.exe";

            vP.Start();
        }

        private void RunCalculatrice() {
            Process vP = new Process();
            vP.StartInfo.FileName = "calc.exe";

            vP.Start();
        }

        private void RunCharmap() {
            Process vP = new Process();
            vP.StartInfo.FileName = "charmap.exe";

            vP.Start();
        }

        private void RunCmd() {
            Process vP = new Process();
            vP.StartInfo.FileName = "cmd.exe";

            vP.Start();
        }



        #endregion
    }
}
