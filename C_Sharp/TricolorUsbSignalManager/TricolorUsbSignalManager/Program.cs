using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Permissions;
using System.Windows.Forms;

namespace TricolorUsbSignalManager {
    static class Program {
        /// <summary>
        /// The main entry point for the application.
        /// 
        /// Exception management: http://msdn.microsoft.com/en-us/library/system.windows.forms.application.setunhandledexceptionmode.aspx
        /// 
        /// </summary>

        [SecurityPermission(SecurityAction.Demand, Flags = SecurityPermissionFlag.ControlAppDomain)]
        [STAThread]
        static void Main() {
            // Add the event handler for handling UI thread exceptions to the event.
            Application.ThreadException += Application_ThreadException;

            // Set the unhandled exception mode to force all Windows Forms errors to go through 
            // our handler.
            Application.SetUnhandledExceptionMode(UnhandledExceptionMode.CatchException);


            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(UnhandledExceptionHandler);

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }

        static void Application_ThreadException(object sender, System.Threading.ThreadExceptionEventArgs e) {
            int i = 0;
        }

        private static void UnhandledExceptionHandler(object sender, UnhandledExceptionEventArgs e) {
            
           
        }
    }
}
