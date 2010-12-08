using System;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;

namespace ARCPO
{
    
    public class ARCPO_Packet {
        public const int CONTENT_LENGTH = 56;

        public byte mID = 0;
        public byte mType = 0;
        public byte mSubType = 0;
        public bool mExpectAcknowledge = false;
        public char[] mContent = new char[CONTENT_LENGTH];

        /// <summary>
        /// Gets/sets the content as a string
        /// </summary>
        public string ContentString {
            get { 
                int vZero = 0;
                for (; vZero < CONTENT_LENGTH; vZero++){
                    if ((byte)this.mContent[vZero] == 0){
                        return new string(this.mContent, 0, vZero);
                    }
                }
                return string.Empty;
            }
            set { 
                this.mContent = new char[CONTENT_LENGTH];
                char[] vV = value.ToCharArray();

                for (int i = 0; vV != null && i < vV.Length && i < CONTENT_LENGTH; i++)
                {
                    this.mContent[i] = vV[i];
                }
            }
        }

        /// <summary>
        /// Sends a packet on the com port
        /// </summary>
        /// <param name="pPort"></param>
        /// <returns></returns>
        public bool SendPacket(SerialPort pPort) {
            pPort.Write("$*");
            pPort.Write(new byte[] { this.mID }, 0, 1);
            pPort.Write(new byte[] { this.mType }, 0, 1);
            pPort.Write(new byte[] { this.mSubType }, 0, 1);
            pPort.Write(new byte[] { (this.mExpectAcknowledge ? (byte)1 : (byte)0) }, 0, 1);

            //pPort.Write(new string(this.mContent).PadRight(CONTENT_LENGTH, ' '));
            for (int i = 0; i < CONTENT_LENGTH; i++) {
                pPort.Write(new byte[] { (byte)this.mContent[i] }, 0, 1);
            }

            pPort.Write("*$");

            return true;
        }

        /// <summary>
        /// Reads the packet from the com port
        /// </summary>
        /// <param name="serialPort"></param>
        /// <returns></returns>
        public bool ReadPacket(SerialPort pPort)
        {
            bool vShortContent = false;

            if (pPort.BytesToRead == 0) {
                return false;
            }

            while ((char)pPort.ReadByte() != '$') { }
            while ((char)pPort.ReadByte() != '*') { }

            
            this.mID = (byte)pPort.ReadByte();
            this.mType = (byte)pPort.ReadByte();
            this.mSubType = (byte)pPort.ReadByte();
            this.mExpectAcknowledge = ((byte)pPort.ReadByte() == 0 ? false : true);

            for (int i = 0; i < CONTENT_LENGTH; ) {
                this.mContent[i++] = (char)pPort.ReadByte();
                if (i < CONTENT_LENGTH - 1 && this.mContent[i-1] == '*')
                {
                    this.mContent[i++] = (char)pPort.ReadByte();
                    if (this.mContent[i-1] == '$')
                    {
                        vShortContent = true;
                        break;
                    }
                }
            }
            if (!vShortContent)
            {
                while ((char)pPort.ReadByte() != '*') { }
                while ((char)pPort.ReadByte() != '$') { }
            }

            //remove trailing *$
            if (this.mContent != null) {
                for (int i = 0; i < CONTENT_LENGTH-1; i++) {
                    if (this.mContent[i] == '*' && this.mContent[i + 1] == '$') {
                        this.mContent[i] = (char)0;
                        break;
                    }
                }
            }
            return true;
        }

        public override string ToString()
        {
            int vIndexOfZero = 0;

            for (int i = 0; i < this.mContent.Length; i++) {
                if (this.mContent[i] == 0) {
                    vIndexOfZero = i;
                    break;
                }
            }

            string vContent = string.Empty;
            if (vIndexOfZero > 0) {
                vContent = new string(this.mContent, 0, vIndexOfZero);
            }

            return
                string.Format(
@"ID: {0} Type: {1}({2}) Subtype: {3}({4}) Body: '{5}'",
this.mID,
this.mType,
(char)this.mType,
this.mSubType,
(char)this.mSubType,
vContent);
        }
    }
}
