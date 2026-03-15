/***
 * Mongoose Interface for BNO055 Testing
 */

#include "mongoose.h"
#include "pico/stdlib.h"
#include <stdio.h>


static const char *s_url = "mqtt://" MQTT_HOST ":" MQTT_PORT;
static const char *s_sub_topic = "mg/123/rx";  // Subscribe topic
static const char *s_pub_topic = "mg/123/tx";  // Publish topic
static int s_qos = 1;                          // MQTT QoS
static struct mg_connection *s_mqtt_conn;      // Client connection

void wifi_setconfig(void *data) {
  struct mg_tcpip_driver_pico_w_data *d = (struct mg_tcpip_driver_pico_w_data *) data;
  struct mg_wifi_data *wifi = &d->wifi;
  wifi->ssid = WIFI_SSID;
  wifi->pass = WIFI_PASS;
  wifi->apssid = WIFI_SSID;
  wifi->appass = WIFI_PASS;
  wifi->apip = MG_IPV4(192, 168, 111, 1);
  wifi->apmask = MG_IPV4(255, 255, 255, 0);
  wifi->security = 0;
  wifi->apsecurity = 0;
  wifi->apchannel = 10;
  wifi->apmode = false;
}

static void subscribe(struct mg_connection *c, const char *topic) {
  struct mg_mqtt_opts opts = {};
  memset(&opts, 0, sizeof(opts));
  opts.topic = mg_str(topic);
  opts.qos = s_qos;
  mg_mqtt_sub(c, &opts);
  MG_INFO(("%lu SUBSCRIBED to %s", c->id, topic));
}

static void publish(struct mg_connection *c, const char *topic,
                    const char *message) {
  struct mg_mqtt_opts opts = {};
  memset(&opts, 0, sizeof(opts));
  opts.topic = mg_str(topic);
  opts.message = mg_str(message);
  opts.qos = s_qos;
  mg_mqtt_pub(c, &opts);
  MG_INFO(("%lu PUBLISHED %s -> %s", c->id, topic, message));
}

static void mqtt_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN) {
    MG_INFO(("%lu CREATED", c->id));
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_CONNECT) {
    if (c->is_tls) {
      struct mg_tls_opts opts = {.ca = mg_unpacked("/certs/ca.pem"),
                                 .name = mg_url_host(s_url)};
      mg_tls_init(c, &opts);
    }
  } else if (ev == MG_EV_ERROR) {
    // On error, log error message
    MG_ERROR(("%lu ERROR %s", c->id, (char *) ev_data));
  } else if (ev == MG_EV_MQTT_OPEN) {
    // MQTT connect is successful
    MG_INFO(("%lu CONNECTED to %s", c->id, s_url));
    subscribe(c, s_sub_topic);
  } else if (ev == MG_EV_MQTT_MSG) {
    // When we get echo response, print it
    char response[100];
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    mg_snprintf(response, sizeof(response), "Got %.*s -> %.*s", mm->topic.len,
                mm->topic.buf, mm->data.len, mm->data.buf);
    publish(c, s_pub_topic, response);
  } else if (ev == MG_EV_MQTT_CMD) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    if (mm->cmd == MQTT_CMD_PINGREQ) mg_mqtt_pong(c);
  } else if (ev == MG_EV_CLOSE) {
    MG_INFO(("%lu CLOSED", c->id));
    s_mqtt_conn = NULL;  // Mark that we're closed
  }
}

// Timer function - recreate client connection if it is closed
static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  struct mg_mqtt_opts opts = {.clean = true,
                              .qos = s_qos,
                              .topic = mg_str(s_pub_topic),
                              .keepalive = 5,
                              .version = 4,
                              .message = mg_str("bye"),
							  .user = mg_str(MQTT_USER),
							  .client_id = mg_str(MQTT_CLIENT),
							  .pass = mg_str(MQTT_PASSWD)
  	  };
  if (s_mqtt_conn == NULL) {
    s_mqtt_conn = mg_mqtt_connect(mgr, s_url, &opts, mqtt_ev_handler, NULL);
  } else {
    mg_mqtt_ping(s_mqtt_conn);
  }
}

int main(void) {
  struct mg_mgr mgr;
  stdio_init_all();
  sleep_ms(2000);
  // do not access the CYW43 LED before Mongoose initializes !
  MG_INFO(("Hardware initialised, starting firmware..."));


  mg_mgr_init(&mgr);
  mg_timer_add(&mgr, 3000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn, &mgr);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);


  return 0;
}
