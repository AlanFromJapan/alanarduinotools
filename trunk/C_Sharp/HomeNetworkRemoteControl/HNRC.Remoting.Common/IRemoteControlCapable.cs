using System;
using System.Collections.Generic;
using System.Text;

namespace HNRC.Remoting.Common {
    public interface IRemoteControlCapable {
        void Stop();
        void Play();
        void PlayPause();
        void Volume(bool pUp);
        void Next();
        void Prev();
        void Mute();

        void SleepPC();
        void ShutdownPC();

        void ServerStop();
    }
}
