; Project name	:	XTIDE Universal BIOS
; Description	:	Device specific equates for IDE drives.

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

%ifndef DEVICEIDE_INC
%define DEVICEIDE_INC

; Reset delays
HSR0_RESET_WAIT_US		EQU		6		; Delay must be at least 5 us
HSR1_RESET_WAIT_US		EQU		2100	; Delay must be at least 2 ms

; Polling timeout delays (system timer ticks, 1 tick = 54.9 ms)
TIMEOUT_DRQ				EQU		255			;   14 s (some CF cards occasionally have long write delays)
TIMEOUT_SELECT_DRIVE_DURING_DRIVE_DETECTION	EQU	(500/55)	;  500 ms
TIMEOUT_MOTOR_STARTUP	EQU		(10000/55)	;   10 s
TIMEOUT_MAXIMUM			EQU		255			; We would actually want 31 seconds here but I don't think there are so slow drives


; ATA specs say that in standby mode time to respond could be as long as 30 seconds
; Note that some low-power drives (like Toshiba 1,8" HDD) have power management enabled by default
; TODO: It might be better to define timeouts in IDEVARS (in ROMVARS) or in DPT. Having even a single bit in DPT means
; we could adjust timeouts on runtime when enabling or disabling power management.
%ifdef MODULE_POWER_MANAGEMENT
	TIMEOUT_BSY			EQU		TIMEOUT_MOTOR_STARTUP
	TIMEOUT_DRDY		EQU		TIMEOUT_MOTOR_STARTUP
%else
	TIMEOUT_BSY			EQU		(2600/55)	; 2600 ms
	TIMEOUT_DRDY		EQU		(2600/55)	; 2600 ms
%endif

%endif ; DEVICEIDE_INC
