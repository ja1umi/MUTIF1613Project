;;; -*- asm -*-
;;;
;;; CELSTE Board : Definitions
;;;

	RELAXED	ON

SMBASE:	EQU	X'0040'

;;;
;;; Offset
;;;

CO_SIG:	EQU	0		; Signature
CO_CMD:	EQU	1		; Command	(n.b. 2 for Emily board)
CO_HSK:	EQU	2		; Handshake	(n.b. 1 for Emily board)
CO_STS:	EQU	3		; Status
CO_DAT:	EQU	4		; Data[0]-[11]

;;;
;;; Signature
;;;

CG_SIG:	EQU	0xA5

;;;
;;; Handshake
;;;

CH_REQ:	EQU	0xCC
CH_ACK:	EQU	0x33

;;;
;;; Command
;;;

CC_INI:	EQU	0x00		; Init
CC_COT:	EQU	0x01		; Console Output
CC_CIN:	EQU	0x02		; Console Input
CC_CST:	EQU	0x03		; Console Status

;;;
;;; Status
;;;

CS_OK:	EQU	0x00		; OK
CS_UK:	EQU	0xFF		; Unknown Command

	RELAXED	OFF
