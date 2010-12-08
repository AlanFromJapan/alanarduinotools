using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.Remoting.Channels.Tcp;
using System.Runtime.Remoting.Channels;
using System.Collections;

namespace HNRC.Remoting.Common {
    public static class RemoteHelper {

        public static IRemoteControlCapable GetReferenceToServer(string pLocation, string pDomain, string pLogin, string pPassword) {
            //!!! IMPORTANT !!!
            //also : http://www.codeproject.com/csharp/remotinggui.asp
            IDictionary props = new Hashtable();

            props["port"] = 0;
            props["secure"] = true;
            props["domain"] = pDomain;
            props["username"] = pLogin;
            props["password"] = pPassword;

            TcpChannel chan = new TcpChannel(props, null, null); //0 car n'importe quelle adresse de réponse est acceptée
            ChannelServices.RegisterChannel(chan, true /*ensureSecurity*/);

            return (IRemoteControlCapable)Activator.GetObject(
                typeof(IRemoteControlCapable),
                pLocation
                );
        }
    }
}
