#include <zephyr.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <zephyr.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <sys/byteorder.h>

#include "log.h"
#include "bt.h"
#include "bt_connect.h"

bt_addr_le_t last_connected_addr;

#define CONNECT_KNOWN_DEVICE
#ifdef CONNECT_KNOWN_DEVICE
u16_t connect_timeout = 10;
#define CONNECT_TIMEOUT 10;
#endif // CONNECT_KNOWN_DEVICE


//#define SCAN_KNOWN_DEVICES
#ifdef SCAN_KNOWN_DEVICES
bool scanning = false;
u16_t scan_timeout = 10;
#define SCAN_TIMEOUT 30;

static bool is_known_addr(bt_addr_le_t* addr)
{
    return memcmp(&last_connected_addr, addr, sizeof(bt_addr_le_t)) == 0;
}

static void device_found(const bt_addr_le_t *addr, s8_t rssi, u8_t type,
			 struct net_buf_simple *ad)
{
	char dev[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(addr, dev, sizeof(dev));
	LOG_INF("[DEVICE]: %s, AD evt type %u, AD data len %u, RSSI %i\n",
	       log_strdup(dev), type, ad->len, rssi);

    if (is_known_addr(addr)) {
        LOG_INF("Found known device %s, connecting...", log_strdup(dev));
        scanning = false;
        scan_timeout = SCAN_TIMEOUT;
        int err = bt_le_scan_stop();
        if (err) {
            LOG_ERR("Stop LE scan failed (err %d)\n", err);
            return;
        }

        bt_conn_create_le(addr, BT_LE_CONN_PARAM_DEFAULT);
    }
}

static int scan_start(void)
{
	struct bt_le_scan_param scan_param = {
		.type       = BT_HCI_LE_SCAN_ACTIVE,
		.filter_dup = BT_HCI_LE_SCAN_FILTER_DUP_DISABLE,
		.interval   = BT_GAP_SCAN_FAST_INTERVAL,
		.window     = BT_GAP_SCAN_FAST_WINDOW,
	};

	return bt_le_scan_start(&scan_param, device_found);
}
#endif // SCAN_KNOWN_DEVICES

static void connected(struct bt_conn *conn, u8_t conn_err)
{
	if (conn_err) {
		return;
	}

#ifdef SCAN_KNOWN_DEVICES
    int err = bt_le_scan_stop();
    if (err) {
        LOG_ERR("Stop LE scan failed (err %d)\n", err);
    }
    scanning = false;
    scan_timeout = SCAN_TIMEOUT;
#endif

#ifdef CONNECT_KNOWN_DEVICE
    connect_timeout = CONNECT_TIMEOUT;
#endif // CONNECT_KNOWN_DEVICE

    bt_addr_le_t* addr = bt_conn_get_dst(conn);
    memcpy(&last_connected_addr, addr, sizeof(bt_addr_le_t));

    char dev[BT_ADDR_LE_STR_LEN];
	bt_addr_le_to_str(addr, dev, sizeof(dev));
    LOG_INF("Connected to %s, marking as a known device.", log_strdup(dev));
}

static void disconnected(struct bt_conn *conn, u8_t reason)
{
#ifdef SCAN_KNOWN_DEVICES
	int err = scan_start();
	if (err) {
        scanning = false;
        scan_timeout = SCAN_TIMEOUT;
		LOG_ERR("Scanning failed to start (err %d)\n", err);
        return;
	}
    scanning = true;
#endif

#ifdef CONNECT_KNOWN_DEVICE
    connect_timeout = CONNECT_TIMEOUT;
#endif // CONNECT_KNOWN_DEVICE
}

static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
};


void bt_connect_init()
{
    bt_conn_cb_register(&conn_callbacks);
}

void bt_connect_loop()
{
#ifdef SCAN_KNOWN_DEVICES
    if (!scanning) {
        if (scan_timeout <= 0) {
            scan_start();
        } else {
            scan_timeout--;
        }
    }
#endif // SCAN_KNOWN_DEVICES

#ifdef CONNECT_KNOWN_DEVICE
    if (bt_get_connection_status() == BT_STATUS_DISCONNECTED) {
        if (connect_timeout <= 0) {
            bt_conn_create_slave_le(&last_connected_addr, BT_LE_ADV_CONN_DIR_LOW_DUTY);
            connect_timeout = CONNECT_TIMEOUT;
            LOG_INF("Trying to connect to last known master...");
        } else {
            connect_timeout--;
        }
    }
#endif // CONNECT_KNOWN_DEVICE
}
