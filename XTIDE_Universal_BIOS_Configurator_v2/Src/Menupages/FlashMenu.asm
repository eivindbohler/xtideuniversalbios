; Project name	:	XTIDE Universal BIOS Configurator v2
; Description	:	"Flash EEPROM" menu structs and functions.

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

; Section containing initialized data
SECTION .data

ALIGN WORD_ALIGN
g_MenupageForFlashMenu:
istruc MENUPAGE
	at	MENUPAGE.fnEnter,			dw	FlashMenu_EnterMenuOrModifyItemVisibility
	at	MENUPAGE.fnBack,			dw	MainMenu_EnterMenuOrModifyItemVisibility
	at	MENUPAGE.wMenuitems,		dw	7
iend

g_MenuitemFlashBackToMainMenu:
istruc MENUITEM
	at	MENUITEM.fnActivate,		dw	MainMenu_EnterMenuOrModifyItemVisibility
	at	MENUITEM.szName,			dw	g_szItemCfgBackToMain
	at	MENUITEM.szQuickInfo,		dw	g_szItemCfgBackToMain
	at	MENUITEM.szHelp,			dw	g_szItemCfgBackToMain
	at	MENUITEM.bFlags,			db	FLG_MENUITEM_VISIBLE
	at	MENUITEM.bType,				db	TYPE_MENUITEM_PAGEBACK
iend

g_MenuitemFlashStartFlashing:
istruc MENUITEM
	at	MENUITEM.fnActivate,		dw	StartFlashing
	at	MENUITEM.szName,			dw	g_szItemFlashStart
	at	MENUITEM.szQuickInfo,		dw	g_szNfoFlashStart
	at	MENUITEM.szHelp,			dw	g_szNfoFlashStart
	at	MENUITEM.bFlags,			db	FLG_MENUITEM_VISIBLE
	at	MENUITEM.bType,				db	TYPE_MENUITEM_ACTION
iend

g_MenuitemFlashEepromType:
istruc MENUITEM
	at	MENUITEM.fnActivate,		dw	Menuitem_ActivateMultichoiceSelectionForMenuitemInDSSI
	at	MENUITEM.fnFormatValue,		dw	MenuitemPrint_WriteLookupValueStringToBufferInESDIfromShiftedItemInDSSI
	at	MENUITEM.szName,			dw	g_szItemFlashEepromType
	at	MENUITEM.szQuickInfo,		dw	g_szNfoFlashEepromType
	at	MENUITEM.szHelp,			dw	g_szNfoFlashEepromType
	at	MENUITEM.bFlags,			db	FLG_MENUITEM_MODIFY_MENU | FLG_MENUITEM_PROGRAMVAR | FLG_MENUITEM_BYTEVALUE | FLG_MENUITEM_VISIBLE
	at	MENUITEM.bType,				db	TYPE_MENUITEM_MULTICHOICE
	at	MENUITEM.itemValue + ITEM_VALUE.wRomvarsValueOffset,		dw	CFGVARS.bEepromType
	at	MENUITEM.itemValue + ITEM_VALUE.szDialogTitle,				dw	g_szDlgFlashEepromType
	at	MENUITEM.itemValue + ITEM_VALUE.szMultichoice,				dw	g_szMultichoiceEepromType
	at	MENUITEM.itemValue + ITEM_VALUE.rgwChoiceToValueLookup,		dw	g_rgwChoiceToValueLookupForEepromType
	at	MENUITEM.itemValue + ITEM_VALUE.rgszValueToStringLookup,	dw	g_rgszValueToStringLookupForEepromType
iend

g_MenuitemFlashSdpCommand:
istruc MENUITEM
	at	MENUITEM.fnActivate,		dw	Menuitem_ActivateMultichoiceSelectionForMenuitemInDSSI
	at	MENUITEM.fnFormatValue,		dw	MenuitemPrint_WriteLookupValueStringToBufferInESDIfromShiftedItemInDSSI
	at	MENUITEM.szName,			dw	g_szItemFlashSDP
	at	MENUITEM.szQuickInfo,		dw	g_szNfoFlashSDP
	at	MENUITEM.szHelp,			dw	g_szHelpFlashSDP
	at	MENUITEM.bFlags,			db	FLG_MENUITEM_PROGRAMVAR | FLG_MENUITEM_BYTEVALUE
	at	MENUITEM.bType,				db	TYPE_MENUITEM_MULTICHOICE
	at	MENUITEM.itemValue + ITEM_VALUE.wRomvarsValueOffset,		dw	CFGVARS.bSdpCommand
	at	MENUITEM.itemValue + ITEM_VALUE.szDialogTitle,				dw	g_szDlgFlashSDP
	at	MENUITEM.itemValue + ITEM_VALUE.szMultichoice,				dw	g_szMultichoiceSdpCommand
	at	MENUITEM.itemValue + ITEM_VALUE.rgwChoiceToValueLookup,		dw	g_rgwChoiceToValueLookupForSdpCommand
	at	MENUITEM.itemValue + ITEM_VALUE.rgszValueToStringLookup,	dw	g_rgszValueToStringLookupForSdpCommand
