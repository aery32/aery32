OUTPUT_FORMAT("elf32-avr32", "elf32-avr32", "elf32-avr32")
OUTPUT_ARCH(avr32:uc)
ENTRY(_start)
MEMORY
{
	FLASH (rxai!w) : ORIGIN = 0x80002000, LENGTH = 128K - 0x2000
	CPUSRAM (wxa!ri) : ORIGIN = 0x00000004, LENGTH = 0x7FFC
	USERPAGE : ORIGIN = 0x80800000, LENGTH = 512
	FACTORYPAGE : ORIGIN = 0x80800200, LENGTH = 512
}
SECTIONS
{
	__stack_size__ = DEFINED(__stack_size__) ? __stack_size__ : 4K;

	. = ORIGIN(FLASH);

	.reset :
		{ *(.reset) } >FLASH
	.rel.got :
		{ *(.rel.got) } >FLASH AT>FLASH
	.rela.got :
		{ *(.rela.got) } >FLASH AT>FLASH
	.init :
		{ KEEP (*(.init)) } >FLASH
	.text :
		{ *(.text .text.* .gnu.linkonce.t.*) _etext = .; } >FLASH
	.fini :
		{ KEEP (*(.fini)) } >FLASH
	.rodata :
		{ *(.rodata .rodata.* .gnu.linkonce.r.*) } >FLASH
	.dalign	:
		{ . = ALIGN(8); _data_lma = .; } >FLASH
	. = ORIGIN(CPUSRAM);
	_data = .;
	.ctors :
		{
			/*
			 * gcc uses crtbegin.o to find the start of the constructors, so
			 * we make sure it is first. Because this is a wildcard, it doesn't
			 * matter if the user does not actually link against crtbegin.o; the
			 * linker won't look for a file to match a wildcard. The wildcard
			 * also means that it doesn't matter which directory crtbegin.o is
			 * in.
			 */
			KEEP (*crtbegin*.o(.ctors))
			/*
			 * We don't want to include the .ctor section from from the crtend.o
			 * file until after the sorted ctors. The .ctor section from the
			 * crtend file contains the end of ctors marker and it must be last.
			 */
			KEEP (*(EXCLUDE_FILE (*crtend*.o ) .ctors))
			KEEP (*(SORT(.ctors.*)))
			KEEP (*(.ctors))
		} >CPUSRAM AT>FLASH
	.dtors :
		{
			KEEP (*crtbegin*.o(.dtors))
			KEEP (*(EXCLUDE_FILE (*crtend*.o ) .dtors))
			KEEP (*(SORT(.dtors.*)))
			KEEP (*(.dtors))
		} >CPUSRAM AT>FLASH
	.jcr :
		{ KEEP (*(.jcr)) } >CPUSRAM AT>FLASH
	.got :
		{ *(.got.plt) *(.got) } >CPUSRAM AT>FLASH
	.data :
		{ *(.data .data.* .gnu.linkonce.d.*) SORT(CONSTRUCTORS)	} >CPUSRAM AT>FLASH
	.balign	:
		{ . = ALIGN(8); _edata = .; } >CPUSRAM AT>FLASH
	.bss :
		{
			*(.dynbss) *(.bss .bss.* .gnu.linkonce.b.*) *(COMMON)
			/*
			 * Align here to ensure that the .bss section occupies space up to
			 * _end. Align after .bss to ensure correct alignment even if the
			 * .bss section disappears because there are no input sections.
			 */
			. = ALIGN(8);
		} >CPUSRAM
	. = ALIGN(8);
	_end = .;
	.heap :
		{
			__heap_start__ = .;
			*(.heap)
			. = ORIGIN(CPUSRAM) + LENGTH(CPUSRAM) - __stack_size__;
			__heap_end__ = .;
		}
	.stack ORIGIN(CPUSRAM) + LENGTH(CPUSRAM) - __stack_size__ :
		{
			_stack = .;
			*(.stack)
			. = __stack_size__;
			_estack = .;
		} >CPUSRAM
	.userpage :
		{ *(.userpage .userpage.*) } >USERPAGE
	.factorypage :
		{ *(.factorypage .factorypage.*)  } >FACTORYPAGE AT>FACTORYPAGE

	/* DWARF-2 debugging data sections */
	.debug_aranges  0 :
		{ *(.debug_aranges) }
	.debug_pubnames 0 :
		{ *(.debug_pubnames) }
	.debug_info    0
		: { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev  0 :
		{ *(.debug_abbrev) }
	.debug_line    0 :
		{ *(.debug_line) }
	.debug_frame   0 :
		{ *(.debug_frame) }
	.debug_str     0 :
		{ *(.debug_str) }
	.debug_loc     0 :
		{ *(.debug_loc) }
	.debug_macinfo 0 :
		{ *(.debug_macinfo) }

	.comment 0 :
		{ *(.comment) }

	/DISCARD/ :
		{ *(.note.GNU-stack) }
}
