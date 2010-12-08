using System;
using System.Collections.Generic;
using System.Text;
using ARCPO;

namespace ardTouchConnector
{
    public interface IPacketInterpreter
    {
        /// <summary>
        /// Does whatever it has to do with the packet
        /// </summary>
        void Interprete(ARCPO_Packet pPacket);

        /// <summary>
        /// Returns the list of accepted packets 
        /// </summary>
        /// <returns></returns>
        List<byte> AcceptedPacket();

    }
}
