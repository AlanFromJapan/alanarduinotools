using System;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.Threading;

namespace ARCPO
{
    public class ARCPO_Connector : IDisposable {
		#region Constants
		/// <summary>
		/// Checks for new message every ...
		/// </summary>
		internal const int RECEIVE_POLLING_IN_MS = 430;
		#endregion


		#region Attributes
		private SerialPort mPort = null;

        private string mPortName = null;
        private int mSpeed = 9600;
        private int mPacketCounter = 0;
        private int mRetryCount = 3;

		/// <summary>
		/// If true, means we are waiting for an acknowledgement so DONT poll the messages automatically
		/// </summary>
		protected bool mWaitingForAck = false;
		/// <summary>
		/// Synchro key for that variable
		/// </summary>
		protected object mWaitingForAckSynchroKey = new object();


		/// <summary>
		/// Synchronization object for polling the data on the serial line
		/// </summary>
		protected object mPollingMessageSynchroKey = new object();
		/// <summary>
		/// If we get the message on the serial line automatically
		/// </summary>
		private bool mPollMessages = false;
        #endregion

        #region Properties
		/// <summary>
		/// Gets or sets if we should get the message automatically on the serial line.
		/// DEFAULT IS FALSE (off)
		/// Setting to true will start the thread.
		/// </summary>
		public bool PollMessages {
			get { return mPollMessages; }
			set { 
				mPollMessages = value;

				//start on a worker thread (sufficient)
				ThreadPool.QueueUserWorkItem(new WaitCallback(this.PollMessageThread));
			}
		}

        /// <summary>
        /// Gets/sets the number of retries before giving up sending/receiving a packet
        /// </summary>
        public int RetryCount {
            get { return this.mRetryCount; }
            set { this.mRetryCount = Math.Abs(value); }
        }

        public SerialPort Port {
            get { return this.mPort; }
        }
        #endregion

        #region Events
		/// <summary>
		/// Raised on packet reception (ON ANOTHER THREAD !!)
		/// </summary>
		public event EventHandler<ARCPO_ReceivedEventArgs> PacketReceived;
        #endregion

        #region Constructor
        /// <summary>
        /// Creates a connector on a specific com port
        /// </summary>
        /// <param name="pSpeed"></param>
        /// <param name="pCOMPort"></param>
        public ARCPO_Connector(int pSpeed, string pCOMPort) {
            this.mPortName = pCOMPort;
            this.mSpeed = pSpeed;

            ConnectSerial();
        }


        #endregion

        #region Methods
        /// <summary>
        /// Connects to the serial port
        /// </summary>
        /// <param name="pSpeed"></param>
        /// <param name="pCOMPort"></param>
        private bool ConnectSerial(int pSpeed, string pCOMPort)
        {
            try {
                mPort = new SerialPort(pCOMPort);
                mPort.BaudRate = pSpeed;
                mPort.DataBits = 8;
                mPort.DiscardNull = false;
                mPort.ReadTimeout = 1000;

                mPort.Open();

                return true;
            }
            catch {
                this.DisconnectSerial();
                return false;
            }
        }

        /// <summary>
        /// Connects to the serial port
        /// </summary>
        /// <param name="pSpeed"></param>
        /// <param name="pCOMPort"></param>
        private bool ConnectSerial() {
            return this.ConnectSerial(this.mSpeed, this.mPortName);
        }

        /// <summary>
        /// Disconnects the current port
        /// </summary>
        private void DisconnectSerial() {
            if (this.mPort != null && this.mPort.IsOpen) {
                this.mPort.Close();
            }
            if (this.mPort != null) {
                this.mPort.Dispose();
            }
            this.mPort = null;
        }

        /// <summary>
        /// Returns true if the serial port is properly connected
        /// </summary>
        /// <returns></returns>
        private bool IsSerialConnected() {
            return
                this.mPort != null
                && this.mPort.IsOpen;
        }

