/*
 *  _____             ___ ___   |
 * |  _  |___ ___ _ _|_  |_  |  |  C/C++ framework for 32-bit AVRs
 * |     | -_|  _| | |_  |  _|  |  
 * |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
 *               |___|          |
 *
 * Copyright (c) 2012, Muiku Oy
 * All rights reserved.
 *
 * LICENSE
 *
 * New BSD License, see the LICENSE.txt bundled with this package. If you did
 * not receive a copy of the license and are unable to obtain it through the
 * world-wide-web, please send an email to contact@muiku.com so we can send
 * you a copy.
 */

.section .exception, "ax", @progbits
.balign 0x200
.global _evba
.type _evba, @function

_evba:
	.org  0x000
 
.balign 4
 
_int_group0:
	mov     r12, 0
	call    _isrhandler_proxy
	rete
 
_int_group1:
	mov     r12, 1
	call    _isrhandler_proxy 
	rete

_int_group2:
	mov     r12, 2
	call    _isrhandler_proxy
	rete
 
_int_group3:
	mov     r12, 3
	call    _isrhandler_proxy
	rete

_int_group4:
	mov     r12, 4
	call    _isrhandler_proxy
	rete

_int_group5:
	mov     r12, 5
	call    _isrhandler_proxy
	rete

_int_group6:
	mov     r12, 6
	call    _isrhandler_proxy
	rete

_int_group7:
	mov     r12, 7
	call    _isrhandler_proxy
	rete

_int_group8:
	mov     r12, 8
	call    _isrhandler_proxy
	rete

_int_group9:
	mov     r12, 9
	call    _isrhandler_proxy
	rete

_int_group10:
	mov     r12, 10
	call    _isrhandler_proxy
	rete

_int_group11:
	mov     r12, 11
	call    _isrhandler_proxy
	rete

_int_group12:
	mov     r12, 12
	call    _isrhandler_proxy
	rete

_int_group13:
	mov     r12, 13
	call    _isrhandler_proxy
	rete

_int_group14:
	mov     r12, 14
	call    _isrhandler_proxy
	rete

_int_group15:
	mov     r12, 15
	call    _isrhandler_proxy
	rete

_int_group16:
	mov     r12, 16
	call    _isrhandler_proxy
	rete

_int_group17:
	mov     r12, 17
	call    _isrhandler_proxy
	rete

_int_group18:
	mov     r12, 18
	call    _isrhandler_proxy
	rete

_int_group19:
	mov     r12, 19
	call    _isrhandler_proxy
	rete

.balign 4
.global _ipr
.type _ipr, @object

_ipr:
	.word   (_int_group0 - _evba),\
	        (_int_group1 - _evba),\
	        (_int_group2 - _evba),\
	        (_int_group3 - _evba),\
	        (_int_group4 - _evba),\
	        (_int_group5 - _evba),\
	        (_int_group6 - _evba),\
	        (_int_group7 - _evba),\
	        (_int_group8 - _evba),\
	        (_int_group9 - _evba),\
	        (_int_group10 - _evba),\
	        (_int_group11 - _evba),\
	        (_int_group12 - _evba),\
	        (_int_group13 - _evba),\
	        (_int_group14 - _evba),\
	        (_int_group15 - _evba),\
	        (_int_group16 - _evba),\
	        (_int_group17 - _evba),\
	        (_int_group18 - _evba),\
	        (_int_group19 - _evba)
