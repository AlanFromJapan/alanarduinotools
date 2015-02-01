using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using GbReaper.Classes;
using GbReaper.Forms;
using GbReaper.Controls;

namespace GbReaper {
    public partial class FrmMain : Form {
        public FrmMain() {
            InitializeComponent();
        }

        private GbProject mCurrentProject = new GbProject();

        private void FrmMain_Load(object sender, EventArgs e) {
            string[] vArgs = Environment.GetCommandLineArgs();
            if (vArgs != null && vArgs.Length > 0) {
                LoadGbProject(vArgs[0]);
            }
            else {
                StartEmptyNewProject();
            }

            //Event handlers
            ucRomViewer1.RomTileViewed += new GbReaper.Controls.UcRomViewer.RomTileSelectDelegate(RomViewer_RomTileViewed);
            this.ucRomViewer1.RomTileSelected += new GbReaper.Controls.UcRomViewer.RomTileSelectDelegate(RomViewer_RomTileSelected);
            this.ucLibView.SelectedTileChanged += new GbReaper.Controls.UcLibraryList.SelectedTileChangedDelegate(LibView_SelectedTileChanged);

            pan32.Paint += new PaintEventHandler(pan32_Paint);
            pan128Alt.Paint += new PaintEventHandler(pan128Alt_Paint);
        }

        /// <summary>
        /// Loads a GB project
        /// </summary>
        private void LoadGbProject(string pFilename) {
            SetStatus("Loading project " + pFilename + " ...");
            try {
                //StartEmptyNewProject();
                this.mCurrentProject = GbProject.LoadFromFile(pFilename);
                this.mCurrentProject.LatestKnownFilename = pFilename;
            }
            catch (Exception) {
                StartEmptyNewProject();
            }

            ucLibView.SetLibrary(mCurrentProject.mLibraries[0]);
            foreach (Map vM in this.mCurrentProject.mMaps) {

                TabPage vTP = new TabPage(vM.Name);
                UcMapEditor vME = new UcMapEditor();
                vME.CurrentMap = vM;
                vME.CurrentTile = ucLibView.SelectedTile;
                vTP.Controls.Add(vME);
                vME.Dock = DockStyle.Fill;
                tabMaps.TabPages.Add(vTP);
                tabMaps.SelectedTab = vTP;

                vME.NewMap += new EventHandler(MapEditor_NewMap);
            }

            SetStatus("Loading project " + pFilename + " completed.");
        }


        void MapEditor_NewMap(object sender, EventArgs e) {
            CreateNewMapAndTab();
        }

        private void CreateNewMapAndTab() {
            using (FrmNewMap vFrm = new FrmNewMap()) {
                if (DialogResult.OK == vFrm.ShowDialog(this)) {
                    Map vNew = new Map(vFrm.CreateWidth, vFrm.CreateHeight);
                    vNew.Name = vFrm.CreateName;
                    this.mCurrentProject.AddMap(vNew);

                    TabPage vTP = new TabPage(vNew.Name);
                    UcMapEditor vME = new UcMapEditor();
                    vME.CurrentMap = vNew;
                    vME.CurrentTile = ucLibView.SelectedTile;
                    vTP.Controls.Add(vME);
                    vME.Dock = DockStyle.Fill;
                    tabMaps.TabPages.Add(vTP);
                    tabMaps.SelectedTab = vTP;

                    vME.NewMap += new EventHandler(MapEditor_NewMap);
                }
            }
        }

        private void StartEmptyNewProject() {
            Library vLib = new Library("my lib01");
            mCurrentProject.mLibraries.Add(vLib);

        }

        void pan128Alt_Paint(object sender, PaintEventArgs e) {
            if (ucRomViewer1.SelectedTile == null)
                return;
            DrawingLogic.ScaledAndTiledPaintBackground(ucRomViewer1.SelectedTile, e, new Rectangle(new Point(0, 0), ((Control)sender).Size), 4);
        }

        void pan32_Paint(object sender, PaintEventArgs e) {
            if (ucRomViewer1.SelectedTile == null)
                return;
            DrawingLogic.ScalePaintBackground(ucRomViewer1.SelectedTile, e, new Rectangle(new Point(0, 0), ((Control)sender).Size));
        }

        void RomViewer_RomTileViewed(Image pImage) {
            pan32.Invalidate();
            pan128Alt.Invalidate();
        }


        void LibView_SelectedTileChanged(Tile pS) {
            ucTileEd.SetTile(pS);

            //should be reccursive to be perfect but will work
            foreach (TabPage vTP in tabMaps.TabPages) {
                foreach (Control vC in vTP.Controls) {
                    if (vC is UcMapEditor) {
                        ((UcMapEditor)vC).CurrentTile = pS;
                    }
                }
            }
        }

        void RomViewer_RomTileSelected(Image pImage) {
            bool pAlreadyExisted;
            mCurrentProject.mLibraries[0].AddTileWithoutDuplicate(new Tile(pImage, Palette.DEFAULT_PALETTE), out pAlreadyExisted);
        }

       

        private void quitToolStripMenuItem_Click(object sender, EventArgs e) {
            Application.Exit();
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e) {
            if (DialogResult.OK == ofdRom.ShowDialog(this)) {
                SetStatus("Loading ROM " + ofdRom.FileName + " ...");
                Image vM = RomReader.GetRomAsImage(ofdRom.FileName, 2);
                ucRomViewer1.SetImage(vM, 2);
                SetStatus("Loading ROM " + ofdRom.FileName + " completed.");
            }
        }

        private void fileToolStripMenuItem_Click(object sender, EventArgs e) {

        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e) {
            if (sfdProject.ShowDialog(this) == DialogResult.OK) {
                this.mCurrentProject.SaveAs(sfdProject.FileName);
                this.mCurrentProject.LatestKnownFilename = sfdProject.FileName;
                SetStatus("Saved as " + this.mCurrentProject.LatestKnownFilename + " completed.");
            }
        }

        private void btnNewMap_Click(object sender, EventArgs e) {
            CreateNewMapAndTab();
        }

        public void SetStatus(string pStatusText) {
            this.stripLabel.Text = pStatusText;
        }

        private void loadProjectToolStripMenuItem_Click(object sender, EventArgs e) {
            if (DialogResult.OK == ofdProject.ShowDialog(this)) {
                LoadGbProject(ofdProject.FileName);
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e) {
            this.mCurrentProject.SaveAs(this.mCurrentProject.LatestKnownFilename);
            SetStatus("Saved as " + this.mCurrentProject.LatestKnownFilename + " completed.");
        }

        private void exportForGBDKToolStripMenuItem_Click(object sender, EventArgs e) {
            if (fbdExport.ShowDialog(this) == System.Windows.Forms.DialogResult.OK) {
                this.mCurrentProject.ExportToGBDK(fbdExport.SelectedPath);
                SetStatus("Export to " + fbdExport.SelectedPath + " completed.");
            }
        }

        private void aboutGbReaperToolStripMenuItem_Click(object sender, EventArgs e) {
            MessageBox.Show("http://kalshagar.wikispaces.com/GbReaper");
        }
    }
}
