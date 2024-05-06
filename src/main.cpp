#include "main/Main.h"
#include <cstdlib>

int main(int argc, char *argv[]) {
	if (Main().Run(argc, argv) == false) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