iend

g_MenuitemFlashPageSize:
istruc MENUITEM
	at	MENUITEM.fnActivate,		dw	Menuitem_ActivateMultichoiceSelectionForMenuitemInDSSI
	at	MENUITEM.fnFormatValue,		dw	MenuitemPrint_WriteLookupValueStringToBufferInESDIfromShiftedItemInDSSI
	at	MENUITEM.szName,			dw	g_szItemFlashPageSize
	at	MENUITEM.szQuickInfo,		dw	g_szNfoFlashPageSize
	at	MENUITEM.szHelp,			dw	g_szHelpFlashPageSize
	at	MENUITEM.bFlags,			db	FLG_MENUITEM_PROGRAMVAR | FLG_MENUITEM_BYTEVALUE
	at	MENUITEM.bType,				db	TYPE_MENUITEM_MULTICHOICE
	at	MENUITEM.itemValue + ITEM_VALUE.wRomvarsValueOffset,		dw	CFGVARS.bEepromPage
	at	MENUITEM.itemValue + ITEM_VALUE.szDialogTitle,				dw	g_szDlgFlashPageSize
	at	MENUITEM.itemValue + ITEM_VALUE.szMultichoice,				dw	g_szMultichoicePageSize
	at	MENUITEM.itemValue + ITEM_VALUE.rgwChoiceToValueLookup,		dw	g_rgwChoiceToValueLookupForPageSize
	at	MENUITEM.itemValue + ITEM_VALUE.rgszValueToStringLookup,	dw	g_rgszValueToStringLookupForPageSize
iend

g_MenuitemFlashEepromAddress:
istruc MENUITEM
	at	MENUITEM.fnActivate,		dw	Menuitem_ActivateHexInputForMenuitemInDSSI
	at	MENUITEM.fnFormatValue,		dw	MenuitemPrint_WriteHexValueStringToBufferInESDIfromItemInDSSI
	at	MENUITEM.szName,			dw	g_szItemFlashAddr
	at	MENUITEM.szQuickInfo,		dw	g_szNfoFlashAddr
	at	MENUITEM.szHelp,			dw	g_szNfoFlashAddr
	at	MENUITEM.bFlags,			db	FLG_MENUITEM_PROGRAMVAR | FLG_MENUITEM_VISIBLE
	at	MENUITEM.bType,				db	TYPE_MENUITEM_HEX
	at	MENUITEM.itemValue + ITEM_VALUE.wRomvarsValueOffset,		dw	CFGVARS.wEepromSegment
	at	MENUITEM.itemValue + ITEM_VALUE.szDialogTitle,				dw	g_szDlgFlashAddr
	at	MENUITEM.itemValue + ITEM_VALUE.wMinValue,					dw	0C000h
	at	MENUITEM.itemValue + ITEM_VALUE.wMaxValue,					dw	0F800h
iend

g_MenuitemFlashGenerateChecksum:
istruc MENUITEM
	at	MENUITEM.fnActivate,		dw	Menuitem_ActivateMultichoiceSelectionForMenuitemInDSSI
	at	MENUITEM.fnFormatValue,		dw	MenuitemPrint_WriteLookupValueStringToBufferInESDIfromShiftedItemInDSSI
	at	MENUITEM.szName,			dw	g_szItemFlashChecksum
	at	MENUITEM.szQuickInfo,		dw	g_szNfoFlashChecksum
	at	MENUITEM.szHelp,			dw	g_szHelpFlashChecksum
	at	MENUITEM.bFlags,			db	FLG_MENUITEM_PROGRAMVAR | FLG_MENUITEM_VISIBLE | FLG_MENUITEM_FLAGVALUE
	at	MENUITEM.bType,				db	TYPE_MENUITEM_MULTICHOICE
	at	MENUITEM.itemValue + ITEM_VALUE.wRomvarsValueOffset,		dw	CFGVARS.wFlags
	at	MENUITEM.itemValue + ITEM_VALUE.szDialogTitle,				dw	g_szDlgFlashChecksum
	at	MENUITEM.itemValue + ITEM_VALUE.szMultichoice,				dw	g_szMultichoiceBooleanFlag
	at	MENUITEM.itemValue + ITEM_VALUE.rgszValueToStringLookup,	dw	g_rgszValueToStringLookupForFlagBooleans
	at	MENUITEM.itemValue + ITEM_VALUE.wValueBitmask,				dw	FLG_CFGVARS_CHECKSUM
