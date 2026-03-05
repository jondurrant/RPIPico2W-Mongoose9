/*
 * prepFS.c
 *
 * Prepare the Filesystem
 *
 *  Created on: 4 Mar 2026
 *      Author: jondurrant
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pico/stdlib.h>
#include "filesystem/vfs.h"


void prepFS(){
	char fn[20];
	fs_init();

	mkdir("/tmp", 0777);

	for (int i=1; i <= 5; i++){
		sprintf(fn, "/tmp/file%d.txt", i);
		FILE *fp = fopen(fn, "w");
			if (fp != NULL){
				fprintf(fp, "This is file %d\nName: %s\nHello World!\n", i, fn);
				fclose(fp);
			} else {
				printf("Open for W failed\n");
			}
	}
}


