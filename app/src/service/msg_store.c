#include "log.h"
#include "msg_store.h"

#define MSG_STORE_MEMPOOL
#ifdef MSG_STORE_MEMPOOL
#define MSG_STORE_MEMPOOL_MAX MSG_STORE_TEXT_LIMIT
K_MEM_POOL_DEFINE(msg_store_pool, MSG_STORE_MEMPOOL_MAX / 4, MSG_STORE_MEMPOOL_MAX, MSG_STORE_SIZE, 4);
#endif


static msg_circular_array_t store;
static msg_store_listener_cb_t listener_cb;

void msg_store_init()
{
    memset(&store, 0, sizeof(store));
    store.size = MSG_STORE_SIZE;
}

void msg_store_loop()
{

}

msg_circular_array_t* msg_store_get_storage()
{
    return &store;
}

u16_t msg_store_get_size()
{
    return store.size;
}

msg_t msg_store_get_message(u16_t depth)
{
    u16_t index = (store.pointer_index
        + (store.size - 1 - (depth % store.size))) % store.size;
    return store.msgs[index];
}

void msg_store_push_message(char* text, u16_t len)
{
    if (len > MSG_STORE_TEXT_LIMIT) {
        LOG_WRN("Msg of length %d B not stored, limit %d B exceeded.",
        len, MSG_STORE_TEXT_LIMIT);
        return;
    }
    LOG_INF("Msg #%d pushed to store (%d B): %s",
        store.pointer_index, len, log_strdup(text));

    char* stored = store.msgs[store.pointer_index].text;
    if (stored) {
#ifdef MSG_STORE_MEMPOOL
        k_free(stored);
#else
        free(stored);
#endif
    }

    store.msgs[store.pointer_index].len = len;
#ifdef MSG_STORE_MEMPOOL
	if (len > MSG_STORE_MEMPOOL_MAX) {
		stored = NULL;
		LOG_WRN("Mem pool alloc for %d B is out of bounds.", len);
	} else if (0 != (stored = (u8_t*)k_mem_pool_malloc(&msg_store_pool, len))) {
		LOG_DBG("Mem pool alloc for %d B succeeded.", len);
	} else {
    	LOG_WRN("Mem pool alloc for %d B failed.", len);
	}
#else
	stored = (char*)malloc(len);
#endif
    
    store.msgs[store.pointer_index].text = stored;
    if (stored) {
        memcpy(stored, text, len);
        store.pointer_index = (store.pointer_index + 1) % store.size;
    } else {
        LOG_ERR("Msg storage of %d B could not be allocated.", len);
    }

    if (listener_cb) {
        listener_cb(text, len);
    }
}

void msg_store_register_listener(msg_store_listener_cb_t msg_store_listener_cb)
{
    listener_cb = msg_store_listener_cb;
}
