using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml;

namespace GbReaper.Classes {
    public class GbProject {
        public List<Map> mMaps = new List<Map>();
        public List<Library> mLibraries = new List<Library>();

        public void SaveAs(string pFilename) {

            
            using (FileStream vFS = new FileStream(pFilename, FileMode.Create, FileAccess.ReadWrite, FileShare.None)) {
                using (StreamWriter vSW = new StreamWriter(vFS)) {
                    vSW.WriteLine(@"<?xml version=""1.0"" encoding=""utf-8"" ?>");
                    vSW.WriteLine("<gbProject>");

                    vSW.WriteLine("\t<libraries>");
                    foreach (Library vLib in this.mLibraries) {
                        vLib.SaveToStream(vSW);
                    }    
                    vSW.WriteLine("\t</libraries>");

                    vSW.WriteLine("\t<maps>");

                    vSW.WriteLine("\t</maps>");

                    vSW.WriteLine("</gbProject>");
                }                
            }
        }

        public static GbProject LoadFromFile(String pFilename) {
            GbProject vResult = new GbProject();

            using (FileStream vFS = new FileStream(pFilename, FileMode.Open, FileAccess.Read, FileShare.Read)) {
                using (StreamReader vSR = new StreamReader(vFS)) {
                    XmlDocument vDoc = new XmlDocument();
                    vDoc.LoadXml(vSR.ReadToEnd());

                    foreach (XmlNode vNode in vDoc.DocumentElement.SelectNodes("/gbProject/libraries/library")) {
                        Library vLib = new Library(vNode.Attributes["name"].Value);

                        foreach (XmlNode vNodeTile in vNode.SelectNodes("./tile")) {
                            Tile vS = Tile.LoadFromXml(vNodeTile);
                            vLib.AddTile(vS);
                        }

                        vResult.mLibraries.Add(vLib);
                    }
                }
            }

            return vResult;
        }
    }
}
