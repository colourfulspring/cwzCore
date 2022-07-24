
../../build/bootblock.bin:     file format binary


Disassembly of section .data:

00000000 <.data>:
   0:	8c c8                	mov    %cs,%ax
   2:	8e d8                	mov    %ax,%ds
   4:	8e c0                	mov    %ax,%es
   6:	8e d0                	mov    %ax,%ss
   8:	bc 00 7c             	mov    $0x7c00,%sp
   b:	b8 00 06             	mov    $0x600,%ax
   e:	bb 00 07             	mov    $0x700,%bx
  11:	b9 00 00             	mov    $0x0,%cx
  14:	ba 4f 18             	mov    $0x184f,%dx
  17:	cd 10                	int    $0x10
  19:	b8 00 02             	mov    $0x200,%ax
  1c:	bb 00 00             	mov    $0x0,%bx
  1f:	ba 00 00             	mov    $0x0,%dx
  22:	cd 10                	int    $0x10
  24:	b8 01 13             	mov    $0x1301,%ax
  27:	bb 0f 00             	mov    $0xf,%bx
  2a:	b9 13 00             	mov    $0x13,%cx
  2d:	ba 00 00             	mov    $0x0,%dx
  30:	50                   	push   %ax
  31:	8c d8                	mov    %ds,%ax
  33:	8e c0                	mov    %ax,%es
  35:	58                   	pop    %ax
  36:	8b 2e 3e 7c          	mov    0x7c3e,%bp
  3a:	cd 10                	int    $0x10
  3c:	eb fe                	jmp    0x3c
  3e:	53                   	push   %bx
  3f:	74 61                	je     0xa2
  41:	72 74                	jb     0xb7
  43:	20 42 6f             	and    %al,0x6f(%bp,%si)
  46:	6f                   	outsw  %ds:(%si),(%dx)
  47:	74 69                	je     0xb2
  49:	6e                   	outsb  %ds:(%si),(%dx)
  4a:	67 2e 2e 2e 2e 2e 2e 	addr32 cs cs cs cs cs cs endbr32 
  51:	f3 0f 1e fb 
  55:	eb fe                	jmp    0x55
	...
 1fb:	00 00                	add    %al,(%bx,%si)
 1fd:	00 55 aa             	add    %dl,-0x56(%di)
