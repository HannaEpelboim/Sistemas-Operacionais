#include <stdio.h>

int main(int argc, char *argv[]) {
	FILE *file;
	char ch;

	for (int i = 1; i < argc; i++) {
    	file = fopen(argv[i], "r");
    	if (file == NULL) {
        	perror("Erro ao abrir o arquivo");
        	return 1;
    	}

    	while ((ch = fgetc(file)) != EOF) {
        	putchar(ch);
    	}
    	fclose(file);
	}
	return 0;
}
