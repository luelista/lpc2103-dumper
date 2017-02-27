
/*OUTPUT_FORMAT("ihex")*/

OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY {
	RAM (rwx) : ORIGIN = 0x40000800, LENGTH = 2k
}

SECTIONS {
	. = 0x40000800;
	.text : {
		* (.text);
	}>RAM
	.data : {
		* (.data);
	}>RAM
}


