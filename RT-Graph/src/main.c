/***
 * Mongoose Interface for BNO055 Testing
 */

#include "mongoose_glue.h"
#include "pico/stdlib.h"
#include "bno055.h"
#include "hardware/i2c.h"
#include <stdio.h>

#define GP_SDA 20
#define GP_SCL 21

typedef struct bno055_t bno055_t;
typedef struct bno055_accel_float_t bno055_accel_float_t;
typedef struct bno055_euler_float_t bno055_euler_float_t;
static bno055_t bno;

void initIMU(){
	 const uint sda_pin = GP_SDA;
	const uint scl_pin = GP_SCL;

	gpio_init(sda_pin);
	gpio_init(scl_pin);
	gpio_set_function(sda_pin, GPIO_FUNC_I2C);
	gpio_set_function(scl_pin, GPIO_FUNC_I2C);
	gpio_pull_up(sda_pin);
	gpio_pull_up(scl_pin);
	i2c_init(i2c0, 400 * 1000);

	int8_t res = bno055_pico_init(&bno, i2c_default, BNO055_I2C_ADDR1);
	if (res) {
		res = bno055_pico_init(&bno, i2c_default, BNO055_I2C_ADDR2);
	}
	if (res) {
		printf("BNO055 inilization failed!\n");
	}
	sleep_ms(100);

	res = bno055_set_power_mode(BNO055_POWER_MODE_NORMAL);
	if (res) {
		printf("BNO055 power mode set failed!\n");
	}
	sleep_ms(100);

	res = bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);
	if (res) {
		printf("BNO055 operation mode set failed!\n");
	}
	sleep_ms(100);
}



static void myAngularSensor(struct angular *data){
	 bno055_euler_float_t eulerAngles;
	bno055_convert_float_euler_hpr_deg(&eulerAngles);
	int d;

	d =   (int)(eulerAngles.h * 100.0 + .5);
	data->yaw = (double)d/100.0;
	d =   (int)(eulerAngles.p * 100.0 + .5);
	data->pitch =(double)d/100.0;
	d =   (int)(eulerAngles.r * 100.0 + .5);
	data->roll = (double)d/100.0;
	//printf("%f %f %f\n", eulerAngles.h, eulerAngles.p, eulerAngles.r);
	//printf("%f %f %f\n", data->yaw, data->pitch, data->roll);
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

static void blink_timer(void *arg) {
  (void) arg;
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, !cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));
}

int main(void) {
  stdio_init_all();
  sleep_ms(2000);
  initIMU();
  // do not access the CYW43 LED before Mongoose initializes !
  MG_INFO(("Hardware initialised, starting firmware..."));
  // This blocks forever. Call it at the end of main()
  mongoose_init();
  mg_timer_add(&g_mgr, 1000, MG_TIMER_REPEAT, blink_timer, NULL);
  mongoose_set_http_handlers("angular", myAngularSensor, NULL);
  mongoose_add_ws_reporter(500, "angular");
  for (;;) {
    mongoose_poll();
  }

  return 0;
}
