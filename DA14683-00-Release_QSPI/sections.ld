ENTRY(Reset_Handler)
SECTIONS
{
        .init_text :
        {
                KEEP(*(.isr_vector))
                . = 0xC0;
                KEEP(*(.patch_table))
                . = 0x100;
                KEEP(*(.patch_table_flash))
                . = 0x130;
                KEEP(*(.default_patch_code_handler_section))
                . = 0x200;
                *(text_reset*)
        } > ROM
        .copy.table :
        {
                . = ALIGN(4);
                __copy_table_start__ = .;
                LONG (__etext + (__data_end__ - __data_start__))
                LONG (__RetRAM0_code_start__)
                LONG (__RetRAM0_code_end__ - __RetRAM0_code_start__)
                LONG (__etext)
                LONG (__data_start__)
                LONG (__data_end__ - __data_start__)
                __copy_table_end__ = .;
                . = ALIGN(16);
        } > ROM
        .zero.table :
        {
                . = ALIGN(4);
                __zero_table_start__ = .;
                LONG (__bss_start__)
                LONG (__bss_end__ - __bss_start__)
                LONG (__RetRAM0_data_start)
                LONG (__RetRAM0_size)
                LONG (__RetRAM1_start__)
                LONG (__RetRAM1_end__ - __RetRAM1_start__)
                LONG (__RetRAM0_ble_variables_start)
                LONG (__RetRAM0_ble_variables_end - __RetRAM0_ble_variables_start)
                __zero_table_end__ = .;
                . = ALIGN(16);
        } > ROM
        PROVIDE_HIDDEN (__stext = .);
        .text :
        {
                *(EXCLUDE_FILE(*libnosys.a:sbrk.o
                               *libgcc.a:_aeabi_uldivmod.o
                               *libgcc.a:_muldi3.o
                               *libgcc.a:_dvmd_tls.o
                               *libgcc.a:bpabi.o
                               *libgcc.a:_udivdi3.o
                               *libgcc.a:_clzdi2.o
                               *libgcc.a:_clzsi2.o) .text*)
                . = ALIGN(4);
                __start_adapter_init_section = .;
                KEEP(*(adapter_init_section))
                __stop_adapter_init_section = .;
                __start_bus_init_section = .;
                KEEP(*(bus_init_section))
                __stop_bus_init_section = .;
                __start_device_init_section = .;
                KEEP(*(device_init_section))
                __stop_device_init_section = .;
                KEEP(*(.init))
                KEEP(*(.fini))
                *crtbegin.o(.ctors)
                *crtbegin?.o(.ctors)
                *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
                *(SORT(.ctors.*))
                *(.ctors)
                *crtbegin.o(.dtors)
                *crtbegin?.o(.dtors)
                *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
                *(SORT(.dtors.*))
                *(.dtors)
                . = ALIGN(4);
                PROVIDE_HIDDEN (__preinit_array_start = .);
                KEEP(*(.preinit_array))
                PROVIDE_HIDDEN (__preinit_array_end = .);
                . = ALIGN(4);
                PROVIDE_HIDDEN (__init_array_start = .);
                KEEP(*(SORT(.init_array.*)))
                KEEP(*(.init_array))
                PROVIDE_HIDDEN (__init_array_end = .);
                . = ALIGN(4);
                PROVIDE_HIDDEN (__fini_array_start = .);
                KEEP(*(SORT(.fini_array.*)))
                KEEP(*(.fini_array))
                PROVIDE_HIDDEN (__fini_array_end = .);
                *(.rodata*)
                KEEP(*(.eh_frame*))
        } > ROM
        .ARM.extab :
        {
                *(.ARM.extab* .gnu.linkonce.armextab.*)
        } > ROM
        __exidx_start = .;
        .ARM.exidx :
        {
                *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        } > ROM
        __exidx_end = .;
        .align_s :
        {
                . = ALIGN(16);
        } > ROM
        __etext = .;
        .data : AT (__etext)
        {
                . = ALIGN(16);
                __data_start__ = .;
                *(vtable)
                *(EXCLUDE_FILE(*\libg_nano.a:* *\libnosys.a:*) .data*)
                . = ALIGN(4);
                KEEP(*(.jcr*))
                . = ALIGN(16);
                __data_end__ = .;
        } > RAM
        .bss :
        {
                . = ALIGN(32);
                __bss_start__ = .;
                KEEP(*(.bss.ecc_buffer))
                *(EXCLUDE_FILE(*\libg_nano.a:* *\libnosys.a:*) .bss*)
                *(COMMON)
                . = ALIGN(32);
                __bss_end__ = .;
        } > RAM
        .heap (COPY):
        {
                __end__ = .;
                PROVIDE(end = .);
                *(.heap*)
                __HeapLimit = .;
        } > RAM
        .stack_dummy (COPY):
        {
                *(.stack*)
        } > RAM
        __StackTop = ORIGIN(RAM) + LENGTH(RAM);
        __StackLimit = __StackTop - SIZEOF(.stack_dummy);
        PROVIDE(__stack = __StackTop);
        ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
        RETENTION_BLE 0x07FC0200 (NOLOAD) :
        {
                __RetRAM0_ble_variables_start = .;
                KEEP(*(ble_variables))
                . = ALIGN(32);
                __RetRAM0_ble_variables_end = .;
        } > RetRAM0
        RETENTION_INIT0 : AT (__etext + (__data_end__ - __data_start__))
        {
                . = ALIGN(16);
                __RetRAM0_code_start__ = .;
                *(text_retained)
                *libgcc.a:_aeabi_uldivmod.o (.text*)
                *libgcc.a:_muldi3.o (.text*)
                *libgcc.a:_dvmd_tls.o (.text*)
                *libgcc.a:bpabi.o (.text*)
                *libgcc.a:_udivdi3.o (.text*)
                *libgcc.a:_clzdi2.o (.text*)
                *libgcc.a:_clzsi2.o (.text*)
                . = ALIGN(4);
                *(retention_mem_init)
                *(retention_mem_const)
                *(privileged_data_init)
                *(.retention)
                *\libg_nano.a:* (.data*)
                *\libnosys.a:* (.data*)
                . = ALIGN(16);
                __RetRAM0_code_end__ = .;
                . = ALIGN(4);
        } > RetRAM0
        RETENTION_RAM0 (COPY) :
        {
                . = ALIGN(4);
                __RetRAM0_start = .;
                *(nmi_info)
                *(hard_fault_info)
                *(retention_mem_uninit)
                . = ALIGN(32);
                __RetRAM0_data_start = .;
                *\libg_nano.a:* (.bss*)
                *\libnosys.a:* (.bss*)
                *(os_heap)
                *(ble_msg_heap)
                *(ble_env_heap)
                *(ble_db_heap)
                *(privileged_data_zi)
                *(retention_mem_zi)
                . = ALIGN(32);
                __RetRAM0_data_end__ = .;
        } > RetRAM0
        RETENTION_RAM1 (COPY) :
        {
                . = ALIGN(32);
                __RetRAM1_start__ = .;
                *(privileged_data_1_zi)
                *(retention_mem_1_zi)
                . = ALIGN(32);
                __RetRAM1_end__ = .;
        } > RetRAM1
        __RetRAM0_size = (__RetRAM0_data_end__ - __RetRAM0_data_start);
        __image_size = __etext + (__data_end__ - __data_start__)
                           + (__RetRAM0_code_end__ - __RetRAM0_code_start__)
                           - ORIGIN(ROM);
        __mirrored_image_size = __etext + (__data_end__ - __data_start__) - ORIGIN(ROM);
        INCLUDE da14683_bb_rom.symbols
}
