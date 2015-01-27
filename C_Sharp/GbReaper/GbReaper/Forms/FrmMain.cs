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

        private GbProject mProject = new GbProject();

        private void FrmMain_Load(object sender, EventArgs e) {
            Image vM = RomReader.GetRomAsImageUnscaled(@"D:\Gameboy.dev\ROMS\tetris_(v1.1)\Tetris.gb");
            ucRomViewer1.SetImage(vM, 1);

            
            Library vLib = new Library("my lib01");
            mProject.mLibraries.Add(vLib);


            ucLibView.SetLibrary(vLib);


            this.ucRomViewer1.RomSpriteSelected += new GbReaper.Controls.UcRomViewer.RomSpriteSelectDelegate(RomViewer_RomSpriteSelected);
            this.ucLibView.SelectedSpriteChanged += new GbReaper.Controls.UcLibraryList.SelectedSpriteChangedDelegate(LibView_SelectedSpriteChanged);
        }

        void LibView_SelectedSpriteChanged(Sprite pS) {
            ucSpriteEd.SetSprite(pS);
        }

        void RomViewer_RomSpriteSelected(Image pImage) {
            mProject.mLibraries[0].AddSprite(new Sprite(pImage, Palette.DEFAULT_PALETTE));
        }

       

        private void quitToolStripMenuItem_Click(object sender, EventArgs e) {
            Application.Exit();
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e) {
            if (DialogResult.OK == ofdRom.ShowDialog(this)) {
                Image vM = RomReader.GetRomAsImageUnscaled(ofdRom.FileName);
                ucRomViewer1.SetImage(vM, 1);
            }
        }

        private void fileToolStripMenuItem_Click(object sender, EventArgs e) {

        }
    }
}
