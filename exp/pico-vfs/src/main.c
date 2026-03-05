/***
 *
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pico/stdlib.h>
#include "filesystem/vfs.h"

int main(void) {
    stdio_init_all();
    fs_init();

    printf("Starting\n");

    FILE *fp = fopen("/HELLO.TXT", "w");
    if (fp != NULL){
		fprintf(fp, "Hello World!\n");
		fclose(fp);
    } else {
    	printf("Open for W failed\n");
    }

    printf("Written\n");

    fp = fopen("/HELLO.TXT", "r");
    if (fp != NULL){
		char myString[100];
		fgets(myString, 100, fp);
		printf("Read: %s\n", myString);
    } else {
    	printf("Open for R failed\n");
    }

    printf("DONE\n");

    for(;;){
    	sleep_ms(5000);
    }


}
