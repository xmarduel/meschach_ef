;	Static Name Aliases
;
	TITLE   flsfit.for
	NAME    flsfit

	.8087
FLSFIT_TEXT	SEGMENT  BYTE PUBLIC 'CODE'
FLSFIT_TEXT	ENDS
_DATA	SEGMENT  WORD PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT  WORD PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT  WORD PUBLIC 'BSS'
_BSS	ENDS
DGROUP	GROUP	CONST, _BSS, _DATA
	ASSUME  CS: FLSFIT_TEXT, DS: DGROUP, SS: DGROUP, ES: DGROUP
EXTRN	__acrtused:ABS
EXTRN	_degree:FAR
FLSFIT_TEXT      SEGMENT
; Line 6
	PUBLIC	DEGREE
DEGREE	PROC FAR
	push	bp
	mov	bp,sp
;	I = 6
	les	bx,DWORD PTR [bp+6]	;I
	push	WORD PTR es:[bx]
	call	FAR PTR _degree
; Line 7
	mov	sp,bp
	pop	bp
	ret	4

DEGREE	ENDP
FLSFIT_TEXT	ENDS
END