iend

g_rgwChoiceToValueLookupForEepromType:
	dw	EEPROM_TYPE.2816_2kiB
	dw	EEPROM_TYPE.2864_8kiB
	dw	EEPROM_TYPE.2864_8kiB_MOD
	dw	EEPROM_TYPE.28256_32kiB
	dw	EEPROM_TYPE.28512_64kiB
	dw	EEPROM_TYPE.SST_39SF

g_rgszValueToStringLookupForEepromType:
	dw	g_szValueFlash2816
	dw	g_szValueFlash2864
	dw	g_szValueFlash2864Mod
	dw	g_szValueFlash28256
	dw	g_szValueFlash28512
	dw	g_szValueFlashSST39SF

g_rgwChoiceToValueLookupForSdpCommand:
	dw	SDP_COMMAND.none
	dw	SDP_COMMAND.enable
	dw	SDP_COMMAND.disable
g_rgszValueToStringLookupForSdpCommand:
	dw	g_szValueFlashNone
	dw	g_szValueFlashEnable
	dw	g_szValueFlashDisable

g_rgwChoiceToValueLookupForPageSize:
	dw	EEPROM_PAGE.1_byte
	dw	EEPROM_PAGE.2_bytes
	dw	EEPROM_PAGE.4_bytes
	dw	EEPROM_PAGE.8_bytes
	dw	EEPROM_PAGE.16_bytes
	dw	EEPROM_PAGE.32_bytes
	dw	EEPROM_PAGE.64_bytes
g_rgszValueToStringLookupForPageSize:
	dw	g_szValueFlash1byte
	dw	g_szValueFlash2bytes
	dw	g_szValueFlash4bytes
	dw	g_szValueFlash8bytes
	dw	g_szValueFlash16bytes
	dw	g_szValueFlash32bytes
	dw	g_szValueFlash64bytes


; Section containing code
SECTION .text

;--------------------------------------------------------------------
; MainMenu_EnterMenuOrModifyItemVisibility
;	Parameters:
;		SS:BP:	Menu handle
;	Returns:
;		Nothing
;	Corrupts registers:
;		All, except BP
;--------------------------------------------------------------------
ALIGN JUMP_ALIGN
FlashMenu_EnterMenuOrModifyItemVisibility:
	push	cs
	pop		ds

	cmp		WORD [g_cfgVars+CFGVARS.wEepromSegment], 0
	jne		SHORT .AlreadySet

	push	es
	push	di
	call	EEPROM_FindXtideUniversalBiosROMtoESDI
	mov		ax, es
	pop		di
	pop		es
	jnc		SHORT .StoreEepromSegment
	mov		ax, DEFAULT_EEPROM_SEGMENT
.StoreEepromSegment:
	mov		[g_cfgVars+CFGVARS.wEepromSegment], ax

.AlreadySet:
	mov		si, g_MenupageForFlashMenu
	ePUSH_T	bx, Menupage_ChangeToNewMenupageInDSSI
	cmp		BYTE [g_cfgVars+CFGVARS.bEepromType], EEPROM_TYPE.SST_39SF
	mov		ax, DisableMenuitemFromCSBX
	je		SHORT .EnableOrDisableMenuitemsUnusedBySstFlash
	mov		ax, EnableMenuitemFromCSBX
	; Fall to .EnableOrDisableMenuitemsUnusedBySstFlash

;--------------------------------------------------------------------
; .EnableOrDisableMenuitemsUnusedBySstFlash
;	Parameters:
;		AX:		Offset to EnableMenuitemFromCSBX / DisableMenuitemFromCSBX
;		SS:BP:	Menu handle
;	Returns:
;		Nothing
;	Corrupts registers:
;		BX
;--------------------------------------------------------------------
.EnableOrDisableMenuitemsUnusedBySstFlash:
	mov		bx, g_MenuitemFlashSdpCommand
	call	ax
	mov		bx, g_MenuitemFlashPageSize
	jmp		ax

;--------------------------------------------------------------------
; MENUITEM activation functions (.fnActivate)
;	Parameters:
;		SS:BP:	Ptr to MENU
;	Returns:
;		Nothing
;	Corrupts registers:
;		All, except segments
;--------------------------------------------------------------------
ALIGN JUMP_ALIGN
StartFlashing:
	call	.MakeSureThatImageFitsInEeprom
	jc		SHORT .InvalidFlashingParameters
	cmp		BYTE [g_cfgVars+CFGVARS.bEepromType], EEPROM_TYPE.SST_39SF
	jne		SHORT .SkipAlignmentCheck
	call	.MakeSureAddress32KAligned
	jnz		SHORT .InvalidFlashingParameters
