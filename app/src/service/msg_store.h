#include <zephyr.h>
#include <zephyr/types.h>

#define MSG_STORE_SIZE 20
#define MSG_STORE_TEXT_LIMIT 64

typedef struct {
    u16_t len;
    char* text;
} msg_t;

typedef struct {
    u16_t size;
    msg_t msgs[MSG_STORE_SIZE];
    u16_t pointer_index;
} msg_circular_array_t;

void msg_store_init();
void msg_store_loop();

msg_circular_array_t* msg_store_get_storage();
u16_t msg_store_get_size();
msg_t msg_store_get_message(u16_t depth);
void msg_store_push_message(char* text, u16_t len);

typedef void (*msg_store_listener_cb_t)(char* text, u16_t len);
void msg_store_register_listener(msg_store_listener_cb_t msg_store_listener_cb);