        /// <summary>
        /// Sends a packet, retrying in case it fails and returns if ok
        /// </summary>
        /// <param name="pPacket"></param>
        /// <returns></returns>
        public bool SendPacket(ARCPO_Packet pPacket) {
            if (!this.IsSerialConnected() && !this.ConnectSerial()) {
                return false;
            }

            bool vOk = false;
            int vTryCount = 0;

			while (!vOk && vTryCount < this.RetryCount) {
				//force the expect acknowledge except if no retry asked
				pPacket.mExpectAcknowledge = (RetryCount > 0 ? true : pPacket.mExpectAcknowledge);

				//try to make sure we release the wait for ack flag
                try {
                    if (pPacket.mExpectAcknowledge) {
                        lock (mWaitingForAckSynchroKey) {
                            this.mWaitingForAck = true;
                        }
                    }

                    bool vSent = pPacket.SendPacket(this.mPort);

                    if (vSent && pPacket.mExpectAcknowledge) {
                        //locks the line
                        lock (this.mPollingMessageSynchroKey) {
                            #region get the acknowledgement packet
                            //tries n times to read a acknowledge
                            while (!vOk && vTryCount < this.RetryCount) {
                                //wait longer and longer
                                Thread.Sleep(100 * vTryCount);

                                bool vAckOk = false;
                                try {
                                    ARCPO_Packet vAck = new ARCPO_Packet();
                                    vAckOk = vAck.ReadPacket(this.mPort);

                                    if (vAckOk && vAck.mID == pPacket.mID && vAck.ContentString != null && vAck.ContentString.StartsWith("ACK")) {
                                        vOk = true;
                                    }
                                }
                                catch (TimeoutException) { }

                                vTryCount++;
                            }
                            #endregion
                        }


                    }
                    else {
                        //retry
                    }
                }
                catch (InvalidOperationException) { 
                    //disconnected ?
                    this.DisconnectSerial();
                }
				finally {
					//anycases, release the line
					this.mWaitingForAck = false;
				}

				//count the tries
				vTryCount++;
			}

            return vOk;
        }

		/// <summary>
		/// Body of the thread that polls the messages.
		/// It will poll data EVEN IF nobody is registered on the event!
		/// </summary>
		protected void PollMessageThread(object pParam) {
			while (this.mPollMessages) {

				if (!this.mWaitingForAck) { 
					//if not waiting for a ACK
					//small probabily of thread collision, but let's see like that first... (should lock the variable before)
					lock (mWaitingForAckSynchroKey) {
						if (!this.mWaitingForAck) {
							//synchro : lock the line
							lock (this.mPollingMessageSynchroKey) {
								try {
									bool vReceiveOk = false;
									ARCPO_Packet vPacket = new ARCPO_Packet();
									vReceiveOk = vPacket.ReadPacket(this.mPort);

									if (vReceiveOk && this.PacketReceived != null) {
										//received + someone waiting for notification : raise the event
										ARCPO_ReceivedEventArgs vE = new ARCPO_ReceivedEventArgs(vPacket);
										this.PacketReceived(this, vE);
									}
								}
								catch (Exception ex) {
									//nothing (for now)
								}
							}
						}
					}
				}

				//get some rest
				Thread.Sleep(RECEIVE_POLLING_IN_MS);
			}
		}


        /// <summary>
        /// DEBUG purpose :Sends a packet and returns as string whatever is answered by arduino
        /// </summary>
        /// <param name="pPacket"></param>
        /// <returns></returns>
        public string DEBUG_SendPacketAndReturnLineContent(ARCPO_Packet pPacket)
        {
            pPacket.mExpectAcknowledge = true;
            bool vSent = pPacket.SendPacket(this.mPort);
            Thread.Sleep(1000);
            if (this.mPort.BytesToRead > 0)
            {
                return this.mPort.ReadExisting();
            }
            else {
                return null;
            }
        }
        #endregion



        #region IDisposable Members
        /// <summary>
        /// Overriden. Close serial port connection.
        /// </summary>
        public void Dispose() {
            this.DisconnectSerial();
        }

        #endregion
    }
}