.SkipAlignmentCheck:
	push	es
	push	ds

	call	.PrepareBuffersForFlashing
	mov		cl, FLASHVARS_size + PROGRESS_DIALOG_IO_size
	call	Memory_ReserveCLbytesFromStackToDSSI
	call	.InitializeFlashvarsFromDSSI
	mov		bx, si							; DS:BX now points to FLASHVARS
	cmp		BYTE [g_cfgVars+CFGVARS.bEepromType], EEPROM_TYPE.SST_39SF
	je		SHORT .FlashWithoutProgressBar
	add		si, BYTE FLASHVARS_size			; DS:SI now points to PROGRESS_DIALOG_IO
	call	Dialogs_DisplayProgressDialogForFlashingWithDialogIoInDSSIandFlashvarsInDSBX
.FlashComplete:
	call	.DisplayFlashingResultsFromFlashvarsInDSBX

	add		sp, BYTE FLASHVARS_size + PROGRESS_DIALOG_IO_size
	pop		ds
	pop		es
.InvalidFlashingParameters:
	ret

.FlashWithoutProgressBar:					; Worst case. SST devices will
	call	FlashSst_WithFlashvarsInDSBX	; either complete flashing
	jmp		SHORT .FlashComplete			; or timeout within 2 seconds.

;--------------------------------------------------------------------
; .MakeSureThatImageFitsInEeprom
;	Parameters:
;		SS:BP:	Ptr to MENU
;	Returns:
;		CF:		Set if EEPROM too small
;	Corrupts registers:
;		AX, BX, DX
;--------------------------------------------------------------------
ALIGN JUMP_ALIGN
.MakeSureThatImageFitsInEeprom:
	call	Buffers_GetSelectedEepromSizeInWordsToAX
	cmp		ax, [g_cfgVars+CFGVARS.wImageSizeInWords]
	jae		SHORT .ImageFitsInSelectedEeprom
	mov		dx, g_szErrEepromTooSmall
	call	Dialogs_DisplayErrorFromCSDX
	stc
ALIGN JUMP_ALIGN, ret
.ImageFitsInSelectedEeprom:
.AlignmentIs32K:
.DoNotGenerateChecksumByte:
	ret

;--------------------------------------------------------------------
; .MakeSureAddress32KAligned
;	Parameters:
;		SS:BP:	Ptr to MENU
;	Returns:
;		ZF:		Cleared if EEPROM segment is not 32K aligned
;	Corrupts registers:
;		AX, DX
;--------------------------------------------------------------------
ALIGN JUMP_ALIGN
.MakeSureAddress32KAligned:
	test	WORD [g_cfgVars+CFGVARS.wEepromSegment], 07FFh
	jz		SHORT .AlignmentIs32K
	mov		dx, g_szErrAddrNot32KAligned
	jmp		Dialogs_DisplayErrorFromCSDX

;--------------------------------------------------------------------
; .PrepareBuffersForFlashing
;	Parameters:
;		SS:BP:	Ptr to MENU
;	Returns:
;		Nothing
;	Corrupts registers:
;		AX, BX, CX, SI, DI
;--------------------------------------------------------------------
ALIGN JUMP_ALIGN
.PrepareBuffersForFlashing:
	call	EEPROM_LoadFromRomToRamComparisonBuffer
	call	Buffers_AppendZeroesIfNeeded
	test	BYTE [g_cfgVars+CFGVARS.wFlags], FLG_CFGVARS_CHECKSUM
	jz		SHORT .DoNotGenerateChecksumByte
	jmp		Buffers_GenerateChecksum

