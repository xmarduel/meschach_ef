;	Static Name Aliases
;
	TITLE   fplot.for
	NAME    fplot

	.8087
FPLOT_TEXT	SEGMENT  BYTE PUBLIC 'CODE'
FPLOT_TEXT	ENDS
_DATA	SEGMENT  WORD PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT  WORD PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT  WORD PUBLIC 'BSS'
_BSS	ENDS
DGROUP	GROUP	CONST, _BSS, _DATA
	ASSUME  CS: FPLOT_TEXT, DS: DGROUP, SS: DGROUP, ES: DGROUP
EXTRN	__acrtused:ABS
EXTRN	_plot2:FAR
FPLOT_TEXT      SEGMENT
; Line 10
	PUBLIC	PLOT2
PLOT2	PROC FAR
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
;	X = 14
;	Y = 10
;	N = 6
;	__V23 = 65532
;	__V24 = 65534
	les	bx,DWORD PTR [bp+6]	;N
	mov	si,WORD PTR es:[bx]
	mov	ax,si
	shl	ax,1
	shl	ax,1
	mov	WORD PTR [bp-4],ax
	mov	WORD PTR [bp-2],ax
	push	si
	push	WORD PTR [bp+12]
	push	WORD PTR [bp+10]	;Y
	push	WORD PTR [bp+16]
	push	WORD PTR [bp+14]	;X
	call	FAR PTR _plot2
	add	sp,10
; Line 11
	pop	si
	mov	sp,bp
	pop	bp
	ret	12

PLOT2	ENDP
FPLOT_TEXT	ENDS
END
