using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// <summary>
/// Found here https://www.codeproject.com/tips/739630/dither-floyd-steinberg-dithering
/// </summary>
namespace FloydSteinberg
{
    public class FloydSteinbergDither
    {
public static byte PlusTruncate(byte a, int b)
{
    if ((a & 0xff) + b < 0)
        return 0;
    else if ((a & 0xff) + b > 255)
        return (byte)255;
    else
        return (byte)(a + b);
}


public static Color FindNearestColor(Color color, Color[] palette)
{
    int minDistanceSquared = 255 * 255 + 255 * 255 + 255 * 255 + 1;
    byte bestIndex = 0;
    for (byte i = 0; i < palette.Length; i++)
    {
        int Rdiff = (color.R & 0xff) - (palette[i].R & 0xff);
        int Gdiff = (color.G & 0xff) - (palette[i].G & 0xff);
        int Bdiff = (color.B & 0xff) - (palette[i].B & 0xff);
        int distanceSquared = Rdiff * Rdiff + Gdiff * Gdiff + Bdiff * Bdiff;
        if (distanceSquared < minDistanceSquared)
        {
            minDistanceSquared = distanceSquared;
            bestIndex = i;
        }
    }
    return (palette[bestIndex] == Color.White) ? Color.Empty : palette[bestIndex];
}

public static Bitmap Process(Bitmap image, Color[] palette)
{
    Bitmap result = new Bitmap(image.Width, image.Height);
    int height = image.Height;
    int width = image.Width;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Color currentPixel = image.GetPixel(x, y); ;
            Color bestColor = FindNearestColor(currentPixel, palette);
            result.SetPixel(x, y, bestColor);

            int errorR = (currentPixel.R & 0xff) - (bestColor.R & 0xff);
            int errorG = (currentPixel.G & 0xff) - (bestColor.G & 0xff);
            int errorB = (currentPixel.B & 0xff) - (bestColor.B & 0xff);
            if (x + 1 < width)
            {
                image.SetPixel(x + 1, y + 0, Color.FromArgb(
                    PlusTruncate(image.GetPixel(x + 1, y + 0).R, (errorR * 7) >> 4),
                    PlusTruncate(image.GetPixel(x + 1, y + 0).G, (errorG * 7) >> 4),
                    PlusTruncate(image.GetPixel(x + 1, y + 0).B, (errorB * 7) >> 4)
                ));
            }
            if (y + 1 < height)
            {
                if (x - 1 > 0)
                {
                    image.SetPixel(x - 1, y + 1, Color.FromArgb(
                        PlusTruncate(image.GetPixel(x - 1, y + 1).R, (errorR * 3) >> 4),
                        PlusTruncate(image.GetPixel(x - 1, y + 1).G, (errorG * 3) >> 4),
                        PlusTruncate(image.GetPixel(x - 1, y + 1).B, (errorB * 3) >> 4)
                    ));
                }
                image.SetPixel(x + 0, y + 1, Color.FromArgb(
                    PlusTruncate(image.GetPixel(x + 0, y + 1).R, (errorR * 5) >> 4),
                    PlusTruncate(image.GetPixel(x + 0, y + 1).G, (errorG * 5) >> 4),
                    PlusTruncate(image.GetPixel(x + 0, y + 1).B, (errorB * 5) >> 4)
                ));
                if (x + 1 < width)
                {
                    image.SetPixel(x + 1, y + 1, Color.FromArgb(
                        PlusTruncate(image.GetPixel(x + 1, y + 1).R, (errorR * 1) >> 4),
                        PlusTruncate(image.GetPixel(x + 1, y + 1).G, (errorG * 1) >> 4),
                        PlusTruncate(image.GetPixel(x + 1, y + 1).B, (errorB * 1) >> 4)
                    ));
                }
            }
        }
    }
    return result;
}
    }
}
