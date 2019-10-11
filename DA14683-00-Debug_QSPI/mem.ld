MEMORY
{
        ROM (rx) : ORIGIN = (0x8000000 + (0)), LENGTH = (256 * 1024)
        RetRAM0 (rwx): ORIGIN = (0x7FC0000) + ((0x200)), LENGTH = (( 72 * 1024) - (0x200))
        RetRAM1 (rwx): ORIGIN = (0), LENGTH = 0
        RAM (rw) : ORIGIN = (0x7FC0000) + (( 72 * 1024)), LENGTH = (( 56 * 1024))
}
