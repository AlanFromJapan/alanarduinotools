using System;
using System.Collections.Generic;
using System.Text;

namespace ardTouchConnector
{
    public class Constants
    {
        public const int SERIAL_COMMUNICATION_SPEED = 9600;



        public const char TYPE_IN_PUSHBUTTON = 'P';
        public const char TYPE_IN_SHOWLOGO = 'L';

        public const char TYPE_OUT_SENDTIME = 't';
        public const char TYPE_OUT_NEWTEXT = 'x';
        public const char TYPE_OUT_NEWMAIL = 'm';
        public const char TYPE_OUT_SETCONTRAST = 'k';


        public const byte ARDTOUCH_TYPE = 98;
        public const byte ARDTOUCH_SUBTYPE = 128;

    }
}
