using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;

namespace ardTouchConnector
{
    public class OutlookConnector
    {

        public int mLastMessageCount = -1;

        //http://www.eggheadcafe.com/articles/20060215.asp
        public int GetUnreadMessagesLateBound()
        {

            Type outlook;
            object oApp;

            outlook = Type.GetTypeFromProgID("Outlook.Application");

            oApp = Activator.CreateInstance(outlook);

            object oNameSpace = oApp.GetType().InvokeMember(
                "GetNamespace",
                BindingFlags.GetProperty,
                null,
                oApp,
                new object[1] { "MAPI" });

            object oFolder = oNameSpace.GetType().InvokeMember(
                "GetDefaultFolder",
                BindingFlags.GetProperty,
                null,
                oNameSpace,
                new object[] { 6 }); // ("6" is inbox)

            object oItems = oFolder.GetType().InvokeMember(
                "UnreadItemCount",
                BindingFlags.GetProperty,
                null,
                oFolder,
                null);

            int vCount = (int)oItems;

            return vCount;
        }


        //http://www.eggheadcafe.com/articles/20060215.asp
        //+
        //http://stackoverflow.com/questions/301882/reading-email-without-outlook-app-open
        public string GetLastMessageTitleLateBound()
        {

            Type outlook;
            object oApp;

            outlook = Type.GetTypeFromProgID("Outlook.Application");

            oApp = Activator.CreateInstance(outlook);

            object oNameSpace = oApp.GetType().InvokeMember(
                "GetNamespace",
                BindingFlags.GetProperty,
                null,
                oApp,
                new object[1] { "MAPI" });

            object oFolder = oNameSpace.GetType().InvokeMember(
                "GetDefaultFolder",
                BindingFlags.GetProperty,
                null,
                oNameSpace,
                new object[] { 6 }); // ("6" is inbox)

            object oFolderItems = oFolder.GetType().InvokeMember(
                "Items",
                BindingFlags.GetProperty,
                null,
                oFolder,
                null);

            object oFirstMail = oFolderItems.GetType().InvokeMember(
                "GetFirst",
                BindingFlags.InvokeMethod,
                null,
                oFolderItems,
                null);

            string vSubject = "" + oFirstMail.GetType().InvokeMember(
                "Subject",
                BindingFlags.GetProperty,
                null,
                oFirstMail,
                null);

            return vSubject;
        }
    }
}
