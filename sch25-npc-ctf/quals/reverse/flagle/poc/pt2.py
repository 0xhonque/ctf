import re

RESULTS_FILE = "results.txt"
FLAG_LENGTH = 59
GREEN_SQUARE = "🟩"
UNKNOWN_CHAR = "?"

def assemble_flag():
  print(f"[*] Parsing '{RESULTS_FILE}'...")
  final_flag_list = [UNKNOWN_CHAR] * FLAG_LENGTH
  
  try:
    with open(RESULTS_FILE, "r") as f:
      lines = f.readlines()
  except FileNotFoundError:
    print(f"[!] Error: '{RESULTS_FILE}' not found.")
    return

  line_regex = re.compile(r"Input: (.)\1* \| Result: (.*)")

  for line in lines:
    match = line_regex.match(line.strip())
    if not match:
      continue

    tested_char = match.group(1)
    result_string = match.group(2)

    for i, emoji in enumerate(result_string):
      if emoji == GREEN_SQUARE:
        final_flag_list[i] = tested_char

  final_flag = "".join(final_flag_list)

  print("\n" + "="*60)
  print("          F I N A L   A S S E M B L E D   F L A G")
  print("="*60)
  print(final_flag)
  print("="*60)

  if UNKNOWN_CHAR in final_flag:
    unsolved_count = final_flag.count(UNKNOWN_CHAR)
    print(f"\n[!] Warning: Flag is incomplete. {unsolved_count} positions remain unsolved.")
  else:
    print("\n[+] Success! Flag appears to be complete.")

if __name__ == "__main__":
  assemble_flag()