using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace GbReaper.Classes {
    public class Palette {
        public string mName = null;

        public static readonly Palette DEFAULT_PALETTE_PASTEL = new Palette("Pastel", new Color[] { Color.White, Color.LightGray, Color.DarkGray, Color.Gray });
        public static readonly Palette DEFAULT_PALETTE_EQUILIBRATED = new Palette("Equilibrated", new Color[] {
            Color.FromArgb(255,255,255),
            Color.FromArgb(3*255/4,3*255/4,3*255/4),
            Color.FromArgb(2*255/4,2*255/4,2*255/4),
            Color.FromArgb(255/4,255/4,255/4)
        });
        /// <summary>
        /// From https://lospec.com/palette-list/nostalgia
        /// </summary>
        public static readonly Palette DEFAULT_PALETTE_GREEN = new Palette("Green", new Color[] {
            ColorTranslator.FromHtml("#d0d058"),
            ColorTranslator.FromHtml("#a0a840"),
            ColorTranslator.FromHtml("#708028"),
            ColorTranslator.FromHtml("#405010")
        });

        public static readonly Palette DEFAULT_PALETTE = DEFAULT_PALETTE_GREEN;

        public static readonly Dictionary<string, Palette> WellknownPalettes = new Dictionary<string, Palette>();
        static Palette() {
            WellknownPalettes.Add(DEFAULT_PALETTE_EQUILIBRATED.mName, DEFAULT_PALETTE_EQUILIBRATED);
            WellknownPalettes.Add(DEFAULT_PALETTE_PASTEL.mName, DEFAULT_PALETTE_PASTEL);
            WellknownPalettes.Add(DEFAULT_PALETTE_GREEN.mName, DEFAULT_PALETTE_GREEN);
        }

        public Color[] mColors;

        public Palette() : this("Pastel", new Color[] {Color.White, Color.LightGray, Color.DarkGray, Color.Gray}) {
            
        }

        public Palette(string pName, Color[] pColors) {
            this.mColors = pColors;
            this.mName = pName;
        }

        public int GetIndexInPalette(Color pColor) {
            for (int i = 0; i < this.mColors.Length; i++) {
                if (pColor == this.mColors[i])
                    return i;
            }

            return -1;
        }

        /// <summary>
        /// Finds the closest color in palette using the minimum square method
        /// http://www.codeproject.com/Articles/17044/Find-the-Nearest-Color-with-C-Using-the-Euclidean
        /// </summary>
        /// <param name="pColor"></param>
        /// <returns></returns>
        public Color GetNearestColor(Color pColor) {
            Color nearest_color = Color.Empty;
            double dbl_input_red = Convert.ToDouble(pColor.R);
            double dbl_input_green = Convert.ToDouble(pColor.G);
            double dbl_input_blue = Convert.ToDouble(pColor.B);
            double distance = 500.0;

            double dbl_test_red = Convert.ToDouble(pColor.R);
            double dbl_test_green = Convert.ToDouble(pColor.G);
            double dbl_test_blue = Convert.ToDouble(pColor.B);
            double temp;

            foreach (Color o in this.mColors) {
                // compute the Euclidean distance between the two colors
                // note, that the alpha-component is not used in this example
                dbl_test_red = Math.Pow(Convert.ToDouble(o.R) - dbl_input_red, 2.0);
                dbl_test_green = Math.Pow(Convert.ToDouble(o.G) - dbl_input_green, 2.0);
                dbl_test_blue = Math.Pow(Convert.ToDouble(o.B) - dbl_input_blue, 2.0);
                // it is not necessary to compute the square root
                // it should be sufficient to use:
                // temp = dbl_test_blue + dbl_test_green + dbl_test_red;
                // if you plan to do so, the distance should be initialized by 250000.0
                temp = Math.Sqrt(dbl_test_blue + dbl_test_green + dbl_test_red);
                // explore the result and store the nearest color
                if (temp == 0.0) {
                    // the lowest possible distance is - of course - zero
                    // so I can break the loop (thanks to Willie Deutschmann)
                    // here I could return the input_color itself
                    // but in this example I am using a list with named colors
                    // and I want to return the Name-property too
                    nearest_color = o;
                    break;
                }
                else if (temp < distance) {
                    distance = temp;
                    nearest_color = o;
                }
            }

            return nearest_color;
        }

        /// <summary>
        /// Finds the closest color in palette using the minimum square method
        /// http://www.codeproject.com/Articles/17044/Find-the-Nearest-Color-with-C-Using-the-Euclidean
        /// </summary>
        /// <param name="pColorInput"></param>
        /// <returns></returns>
        public Color GetNearestColorHSL(Color pColorInput) {
            Color nearest_color = Color.Empty;

            GbReaper.Classes.RGBHSL.HSL vHSLin = RGBHSL.RGB_to_HSL(pColorInput);
            double dbl_input_h = vHSLin.H;
            double dbl_input_s = vHSLin.S;
            double dbl_input_l = vHSLin.L;
            double distance = 500.0;

            double dbl_test_h ;
            double dbl_test_s ;
            double dbl_test_l ;
            double temp;

            foreach (Color o in this.mColors) {
                GbReaper.Classes.RGBHSL.HSL vHSLo = RGBHSL.RGB_to_HSL(o);
                // compute the Euclidean distance between the two colors
                // note, that the alpha-component is not used in this example
                dbl_test_h = Math.Pow(vHSLo.H - dbl_input_h, 2.0);
                dbl_test_s = Math.Pow(vHSLo.S - dbl_input_s, 2.0);
                dbl_test_l = Math.Pow(vHSLo.L - dbl_input_l, 2.0);
                // it is not necessary to compute the square root
                // it should be sufficient to use:
                // temp = dbl_test_l + dbl_test_s + dbl_test_h;
                // if you plan to do so, the distance should be initialized by 250000.0
                temp = Math.Sqrt(dbl_test_l + dbl_test_s + dbl_test_h);
                // explore the result and store the nearest color
                if (temp == 0.0) {
                    // the lowest possible distance is - of course - zero
                    // so I can break the loop (thanks to Willie Deutschmann)
                    // here I could return the input_color itself
                    // but in this example I am using a list with named colors
                    // and I want to return the Name-property too
                    nearest_color = o;
                    break;
                }
                else if (temp < distance) {
                    distance = temp;
                    nearest_color = o;
                }
            }

            return nearest_color;
        }
    }
}
