; Project name	:	Emulation library
; Description	:	Macros for emulating later x86 instructions with older
;					processors.
;					Macros are used so optimized builds can be done
;					easily for different processors.
;
;					This file must be first to be included to
;					any source file.
%ifndef EMULATE_INC
%define EMULATE_INC

; Defines for processor support (should be set in makefile).
; Unsupported instructions will be emulated using macros.
; If AT class PC is used (instead of XT), define USE_AT

;%define USE_186				; Define to use 18x/V20/V30 instructions
;%define USE_286				; Define to use 286 instructions
;%define USE_386				; Define to use 386 instructions
;%define USE_AT					; Define for AT class machine

%ifdef USE_NEC_V				; This will run on NEC V20/V30 processors only
	%define USE_186				; Define to use 18x/V20/V30 instructions
	%ifdef USE_UNDOC_INTEL OR USE_286 OR USE_386
		%fatal "Conflicting processor define used together with USE_NEC_V!"
	%endif
%endif

%ifdef USE_386
	%define USE_286				; Define to use 286 instructions
%endif
%ifdef USE_286
	%define USE_186				; Define to use 18x/V20/V30 instructions
	%define USE_UNDOC_INTEL		; Not supported by NEC V20/V30
%endif

%ifdef USE_386
	CPU 386						; Allow instructions up to 386
%elifdef USE_286
	CPU 286						; Allow instructions up to 286
%elifdef USE_186
	CPU 186						; Allow instructions up to 188/186/V20/V30
%else
	CPU 8086					; Allow 8088/8086 instructions only
%endif

BITS 16							; Set 16 bit code generation

; Alignments for jump targets.
; Following values are optimal for different processor types:
; 286 and 386SX			WORD (16-bit, 2 bytes)
; 386DX and 486			DWORD (32-bit, 4 bytes)
; Pentium and later		QWORD (64-bit, 8 bytes)
%ifdef USE_AT
	%ifdef USE_386
		JUMP_ALIGN		EQU		4
		WORD_ALIGN		EQU		2
	%else ; USE_286
		JUMP_ALIGN		EQU		2
		WORD_ALIGN		EQU		2
	%endif
%else ; XT
	%ifndef USE_086 ; 8088/V20 CPUs
		JUMP_ALIGN		EQU		1	; 2 is optimal for 8086 and NEC V30 CPUs but it's not worth the ROM space for most XT machines with 8088 or NEC V20 CPUs.
		WORD_ALIGN		EQU		2	; The same applies here but the cost of ROM space is negligible.
	%else ; 8086/V30 CPUs
		JUMP_ALIGN		EQU		2
		WORD_ALIGN		EQU		2
	%endif
%endif

;==========================================================================

;--------------------------------------------------------------------
; Find String In String
;
; FSIS
;	Parameters:
;		%1:		String to search for (case-insensitive)
;		%2:		String to search in
;	Returns:
;	strpos:		Position of %1 in %2 if found, 0 if not found
;--------------------------------------------------------------------
%macro FSIS 2.nolist
%defstr	s1 %1
%defstr	s2 %2
%strlen sl1 s1
%strlen sl2 s2
%assign strpos 0
	%if sl1 <= sl2
		%assign strpos sl2 - sl1 + 1
		%rep strpos
			%substr %%ss s2 strpos, sl1
			%ifidni %%ss, s1
				%exitrep
			%else
				%assign strpos strpos - 1
			%endif
		%endrep
	%endif
%endmacro


%ifdef USE_NEC_V
	%include "NEC_V.inc"
%endif


;--------------------------------------------------------------------
; The undocumented instruction SALC (Set AL According to CF).
; Available on all Intel processors and truly compatible clones.
; Does not work on the NEC V20/V30 or Sony CXQ70108 processors.
;
; eSALC
;	Parameters:
;		Nothing
;	Returns:
;		AL:		FFh if CF=1
;				00h if CF=0
;	Corrupts registers:
;		Nothing
;--------------------------------------------------------------------
%macro eSALC 0
;	db		0D6h
	salc
