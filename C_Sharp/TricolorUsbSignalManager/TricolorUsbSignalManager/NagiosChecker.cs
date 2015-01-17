using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Text.RegularExpressions;

namespace TricolorUsbSignalManager {
    class NagiosChecker {

        public delegate void LogLineDelegate(string pMessage);
        private LogLineDelegate mLogLineCallback;
        private DateTime mIgnoreBefore = DateTime.MinValue;


        private static readonly Regex REGEX_EVENT_LINE = new Regex(
            //get the tr and the date
            @"\<tr [^>]+\>\<td [^>]+\>(?<date>[^<]+)\</td\>"
            //skip the alert type
            + @"\<td [^>]+\>[^<]+\</td\>"
            //get the host
            + @"\<td [^>]+\>\<a [^>]+\>(?<host>[^<]+)\</a\>\</td\>"
            //get the service
            + @"\<td [^>]+\>\<a [^>]+\>(?<service>[^<]+)\</a\>\</td\>"
            //get the state
            + @"\<td [^>]+\>(?<state>[^<]+)\</td\>"
            , RegexOptions.Compiled | RegexOptions.CultureInvariant | RegexOptions.IgnoreCase);

        private const string TABLE_START = @"<TABLE BORDER=0 CLASS='data'>";
        private const string TABLE_END = @"</TABLE>";

        private string mNagiosURL = null;
        private string mNagiosLogin = null;
        private string mNagiosPassword = null;

        public enum NagiosEventState { OK, WARNING, CRITICAL, UNKNOWN}

        class NagEvent {
            internal int UID;
            internal DateTime mTime;
            internal string mHost;
            internal string mService;
            internal NagiosEventState mState;
            internal bool mFixed = false;

            internal static NagEvent Parse(string pLine, DateTime pIgnoreBefore) {
                if (!REGEX_EVENT_LINE.IsMatch(pLine))
                    return null;

                Match vM = REGEX_EVENT_LINE.Match(pLine);
                NagEvent vEvent = new NagEvent();

                vEvent.mTime = DateTime.ParseExact(vM.Groups["date"].Value, @"MM-dd-yyyy HH:mm:ss", null);
                vEvent.mHost = vM.Groups["host"].Value;
                vEvent.mService = vM.Groups["service"].Value;
                vEvent.mState = (NagiosEventState)Enum.Parse(typeof(NagiosEventState), vM.Groups["state"].Value, true);
                vEvent.mFixed = vEvent.mState == NagiosEventState.OK || vEvent.mState == NagiosEventState.UNKNOWN;

                //consider events BEFORE the limit to be all fixed anyway
                if (vEvent.mTime < pIgnoreBefore) {
                    vEvent.mFixed = true;
                }

                return vEvent;
            }

            public override string ToString() {
                return string.Format("[{3}]date:{0:yyyyMMdd HH:mm:ss} - {1} - {2} [{4}]", this.mTime, this.mHost, this.mService, this.mState, this.mFixed);
            }
        }

        public NagiosChecker(LogLineDelegate pLogger, string pURL, string pLogin, string pPassword)
            : this(pLogger, DateTime.MinValue, pURL, pLogin, pPassword) {
            
        }

        public NagiosChecker(LogLineDelegate pLogger, DateTime pIgnoreBefore, string pURL, string pLogin, string pPassword) {
            this.mLogLineCallback = pLogger;
            this.mIgnoreBefore = pIgnoreBefore;

            this.mNagiosURL = pURL;
            this.mNagiosLogin = pLogin;
            this.mNagiosPassword = pPassword; 
        }

        /// <summary>
        /// Checks the status of Nagios using a custom report: all the HARD alerts over the last 7 days
        /// </summary>
        /// <returns></returns>
        public NagiosEventState CheckStatus() {
            try {
                HttpWebRequest vRequest = (HttpWebRequest)WebRequest.Create(this.mNagiosURL);
                vRequest.Credentials = new NetworkCredential(this.mNagiosLogin, this.mNagiosPassword);
                vRequest.PreAuthenticate = true;

                using (WebResponse vResponse = vRequest.GetResponse()) {
                    using (StreamReader vSR = new StreamReader(vResponse.GetResponseStream())) {
                        string vLine = null;

                        //trash everything line by line before the table
                        do {
                            vLine = vSR.ReadLine().Trim();
                            if (vLine == null) {
                                //something wrong happened
                                throw new Exception("Could not get content table from Nagios.");
                            }
                        }
                        while (!TABLE_START.Equals(vLine, StringComparison.InvariantCultureIgnoreCase));

                        //now we're in the table, parse it
                        int vEventUIDCount = int.MaxValue;
                        SortedList<int, NagEvent> vEventList = new SortedList<int, NagEvent>();
                        do {
                            vLine = vSR.ReadLine().Trim();
                            if (vLine == null) {
                                //something wrong happened
                                throw new Exception("Unexpected EOF from Nagios");
                            }

                            NagEvent vEvt = NagEvent.Parse(vLine, this.mIgnoreBefore);
                            if (vEvt != null) {
                                vEvt.UID = vEventUIDCount--;
                                vEventList.Add(vEvt.UID, vEvt);
                            }
                        }
                        while (!TABLE_END.Equals(vLine, StringComparison.InvariantCultureIgnoreCase));

                        //now we have an ordered list of events. Let's extract the status
                        NagiosEventState vResult = ComputeCurrentStatus(vEventList);

                        if (this.mLogLineCallback != null) {
                            this.mLogLineCallback("Successfully checked Nagios status with result : " + vResult);
                        }

                        return vResult;
                    }
                }   
            }
            catch (Exception ex) {
                string vErrMsg ="ERROR getting Nagios data: " + ex.Message; 
                Debug.WriteLine(vErrMsg);
                if (this.mLogLineCallback != null) {
                    this.mLogLineCallback(vErrMsg);
                }
                return NagiosEventState.WARNING;
            }
        }

        /// <summary>
        /// Parse the list of events (oldest first) and apply the logic: is there an event WARN or CRITICAL that is not followed by a OK?
        /// </summary>
        /// <param name="pEventList"></param>
        /// <returns></returns>
        private static NagiosEventState ComputeCurrentStatus(SortedList<int, NagEvent> pEventList) {
            if (pEventList == null || pEventList.Count == 0)
                return NagiosEventState.OK;

            //go through all the events and set the fixed/not fixed status
            for (int i = 0; i < pEventList.Count; i++) {
                NagEvent e = pEventList.Values[i];

                //skip OK states
                if (e.mState == NagiosEventState.OK)
                    continue;

                for (int j = i + 1; j < pEventList.Count; j++) {
                    NagEvent efix = pEventList.Values[j];

                    if (e.mHost == efix.mHost && e.mService == efix.mService) { 
                        //same host, same service

                        //is it a fix ?
                        if (efix.mState == NagiosEventState.OK) {
                            e.mFixed = true;
                            break;
                        }
                    }
                }
            }

            //now check if there's "not fixed" issues
            NagiosEventState vResult = NagiosEventState.OK;
            for (int i = 0; i < pEventList.Count; i++) { 
                NagEvent e = pEventList.Values[i];
                if (!e.mFixed) {
                    //if ok get wrong
                    if (vResult == NagiosEventState.OK) {
                        vResult = e.mState;
                    }
                    else {
                        //if warn stay warn or escalate
                        if (vResult == NagiosEventState.WARNING) {
                            vResult = e.mState;
                        }                    
                    }
                }
            } 
            return vResult;
        }
    }
}
