void storage_init();
void storage_loop();
int storage_read(int address, char* buffer, size_t size);
int storage_write(int address, char* buffer, size_t size);
void storage_delete(int address);
