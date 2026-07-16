using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;

namespace BabyMalware
{
  public class Program
  {
    static void Main(string[] args)
    {
      string exePath = AppDomain.CurrentDomain.BaseDirectory;

      string wallpaperPath = Path.Combine(exePath, "ransom.jpg");
      string flagFile = Path.Combine(exePath, "SUPERIMPORTANTFILE.flag");

      if (!File.Exists(wallpaperPath) || !File.Exists(flagFile))
        return;

      WallpaperChanger.Set(wallpaperPath);

      string[] lines = {
        "U hav been HAXX3D. Send $$$ to restore ur pc",
        "I has encrypted ur SUPERIMPORTANTFILE.flag -> enc.SUPERIMPORTANT.1337"
      };

      string hexCipherText = File.ReadAllText(flagFile).Trim();
      byte[] encryptedData = Convert.FromHexString(hexCipherText);

      byte[] partiallyDecrypted = DecryptOneLayer(encryptedData);
      byte[] finalEncryptedData = EncryptWithXor(partiallyDecrypted);

      string outFile = Path.Combine(exePath, "enc.SUPERIMPORTANT.1337");
      File.WriteAllBytes(outFile, finalEncryptedData);
      File.Delete(flagFile);

      using (StreamWriter outputFile = new StreamWriter(Path.Combine(exePath, "Ran$omeNotezzz.txt")))
      {
        foreach (string line in lines)
          outputFile.WriteLine(line);
      }
    }

    public static byte[] DecryptOneLayer(byte[] cipherText)
    {
      byte[] key = Encoding.UTF8.GetBytes("ctfcompit2025rev");
      byte[] iv  = Encoding.UTF8.GetBytes("reverseengineer!");

      using (Aes aesAlg = Aes.Create())
      {
        aesAlg.Mode = CipherMode.CBC;
        aesAlg.Padding = PaddingMode.PKCS7;
        aesAlg.Key = key;
        aesAlg.IV = iv;

        using (var decryptor = aesAlg.CreateDecryptor(aesAlg.Key, aesAlg.IV))
        using (var ms = new MemoryStream(cipherText))
        using (var cs = new CryptoStream(ms, decryptor, CryptoStreamMode.Read))
        using (var result = new MemoryStream())
        {
          cs.CopyTo(result);
          return result.ToArray();
        }
      }
    }

    public static byte[] EncryptWithXor(byte[] data)
    {
      byte[] key = Encoding.UTF8.GetBytes("r!zzT3c|-|");
      byte[] result = new byte[data.Length];

      for (int i = 0; i < data.Length; i++)
      {
        result[i] = (byte)(data[i] ^ key[i % key.Length]);
      }

      return result;
    }
  }

  public static class WallpaperChanger
  {
    public static bool Set(string imagePath)
    {
      int result = NativeMethods.SystemParametersInfo(
        NativeMethods.SPI_SETDESKWALLPAPER,
        0,
        imagePath,
        NativeMethods.SPIF_UPDATEINIFILE | NativeMethods.SPIF_SENDCHANGE
      );
      return result != 0;
    }
  }

  internal static class NativeMethods
  {
    public const int SPI_SETDESKWALLPAPER = 20;
    public const int SPIF_UPDATEINIFILE = 1;
    public const int SPIF_SENDCHANGE = 2;

    [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
    public static extern int SystemParametersInfo(int uAction, int uParam, string lpvParam, int fuWinIni);
  }
}