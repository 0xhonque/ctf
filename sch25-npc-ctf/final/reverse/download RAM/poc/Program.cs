using System;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using Konscious.Security.Cryptography;

namespace MeddleDecryptor
{
    internal static class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("--- Meddle Ransomware Decryptor ---");
            Console.WriteLine("Enter the path to the directory containing the encrypted files:");
            string? directoryPath = Console.ReadLine();

            if (string.IsNullOrEmpty(directoryPath) || !Directory.Exists(directoryPath))
            {
                Console.WriteLine("Invalid directory path.");
                return;
            }

            DirectoryInfo dir = new DirectoryInfo(directoryPath);
            FileInfo[] files = dir.GetFiles("*.enc", SearchOption.AllDirectories);

            if (files.Length == 0)
            {
                Console.WriteLine("No .enc files found in the specified directory.");
                return;
            }

            foreach (FileInfo file in files)
            {
                try
                {
                    DecryptFile(file.FullName);
                    Console.WriteLine($"[SUCCESS] Decrypted: {file.Name}");
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[FAILURE] Failed to decrypt {file.Name}: {ex.Message}");
                }
            }
        }

        private static void DecryptFile(string filePath)
        {
            string[] lines = File.ReadAllLines(filePath);

            string[] parts = lines[1].Split('-');
            string timestampB64 = parts[0];
            string saltB64 = parts[1];
            string nonceOrIvB64 = parts[2];

            string timestamp = Base64Utils.DecodeToString(timestampB64);
            byte[] salt = Base64Utils.DecodeToBytes(saltB64);
            byte[] nonceOrIv = Base64Utils.DecodeToBytes(nonceOrIvB64);

            bool isGcm = nonceOrIv.Length == 12;

            string ciphertextB64;
            string tagB64 = "";

            if (isGcm)
            {
                tagB64 = lines[lines.Length - 2];
                ciphertextB64 = string.Concat(lines.Skip(2).Take(lines.Length - 4));
            }
            else
            {
                ciphertextB64 = string.Concat(lines.Skip(2).Take(lines.Length - 3));
            }

            byte[] ciphertext = Base64Utils.DecodeToBytes(ciphertextB64);

            var (iterations, spice) = DeriveParameters(timestamp);
            byte[] key = Keygen(timestamp, salt, spice, iterations);

            byte[] originalFileContent;

            if (isGcm)
            {
                byte[] tag = Base64Utils.DecodeToBytes(tagB64);
                originalFileContent = AesGcmDecrypt(ciphertext, key, nonceOrIv, tag);
            }
            else
            {
                originalFileContent = AesDecrypt(ciphertext, key, nonceOrIv);
            }

            string encryptedFileName = Path.GetFileNameWithoutExtension(filePath);
            string[] nameParts = encryptedFileName.Split(new[] { '.' }, 2);

            if (nameParts.Length < 2)
            {
                throw new Exception("Could not parse original filename from encrypted file name.");
            }

            string originalFileName = nameParts[1];

            string originalFilePath = Path.Combine(Path.GetDirectoryName(filePath) ?? "", originalFileName);
            File.WriteAllBytes(originalFilePath, originalFileContent);
            File.Delete(filePath);
        }

        private static byte[] Keygen(string timestamp, byte[] salt, string spice, int iterations)
        {
            char[] charArray = timestamp.ToCharArray();
            Array.Reverse(charArray);
            string reversedTimestamp = new string(charArray);

            string password = reversedTimestamp + spice;
            var passwordBytes = Encoding.UTF8.GetBytes(password);

            using (var argon2 = new Argon2id(passwordBytes))
            {
                argon2.Salt = salt;
                argon2.DegreeOfParallelism = 8;
                argon2.MemorySize = 65536;
                argon2.Iterations = 4;

                return argon2.GetBytes(32);
            }
        }

        private static (int iterations, string spice) DeriveParameters(string timestamp)
        {
            long ticks = DateTime.ParseExact(timestamp, "yyyyMMdd-HHmmss", null).Ticks;
            int iterations = 30000 + (int)(ticks % 1337);

            string initSpice = SHA256(timestamp + Base64Utils.DecodeToString("QW5kX25vX29uZV9zaW5nc19tZV9sdWxsYWJpZXNfX0FuZF9ub19vbmVfbWFrZXNfbWVfY2xvc2VfbXlfZXllc19fU29fSV90aHJvd190aGVfd2luZG93c193aWRlX19DYWxsX3RvX3lvdV9hY3Jvc3NfdGhlX3NreQ=="));
            string spice = SHA256(initSpice);
            for (int i = 0; i < (ticks % 67); i++)
            {
                spice = SHA256(spice + timestamp);
            }

            return (iterations, spice);
        }

        private static byte[] AesDecrypt(byte[] ciphertext, byte[] Key, byte[] IV)
        {
            using (Aes aesAlg = Aes.Create())
            {
                aesAlg.Key = Key;
                aesAlg.IV = IV;
                aesAlg.Mode = CipherMode.CBC;
                aesAlg.Padding = PaddingMode.PKCS7;

                ICryptoTransform decryptor = aesAlg.CreateDecryptor(aesAlg.Key, aesAlg.IV);

                using (MemoryStream msDecrypt = new MemoryStream(ciphertext))
                {
                    using (CryptoStream csDecrypt = new CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Read))
                    {
                        using (MemoryStream msPlain = new MemoryStream())
                        {
                            csDecrypt.CopyTo(msPlain);
                            return msPlain.ToArray();
                        }
                    }
                }
            }
        }

        private static byte[] AesGcmDecrypt(byte[] ciphertext, byte[] key, byte[] nonce, byte[] tag)
        {
            byte[] plaintextBytes = new byte[ciphertext.Length];

            using (var aesGcm = new AesGcm(key))
            {
                aesGcm.Decrypt(nonce, ciphertext, tag, plaintextBytes);
            }

            return plaintextBytes;
        }

        public static string SHA256(string input)
        {
            using (var crypt = System.Security.Cryptography.SHA256.Create())
            {
                var hash = new StringBuilder();
                byte[] crypto = crypt.ComputeHash(Encoding.UTF8.GetBytes(input));
                foreach (byte theByte in crypto)
                {
                    hash.Append(theByte.ToString("x2"));
                }
                return hash.ToString();
            }
        }
    }

    public static class Base64Utils
    {
        public static string DecodeToString(string base64)
        {
            if (base64 == null) throw new ArgumentNullException(nameof(base64));
            byte[] bytes = Convert.FromBase64String(base64);
            return Encoding.UTF8.GetString(bytes);
        }

        public static byte[] DecodeToBytes(string base64)
        {
            if (base64 == null) throw new ArgumentNullException(nameof(base64));
            return Convert.FromBase64String(base64);
        }
    }
}