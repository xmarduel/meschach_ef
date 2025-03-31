;	Static Name Aliases
;
	TITLE   fswitch.for
	NAME    fswitch

	.8087
FSWITCH_TEXT	SEGMENT  BYTE PUBLIC 'CODE'
FSWITCH_TEXT	ENDS
_DATA	SEGMENT  WORD PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT  WORD PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT  WORD PUBLIC 'BSS'
_BSS	ENDS
DGROUP	GROUP	CONST, _BSS, _DATA
	ASSUME  CS: FSWITCH_TEXT, DS: DGROUP, SS: DGROUP, ES: DGROUP
EXTRN	__acrtused:ABS
EXTRN	_markerscale:FAR
EXTRN	_graphtitle:FAR
EXTRN	T_STRING:FAR
EXTRN	_fit:FAR
EXTRN	_drawtitle:FAR
EXTRN	_range:FAR
EXTRN	_endslopes:FAR
EXTRN	_gridspacing:FAR
EXTRN	_scaling:FAR
EXTRN	_tickmarks:FAR
EXTRN	__elds:FAR
EXTRN	__dcvt:FAR
EXTRN	__fstdp:FAR
EXTRN	_tickspacing:FAR
EXTRN	_annotate:FAR
EXTRN	_arrayindex:FAR
EXTRN	_marker:FAR
EXTRN	_markerspacing:FAR
EXTRN	__FCclenv:DWORD
FSWITCH_TEXT      SEGMENT
; Line 70
	PUBLIC	FIT
FIT	PROC FAR
	push	bp
	mov	bp,sp
;	I = 6
	les	bx,DWORD PTR [bp+6]	;I
	push	WORD PTR es:[bx]
	call	FAR PTR _fit
; Line 71
	mov	sp,bp
	pop	bp
	ret	4

FIT	ENDP
; Line 74
	PUBLIC	SKIP
SKIP	PROC FAR
	push	bp
	mov	bp,sp
;	I = 6
	les	bx,DWORD PTR [bp+6]	;I
	push	WORD PTR es:[bx]
	call	FAR PTR _fit
; Line 75
	mov	sp,bp
	pop	bp
	ret	4

SKIP	ENDP
; Line 78
	PUBLIC	ENDSLOPES
ENDSLOPES	PROC FAR
	push	bp
	mov	bp,sp
;	A = 10
;	B = 6
	les	bx,DWORD PTR [bp+6]	;B
	call	FAR PTR __elds
	call	FAR PTR __dcvt
	sub	sp,8
	mov	bx,sp
	call	FAR PTR __fstdp
	les	bx,DWORD PTR [bp+10]	;A
	call	FAR PTR __elds
	call	FAR PTR __dcvt
	sub	sp,8
	mov	bx,sp
	call	FAR PTR __fstdp
	call	FAR PTR _endslopes
; Line 79
	mov	sp,bp
	pop	bp
	ret	8

ENDSLOPES	ENDP
; Line 82
	PUBLIC	GRIDSPACING
GRIDSPACING	PROC FAR
	push	bp
	mov	bp,sp
;	I = 6
	les	bx,DWORD PTR [bp+6]	;I
	push	WORD PTR es:[bx]
	call	FAR PTR _gridspacing
; Line 83
	mov	sp,bp
	pop	bp
	ret	4

GRIDSPACING	ENDP
; Line 87
	PUBLIC	SCALING
SCALING	PROC FAR
	push	bp
	mov	bp,sp
;	I = 10
;	AX = 6
	les	bx,DWORD PTR [bp+6]	;AX
	push	WORD PTR es:[bx]
	les	bx,DWORD PTR [bp+10]	;I
	push	WORD PTR es:[bx]
	call	FAR PTR _scaling
; Line 88
	mov	sp,bp
	pop	bp
	ret	8

SCALING	ENDP
; Line 92
	PUBLIC	TICKMARKS
TICKMARKS	PROC FAR
	push	bp
	mov	bp,sp
;	I = 10
;	AX = 6
	les	bx,DWORD PTR [bp+6]	;AX
	push	WORD PTR es:[bx]
	les	bx,DWORD PTR [bp+10]	;I
	push	WORD PTR es:[bx]
	call	FAR PTR _tickmarks
; Line 93
	mov	sp,bp
	pop	bp
	ret	8

TICKMARKS	ENDP
; Line 97
	PUBLIC	TICKSPACING
TICKSPACING	PROC FAR
	push	bp
	mov	bp,sp
;	I = 10
;	AX = 6
	les	bx,DWORD PTR [bp+6]	;AX
	push	WORD PTR es:[bx]
	les	bx,DWORD PTR [bp+10]	;I
	push	WORD PTR es:[bx]
	call	FAR PTR _tickspacing
; Line 98
	mov	sp,bp
	pop	bp
	ret	8

TICKSPACING	ENDP
; Line 104
	PUBLIC	ANNOTATE
ANNOTATE	PROC FAR
	push	bp
	mov	bp,sp
	sub	sp,268
	push	di
	push	si