%endmacro


;--------------------------------------------------------------------
; The AAD instruction (ASCII Adjust before Division).
; Available on all Intel processors and truly compatible clones.
; Does not work on the NEC V20/V30 or Sony CXQ70108 processors
; unless %1 is 10 (0Ah).
;
; eAAD
;	Parameters:
;		%1:		Any 8 bit number (0...255)
;	Returns:
;		AL:		AH * %1 + AL
;		AH:		0
;		Flags:	Set according to result
;	Corrupts registers:
;		Nothing
;--------------------------------------------------------------------
%macro eAAD 1
%ifndef CHECK_FOR_UNUSED_ENTRYPOINTS
	%if %1 > 255
		%error Invalid parameter passed to eAAD (%1 > 255)
	%else
		db		0D5h, %1
	%endif
%endif
%endmacro


;--------------------------------------------------------------------
; The AAM instruction (ASCII Adjust after Multiplication).
;
; eAAM
;	Parameters:
;		%1:		Any 8 bit number except 0 (1...255)
;	Returns:
;		AL:		AL MOD %1
;		AH:		AL / %1
;		Flags:	Set according to result
;	Corrupts registers:
;		Nothing
;--------------------------------------------------------------------
%macro eAAM 1
%ifndef CHECK_FOR_UNUSED_ENTRYPOINTS
	%if %1 > 255
		%error Invalid parameter passed to eAAM (%1 > 255)
	%elif %1 = 0
		%error Invalid parameter passed to eAAM (%1 = 0). This would cause a divide-by-zero exception!
	%else
		db		0D4h, %1
	%endif
%endif
%endmacro


;--------------------------------------------------------------------
; Emulates BSF (Bit Scan Forward) instruction when necessary.
; BSF is used to find index of least significant bit.
;
; eBSF
;	Parameters:
;		%1:		Destination WORD Register for bit index (not CX or same as %2!)
;		%2:		Source WORD operand where to search bit (not CX or same as %1!)
;	Returns:
;		%1:		Index of lowest order bit from %2
;		ZF:		Set if %2 is zero
;				Cleared if %2 is non-zero
;	Corrupts registers:
;		Nothing
;--------------------------------------------------------------------
%macro eBSF 2
%ifndef USE_386
	FSIS	], %2				; %2 is a memory operand?
	%if strpos
		cmp		WORD %2, BYTE 0	; Source operand is zero?
		je		SHORT %%Return	;  If so, return with ZF set
	%else						; No, %2 is a register
		test	%2, %2
		jz		SHORT %%Return
	%endif

	push	cx

%ifdef USE_NEC_V
	mov		cx, -1

ALIGN JUMP_ALIGN
%%BitLoop:
	inc		cx
	eTEST1	%2, cl
	jz		SHORT %%BitLoop
	mov		%1, cx

%else ; ~USE_NEC_V
	mov		cx, 1<<15
	mov		%1, -1

ALIGN JUMP_ALIGN
%%BitLoop:
	rol		cx, 1				; Prepare to test next bit
	inc		%1					; Increment bit index
	test	%2, cx				; Bit set?
	jz		SHORT %%BitLoop
%endif

	pop		cx

%%Return:
;-----------------------------------
%else ; USE_386
	bsf		%1, %2
%endif
%endmacro


;--------------------------------------------------------------------
; Emulates BSR (Bit Scan Reverse) instruction when necessary.
; BSR is used to find index of most significant bit.
;
; eBSR
;	Parameters:
;		%1:		Destination WORD Register for bit index (not CX or same as %2!)
;		%2:		Source WORD operand where to search bit (not CX or same as %1!)
;	Returns:
;		%1:		Index of highest order bit from %2
;		ZF:		Set if %2 is zero
;				Cleared if %2 is non-zero
;	Corrupts registers:
;		Nothing
;--------------------------------------------------------------------
%macro eBSR 2
%ifndef USE_386
	FSIS	], %2				; %2 is a memory operand?
	%if strpos
		cmp		WORD %2, BYTE 0	; Source operand is zero?
		je		SHORT %%Return	;  If so, return with ZF set
	%else						; No, %2 is a register
		test	%2, %2
		jz		SHORT %%Return
	%endif

	push	cx

