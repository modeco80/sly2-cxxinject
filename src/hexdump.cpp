#include <ml/string.h>
#include <utils/log.hpp>

#define HEXDUMP_COLS 16

inline int isprint(char c) {
	return (c >= 0x20 && c <= 0x7E);
}

void hexdump(const void *mem, size_t len, unsigned int prefix, bool printChar) {
	char tempBuffer[10 + HEXDUMP_COLS * 3 + (1 + HEXDUMP_COLS + 2)];
	size_t offset = 0;

	for (unsigned i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++) {
		if (i % HEXDUMP_COLS == 0) {
			offset += sprintf(tempBuffer + offset, "%08x: ", prefix + i);
		} else if ((i + HEXDUMP_COLS / 2) % HEXDUMP_COLS == 0) {
			offset += sprintf(tempBuffer + offset, " ");
		}

		if (i < len) {
			offset += sprintf(tempBuffer + offset, "%02x ", 0xFF & ((unsigned char *)mem)[i]);
		} else if (printChar) {
			offset += sprintf(tempBuffer + offset, "   ");
		}

		if (i % HEXDUMP_COLS == (HEXDUMP_COLS - 1)) {
			if (printChar) {
				offset += sprintf(tempBuffer + offset, "|");
				for (unsigned j = i - (HEXDUMP_COLS - 1); j <= i; j++)
				{
					if (j >= len) {
						offset += sprintf(tempBuffer + offset, " ");
					} else if (isprint(((unsigned char *)mem)[j])) {
						offset += sprintf(tempBuffer + offset, "%c", 0xFF & ((char *)mem)[j]);
					} else {
						// non-printable character
						offset += sprintf(tempBuffer + offset, ".");
					}
				}
				offset += sprintf(tempBuffer + offset, "|");
			}

			tempBuffer[offset] = '\0';
			utilLog(LogInfo, tempBuffer);
			offset = 0;
		}
	}
}
