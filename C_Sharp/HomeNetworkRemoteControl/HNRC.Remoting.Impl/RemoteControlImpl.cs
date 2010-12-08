using System;
using System.Collections.Generic;
using System.Text;
using HNRC.Remoting.Common;
using System.Runtime.Remoting.Channels;
using System.Runtime.Serialization.Formatters;
using System.Collections;
using System.Runtime.Remoting.Channels.Tcp;
using System.Runtime.Remoting;

using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace HNRC.Remoting.Impl {
    public class RemoteControlImpl : MarshalByRefObject,  IRemoteControlCapable {
        [DllImport("User32.dll")]
        public static extern int FindWindow(string strClassName, string strWindowName);
        [DllImport("User32.dll")]
        public static extern Int32 SendMessage(
            int hWnd,               // handle to destination window
            int Msg,                // message
            int wParam,             // first message parameter
            int lParam);            // second message parameter

        private const int WM_APPCOMMAND = 0x0319;
        private System.Int32 mWindowHandle = 0;


        [Flags]
        public enum ExitWindows : uint {
            // ONE of the following five:
            LogOff = 0x00,
            ShutDown = 0x01,
            Reboot = 0x02,
            PowerOff = 0x08,
            RestartApps = 0x40,
            // plus AT MOST ONE of the following two:
            Force = 0x04,
            ForceIfHung = 0x10,
        }

        [Flags]
        enum ShutdownReason : uint {
            MajorApplication = 0x00040000,
            MajorHardware = 0x00010000,
            MajorLegacyApi = 0x00070000,
            MajorOperatingSystem = 0x00020000,
            MajorOther = 0x00000000,
            MajorPower = 0x00060000,
            MajorSoftware = 0x00030000,
            MajorSystem = 0x00050000,

            MinorBlueScreen = 0x0000000F,
            MinorCordUnplugged = 0x0000000b,
            MinorDisk = 0x00000007,
            MinorEnvironment = 0x0000000c,
            MinorHardwareDriver = 0x0000000d,
            MinorHotfix = 0x00000011,
            MinorHung = 0x00000005,
            MinorInstallation = 0x00000002,
            MinorMaintenance = 0x00000001,
            MinorMMC = 0x00000019,
            MinorNetworkConnectivity = 0x00000014,
            MinorNetworkCard = 0x00000009,
            MinorOther = 0x00000000,
            MinorOtherDriver = 0x0000000e,
            MinorPowerSupply = 0x0000000a,
            MinorProcessor = 0x00000008,
            MinorReconfig = 0x00000004,
            MinorSecurity = 0x00000013,
            MinorSecurityFix = 0x00000012,
            MinorSecurityFixUninstall = 0x00000018,
            MinorServicePack = 0x00000010,
            MinorServicePackUninstall = 0x00000016,
            MinorTermSrv = 0x00000020,
            MinorUnstable = 0x00000006,
            MinorUpgrade = 0x00000003,
            MinorWMI = 0x00000015,

            FlagUserDefined = 0x40000000,
            FlagPlanned = 0x80000000
        }

        [DllImport("user32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool ExitWindowsEx(ExitWindows uFlags, ShutdownReason dwReason);


        #region Constants
        public const int APPCOMMAND_BROWSER_BACKWARD = 1;
        public const int APPCOMMAND_BROWSER_FORWARD = 2;
        public const int APPCOMMAND_BROWSER_REFRESH = 3;
        public const int APPCOMMAND_BROWSER_STOP = 4;
        public const int APPCOMMAND_BROWSER_SEARCH = 5;
        public const int APPCOMMAND_BROWSER_FAVORITES = 6;
        public const int APPCOMMAND_BROWSER_HOME = 7;
        public const int APPCOMMAND_VOLUME_MUTE = 8;
        public const int APPCOMMAND_VOLUME_DOWN = 9;
        public const int APPCOMMAND_VOLUME_UP = 10;
        public const int APPCOMMAND_MEDIA_NEXTTRACK = 11;
        public const int APPCOMMAND_MEDIA_PREVIOUSTRACK = 12;
        public const int APPCOMMAND_MEDIA_STOP = 13;
        public const int APPCOMMAND_MEDIA_PLAY_PAUSE = 14;
        public const int APPCOMMAND_LAUNCH_MAIL = 15;
        public const int APPCOMMAND_LAUNCH_MEDIA_SELECT = 16;
        public const int APPCOMMAND_LAUNCH_APP1 = 17;
        public const int APPCOMMAND_LAUNCH_APP2 = 18;
        public const int APPCOMMAND_BASS_DOWN = 19;
        public const int APPCOMMAND_BASS_BOOST = 20;
        public const int APPCOMMAND_BASS_UP = 21;
        public const int APPCOMMAND_TREBLE_DOWN = 22;
        public const int APPCOMMAND_TREBLE_UP = 23;
        public const int APPCOMMAND_MICROPHONE_VOLUME_MUTE = 24;
        public const int APPCOMMAND_MICROPHONE_VOLUME_DOWN = 25;
        public const int APPCOMMAND_MICROPHONE_VOLUME_UP = 26;
        public const int APPCOMMAND_HELP = 27;
        public const int APPCOMMAND_FIND = 28;
        public const int APPCOMMAND_NEW = 29;
        public const int APPCOMMAND_OPEN = 30;
        public const int APPCOMMAND_CLOSE = 31;
        public const int APPCOMMAND_SAVE = 32;
        public const int APPCOMMAND_PRINT = 33;
        public const int APPCOMMAND_UNDO = 34;
        public const int APPCOMMAND_REDO = 35;
        public const int APPCOMMAND_COPY = 36;
        public const int APPCOMMAND_CUT = 37;
        public const int APPCOMMAND_PASTE = 38;
        public const int APPCOMMAND_REPLY_TO_MAIL = 39;
        public const int APPCOMMAND_FORWARD_MAIL = 40;
        public const int APPCOMMAND_SEND_MAIL = 41;
        public const int APPCOMMAND_SPELL_CHECK = 42;
        public const int APPCOMMAND_DICTATE_OR_COMMAND_CONTROL_TOGGLE = 43;
        public const int APPCOMMAND_MIC_ON_OFF_TOGGLE = 44;
        public const int APPCOMMAND_CORRECTION_LIST = 45;
        public const int APPCOMMAND_MEDIA_PLAY = 46;
        public const int APPCOMMAND_MEDIA_PAUSE = 47;
        public const int APPCOMMAND_MEDIA_RECORD = 48;
        public const int APPCOMMAND_MEDIA_FAST_FORWARD = 49;
        public const int APPCOMMAND_MEDIA_REWIND = 50;
        public const int APPCOMMAND_MEDIA_CHANNEL_UP = 51;
        public const int APPCOMMAND_MEDIA_CHANNEL_DOWN = 52;
		#endregion

		#region Attributes

        #endregion

        #region Properties

        #endregion

        #region Events

        #endregion

        #region Constructor

        #endregion

        #region Methods

        public void Stop() { SendWMAppCommand(APPCOMMAND_MEDIA_STOP, true); }
        public void Play() { SendWMAppCommand(APPCOMMAND_MEDIA_PLAY, true); }
        public void PlayPause() { SendWMAppCommand(APPCOMMAND_MEDIA_PLAY_PAUSE, true); }
        public void Volume(bool pUp) {
            if (pUp) { SendWMAppCommand(APPCOMMAND_VOLUME_UP, true); }
            else { SendWMAppCommand(APPCOMMAND_VOLUME_DOWN, true); }
        }
        public void Next() { SendWMAppCommand(APPCOMMAND_MEDIA_NEXTTRACK, true); }
        public void Prev() { SendWMAppCommand(APPCOMMAND_MEDIA_PREVIOUSTRACK, true); }
        public void Mute() { SendWMAppCommand(APPCOMMAND_VOLUME_MUTE, true); }

        /// <summary>
        /// Sends the APP_COMMAND command to the given windows handler. Code MUST NOT be shifted already
        /// </summary>
        /// <param name="pCommand"></param>
        private void SendWMAppCommand(int pCommand, bool pWithRetry) {
            if (mWindowHandle <= 0) {
                //get the win handle
                mWindowHandle = FindWindow("WMPlayerApp", "Windows Media Player");
            }

            if (mWindowHandle > 0) {
                /*
                 How to call the WM_APPCOMMAND:
                 * 1- Find the window handle and send the message (usual stuff)
                 * 2- 1st parameter of WM_COMMAND (aka the 3rd in the call here under) is 0 (zero)
                 * 3- The second parameter (4th in the call) is the constant value having been shifted left by 16 bite (or * 65536, that's the same) 
                 * 4- Returns 1 when happy, 0 when not happy
                 */
                int vCallReturn = SendMessage(mWindowHandle, WM_APPCOMMAND, 0x00000000, pCommand << 16);
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


        public static void ExposeByRemoting(int pPort) {
            // !! MARCHE !! Pris des Quick Start .NET !!
            // creating a custom formatter for your TcpChannel sink chain.
            BinaryServerFormatterSinkProvider provider = new BinaryServerFormatterSinkProvider();

            provider.TypeFilterLevel = TypeFilterLevel.Full;

            // creating the dictionary to set the port on the channel instance.
            IDictionary props = new Hashtable();

            props["port"] = pPort;

            // pass the props for the port setting and the server provider in the server chain argument. (Client remains null here.)
            TcpChannel chan = new TcpChannel(props, null, provider);

            ChannelServices.RegisterChannel(chan, true); // ensureSecurity
            RemotingConfiguration.RegisterWellKnownServiceType(typeof(RemoteControlImpl), "RemoteControlEndpoint", WellKnownObjectMode.Singleton);
        }


        public void SleepPC() { }
        public void ShutdownPC() { 
            //http://www.pinvoke.net/default.aspx/user32.exitwindowsex
            ExitWindowsEx(ExitWindows.ShutDown, ShutdownReason.MajorOther & ShutdownReason.MinorOther); 
        }

        public void ServerStop() {
            Application.Exit();
        }

        #endregion
    }
}
