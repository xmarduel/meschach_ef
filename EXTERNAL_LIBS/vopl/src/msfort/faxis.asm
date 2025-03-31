;	Static Name Aliases
;
	TITLE   faxis.for
	NAME    faxis

	.8087
FAXIS_TEXT	SEGMENT  BYTE PUBLIC 'CODE'
FAXIS_TEXT	ENDS
_DATA	SEGMENT  WORD PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT  WORD PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT  WORD PUBLIC 'BSS'
_BSS	ENDS
DGROUP	GROUP	CONST, _BSS, _DATA
	ASSUME  CS: FAXIS_TEXT, DS: DGROUP, SS: DGROUP, ES: DGROUP
EXTRN	__acrtused:ABS
EXTRN	_axistitle:FAR
EXTRN	_drawaxis:FAR
EXTRN	_drawaxes2:FAR
EXTRN	_drawaxes:FAR
EXTRN	T_STRING:FAR
EXTRN	__FCclenv:DWORD
FAXIS_TEXT      SEGMENT
; Line 22
	PUBLIC	AXISTITLE
AXISTITLE	PROC FAR
	push	bp
	mov	bp,sp
	sub	sp,268
	push	di
	push	si
;	T = 10
;	AX = 6
	mov	ax,WORD PTR __FCclenv
	mov	dx,WORD PTR __FCclenv+2
	mov	WORD PTR [bp-12],ax
	mov	WORD PTR [bp-10],dx
	mov	WORD PTR [bp-2],128
	les	bx,DWORD PTR [bp-12]
	mov	ax,WORD PTR es:[bx+2]
	mov	WORD PTR [bp-6],ax
	push	WORD PTR [bp+12]
	push	WORD PTR [bp+10]	;T
	lea	ax,WORD PTR [bp-140]
	push	ax
	mov	WORD PTR [bp-8],128
	lea	ax,WORD PTR [bp-8]
	mov	dx,ss
	mov	WORD PTR __FCclenv,ax
	mov	WORD PTR __FCclenv+2,dx
	call	FAR PTR T_STRING
	lea	di,WORD PTR [bp-268]
	mov	si,ax
	push	ss
	pop	es
	mov	cx,64
	rep	movsw
	les	bx,DWORD PTR [bp+6]	;AX
	push	WORD PTR es:[bx]
	lea	ax,WORD PTR [bp-268]
	push	ss
	push	ax
	lea	ax,WORD PTR [bp-4]
	mov	WORD PTR __FCclenv,ax
	mov	WORD PTR __FCclenv+2,ss
	call	FAR PTR _axistitle
	add	sp,6
; Line 23
	pop	si
	pop	di
	mov	sp,bp
	pop	bp
	ret	8

AXISTITLE	ENDP
; Line 27
	PUBLIC	DRAWAXIS
DRAWAXIS	PROC FAR
	push	bp
	mov	bp,sp
;	AX = 6
	les	bx,DWORD PTR [bp+6]	;AX
	push	WORD PTR es:[bx]
	call	FAR PTR _drawaxis
; Line 28
	mov	sp,bp
	pop	bp
	ret	4

DRAWAXIS	ENDP
; Line 31
	PUBLIC	DRAWAXES2
DRAWAXES2	PROC FAR
	push	bp
	mov	bp,sp
	call	FAR PTR _drawaxes2
; Line 32
	mov	sp,bp
	pop	bp
	ret	

DRAWAXES2	ENDP
; Line 35
	PUBLIC	DRAWAXES
DRAWAXES	PROC FAR
	push	bp
	mov	bp,sp
	call	FAR PTR _drawaxes
; Line 36
	mov	sp,bp
	pop	bp
	ret	

DRAWAXES	ENDP
FAXIS_TEXT	ENDS
END
