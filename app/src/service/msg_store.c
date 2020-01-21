#include "log.h"
#include "msg_store.h"

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
    LOG_INF("Msg #%d pushed to store: (%d) %s",
        store.pointer_index, len, log_strdup(text));

    char* stored = store.msgs[store.pointer_index].text;
    if (stored) {
        k_free(stored);
    }

    store.msgs[store.pointer_index].len = len;
    stored = k_malloc(len);
    memcpy(stored, text, len);
    store.msgs[store.pointer_index].text = text;
    store.pointer_index = (store.pointer_index + 1) % store.size;

    if (listener_cb) {
        listener_cb(text, len);
    }
}

void msg_store_register_listener(msg_store_listener_cb_t msg_store_listener_cb)
{
    listener_cb = msg_store_listener_cb;
}
