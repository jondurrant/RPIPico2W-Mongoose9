/***
 * Mongoose UI as a File Manager of files held in Pico Flash
 * Using pico_vfs
 * Jon Durrant - 5-mar-2026
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pico/stdlib.h>
#include "filesystem/vfs.h"
#include "mongoose_glue.h"

const char*   TARGET_FOLDER = "/tmp";
#define FN_LEN 256

void prepFS();

bool my_get_files(struct files *data, size_t i) {
	DIR*           fd;
	int fildes;
	struct dirent* in_file;
	size_t count =0;
	bool found = false;
    struct stat buffer;
    int         status;
    char fullname[FN_LEN];

	fd = opendir(TARGET_FOLDER);
	if (fd == NULL)
	{
		printf("Error : Failed to open target directory - %s\n", strerror(errno));
		return false;
	}

	while ((in_file = readdir(fd))){
		if (DT_REG == in_file->d_type){
			printf("%s is file\n", in_file->d_name);

			if (count == i){
				strcpy(data->name, in_file->d_name);
				data->size=0;
				snprintf(fullname, FN_LEN, "%s/%s", TARGET_FOLDER,  in_file->d_name);
				fildes = open(fullname, O_RDWR);
				if (fildes > 0){
					status = fstat(fildes, &buffer);
					if (status >= 0)
						data->size=buffer.st_size;
					else
						printf("ERROR: stat %s failed with code %d\n", fullname, status);
					close (fildes);
				} else {
					printf("ERROR: open %s failed with code %d\n", fullname, fildes);
				}
				found = true;
				break;
			}
			count ++;
		}
	}

	/* Close the directory */
	closedir(fd);

	return found;
}

void my_set_files(struct files *data, size_t i) {
	DIR*           fd;
	struct dirent* in_file;
	size_t count =0;

    char oldname[FN_LEN];
    char newname[FN_LEN];

    snprintf(newname, FN_LEN, "%s/%s", TARGET_FOLDER,  data->name);

	fd = opendir(TARGET_FOLDER);
	if (fd == NULL)
	{
		printf("Error : Failed to open target directory - %s\n", strerror(errno));
		return ;
	}

	while ((in_file = readdir(fd))){
		if (DT_REG == in_file->d_type){
			//printf("%s is file\n", in_file->d_name);

			if (count == i){
				strcpy(data->name, in_file->d_name);
				data->size=0;
				snprintf(oldname, FN_LEN, "%s/%s", TARGET_FOLDER,  in_file->d_name);

				int  res = rename(oldname, newname);

			    if (!res) {
			        printf("File name changed successfully\n");
			    }
			    else {
			        printf("File rename failed\n");
			    }

				break;
			}
			count ++;
		}
	}

	/* Close the directory */
	closedir(fd);
}


bool my_check_delete(void) {
  return false;
}

void my_start_delete(struct mg_str params) {
	char fullname[FN_LEN];
	//MG_DEBUG(("Passed parameters: [%.*s]", params.len, params.buf));

	snprintf(fullname, FN_LEN, "%s/%.*s", TARGET_FOLDER,  (int)params.len, params.buf);
	remove(fullname);
}

size_t my_file_read_upload(char *path, size_t offset, void *buf, size_t len) {
	FILE *fp ;
	char fullname[FN_LEN];
	size_t res = 0;

	snprintf(fullname, FN_LEN, "%s/%s", TARGET_FOLDER,  path);

    fp = fopen(fullname, "rb");
    if (fp != NULL){
    	printf("Opened %s (offset %zu, len %zu)\n", fullname, offset, len);
    	if (offset > 0){
			 if( fseek(fp, offset, SEEK_SET) != 0){
				 printf("Error - failed to seek pos in %s\n", fullname);
				 fclose(fp);
				 return 0;
			 }
    	 }

		res  = fread(
				buf,
				1,
				len,
				fp
				);
		printf("read %zu\n", res);
    	fclose(fp);
    } else {
    	printf("Open for R failed\n");
    }

    return res;
}

bool my_file_write_upload(char *path, size_t offset, void *buf, size_t len) {
	FILE *fp ;
	char fullname[FN_LEN];
	size_t res = 0;

	snprintf(fullname, FN_LEN, "%s/%s", TARGET_FOLDER,  path);

	if (offset == 0)
		fp = fopen(fullname, "w");
	else
		fp = fopen(fullname, "a");

	if (fp != NULL){
		//printf("Opened %s (offset %zu, len %zu)\n", fullname, offset, len);
		res = fwrite(
				buf,
				1,
				len,
				fp
				);
		fclose(fp);
	} else  {
		printf("Open for W/A failed\n");
	}

	printf("Upload %s %zu (%zu) achieved %zu\n",  fullname,  len, offset, res);
	return (res == len);
}





static void blink_timer(void *arg) {
  (void) arg;
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, !cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));
}

void wifi_setconfig(void *data) {
  struct mg_tcpip_driver_pico_w_data *d = (struct mg_tcpip_driver_pico_w_data *) data;
  struct mg_wifi_data *wifi = &d->wifi;
  wifi->ssid = WIZARD_WIFI_NAME;
  wifi->pass = WIZARD_WIFI_PASS;
  wifi->apssid = WIZARD_WIFI_AP_NAME;
  wifi->appass = WIZARD_WIFI_AP_PASS;
  wifi->apip = MG_IPV4(192, 168, 111, 1);
  wifi->apmask = MG_IPV4(255, 255, 255, 0);
  wifi->security = 0;
  wifi->apsecurity = 0;
  wifi->apchannel = 10;
  wifi->apmode = WIZARD_ENABLE_WIFI_AP;
}


int main(void) {
    stdio_init_all();
    sleep_ms(2000);

    printf("Starting\n");
    prepFS();


	// do not access the CYW43 LED before Mongoose initializes !
	MG_INFO(("Hardware initialised, starting firmware..."));
	// This blocks forever. Call it at the end of main()
	mongoose_init();
	mg_timer_add(&g_mgr, 1000, MG_TIMER_REPEAT, blink_timer, NULL);
	mongoose_set_http_handlers("files", my_get_files, my_set_files);
	mongoose_set_http_handlers("delete", my_check_delete, my_start_delete);
	mongoose_set_http_handlers("upload", my_file_read_upload, my_file_write_upload);
	for (;;) {
		mongoose_poll();
	}

     return 0;
}