;--------------------------------------------------------------------
; .InitializeFlashvarsFromDSSI
;	Parameters:
;		DS:SI:	Ptr to FLASHVARS to initialize
;		SS:BP:	Ptr to MENU
;	Returns:
;		Nothing
;	Corrupts registers:
;		AX, BX, DX, DI, ES
;--------------------------------------------------------------------
ALIGN JUMP_ALIGN
.InitializeFlashvarsFromDSSI:
	call	Buffers_GetFileBufferToESDI
	mov		[si+FLASHVARS.fpNextSourcePage], di
	mov		[si+FLASHVARS.fpNextSourcePage+2], es

	call	Buffers_GetFlashComparisonBufferToESDI
	mov		[si+FLASHVARS.fpNextComparisonPage], di
	mov		[si+FLASHVARS.fpNextComparisonPage+2], es

	mov		ax, [g_cfgVars+CFGVARS.wEepromSegment]
	mov		WORD [si+FLASHVARS.fpNextDestinationPage], 0
	mov		[si+FLASHVARS.fpNextDestinationPage+2], ax

	mov		al, [g_cfgVars+CFGVARS.bEepromType]
	mov		[si+FLASHVARS.bEepromType], al

	mov		al, [g_cfgVars+CFGVARS.bSdpCommand]
	mov		[si+FLASHVARS.bEepromSdpCommand], al

	mov		ax, SST_PAGE_SIZE
	cmp		BYTE [g_cfgVars+CFGVARS.bEepromType], EEPROM_TYPE.SST_39SF
	je		SHORT .UseSstPageSize

	eMOVZX	bx, [g_cfgVars+CFGVARS.bEepromPage]
	mov		ax, [bx+g_rgwEepromPageToSizeInBytes]
.UseSstPageSize:
	mov		[si+FLASHVARS.wEepromPageSize], ax

	call	.GetNumberOfPagesToFlashToAX
	mov		[si+FLASHVARS.wPagesToFlash], ax
	ret

;--------------------------------------------------------------------
; .GetNumberOfPagesToFlashToAX
;	Parameters:
;		DS:SI:	Ptr to FLASHVARS to initialize
;	Returns:
;		AX:		Number of pages to flash (0 = 65536)
;	Corrupts registers:
;		BX, DX
;--------------------------------------------------------------------
ALIGN JUMP_ALIGN
.GetNumberOfPagesToFlashToAX:
	call	Buffers_GetSelectedEepromSizeInWordsToAX
	xor		dx, dx
	eSHL_IM	ax, 1		; Size in bytes to...
	eRCL_IM	dx, 1		; ...DX:AX

	cmp		WORD [si+FLASHVARS.wEepromPageSize], BYTE 1
	jbe		SHORT .PreventDivideException
	div		WORD [si+FLASHVARS.wEepromPageSize]
.PreventDivideException:
	ret

;--------------------------------------------------------------------
; .DisplayFlashingResultsFromFlashvarsInDSBX
;	Parameters:
;		DS:BX:	Ptr to FLASHVARS
;		SS:BP:	Ptr to MENU
;	Returns:
;		Nothing
;	Corrupts registers:
;		AX, BX, DX
;--------------------------------------------------------------------
ALIGN JUMP_ALIGN
.DisplayFlashingResultsFromFlashvarsInDSBX:
	eMOVZX	bx, [bx+FLASHVARS.flashResult]
	jmp		[cs:bx+.rgfnFlashResultMessage]

ALIGN WORD_ALIGN
.rgfnFlashResultMessage:
	dw		.DisplayFlashSuccessful
	dw		.DisplayDeviceDetectionError
	dw		.DisplayPollingError
	dw		.DisplayDataVerifyError


;--------------------------------------------------------------------
; .DisplayDeviceDetectionError
; .DisplayPollingError
; .DisplayDataVerifyError
; .DisplayFlashSuccessful
;	Parameters:
;		SS:BP:	Ptr to MENU
;	Returns:
;		Nothing
;	Corrupts registers:
;		AX, DX, DI, ES
;--------------------------------------------------------------------
ALIGN JUMP_ALIGN
.DisplayDeviceDetectionError:
	mov		dx, g_szErrEepromDetection
	jmp		Dialogs_DisplayErrorFromCSDX

ALIGN JUMP_ALIGN
.DisplayPollingError:
	mov		dx, g_szErrEepromPolling
	jmp		Dialogs_DisplayErrorFromCSDX

ALIGN JUMP_ALIGN
.DisplayDataVerifyError:
	mov		dx, g_szErrEepromVerify
	jmp		Dialogs_DisplayErrorFromCSDX

ALIGN JUMP_ALIGN
.DisplayFlashSuccessful:
	call	Buffers_GetFileBufferToESDI
	cmp		WORD [es:di+ROMVARS.wRomSign], 0AA55h	; PC ROM?
	je		SHORT .DisplayRebootMessageAndReboot
	mov		dx, g_szForeignFlash
	jmp		Dialogs_DisplayNotificationFromCSDX
ALIGN JUMP_ALIGN
.DisplayRebootMessageAndReboot:
	mov		dx, g_szPCFlashSuccessful
	call	Dialogs_DisplayNotificationFromCSDX
	xor		ax, ax			; Cold boot flag
	jmp		Reboot_ComputerWithBootFlagInAX