;	F = 10
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
	push	WORD PTR [bp+10]	;F
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
	call	FAR PTR _annotate
	add	sp,6
; Line 105
	pop	si
	pop	di
	mov	sp,bp
	pop	bp
	ret	8

ANNOTATE	ENDP
; Line 108
	PUBLIC	ARRAYINDEX
ARRAYINDEX	PROC FAR
	push	bp
	mov	bp,sp
;	I = 6
	les	bx,DWORD PTR [bp+6]	;I
	push	WORD PTR es:[bx]
	call	FAR PTR _arrayindex
; Line 109
	mov	sp,bp
	pop	bp
	ret	4

ARRAYINDEX	ENDP
; Line 118
	PUBLIC	MARKER
MARKER	PROC FAR
	push	bp
	mov	bp,sp
	sub	sp,268
	push	di
	push	si
;	S = 6
	mov	ax,WORD PTR __FCclenv
	mov	dx,WORD PTR __FCclenv+2
	mov	WORD PTR [bp-12],ax
	mov	WORD PTR [bp-10],dx
	mov	WORD PTR [bp-2],128
	les	bx,DWORD PTR [bp-12]
	mov	ax,WORD PTR es:[bx+2]
	mov	WORD PTR [bp-6],ax
	push	WORD PTR [bp+8]
	push	WORD PTR [bp+6]	;S
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
	lea	ax,WORD PTR [bp-268]
	push	ss
	push	ax
	lea	ax,WORD PTR [bp-4]
	mov	WORD PTR __FCclenv,ax
	mov	WORD PTR __FCclenv+2,ss
	call	FAR PTR _marker
	add	sp,4
; Line 119
	pop	si
	pop	di
	mov	sp,bp
	pop	bp
	ret	4

MARKER	ENDP
; Line 122
	PUBLIC	MARKERSPACING
MARKERSPACING	PROC FAR
	push	bp
	mov	bp,sp
;	I = 6
	les	bx,DWORD PTR [bp+6]	;I
	push	WORD PTR es:[bx]
	call	FAR PTR _markerspacing
; Line 123
	mov	sp,bp
	pop	bp
	ret	4

MARKERSPACING	ENDP
; Line 126
	PUBLIC	MARKERSCALE
MARKERSCALE	PROC FAR
	push	bp
	mov	bp,sp
;	A = 6
	les	bx,DWORD PTR [bp+6]	;A
	call	FAR PTR __elds
	call	FAR PTR __dcvt
	sub	sp,8
	mov	bx,sp
	call	FAR PTR __fstdp
	call	FAR PTR _markerscale
; Line 127
	mov	sp,bp
	pop	bp
	ret	4

MARKERSCALE	ENDP
; Line 132
	PUBLIC	GRAPHTITLE
GRAPHTITLE	PROC FAR
	push	bp
	mov	bp,sp
	sub	sp,268
	push	di
	push	si
;	S = 6
	mov	ax,WORD PTR __FCclenv
	mov	dx,WORD PTR __FCclenv+2
	mov	WORD PTR [bp-12],ax
	mov	WORD PTR [bp-10],dx
	mov	WORD PTR [bp-2],128
	les	bx,DWORD PTR [bp-12]
	mov	ax,WORD PTR es:[bx+2]
	mov	WORD PTR [bp-6],ax
	push	WORD PTR [bp+8]
	push	WORD PTR [bp+6]	;S
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
	lea	ax,WORD PTR [bp-268]
	push	ss
	push	ax
	lea	ax,WORD PTR [bp-4]
	mov	WORD PTR __FCclenv,ax
	mov	WORD PTR __FCclenv+2,ss
	call	FAR PTR _graphtitle
	add	sp,4
; Line 133
	pop	si
	pop	di
	mov	sp,bp
	pop	bp
	ret	4

GRAPHTITLE	ENDP
; Line 136
	PUBLIC	DRAWTITLE
DRAWTITLE	PROC FAR
	push	bp
	mov	bp,sp
	call	FAR PTR _drawtitle
; Line 137
	mov	sp,bp
	pop	bp
	ret	

DRAWTITLE	ENDP
; Line 141
	PUBLIC	RANGE
RANGE	PROC FAR
	push	bp
	mov	bp,sp
;	A = 14
;	B = 10
;	AX = 6
	les	bx,DWORD PTR [bp+6]	;AX
	push	WORD PTR es:[bx]
	les	bx,DWORD PTR [bp+10]	;B
	call	FAR PTR __elds
	call	FAR PTR __dcvt
	sub	sp,8
	mov	bx,sp
	call	FAR PTR __fstdp
	les	bx,DWORD PTR [bp+14]	;A
	call	FAR PTR __elds
	call	FAR PTR __dcvt
	sub	sp,8
	mov	bx,sp
	call	FAR PTR __fstdp
	call	FAR PTR _range
; Line 142
	mov	sp,bp
	pop	bp
	ret	12

RANGE	ENDP
FSWITCH_TEXT	ENDS
END
