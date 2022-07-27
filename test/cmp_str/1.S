
1.bin:     file format binary


1.bin:     file format binary


1.bin:     file format binary


1.bin:     file format binary


Disassembly of section .data:

00000000 <.data>:
   0:	66 81 3d 49 4e 49 54 	cmpw   $0x8166,0x54494e49
   7:	66 81 
   9:	7d 04                	jge    0xf
   b:	49                   	dec    %ecx
   c:	41                   	inc    %ecx
   d:	4c                   	dec    %esp
   e:	20 66 81             	and    %ah,-0x7f(%esi)
  11:	7d 08                	jge    0x1b
  13:	42                   	inc    %edx
  14:	49                   	dec    %ecx
  15:	4e                   	dec    %esi
  16:	20                   	.byte 0x20
