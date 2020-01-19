#include <zephyr.h>
#include <power/reboot.h>
#include <device.h>
#include <string.h>
#include <drivers/flash.h>
#include <fs/nvs.h>
#include <settings/settings.h>
#include "log.h"
#include "storage.h"

static struct nvs_fs fs;
#define TEST_ID 99

int storage_read(int address, char* buffer, size_t size)
{
    return nvs_read(&fs, address, buffer, size);
}

int storage_write(int address, char* buffer, size_t size)
{
    return nvs_write(&fs, address, buffer, size);
}

void storage_delete(int address)
{
    nvs_delete(&fs, address);
}

void storage_init()
{
    int rc = 0;
	struct flash_pages_info info;

	fs.offset = DT_FLASH_AREA_STORAGE_OFFSET;
	rc = flash_get_page_info_by_offs(device_get_binding(DT_FLASH_DEV_NAME),
					 fs.offset, &info);
	if (rc) {
		LOG_ERR("Storage init: Unable to get page info");
        return;
	}
	fs.sector_size = info.size;
	fs.sector_count = 3U;

	rc = nvs_init(&fs, DT_FLASH_DEV_NAME);
	if (rc) {
		LOG_ERR("Storage init: Flash Init failed\n");
        return;
	}

    /*
    uint8_t test = 0;
    storage_read(TEST_ID, &test, 1);
    LOG_INF("Storage test | loaded: %d", test);
    test++;
    storage_write(TEST_ID, &test, 1);
    */

    LOG_INF("Storage inited.");
}

void storage_loop()
{

}