%ifdef USE_NEC_V
	mov		cx, 16

ALIGN JUMP_ALIGN
%%BitLoop:
	dec		cx
	eTEST1	%2, cl
	jz		SHORT %%BitLoop
	mov		%1, cx

%else ; ~USE_NEC_V
	mov		cx, 1
	mov		%1, 16

ALIGN JUMP_ALIGN
%%BitLoop:
	ror		cx, 1				; Prepare to test next bit
	dec		%1					; Decrement bit index
	test	%2, cx				; Bit set?
	jz		SHORT %%BitLoop
%endif

	pop		cx

%%Return:
;-----------------------------------
%else ; USE_386
	bsr		%1, %2
%endif
%endmacro


;--------------------------------------------------------------------
; Conditional Move.
;
; eCMOVcc
;	Parameters:
;		%1:		Destination data
;		%2:		Source data
;	Returns:
;		Nothing
;	Corrupts registers:
;		Nothing
;--------------------------------------------------------------------
%macro eCMOVA 2
	jbe		SHORT %%Return
	mov		%1, %2
%%Return:
%endmacro

%macro eCMOVC 2
	jnc		SHORT %%Return
	mov		%1, %2
%%Return:
%endmacro

%macro eCMOVNC 2
	jc		SHORT %%Return
	mov		%1, %2
%%Return:
%endmacro

%macro eCMOVZ 2
	jnz		SHORT %%Return
	mov		%1, %2
%%Return:
%endmacro

%macro eCMOVNZ 2
	jz		SHORT %%Return
	mov		%1, %2
%%Return:
%endmacro

%macro eCMOVE 2
	eCMOVZ	%1, %2
%endmacro

%macro eCMOVNE 2
	eCMOVNZ	%1, %2
%endmacro

%macro eCMOVB 2
	eCMOVC	%1, %2
%endmacro

%macro eCMOVS 2
	jns		SHORT %%Return
	mov		%1, %2
%%Return:
%endmacro

%macro eCMOVNS 2
	js		SHORT %%Return
	mov		%1, %2
%%Return:
%endmacro


;--------------------------------------------------------------------
; Conditional Set.
;
; eSETcc
;	Parameters:
;		%1:		Destination data
;	Returns:
;		Nothing
;	Corrupts registers:
;		Flags
;--------------------------------------------------------------------
%macro eSETZ 1
	mov		%1, 0			; Clear while preserving flags
	jnz		SHORT %%Return	; Nothing to set
	inc		%1
%%Return:
%endmacro

%macro eSETNZ 1
	mov		%1, 0			; Clear while preserving flags
	jz		SHORT %%Return	; Nothing to set
	inc		%1
%%Return:
%endmacro


;--------------------------------------------------------------------
; Moves byte with zero-extending to any Register.
;
; eMOVZX
;	Parameters:
;		%1:		Destination Register (SP not supported)
;		%2:		Byte register or byte address
;	Returns:
;		Nothing
;	Corrupts registers:
;		FLAGS
;--------------------------------------------------------------------
%macro eMOVZX 2
%ifndef USE_386
	%ifidni %1, ax
		mov		al, %2
		xor		ah, ah
	%elifidni %1, bx
		mov		bl, %2
		xor		bh, bh		; %2 may use BX in effective address
	%elifidni %1, cx
		mov		cl, %2
		xor		ch, ch
	%elifidni %1, dx
		mov		dl, %2
		xor		dh, dh
	%else	; SI, DI, BP (all may be used in effective address)
		FSIS	%1, %2
		%if strpos
			push	ax
			mov		al, %2
			xor		ah, ah
			xchg	%1, ax
			pop		ax
		%else
			xchg	%1, ax
			mov		al, %2
			xor		ah, ah
			xchg	%1, ax
		%endif
	%endif
