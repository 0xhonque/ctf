JMP main

fail:
  MOV R1, 'T'
  PRINT R1
  MOV R1, 'e'
  PRINT R1
  MOV R1, 't'
  PRINT R1
  MOV R1, '-'
  PRINT R1
  MOV R1, 't'
  PRINT R1
  MOV R1, 'o'
  PRINT R1
  MOV R1, 't'
  PRINT R1
  MOV R1, '!'
  PRINT R1
  HALT

success:
  MOV R1, 'B'
  PRINT R1
  MOV R1, 'e'
  PRINT R1
  MOV R1, 'r'
  PRINT R1
  MOV R1, 'o'
  PRINT R1
  MOV R1, 't'
  PRINT R1
  MOV R1, 'a'
  PRINT R1
  MOV R1, 'k'
  PRINT R1
  MOV R1, ' '
  PRINT R1
  MOV R1, 's'
  PRINT R1
  MOV R1, 'e'
  PRINT R1
  MOV R1, 'n'
  PRINT R1
  MOV R1, 'k'
  PRINT R1
  MOV R1, 'u'
  PRINT R1
  HALT

main:
  MOV R1, 'E'
  PRINT R1
  MOV R1, 'n'
  PRINT R1
  MOV R1, 't'
  PRINT R1
  MOV R1, 'e'
  PRINT R1
  MOV R1, 'r'
  PRINT R1
  MOV R1, ' '
  PRINT R1
  MOV R1, 'f'
  PRINT R1
  MOV R1, 'l'
  PRINT R1
  MOV R1, 'a'
  PRINT R1
  MOV R1, 'g'
  PRINT R1
  MOV R1, ':'
  PRINT R1
  MOV R1, ' '
  PRINT R1

  ; c
  INPUT R0
  MOV R1, 0x72
  XOR R0, R1
  CMP R0, 0x11
  JNE fail

  ; o
  INPUT R0
  MOV R1, 0x65
  ADD R0, R1
  CMP R0, 0xd4
  JNE fail

  ; m
  INPUT R0
  MOV R1, 0x76
  XOR R0, R1
  CMP R0, 0x1b
  JNE fail

  ; p
  INPUT R0
  MOV R1, 0x65
  ADD R0, R1
  CMP R0, 0xd5
  JNE fail

  ; i
  INPUT R0
  MOV R1, 0x72
  XOR R0, R1
  CMP R0, 0x1b
  JNE fail

  ; t
  INPUT R0
  MOV R1, 0x73
  ADD R0, R1
  CMP R0, 0xe7
  JNE fail

  ; {
  INPUT R0
  MOV R1, 0x65
  XOR R0, R1
  CMP R0, 0x1e
  JNE fail

  ; k
  INPUT R0
  MOV R1, 0x5f
  ADD R0, R1
  CMP R0, 0xca
  JNE fail

  ; i
  INPUT R0
  MOV R1, 0x65
  XOR R0, R1
  CMP R0, 0xc
  JNE fail

  ; r
  INPUT R0
  MOV R1, 0x6e
  ADD R0, R1
  CMP R0, 0xe0
  JNE fail

  ; a
  INPUT R0
  MOV R1, 0x67
  XOR R0, R1
  CMP R0, 0x6
  JNE fail

  ; i
  INPUT R0
  MOV R1, 0x69
  ADD R0, R1
  CMP R0, 0xd2
  JNE fail

  ; n
  INPUT R0
  MOV R1, 0x6e
  XOR R0, R1
  CMP R0, 0x0
  JNE fail

  ; _
  INPUT R0
  MOV R1, 0x65
  ADD R0, R1
  CMP R0, 0xc4
  JNE fail

  ; v
  INPUT R0
  MOV R1, 0x65
  XOR R0, R1
  CMP R0, 0x13
  JNE fail

  ; i
  INPUT R0
  MOV R1, 0x72
  ADD R0, R1
  CMP R0, 0xdb
  JNE fail

  ; r
  INPUT R0
  MOV R1, 0x69
  XOR R0, R1
  CMP R0, 0x1b
  JNE fail

  ; t
  INPUT R0
  MOV R1, 0x6e
  ADD R0, R1
  CMP R0, 0xe2
  JNE fail

  ; u
  INPUT R0
  MOV R1, 0x67
  XOR R0, R1
  CMP R0, 0x12
  JNE fail

  ; a
  INPUT R0
  MOV R1, 0x72
  ADD R0, R1
  CMP R0, 0xd3
  JNE fail

  ; l
  INPUT R0
  MOV R1, 0x65
  XOR R0, R1
  CMP R0, 0x9
  JNE fail

  ; _
  INPUT R0
  MOV R1, 0x76
  ADD R0, R1
  CMP R0, 0xd5
  JNE fail

  ; m
  INPUT R0
  MOV R1, 0x65
  XOR R0, R1
  CMP R0, 0x8
  JNE fail

  ; a
  INPUT R0
  MOV R1, 0x72
  ADD R0, R1
  CMP R0, 0xd3
  JNE fail

  ; c
  INPUT R0
  MOV R1, 0x73
  XOR R0, R1
  CMP R0, 0x10
  JNE fail

  ; h
  INPUT R0
  MOV R1, 0x65
  ADD R0, R1
  CMP R0, 0xcd
  JNE fail

  ; i
  INPUT R0
  MOV R1, 0x5f
  XOR R0, R1
  CMP R0, 0x36
  JNE fail

  ; n
  INPUT R0
  MOV R1, 0x65
  ADD R0, R1
  CMP R0, 0xd3
  JNE fail

  ; e
  INPUT R0
  MOV R1, 0x6e
  XOR R0, R1
  CMP R0, 0xb
  JNE fail

  ; _
  INPUT R0
  MOV R1, 0x67
  ADD R0, R1
  CMP R0, 0xc6
  JNE fail

  ; k
  INPUT R0
  MOV R1, 0x69
  XOR R0, R1
  CMP R0, 0x2
  JNE fail

  ; a
  INPUT R0
  MOV R1, 0x6e
  ADD R0, R1
  CMP R0, 0xcf
  JNE fail

  ; y
  INPUT R0
  MOV R1, 0x65
  XOR R0, R1
  CMP R0, 0x1c
  JNE fail

  ; a
  INPUT R0
  MOV R1, 0x65
  ADD R0, R1
  CMP R0, 0xc6
  JNE fail

  ; _
  INPUT R0
  MOV R1, 0x72
  XOR R0, R1
  CMP R0, 0x2d
  JNE fail

  ; v
  INPUT R0
  MOV R1, 0x69
  ADD R0, R1
  CMP R0, 0xdf
  JNE fail

  ; m
  INPUT R0
  MOV R1, 0x6e
  XOR R0, R1
  CMP R0, 0x3
  JNE fail

  ; w
  INPUT R0
  MOV R1, 0x67
  ADD R0, R1
  CMP R0, 0xde
  JNE fail

  ; a
  INPUT R0
  MOV R1, 0x72
  XOR R0, R1
  CMP R0, 0x13
  JNE fail

  ; r
  INPUT R0
  MOV R1, 0x65
  ADD R0, R1
  CMP R0, 0xd7
  JNE fail

  ; e
  INPUT R0
  MOV R1, 0x76
  XOR R0, R1
  CMP R0, 0x13
  JNE fail

  ; _
  INPUT R0
  MOV R1, 0x65
  ADD R0, R1
  CMP R0, 0xc4
  JNE fail

  ; a
  INPUT R0
  MOV R1, 0x72
  XOR R0, R1
  CMP R0, 0x13
  JNE fail

  ; t
  INPUT R0
  MOV R1, 0x73
  ADD R0, R1
  CMP R0, 0xe7
  JNE fail

  ; a
  INPUT R0
  MOV R1, 0x65
  XOR R0, R1
  CMP R0, 0x4
  JNE fail

  ; u
  INPUT R0
  MOV R1, 0x5f
  ADD R0, R1
  CMP R0, 0xd4
  JNE fail

  ; _
  INPUT R0
  MOV R1, 0x65
  XOR R0, R1
  CMP R0, 0x3a
  JNE fail

  ; v
  INPUT R0
  MOV R1, 0x6e
  ADD R0, R1
  CMP R0, 0xe4
  JNE fail

  ; i
  INPUT R0
  MOV R1, 0x67
  XOR R0, R1
  CMP R0, 0xe
  JNE fail

  ; r
  INPUT R0
  MOV R1, 0x69
  ADD R0, R1
  CMP R0, 0xdb
  JNE fail

  ; t
  INPUT R0
  MOV R1, 0x6e
  XOR R0, R1
  CMP R0, 0x1a
  JNE fail

  ; u
  INPUT R0
  MOV R1, 0x65
  ADD R0, R1
  CMP R0, 0xda
  JNE fail

  ; a
  INPUT R0
  MOV R1, 0x65
  XOR R0, R1
  CMP R0, 0x4
  JNE fail

  ; l
  INPUT R0
  MOV R1, 0x72
  ADD R0, R1
  CMP R0, 0xde
  JNE fail

  ; _
  INPUT R0
  MOV R1, 0x69
  XOR R0, R1
  CMP R0, 0x36
  JNE fail

  ; b
  INPUT R0
  MOV R1, 0x6e
  ADD R0, R1
  CMP R0, 0xd0
  JNE fail

  ; o
  INPUT R0
  MOV R1, 0x67
  XOR R0, R1
  CMP R0, 0x8
  JNE fail

  ; x
  INPUT R0
  MOV R1, 0x72
  ADD R0, R1
  CMP R0, 0xea
  JNE fail

  ; }
  INPUT R0
  MOV R1, 0x65
  XOR R0, R1
  CMP R0, 0x18
  JNE fail
  JMP success