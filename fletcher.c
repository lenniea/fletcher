#include "fletcher.h"

/*
 *  Derived from https://en.wikipedia.org/wiki/Fletcher%27s_checksum
 */
uint16_t fletcher16(const uint8_t *data, size_t len) {
	uint32_t c0, c1;

	/*  Found by solving for c1 overflow: */
	/* n > 0 and n * (n+1) / 2 * (2^8-1) < (2^32-1). */
	for (c0 = c1 = 0; len > 0; ) {
		size_t blocklen = len;
		if (blocklen > 5002) {
			blocklen = 5002;
		}
		len -= blocklen;
		do {
			c0 = c0 + *data++;
			c1 = c1 + c0;
		} while (--blocklen);
		c0 = c0 % 255;
		c1 = c1 % 255;
   }
   return (c1 << 8 | c0);
}

uint32_t fletcher32(const uint16_t *data, size_t len) {
	uint32_t c0, c1;
	len = (len + 1) & ~1;      /* Round up len to words */

	/* We similarly solve for n > 0 and n * (n+1) / 2 * (2^16-1) < (2^32-1) here. */
	/* On modern computers, using a 64-bit c0/c1 could allow a group size of 23726746. */
	for (c0 = c1 = 0; len > 0; ) {
		size_t blocklen = len;
		if (blocklen > 360*2) {
			blocklen = 360*2;
		}
		len -= blocklen;
		do {
			c0 = c0 + *data++;
			c1 = c1 + c0;
		} while ((blocklen -= 2));
		c0 = c0 % 65535;
		c1 = c1 % 65535;
	}
	return (c1 << 16 | c0);
}

#ifdef UNIT_TEST

#include <stdio.h>
#include <string.h>

/* Verify Test Vectors */

int main(int argc, char* argv[])
{
	const char str1[] = "abcde\0";		/* '\0' for padding */
	const char str2[] = "abcdef";
	const char str3[] = "abcdefgh";

	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);
	size_t len3 = strlen(str3); 

	uint16_t f16 = fletcher16((const uint8_t*) str1,  len1);
	printf("fletcher16(%s,%u) -> %u (0x%04X)\n",  str1, len1, f16,f16);

	f16 = fletcher16((const uint8_t*) str2,  len2);
	printf("fletcher16(%s,%u) -> %u (0x%04X)\n", str2, len2, f16,f16);

	f16 = fletcher16((const uint8_t*) str3,  len3);
	printf("fletcher16(%s,%u) -> %u (0x%04X)\n", str2, len3, f16,f16);

	uint32_t f32 = fletcher32((const uint16_t*) str1,  len1);
	printf("fletcher32(%s,%u) -> %u (0x%08X)\n",  str1, len1, f32,f32);

	f32 = fletcher32((const uint16_t*) str2,  len2);
	printf("fletcher32(%s,%u) -> %u (0x%08X)\n", str2, len2, f32,f32);

	f32 = fletcher32((const uint16_t*) str3,  len3);
	printf("fletcher32(%s,%u) -> %u (0x%08X)\n", str3, len3, f32,f32);

	return 0;
}

#endif
