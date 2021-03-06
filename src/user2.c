#include "user_vga.h"
#include "syscall.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "user.h"
#include <stdlib.h>
#include "nanos_user.h"

int atoi(const char *a);

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: %s [color_decimal] [row]\n", argv[0]);
		return 1;
	}

	int color = atoi(argv[1]) << 8;
	int row = atoi(argv[2]);

	if (row < 0 || row > 25) {
		printf("Bad row, 0 <= row <= 25\n");
		return 1;
	}

	void *blah = malloc(8);
	*(uint64_t*)USER_HEAP = 50;
	free(blah);

	srand(get_ticks() + row);

	for (int i = 0; i < 80; i++) {

		int delta = (rand() % 50000) - 50000;
		print(row, i, color, "\07");
		usleep(100000 + delta);
	}
}

int atoi(const char *a) {
	int num = 0;
	int mult = 1;
	int len = strlen(a);

	for (int i = len - 1; i >= 0; i--) {
		char c = a[i];
		num += (c - '0') * mult;
		mult *= 10;
	}
	return num;
}
