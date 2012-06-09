/**
 *  _____             ___ ___   |
 * |  _  |___ ___ _ _|_  |_  |  |  Teh framework for 32-bit AVRs
 * |     | -_|  _| | |_  |  _|  |  
 * |__|__|___|_| |_  |___|___|  |  https://github.com/aery32
 *               |___|          |
 * 
 * Copyright (c) 2012, Muiku Oy
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 *    * Neither the name of Muiku Oy nor the names of its contributors may be
 *      used to endorse or promote products derived from this software without
 *      specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
