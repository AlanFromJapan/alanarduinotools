using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;
using System.Configuration;
using System.Diagnostics;

namespace AvrStudioFlasher {
    class Program {
        public const string HEX_FLASH_FILE_PATH = "HEX_FLASH_FILE_PATH";

        private static readonly Regex REGEX_MCU = new Regex(@"-mmcu=(?<mcutype>[^ ]+)", RegexOptions.IgnoreCase | RegexOptions.Compiled);

        #region Nested classes
        class UploadInfo {
            public string mMakefilePath = null;
            public string mProgrammerProtocol = "avrispv2";
            public string mCOMPort = "COM1";
            public string mHexFilepath = null;
            public string mMCU = null;
            public string mParams = null;
            public string mUploader = null;
        }
        #endregion

        public static void Main(string[] args) {
            Console.WriteLine("AVR Studio Flasher (http://kalshagar.wikispaces.com/AVRStudioFlasher by AlanFromJapan)");
            Console.WriteLine("--------------------------------------------------------------------------------------");
            Console.WriteLine("Parses the AVR Studio 5 makefile and runs a programmer (like a pololu)");
            Console.WriteLine("");
            Console.WriteLine("Usage : AvrStudioFlasher.exe path");
            Console.WriteLine("        path : The path to the makefile to parse OR the directory of the project");
            Console.WriteLine("FYI: COM port and protocol are defined in AvrStudioFlasher.exe.config");

            if (args.Length == 0) {
                Console.WriteLine("\r\n\r\nERROR: you must pass a parameter!");
                return;
            }

            UploadInfo vInfo = ReadMakefile(args[0]);
            RunUploader(vInfo);
        }

        private static void RunUploader(UploadInfo pInfo) {
            string vParams = pInfo.mParams;

            //replacement if any
            vParams = vParams
                .Replace("{comport}", pInfo.mCOMPort)
                .Replace("{hexfile}", pInfo.mHexFilepath)
                .Replace("{mcu}", pInfo.mMCU)
                .Replace("{protocol}", pInfo.mProgrammerProtocol)
                ;

            Console.WriteLine("");
            Console.WriteLine("Using this makefile : " + pInfo.mMakefilePath);
            Console.WriteLine("Ready to launch command :");
            Console.WriteLine(pInfo.mUploader + " " + vParams);

            ProcessStartInfo vStartinfo = new ProcessStartInfo(
                pInfo.mUploader,
                vParams
                ) {
                    UseShellExecute = false
                };

            Process vProc = Process.Start(vStartinfo);

            vProc.WaitForExit();
        }

        private static UploadInfo ReadMakefile(string pPath) {
            UploadInfo vInfo = new UploadInfo();
            string vPath = pPath;
            if (!vPath.EndsWith("makefile", StringComparison.InvariantCultureIgnoreCase)) {
                vPath = Path.Combine(vPath, "makefile");
            }

            if (!File.Exists(vPath)) {
                //second chance, search in bin\debug
                vPath = Path.Combine(
                    Path.GetDirectoryName(vPath),
                    @"debug\makefile"); 

                if (!File.Exists(vPath)) {
                    throw new FileNotFoundException("Missing makefile : " + vPath);
                }
            }
            vInfo.mMakefilePath = vPath;

            using (StreamReader vSR = new StreamReader(new FileStream(vPath, FileMode.Open, FileAccess.Read, FileShare.Read))) {
                string vLine = null;
                while ((vLine = vSR.ReadLine()) != null) {

                    if (string.IsNullOrEmpty(vInfo.mHexFilepath) && vLine.StartsWith(HEX_FLASH_FILE_PATH)) {
                        //HEX_FLASH_FILE_PATH +=Blink.hex
                        //get the .hex name
                        string vHexfile = vLine.Substring(vLine.LastIndexOf("=") + 1).Trim();

                        if (!string.IsNullOrEmpty(vHexfile)) {
                            vInfo.mHexFilepath = Path.Combine(Path.GetDirectoryName(vPath), vHexfile);
                        }
                    }

                    if (vInfo.mMCU == null && REGEX_MCU.IsMatch(vLine)) {
                        //... -std=gnu99  -mmcu=attiny13a   -MD ...
                        //get the mcu type
                        vInfo.mMCU = REGEX_MCU.Match(vLine).Result("${mcutype}");
                    }
                }
            }

            vInfo.mCOMPort = ConfigurationManager.AppSettings["COMport"];
            vInfo.mParams = ConfigurationManager.AppSettings["Params"];
            vInfo.mUploader = ConfigurationManager.AppSettings["UploaderExe"];
            vInfo.mProgrammerProtocol = ConfigurationManager.AppSettings["ProgrammerProtocol"];

            return vInfo;
        }
    }
}
