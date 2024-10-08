; Project name	:	XTIDE Universal BIOS
; Description	:	Defines for BOOTVARS struct used by boot menu
;					and drive initialization.

;
; XTIDE Universal BIOS and Associated Tools
; Copyright (C) 2009-2010 by Tomi Tilli, 2011-2013 by XTIDE Universal BIOS Team.
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
; Visit http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
;

%ifndef BOOTVARS_INC
%define BOOTVARS_INC


; Temporary locations for system interrupt handlers. Stored during ROM initialization
; and needed when we do full initialization and drive detection on INT 19h
TEMPORARY_VECTOR_FOR_SYSTEM_INT13h		EQU		32h	; Unused by BIOS (reserved for DOS)
TEMPORARY_VECTOR_FOR_SYSTEM_INT19h		EQU		33h	; Unused by BIOS (reserved for DOS)


; Default drives
DEFAULT_FLOPPY_DRIVE_LETTER				EQU	'A'
DEFAULT_HARD_DRIVE_LETTER				EQU	'C'

; Number of times to retry booting before accepting error
BOOT_READ_RETRY_TIMES		EQU		3


%ifdef MODULE_HOTKEYS

struc HOTKEYVARS
	.fpPrevTimerHandler	resb	4		; Previous 08h timer handler
	.wTimeWhenDisplayed	resb	2		; System time (ticks) when Hotkey bar was first displayed
	.wFddAndHddLetters:
	.bFddLetter			resb	1		; Floppy Drive letter hotkey (upper case)
	.wHddLetterAndFlags:
	.bHddLetter			resb	1		; Hard Drive letter hotkey (upper case). Must be after .bFddLetter!
	.bFlags				resb	1		; Must be just after .bHddLetter!  (dependency in Hotkeybar.asm)
	.bScancode			resb	1		; Function hotkey scancode, must be just after .bFlags!
endstruc

%ifndef CHECK_FOR_UNUSED_ENTRYPOINTS
%if HOTKEYVARS.bFddLetter+1 != HOTKEYVARS.bHddLetter || HOTKEYVARS.bHddLetter+1 != HOTKEYVARS.bFlags || HOTKEYVARS.bFlags+1 != HOTKEYVARS.bScancode
%error "HOTKEYVARS: bytes need to come in the order .bFddLetter, then .bHddLetter, then .bFlags, then .bScancode"
%endif
%endif

; Bit defines for HOTKEYVARS.bFlags
FLG_HOTKEY_HD_FIRST			EQU		(1<<0)	; First try to boot from HDD, then FDD

%endif ; MODULE_HOTKEYS


; Pre-boot variables. These do not exist after successful boot to OS.
; Segment is always 0000h, same as BDA segment
struc BOOTVARS
						resb	7C00h
	.rgbAtaInfo:						; 7C00h, ATA Information for drive detection
	.rgbBootSect		resb 	512		; 7C00h, Boot sector
						resb	256		; Boot Menu stack
	.rgbMnuStack:
	.dwPostStack		resb	4		; POST stack pointer when entering INT 19h
%ifdef MODULE_HOTKEYS
	.hotkeyVars			resb	HOTKEYVARS_size	; Must be located just before DRVDETECTINFO structs
%endif
	.rgDrvDetectInfo:					; Array containing DRVDETECTINFO structs
endstruc


; MAX_HARD_DISK_NAME_LENGTH must be defined ahead of the DRVDETECTINFO structure to avoid problems with NASM
MAX_HARD_DISK_NAME_LENGTH	EQU		30	; Bytes reserved for drive name

struc DRVDETECTINFO
	.StartOfDrvDetectInfo:
	.szDrvName				resb	MAX_HARD_DISK_NAME_LENGTH
							resb	2	; Zero word (ensures string terminates)
	.wInitErrorFlags		resb	2	; Errors during initialization

	; DRVDETECTINFO's size must be an even multiple of DPT's size
	.EndOfDriveDetectInfo:	resb	LARGEST_DPT_SIZE - (.EndOfDriveDetectInfo % LARGEST_DPT_SIZE)
endstruc

DPT_DRVDETECTINFO_SIZE_MULTIPLIER	EQU		DRVDETECTINFO_size / LARGEST_DPT_SIZE

%ifndef CHECK_FOR_UNUSED_ENTRYPOINTS
%if MAX_HARD_DISK_NAME_LENGTH % 2 <> 0
	%error "MAX_HARD_DISK_NAME_LENGTH needs to be a multiple of 2, memory is moved with word operations."
%endif
%endif


;--------------------------------------------------------------------
; Stores POST stack pointer to BOOTVARS.
;
; STORE_POST_STACK_POINTER
;	Parameters:
;		ES:		BDA and Interrupt Vector segment (zero)
;	Returns:
;		Nothing
;	Corrupts registers:
;		Nothing
;--------------------------------------------------------------------
%macro STORE_POST_STACK_POINTER 0
	mov		[es:BOOTVARS.dwPostStack], sp
	mov		[es:BOOTVARS.dwPostStack+2], ss
%endmacro


;--------------------------------------------------------------------
; Initializes stack for boot menu usage.
; POST stack is not large enough when DPTs are stored to 30:0h.
;
; Note regarding LOAD_BDA_SEGMENT_TO: If you force the use of SP
; then you also have to unconditionally enable the CLI/STI pair.
; The reason for this is that only some buggy 808x CPU:s need the
; CLI/STI instruction pair when changing stacks. Other CPU:s disable
; interrupts automatically when SS is modified for the duration of
; the immediately following instruction to give time to change SP.
;
; SWITCH_TO_BOOT_MENU_STACK
;	Parameters:
;		Nothing
;	Returns:
;		SS:SP:	Pointer to top of Boot Menu stack
;	Corrupts registers:
;		Nothing
;--------------------------------------------------------------------
%macro SWITCH_TO_BOOT_MENU_STACK 0
%ifndef USE_186
	cli									; Disable interrupts
%endif
	LOAD_BDA_SEGMENT_TO	ss, sp
	mov		sp, BOOTVARS.rgbMnuStack	; Load offset to stack
%ifndef USE_186
	sti									; Enable interrupts
%endif
%endmacro


;--------------------------------------------------------------------
; Restores SS and SP to initial boot loader values.
;
; Note! Must return with AX=0 and CF preserved.
; See Int19hMenu_JumpToBootSector_or_RomBoot.
;
; SWITCH_BACK_TO_POST_STACK
;	Parameters:
;		AX:		BDA and Interrupt Vector segment (zero)
;	Returns:
;		SS:SP:	Ptr to POST stack
;	Corrupts registers:
;		Nothing (not even FLAGS)
;--------------------------------------------------------------------
%macro SWITCH_BACK_TO_POST_STACK 0
%ifndef USE_386
	cli
	mov		ss, ax
	mov		sp, [ss:BOOTVARS.dwPostStack]
	mov		ss, [ss:BOOTVARS.dwPostStack+2]
	sti
%else
	mov		ss, ax
	lss		sp, [ss:BOOTVARS.dwPostStack]
%endif
%endmacro


%endif ; BOOTVARS_INC