;-----------------------------------
%else
	movzx	%1, %2
%endif
%endmacro


;--------------------------------------------------------------------
; Emulates PUSHA instruction when necessary.
;
; ePUSHA
;	Parameters:
;		Nothing
;	Returns:
;		Nothing
;	Corrupts registers:
;		Nothing
;--------------------------------------------------------------------
%macro ePUSHA 0
%ifndef USE_186
	push	ax
	push	cx
	push	dx
	push	bx
	push	sp
	push	bp
	push	si
	push	di
;-----------------------------------
%else
	pusha
%endif
%endmacro


;--------------------------------------------------------------------
; Emulates POPA instruction when necessary.
;
; ePOPA
;	Parameters:
;		Nothing
;	Returns:
;		Nothing
;	Corrupts registers:
;		Nothing
;--------------------------------------------------------------------
%macro ePOPA 0
%ifndef USE_186
	pop		di
	pop		si
	pop		bp
	pop		ax		; Skip SP
	pop		bx
	pop		dx
	pop		cx
	pop		ax
;-----------------------------------
%else
	popa
%endif
%endmacro


;--------------------------------------------------------------------
; Emulates ENTER instruction when necessary.
;
; eENTER
;	Parameters:
;		%1:		Number of bytes to reserve from stack
;		%2:		The lexical nesting level (not emulated, set to 0)
;	Returns:
;		SS:BP:	Ptr to old BP
;				([bp-2] points to highest local stack frame word)
;	Corrupts registers:
;		FLAGS
;--------------------------------------------------------------------
%macro eENTER 2
%ifndef USE_186
	push	bp
	mov		bp, sp
	sub		sp, %1
;-----------------------------------
%else
	enter	%1, %2
%endif
%endmacro

;--------------------------------------------------------------------
; Emulates LEAVE instruction when necessary.
;
; eLEAVE
;	Parameters:
;		Nothing
;	Returns:
;		BP:		What it was before eENTER
;	Corrupts registers:
;		Nothing
;--------------------------------------------------------------------
%macro eLEAVE 0
%ifndef USE_186
	mov		sp, bp
	pop		bp
;-----------------------------------
%else
	leave
%endif
%endmacro


;--------------------------------------------------------------------
; Emulates LSS instruction when necessary.
;
; eLSS
;	Parameters:
;		%1:		Destination register
;		%2:		Source memory address without brackets
;	Returns:
;		IF:		0 (interrupts disabled)
;	Corrupts registers:
;		Nothing
;--------------------------------------------------------------------
%macro eLSS 2
%ifndef USE_386
	cli							; Disable interrupts
	mov		%1, [%2]			; Load offset
	mov		ss, [%2+2]			; Load segment
;-----------------------------------
%else
	lss		%1, [%2]
%endif
%endmacro


;--------------------------------------------------------------------
; Repeats string instruction with segment override.
; This macro prevents 8088/8086 restart bug.
;
; eSEG_STR
;	Parameters:
;		%1:		REP/REPE/REPZ or REPNE/REPNZ prefix
;		%2:		Source segment override (destination is always ES)
;		%3:		String instruction
;		%4:		An exclamation mark (!) if the state of the IF must
;				be preserved (can not be used together with CMPS or
;				SCAS instructions), otherwise it will be set on
;				return from the macro (i.e. interrupts will be on)
;		CX:		Repeat count
;	Returns:
;		FLAGS for CMPS and SCAS only
;	Corrupts registers:
;		FLAGS
;--------------------------------------------------------------------
%macro eSEG_STR 3-4
%ifndef USE_186	; 8088/8086 has string instruction restart bug when using more than one prefix
%ifidn %4, !				; Preserve the IF
	FSIS	cmps, %3
%ifn strpos
	FSIS	scas, %3
%endif
%if strpos
	%error "The state of the IF can not be preserved when using CMPS or SCAS!"
