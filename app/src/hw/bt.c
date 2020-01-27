/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/printk.h>
#include <sys/byteorder.h>
#include <zephyr.h>

#include <settings/settings.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/services/bas.h>
#include <bluetooth/services/hrs.h>

#include "log.h"
#include "bt.h"

static uint8_t bt_connection_status = BT_STATUS_DISCONNECTED;

static bool advertising = false;
#define ADVERTISING_TIMEOUT 10
static u16_t advertising_timeout = ADVERTISING_TIMEOUT;

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,
		      0x0d, 0x18, 0x0f, 0x18, 0x05, 0x18),
	BT_DATA_BYTES(BT_DATA_UUID128_ALL,
		      0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
		      0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12),
};

static void connected(struct bt_conn *conn, u8_t err)
{
	if (err) {
		LOG_WRN("BT Connection failed (err 0x%02x).", err);
	} else {
		LOG_INF("BT Connected.");
		bt_connection_status = BT_STATUS_CONNECTED;
	}
}

static void disconnected(struct bt_conn *conn, u8_t reason)
{
	LOG_INF("BT Disconnected (reason 0x%02x).", reason);
	bt_connection_status = BT_STATUS_DISCONNECTED;
}

static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
};

static void bt_advertising_stop()
{
	if (!advertising) {
		return;
	}

	int err = bt_le_adv_stop();
	if (err) {
		LOG_ERR("Bluetooth advertising failed to stop (err %d).", err);
		advertising = true;
		return;
	}
	advertising = false;
	advertising_timeout = ADVERTISING_TIMEOUT;
	LOG_INF("Bluetooth advertising successfully stopped.");
}

static void bt_advertising_start()
{
	if (advertising) {
		return;
	}

	int err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		LOG_ERR("Bluetooth advertising failed to start (err %d).", err);
		advertising = false;
		advertising_timeout = ADVERTISING_TIMEOUT;
		return;
	}
	advertising = true;
	LOG_INF("Bluetooth advertising successfully started.");
}

static void bt_ready(void)
{
	if (IS_ENABLED(CONFIG_SETTINGS)) {
		LOG_INF("Settings loading.");
		settings_load();
	} else {
		LOG_INF("Settings not enabled");
	}

	bt_advertising_start();
}

static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	LOG_INF("Passkey for %s: %06u\n", addr, passkey);
}

static void auth_cancel(struct bt_conn *conn)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	LOG_INF("Pairing cancelled: %s\n", addr);
}

static struct bt_conn_auth_cb auth_cb_display = {
	.passkey_display = auth_passkey_display,
	.passkey_entry = NULL,
	.cancel = auth_cancel,
};

static void bas_notify(void)
{
	u8_t battery_level = bt_gatt_bas_get_battery_level();

	battery_level--;

	if (!battery_level) {
		battery_level = 100U;
	}

	bt_gatt_bas_set_battery_level(battery_level);
}

static void hrs_notify(void)
{
	static u8_t heartrate = 90U;

	/* Heartrate measurements simulation */
	heartrate++;
	if (heartrate == 160U) {
		heartrate = 90U;
	}

	bt_gatt_hrs_notify(heartrate);
}

void bt_setup(void)
{
	int err;

	err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)\n", err);
		return;
	}

	bt_ready();

	bt_conn_cb_register(&conn_callbacks);
	bt_conn_auth_cb_register(&auth_cb_display);

	LOG_INF("Bluetooth inited.");
}

void bt_loop()
{
	/* Implement notification. At the moment there is no suitable way
	 * of starting delayed work so we do it here
	 */

	/* Heartrate measurements simulation */
	hrs_notify();

	/* Battery level simulation */
	bas_notify();

	if (!advertising) {
		if (advertising_timeout <= 0) {
			bt_advertising_start();
			advertising_timeout = ADVERTISING_TIMEOUT;
		} else {
			advertising_timeout--;
		}
	}
}

uint8_t bt_get_connection_status()
{
	return bt_connection_status;
}
