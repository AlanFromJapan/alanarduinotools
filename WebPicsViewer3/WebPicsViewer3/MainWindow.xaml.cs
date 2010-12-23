using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Net;
using System.IO;
using System.Text.RegularExpressions;

namespace WebPicsViewer3 {
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window {
        protected static readonly Regex sREGEX_IMG = new Regex(@"<img (?<attributes>[^>]+)>", RegexOptions.IgnoreCase | RegexOptions.Multiline | RegexOptions.CultureInvariant | RegexOptions.Compiled);
        protected static readonly Regex sREGEX_IMG_SRC = new Regex(@"src=""(?<imageName>[^""]+)""", RegexOptions.IgnoreCase | RegexOptions.Multiline | RegexOptions.CultureInvariant | RegexOptions.Compiled);

        protected string mURL = null;

        public MainWindow() {
            InitializeComponent();
        }


        /// <summary>
        /// Loads a page in the system
        /// </summary>
        /// <param name="pURL"></param>
        private void LoadPage(string pURL) {
            InterfaceReset(pURL);
            webBrowser.Navigate(pURL);
            
            LoadThumbs(pURL);
        }

        /// <summary>
        /// Loads thumbs for the current document
        /// </summary>
        /// <param name="p"></param>
        private void LoadThumbs(string pURL) {
            HttpWebRequest vReq = (HttpWebRequest)WebRequest.Create(pURL);
            HttpWebResponse vResponse = (HttpWebResponse)vReq.GetResponse();
            string vPageContent = null;
            using (StreamReader vSR = new StreamReader(vResponse.GetResponseStream())) {
                vPageContent = vSR.ReadToEnd();
            }

            //for all images
            foreach (Match vMatchImg in sREGEX_IMG.Matches(vPageContent)) { 
                //search the attributes
                string vImageAttributes = vMatchImg.Result("${attributes}");

                if (sREGEX_IMG_SRC.IsMatch(vImageAttributes)) {
                    string vImageSrc = sREGEX_IMG_SRC.Match(vImageAttributes).Result("${imageName}");
                    string vImageURL = pURL.Substring(0, pURL.LastIndexOf("/") + 1) + vImageSrc;

                    AddThumbnail(vImageURL);
                }
            }

        }

        /// <summary>
        /// Add thumbnail image
        /// </summary>
        /// <param name="pImageSrc"></param>
        private void AddThumbnail(string pImageSrc) {
            /*
            Label vLbl = new Label();
            vLbl.Content = pImageSrc;
            wrpThumbsPanel.Children.Add(vLbl);
             */

            Image vImg = new Image();
            vImg.Width = 120;
            vImg.Height = 120;
            vImg.Stretch = Stretch.Uniform;
            
            vImg.Source = new BitmapImage(new Uri(pImageSrc));
            wrpThumbsPanel.Children.Add(vImg);
        }

        /// <summary>
        /// Resets the interface to prepare display of a new page
        /// </summary>
        /// <param name="pURL"></param>
        private void InterfaceReset(string pURL) {
            wrpThumbsPanel.Children.Clear();
        }

        private void txbURL_PreviewKeyUp(object sender, KeyEventArgs e) {
            if (e.Key == Key.Enter) {
                e.Handled = true;
                mURL = txbURL.Text;
                LoadPage(this.mURL);
            }
        }
    }
}
