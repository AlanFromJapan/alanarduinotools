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
            Image vM = RomReader.GetRomAsImage(@"D:\Gameboy.dev\ROMS\tetris_(v1.1)\Tetris.gb", 2);
            ucRomViewer1.SetImage(vM, 2);


            LoadGbProject(@"D:\Gameboy.dev\ROMS\Legend of Zelda, The - Link's Awakening (USA, Europe) (Rev B)\test001.gbxml");

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
            try {
                //StartEmptyNewProject();
                this.mCurrentProject = GbProject.LoadFromFile(pFilename);
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
            mCurrentProject.mLibraries[0].AddTile(new Tile(pImage, Palette.DEFAULT_PALETTE));
        }

       

        private void quitToolStripMenuItem_Click(object sender, EventArgs e) {
            Application.Exit();
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e) {
            if (DialogResult.OK == ofdRom.ShowDialog(this)) {
                Image vM = RomReader.GetRomAsImage(ofdRom.FileName, 2);
                ucRomViewer1.SetImage(vM, 2);
            }
        }

        private void fileToolStripMenuItem_Click(object sender, EventArgs e) {

        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e) {
            if (sfdProject.ShowDialog(this) == DialogResult.OK) {
                this.mCurrentProject.SaveAs(sfdProject.FileName);    
            }
        }

        private void btnNewMap_Click(object sender, EventArgs e) {
            CreateNewMapAndTab();
        }

        public void SetStatus(string pStatusText) {
            this.stripLabel.Text = pStatusText;
        }
    }
}