%endif
	pushf
	cli
	%1						; REP is the prefix that can be lost
	%2						; SEG is the prefix that won't be lost
	%3						; String instruction
	popf
%else						; No need to preserve the IF
	cli
	%1
	%2
	%3
	sti
%endif
%else	; No bug on V20/V30/188/186 and later
	%2
	%1 %3
%endif
%endmacro


;--------------------------------------------------------------------
; Bit shifts and rotates with immediate.
;
; eSHIFT_IM
;	Parameters:
;		%1:		Shift target
;		%2:		Number of bits to shift
;		%3:		Instruction (SHL, SHR, ROL, ROR, RCL, RCR)
;	Returns:
;		FLAGS
;	Corrupts registers:
;		Nothing
;--------------------------------------------------------------------
%macro eSHIFT_IM 3
%ifndef CHECK_FOR_UNUSED_ENTRYPOINTS
%ifndef USE_186
	%ifidni %1, cl
		times %2	%3		%1, 1
	%elifidni %1, ch
		times %2	%3		%1, 1
	%elifidni %1, cx
		times %2	%3		%1, 1
	%else
		%if %2 > 3	; Size optimized value
			push	cx
			mov		cl, %2
			%3		%1, cl
			pop		cx
		%else
			times %2	%3		%1, 1
		%endif
	%endif
;-----------------------------------
%else
	%3		%1, %2
%endif
%endif
%endmacro

%macro eSHR_IM 2
	eSHIFT_IM	%1, %2, shr
%endmacro

%macro eSHL_IM 2
%ifndef CHECK_FOR_UNUSED_ENTRYPOINTS
%ifdef USE_386
	%if %2 = 1
		FSIS	], %1
		%if strpos
			eSHIFT_IM	%1, %2, shl
		%else
			add		%1, %1	; Same size but faster on 386 and 486.
		%endif
	%else
		eSHIFT_IM	%1, %2, shl
	%endif
%else
	eSHIFT_IM	%1, %2, shl
%endif
%endif
%endmacro

%macro eROR_IM 2
	eSHIFT_IM	%1, %2, ror
%endmacro

%macro eROL_IM 2
	eSHIFT_IM	%1, %2, rol
%endmacro

%macro eRCR_IM 2
	eSHIFT_IM	%1, %2, rcr
%endmacro

%macro eRCL_IM 2
%ifndef CHECK_FOR_UNUSED_ENTRYPOINTS
%ifdef USE_386
	%if %2 = 1
		FSIS	], %1
		%if strpos
			eSHIFT_IM	%1, %2, rcl
		%else
			adc		%1, %1	; Same size but faster on 386 and 486.
		%endif
	%else
		eSHIFT_IM	%1, %2, rcl
	%endif
%else
	eSHIFT_IM	%1, %2, rcl
%endif
%endif
%endmacro


;--------------------------------------------------------------------
; Emulates PUSH imm instruction when necessary.
;
; ePUSH_I
;	Parameters:
;		%1:		Immediate to push
;	Returns:
;		Nothing
;	Corrupts registers:
;		Nothing
;--------------------------------------------------------------------
%macro ePUSH_I 1
%ifndef USE_186
	push	bp					; Immediate goes here
	push	bp
	mov		bp, sp
	mov		WORD [bp+2], %1
	pop		bp
;-----------------------------------
%else
	push	%1
%endif
%endmacro


;--------------------------------------------------------------------
; Emulates PUSH imm instruction when necessary.
; ePUSH_T uses temporary register for faster performance
; and smaller code size than ePUSH_I.
;
; ePUSH_T
;	Parameters:
;		%1:		Temporary Register
;		%2:		Immediate to push
;	Returns:
;		Nothing
;	Corrupts registers:
;		%1
;--------------------------------------------------------------------
%macro ePUSH_T 2
%ifndef USE_186
	%ifidni %2, 0
		xor		%1, %1
	%else
		mov		%1, %2
	%endif
	push	%1
;-----------------------------------
%else
	push	%2
%endif
%endmacro


%endif ; EMULATE_INC
