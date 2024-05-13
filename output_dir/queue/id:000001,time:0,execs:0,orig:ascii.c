#include <stdio.h>

int main () {
	for (int i = 0; i < 256; i++)
		printf("char %d = <%c>\n", i, i);
	return 0;
}
