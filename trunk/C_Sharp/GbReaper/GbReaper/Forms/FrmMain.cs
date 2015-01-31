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

namespace GbReaper {
    public partial class FrmMain : Form {
        public FrmMain() {
            InitializeComponent();
        }

        private GbProject mCurrentProject = new GbProject();

        private void FrmMain_Load(object sender, EventArgs e) {
            Image vM = RomReader.GetRomAsImage(@"D:\Gameboy.dev\ROMS\tetris_(v1.1)\Tetris.gb", 2);
            ucRomViewer1.SetImage(vM, 2);
            ucRomViewer1.RomSpriteViewed += new GbReaper.Controls.UcRomViewer.RomSpriteSelectDelegate(RomViewer_RomSpriteViewed);


            try {
                //StartEmptyNewProject();
                this.mCurrentProject = GbProject.LoadFromFile(@"D:\Gameboy.dev\ROMS\Legend of Zelda, The - Link's Awakening (USA, Europe) (Rev B)\test001.gbxml");
            }
            catch (Exception) {
                StartEmptyNewProject();
            }

            ucLibView.SetLibrary(mCurrentProject.mLibraries[0]);


            this.ucRomViewer1.RomSpriteSelected += new GbReaper.Controls.UcRomViewer.RomSpriteSelectDelegate(RomViewer_RomSpriteSelected);
            this.ucLibView.SelectedSpriteChanged += new GbReaper.Controls.UcLibraryList.SelectedSpriteChangedDelegate(LibView_SelectedSpriteChanged);

            pan32.Paint += new PaintEventHandler(pan32_Paint);
            pan128Alt.Paint += new PaintEventHandler(pan128Alt_Paint);
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

        void RomViewer_RomSpriteViewed(Image pImage) {
            pan32.Invalidate();
            pan128Alt.Invalidate();
        }


        void LibView_SelectedSpriteChanged(Sprite pS) {
            ucSpriteEd.SetSprite(pS);
        }

        void RomViewer_RomSpriteSelected(Image pImage) {
            mCurrentProject.mLibraries[0].AddSprite(new Sprite(pImage, Palette.DEFAULT_PALETTE));
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
    }
}
