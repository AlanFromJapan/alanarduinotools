using System;
using System.Collections.Generic;
using System.Text;

namespace ardTouchConnector {
    public interface ICommandRunnable {
        string GetLabel();
        string GetCommand();
        string GetParams();
        bool GetMustConfirm();
    }

    public class CommandRun : ICommandRunnable {
        public string mLabel = null;
        public string mKey = null;
        public string mCommand = null;
        public string mParams = null;
        public bool mMustConfirm = false;

        public List<CommandRunExt> mSubCommands = new List<CommandRunExt>();

        /// <summary>
        /// Gets if the command is an extended one (has sub commands)
        /// </summary>
        public bool IsExtended {
            get { return this.mSubCommands != null && this.mSubCommands.Count > 0; }
        }

        #region ICommandRunnable Members

        public string GetLabel() {
            return this.mLabel;
        }

        public string GetCommand() {
            return this.mCommand;
        }

        public string GetParams() {
            return this.mParams;
        }

        public bool GetMustConfirm() {
            return this.mMustConfirm;
        }

        #endregion
    }

    public class CommandRunExt : ICommandRunnable {
        public string mLabel = null;
        public string mPacketSubtype = null;
        public string mPacketBody = null;
        public string mCommand = null;
        public string mParams = null;
        public bool mMustConfirm = false;

        #region ICommandRunnable Members

        public string GetLabel() {
            return this.mLabel;
        }

        public string GetCommand() {
            return this.mCommand;
        }

        public string GetParams() {
            return this.mParams;
        }

        public bool GetMustConfirm() {
            return this.mMustConfirm;
        }

        #endregion
    }
}
