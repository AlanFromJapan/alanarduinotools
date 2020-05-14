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

        internal GbProject mCurrentProject = new GbProject();

        private void FrmMain_Load(object sender, EventArgs e) {
            string[] vArgs = Environment.GetCommandLineArgs();
            if (vArgs != null && vArgs.Length > 1) {
                LoadGbProject(vArgs[1]);
            }
            else {
                StartEmptyNewProject();
                LoadGbProject();
            }

            foreach (Palette p in Palette.WellknownPalettes.Values) {
                cbxPalette.Items.Add(p.mName);
                if (p == Palette.DEFAULT_PALETTE) {
                    cbxPalette.SelectedItem = p.mName;
                }
            }


            //Event handlers
            ucRomViewer1.RomTileViewed += new GbReaper.Controls.UcRomViewer.RomTileSelectDelegate(RomViewer_RomTileViewed);
            ucRomViewer1.RomTileSelectedMultiple += UcRomViewer1_RomTileSelectedMultiple;
            this.ucRomViewer1.RomTileSelected += new GbReaper.Controls.UcRomViewer.RomTileSelectDelegate(RomViewer_RomTileSelected);
            this.ucLibView.SelectedTileChanged += new GbReaper.Controls.UcLibraryList.SelectedTileChangedDelegate(LibView_SelectedTileChanged);
            this.ucLibView.TilesDeleted += UcLibView_TilesDeleted;

            pan32.Paint += new PaintEventHandler(pan32_Paint);
            pan128Alt.Paint += new PaintEventHandler(pan128Alt_Paint);


        }

        private void UcLibView_TilesDeleted(IList<Tile> pDeletedTiles) {
            foreach (TabPage vTP in tabMaps.TabPages) {
                foreach (Control vC in vTP.Controls) {
                    if (vC is UcMapEditor) {
                        ((UcMapEditor)vC).DeleteTiles(pDeletedTiles);
                    }
                }
            }

        }

        private void UcRomViewer1_RomTileSelectedMultiple(IList<Image> pImages) {
            bool pAlreadyExisted;
            if (pImages == null)
                return;

            string vName = null;
            using (FrmTileDetails vFrm = new FrmTileDetails()) {
                if (askTileNameOnImportToolStripMenuItem.Checked && vFrm.ShowDialog(this) == DialogResult.OK) {
                    vName = vFrm.TileName;
                }
            }

            if (!this.horizontalToVerticalWhenROMImportBy4ToolStripMenuItem.Checked || pImages.Count != 4) {
                //not checked or not 4 images
                int i = 1;
                foreach (Image vImage in pImages) {
                    mCurrentProject.mLibraries[0].AddTileWithoutDuplicate(new Tile((vName == null ? string.Empty : string.Format("{0} {1}", vName, i++)), vImage, mCurrentProject.Palette), out pAlreadyExisted);
                }
            }
            else {
                //import order 1-3-2-4 so that the sprite loading is easier
                mCurrentProject.mLibraries[0].AddTileWithoutDuplicate(new Tile((vName == null ? string.Empty : string.Format("{0} {1}", vName, "NW")), pImages[0], mCurrentProject.Palette), out pAlreadyExisted);
                mCurrentProject.mLibraries[0].AddTileWithoutDuplicate(new Tile((vName == null ? string.Empty : string.Format("{0} {1}", vName, "SW")), pImages[2], mCurrentProject.Palette), out pAlreadyExisted);
                mCurrentProject.mLibraries[0].AddTileWithoutDuplicate(new Tile((vName == null ? string.Empty : string.Format("{0} {1}", vName, "NE")), pImages[1], mCurrentProject.Palette), out pAlreadyExisted);
                mCurrentProject.mLibraries[0].AddTileWithoutDuplicate(new Tile((vName == null ? string.Empty : string.Format("{0} {1}", vName, "SE")), pImages[3], mCurrentProject.Palette), out pAlreadyExisted);
            }
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

            LoadGbProject();

            SetStatus("Loading project " + pFilename + " completed.");
        }

        private void LoadGbProject() {

            ucLibView.SetLibrary(mCurrentProject.mLibraries[0]);
            tabMaps.TabPages.Clear();
            foreach (Map vM in this.mCurrentProject.mMaps) {

                TabPage vTP = new TabPage(vM.Name);
                UcMapEditor vME = new UcMapEditor();
                vME.CurrentMap = vM;
                vME.CurrentTile = ucLibView.SelectedTile;
                vME.LibraryList = this.ucLibView;
                vTP.Controls.Add(vME);
                vME.Dock = DockStyle.Fill;
                tabMaps.TabPages.Add(vTP);
                tabMaps.SelectedTab = vTP;

                vME.NewMap += new EventHandler(MapEditor_NewMap);
                vME.DuplicateMap += new EventHandler(MapEditor_DuplicateMap);
            }
        }

        void MapEditor_NewMap(object sender, EventArgs e) {
            CreateNewMapAndTab(null);
        }

        void MapEditor_DuplicateMap(object sender, EventArgs e) {
            //not a fan of doing like that but too lazy to do it right
            Map vOriginalMap = (tabMaps.SelectedTab.Controls[0] as UcMapEditor).CurrentMap;
            UcMapEditor vME = CreateNewMapAndTab(vOriginalMap);

            //copy old to new
            if (vME != null) {
                vME.CurrentMap.Duplicate(vOriginalMap);
            }

        }

        UcMapEditor CreateNewMapAndTab(Map pPrototype) {
            using (FrmNewMap vFrm = new FrmNewMap(pPrototype)) {
                if (DialogResult.OK == vFrm.ShowDialog(this)) {
                    Map vNew = new Map(vFrm.CreateWidth, vFrm.CreateHeight);
                    vNew.Name = vFrm.CreateName;
                    this.mCurrentProject.AddMap(vNew);

                    TabPage vTP = new TabPage(vNew.Name);
                    UcMapEditor vME = new UcMapEditor();
                    vME.CurrentMap = vNew;
                    vME.CurrentTile = ucLibView.SelectedTile;
                    vME.LibraryList = this.ucLibView;
                    vTP.Controls.Add(vME);
                    vME.Dock = DockStyle.Fill;
                    tabMaps.TabPages.Add(vTP);
                    tabMaps.SelectedTab = vTP;

                    vME.NewMap += new EventHandler(MapEditor_NewMap);
                    vME.DuplicateMap += new EventHandler(MapEditor_DuplicateMap);

                    return vME;
                }
                return null;
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
            string vName = null;
            using (FrmTileDetails vFrm = new FrmTileDetails()) {
                if (askTileNameOnImportToolStripMenuItem.Checked && vFrm.ShowDialog(this) == DialogResult.OK) {
                    vName = vFrm.TileName;
                }
            }

            Tile vT = new Tile((vName == null? string.Empty : vName), pImage, mCurrentProject.Palette);
            if (allowImportOfDuplicateToolStripMenuItem.Checked) {
                mCurrentProject.mLibraries[0].AddTile(vT);
                SetStatus("Tile import success.");
            }
            else {
                bool pAlreadyExisted;
                mCurrentProject.mLibraries[0].AddTileWithoutDuplicate(vT, out pAlreadyExisted);

                if (pAlreadyExisted)
                    SetStatus("Tile import cancelled - duplicate.");
                else
                    SetStatus("Tile import success.");
            }
        }

       

        private void quitToolStripMenuItem_Click(object sender, EventArgs e) {
            Application.Exit();
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e) {
            if (DialogResult.OK == ofdRom.ShowDialog(this)) {
                SetStatus("Loading ROM " + ofdRom.FileName + " ...");
                Image vM = RomReader.GetRomAsImage(ofdRom.FileName, 2, mCurrentProject.Palette);
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
            CreateNewMapAndTab(null);
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
            if (this.mCurrentProject.LatestKnownFilename == null)
            {
                saveAsToolStripMenuItem_Click(sender, e);
            }
            else
            {
                this.mCurrentProject.SaveAs(this.mCurrentProject.LatestKnownFilename);
                SetStatus("Saved as " + this.mCurrentProject.LatestKnownFilename + " completed.");
            }
        }

        private void exportForGBDKToolStripMenuItem_Click(object sender, EventArgs e) {
            if (this.mCurrentProject.LatestKnownFilename != null) {
                fbdExport.SelectedPath = Path.GetDirectoryName(this.mCurrentProject.LatestKnownFilename);
            }

            if (fbdExport.ShowDialog(this) == System.Windows.Forms.DialogResult.OK) {
                this.mCurrentProject.ExportToGBDK(fbdExport.SelectedPath, generateStubMaincToolStripMenuItem.Checked, exportGridOnTheMapsToolStripMenuItem.Checked);
                SetStatus("Export to " + fbdExport.SelectedPath + " completed.");
            }
        }

        private void aboutGbReaperToolStripMenuItem_Click(object sender, EventArgs e) {
            MessageBox.Show("http://electrogeek.cc/GbReaper");
        }

        private void cbxPalette_SelectedIndexChanged(object sender, EventArgs e) {
            this.mCurrentProject.Palette = Palette.WellknownPalettes[cbxPalette.SelectedItem.ToString()];
        }

        private void removeUnusedTilesToolStripMenuItem_Click(object sender, EventArgs e) {
            if (DialogResult.No == MessageBox.Show("Tiles unused in any map will be deleted, you're sure?", "Confirm delete unused tiles?", MessageBoxButtons.YesNo, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2)) {
                return;
            }

            if (this.mCurrentProject == null) {
                MessageBox.Show("Load a project first, does not work with unsaved projects.");
                return;
            }

            int vBefore = this.mCurrentProject.mLibraries[0].Count<Tile>();
            this.mCurrentProject.RemoveUnusedTiles();
            int vAfter = this.mCurrentProject.mLibraries[0].Count<Tile>();
            LoadGbProject();

            SetStatus(string.Format("Optimizing current project completed (Tile count {0} => {1}).", vBefore, vAfter));
        }

        private void createMapsToolStripMenuItem_Click(object sender, EventArgs e) {
            CreateNewMapAndTab(null);
        }

        private void reloadToolStripMenuItem_Click(object sender, EventArgs e) {
            if (this.mCurrentProject != null) {
                LoadGbProject();

                SetStatus("Reloaded.");
            }
        }
    }
}
