#include <stdio.h>
#include <stdint.h>
#include "resource/emulator.h"


int main()
{
	printf("R1\n");
	
	int a;
	scanf_s("%d", &a);

	opcode_t opcode = { a };

	Emulator emulator = Emulator();

	scanf_s("%d", &a);
	return 0;
}

