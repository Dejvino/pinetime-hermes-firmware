#include <zephyr.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/byteorder.h>

#include <settings/settings.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/services/bas.h>
#include <bluetooth/services/hrs.h>

#include "log.h"
#include <hw/bt.h>
#include <service/msg_store.h>

/* Custom Service Variables */
static struct bt_uuid_128 vnd_uuid = BT_UUID_INIT_128(
	0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
    0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12);
static struct bt_uuid_128 vnd_notify_uuid = BT_UUID_INIT_128(
	0xde, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab);

static u8_t vnd_value[MSG_STORE_TEXT_LIMIT];

static ssize_t read_vnd(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			void *buf, u16_t len, u16_t offset)
{
	char *value = attr->user_data;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
				 strlen(value));
}

static ssize_t write_notify(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 const void *buf, u16_t len, u16_t offset,
			 u8_t flags)
{
	u8_t header_length = 1;
	u8_t buffer_length = sizeof(vnd_value);

	u8_t *value = attr->user_data;

	if (offset + len > sizeof(vnd_value)) {
		LOG_WRN("BT attempted to send more data than the buffer can hold.");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	if (offset == 0) {
		memset(value, 0, sizeof(vnd_value));
		u8_t msg_length = ((u8_t*)buf)[0];
		u8_t requested = msg_length + header_length;
		if (requested > buffer_length) {
			LOG_WRN("BT requested to send more data than the buffer can hold: %d / %d",
				msg_length, buffer_length);
			return BT_GATT_ERR(BT_ATT_ERR_INSUFFICIENT_RESOURCES);
		}
		LOG_DBG("BT Notify msg initiated, expecting %d bytes.", msg_length);
	}

	memcpy(value + offset, buf, len);

	u8_t msg_length = value[0];
	u8_t sent_length = offset + len;
	bool last_message = (sent_length >= msg_length);
	if (last_message) {
		LOG_DBG("BT Notify msg ended. Recvd %d bytes, expected %d.",
			sent_length, msg_length);
		msg_store_push_message((char*)value + 1, msg_length);
	} else {
		LOG_DBG("BT Notify msg ongoing. Recvd %d bytes, expected %d.",
			sent_length, msg_length);
	}

	return len;
}

/* Vendor Primary Service Declaration */
BT_GATT_SERVICE_DEFINE(vnd_svc,
	BT_GATT_PRIMARY_SERVICE(&vnd_uuid),
	BT_GATT_CHARACTERISTIC(&vnd_notify_uuid.uuid,
			       BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_WRITE_ENCRYPT,
			       read_vnd, write_notify, vnd_value),
);

void bt_notify_service_init()
{

}

void bt_notify_service_loop()
{

}
