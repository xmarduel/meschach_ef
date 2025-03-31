;	Static Name Aliases
;
	TITLE   fsadjust.for
	NAME    fsadjust

	.8087
FSADJUST_TEXT	SEGMENT  BYTE PUBLIC 'CODE'
FSADJUST_TEXT	ENDS
_DATA	SEGMENT  WORD PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT  WORD PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT  WORD PUBLIC 'BSS'
_BSS	ENDS
DGROUP	GROUP	CONST, _BSS, _DATA
	ASSUME  CS: FSADJUST_TEXT, DS: DGROUP, SS: DGROUP, ES: DGROUP
EXTRN	__acrtused:ABS
EXTRN	_adjustscale:FAR
EXTRN	__FCclenv:DWORD
FSADJUST_TEXT      SEGMENT
; Line 10
	PUBLIC	ADJUSTSCALE
ADJUSTSCALE	PROC FAR
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
;	X = 14
;	N = 10
;	A = 6
;	__V23 = 65534
	les	bx,DWORD PTR [bp+10]	;N
	mov	si,WORD PTR es:[bx]
	mov	ax,si
	shl	ax,1
	shl	ax,1
	mov	WORD PTR [bp-2],ax
	les	bx,DWORD PTR [bp+6]	;A
	push	WORD PTR es:[bx]
	push	si
	push	WORD PTR [bp+16]
	push	WORD PTR [bp+14]	;X
	call	FAR PTR _adjustscale
	add	sp,8
; Line 11
	pop	si
	mov	sp,bp
	pop	bp
	ret	12

ADJUSTSCALE	ENDP
FSADJUST_TEXT	ENDS
END
