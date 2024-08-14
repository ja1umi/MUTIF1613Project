;;;
;;; Firmware for MN1613 board
;;;

			CPU		MN1613

ROM_START:	EQU		X'F800'
MBOX:		EQU		X'0042'
RESULT: 	EQU		MBOX
RAMLOC: 	EQU		X'1000'
STACK:		EQU		X'0FFF' 

			ORG		ROM_START

			DC	0
			DC	0

			DC	0
JVEC_ROM:	DC	ROM_PROG

			DC	0
JVEC2_ROM:	DC	ROM_PROG2

			DC	0
JVEC3_ROM:	DC	ROM_PROG3

C_SQRT2:	DC	1.41421				; useful constants
C_PI:		DC	3.14159
C_E:		DC	2.71828

ENTTABLE:
		DC	INIT
		DC	CONOUT
		DC	STROUT
		DC	CONIN
		DC	CONST

;
;			CELESTE Board Console Driver
;
			INCLUDE	"./dev_celeste.asm"

;
; test program starts here
;
ROM_PROG:
			MVWI	R0, X'AA55'
			ST		R0, RESULT
			STD		R0, RAMLOC
			B		ROM_PROG

ROM_PROG2:
			MVI		R0, 0
			ST		R0, RESULT
			STD		R0, RAMLOC
			MVI		R1, 10
-			A		R0, R1
			SI		R1, 1, Z
			B		-
			ST		R0, RESULT	 	; calculate the sum from 1 to 10 and put the result
			STD		R0, RAMLOC
			B		ROM_PROG2

ROM_PROG3:
			MVWI	SP, STACK
			BALD	INIT
			MVWI	X0, MSG
			BALD	STROUT
-			B		-

STROUT:
			L		R0, 0(X0)
			BSWP	R0, R0
			ANDI	R0, X'00FF', NZ
			B		SOUTR
			BALD	CONOUT
			L		R0, 0(X0)
			ANDI	R0, X'00FF', NZ
			B		SOUTR
			BALD	CONOUT
			AI		X0, 1
			B		STROUT
SOUTR:		RET

MSG:
	DC	"\r\nHello, MN1613\r\n\0"

