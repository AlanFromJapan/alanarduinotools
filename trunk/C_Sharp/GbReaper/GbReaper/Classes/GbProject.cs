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
        protected string mLatestKnownFilename = null;

        public string LatestKnownFilename {
            get { return mLatestKnownFilename; }
            set { mLatestKnownFilename = value; }
        }

        public GbProject() { 
            
        }

        public void AddMap(Map pNewMap) {
            this.mMaps.Add(pNewMap);
            pNewMap.ParentProject = this;
        }

        public void ExportToGBDK(string pPath) {
            foreach (Library vLin in mLibraries) {
                vLin.ExportToGBDK(pPath);
            }

            foreach (Map vM in mMaps) {
                vM.ExportToGBDK(pPath);
            }
        }

        public void SaveAs(string pFilename) {

            
            using (FileStream vFS = new FileStream(pFilename, FileMode.Create, FileAccess.ReadWrite, FileShare.None)) {
                using (StreamWriter vSW = new StreamWriter(vFS)) {
                    vSW.WriteLine(@"<?xml version=""1.0"" encoding=""utf-8"" ?>");
                    vSW.WriteLine("<gbProject format=\"1.0\">");

                    vSW.WriteLine("\t<libraries>");
                    foreach (Library vLib in this.mLibraries) {
                        vLib.SaveToStream(vSW);
                    }    
                    vSW.WriteLine("\t</libraries>");

                    vSW.WriteLine("\t<maps>");
                    foreach (Map vM in this.mMaps) {
                        vM.SaveToStream(vSW);
                    }    
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

                    //First, load the library and its tiles
                    foreach (XmlNode vNode in vDoc.DocumentElement.SelectNodes("/gbProject/libraries/library")) {
                        Library vLib = new Library(vNode.Attributes["name"].Value);

                        foreach (XmlNode vNodeTile in vNode.SelectNodes("./tile")) {
                            Tile vS = Tile.LoadFromXml(vNodeTile);
                            vLib.AddTile(vS);
                        }

                        vResult.mLibraries.Add(vLib);
                    }

                    //Second, load the maps
                    foreach (XmlNode vNode in vDoc.DocumentElement.SelectNodes("/gbProject/maps/map")) {
                        Map vMap = new Map(
                            Convert.ToInt32(vNode.Attributes["width"].Value),
                            Convert.ToInt32(vNode.Attributes["height"].Value)
                            );
                        vMap.Name = vNode.Attributes["name"].Value;

                        foreach (XmlNode vNodeCell in vNode.SelectNodes("./cell")) {
                            Guid vG = new Guid(vNodeCell.Attributes["tileID"].Value);
                            Tile vT = null;

                            foreach (Library vL in vResult.mLibraries) {
                                vT = vL.GetTileByID(vG);
                                if (vT != null)
                                    break;
                            }

                            if (vT != null) {
                                vMap.SetTile(
                                    vT,
                                    Convert.ToInt32(vNodeCell.Attributes["x"].Value),
                                    Convert.ToInt32(vNodeCell.Attributes["y"].Value)
                                );
                            }
                        }

                        vResult.AddMap(vMap);
                    }
                }
            }

            return vResult;
        }
    }
}
