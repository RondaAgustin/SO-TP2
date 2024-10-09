/* _loader.c */
#include <stdint.h>
#include "includes/std.h"

extern char bss;
extern char endOfBinary;

int main();

int _start() {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	return main();
}