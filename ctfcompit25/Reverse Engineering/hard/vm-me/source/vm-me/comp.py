import re
import sys

# Instruction encodings
OPCODES = {
  "MOV":   0x01,
  "ADD":   0x02,
  "XOR":   0x03,
  "CMP":   0x04,
  "JMP":   0x05,
  "JNE":   0x06,
  "PRINT": 0x07,
  "INPUT": 0x08,
  "HALT":  0xFF,
}

# Registers mapping
REGS = {
  "R0": 0x00,
  "R1": 0x01,
  "R2": 0x02,
  "R3": 0x03,
}

def parse_imm(token: str) -> int:
  token = token.strip()

  # Handle decimal
  try:
    return int(token, 0)  # auto-detects 0x.., 0o.., etc.
  except ValueError:
    pass

  # Handle character literal like 'A' or '\n'
  if token.startswith("'") and token.endswith("'") and len(token) >= 3:
    inner = token[1:-1]

    # Handle escape sequences
    if inner.startswith("\\"):
      escapes = {
        "n": "\n",
        "t": "\t",
        "r": "\r",
        "0": "\0",
        "'": "'",
        '"': '"',
        "\\": "\\"
      }
      if inner[1:] in escapes:
        return ord(escapes[inner[1:]])
      else:
        raise ValueError(f"Unknown escape sequence: {inner}")
    else:
      return ord(inner)

  raise ValueError(f"Invalid immediate: {token}")



def assemble(asm_lines):
  # Clean lines
  code = []
  labels = {}
  pc = 0

  # --- First pass: find labels and compute offsets ---
  for line in asm_lines:
    line = line.split(";")[0].strip()   # remove comments
    if not line:
      continue

    if line.endswith(":"):  # label definition
      label = line[:-1]
      labels[label] = pc
      continue

    parts = re.findall(r"'[^']*'|\S+", line)
    op = parts[0].upper()

    if op == "MOV" or op == "CMP":
      pc += 3
    elif op in ("ADD", "XOR"):
      pc += 3
    elif op in ("JMP", "JNE"):
      pc += 2
    elif op in ("PRINT", "INPUT"):
      pc += 2
    elif op == "HALT":
      pc += 1
    else:
      raise ValueError(f"Unknown instruction {op}")

  # --- Second pass: generate bytecode ---
  pc = 0
  for line in asm_lines:
    line = line.split(";")[0].strip()
    if not line or line.endswith(":"):
      continue

    parts = re.findall(r"'[^']*'|\S+", line)
    op = parts[0].upper()

    if op == "MOV":
      reg = REGS[parts[1].strip(",")]
      imm = parse_imm(parts[2])
      code.extend([OPCODES[op], reg, imm])

    elif op == "ADD" or op == "XOR":
      regA = REGS[parts[1].strip(",")]
      regB = REGS[parts[2]]
      code.extend([OPCODES[op], regA, regB])

    elif op == "CMP":
      reg = REGS[parts[1].strip(",")]
      imm = parse_imm(parts[2])
      code.extend([OPCODES[op], reg, imm])

    elif op in ("JMP", "JNE"):
      target = parts[1]
      if target in labels:
        addr = labels[target]
      else:
        addr = parse_imm(target)
      code.extend([OPCODES[op], addr])

    elif op in ("PRINT", "INPUT"):
      reg = REGS[parts[1]]
      code.extend([OPCODES[op], reg])

    elif op == "HALT":
      code.append(OPCODES[op])

  return bytes(code)


if __name__ == "__main__":
  if len(sys.argv) != 3:
    print("Usage: python assembler.py input.asm output.bin")
    sys.exit(1)

  with open(sys.argv[1]) as f:
    asm_lines = f.readlines()

  bytecode = assemble(asm_lines)

  with open(sys.argv[2], "wb") as f:
    f.write(bytecode)

  print(f"Assembled {sys.argv[1]} -> {sys.argv[2]} ({len(bytecode)} bytes)")
