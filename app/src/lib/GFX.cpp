#include "log.h"
#include "GFX.h"
#include "Adafruit-GFX-Library/Adafruit_GFX.h"

#define GFX_MEMPOOL
#ifdef GFX_MEMPOOL
#define GFX_MEMPOOL_MAX 16384
K_MEM_POOL_DEFINE(gfx_pool, 64, GFX_MEMPOOL_MAX, 1, 4);
#endif

#define color_size 2

inline void write_buffer(device* display_dev, int16_t x, int16_t y, int16_t w, int16_t h, u8_t* b) {
  struct display_buffer_descriptor d;
  d.pitch = w;
  d.width = w;
  d.height = h;
  display_write(display_dev, x, y, &d, b);
}

GFX::GFX(struct device* display_dev, int16_t w, int16_t h) : Adafruit_GFX(w,h) {
	this->display_dev = display_dev;
        this->pixelBufferDesc.pitch = 1;
        this->pixelBufferDesc.width = 1;
        this->pixelBufferDesc.height = 1;
};

void GFX::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	if (this->buffer == NULL) {
		u8_t b[2];
		b[0] = (color >> 8);
		b[1] = (color >> 0);
		display_write(display_dev, x, y, &this->pixelBufferDesc, &b);
	} else {
		this->writePixel(x, y, color);
	}
}

void GFX::startWrite() {}

void GFX::writePixel(int16_t x, int16_t y, uint16_t color) {
	if (this->buffer == NULL) {
		drawPixel(x, y, color);
	} else {
		if (x < this->bx || x >= this->bx + this->bw
			|| y < this->by || y >= this->by + this->bh) {
			return; // out of buffer bounds
		}
		
		int i = x - this->bx + (y - this->by) * this->bw;
		this->buffer[i * color_size + 0] = color >> 8;
		this->buffer[i * color_size + 1] = color >> 0;
		// waiting to flush the buffer
	}
}

void GFX::writeFastVLine(int16_t x, int16_t y, int16_t h,
                                  uint16_t color) {
  writeFillRect(x, y, 1, h, color);
}

void GFX::writeFastHLine(int16_t x, int16_t y, int16_t w,
                                  uint16_t color) {
  writeFillRect(x, y, w, 1, color);
}

void GFX::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                                 uint16_t color) {
	if (this->buffer == NULL) {
		u8_t b[2 * w * h];
		// TODO: optimize
	  	for (int i = 0; i < w * h; i++) {
    			b[i * 2 + 0] = color >> 8;
	 		b[i * 2 + 1] = color >> 0;
	  	}
  		write_buffer(display_dev, x, y, w, h, b);
	} else {
	  	for (int rx = 0; rx < w; rx++) {
			for (int ry = 0; ry < h; ry++) {
				this->writePixel(x + rx, y + ry, color);
			}
	  	}
		// waiting to flush the buffer
	}
}

void GFX::endWrite() {}

void GFX::openBuffer(int16_t x, int16_t y, int16_t w, int16_t h) {
	size_t bufsize = w*h*color_size;
#ifdef GFX_MEMPOOL
	if (bufsize > GFX_MEMPOOL_MAX) {
		this->buffer = NULL;
		LOG_WRN("Mem pool alloc for %d B is out of bounds.", bufsize);
	} else if (0 != (this->buffer = (u8_t*)k_mem_pool_malloc(&gfx_pool, bufsize))) {
    	memset(this->buffer, 0, bufsize);
		LOG_DBG("Mem pool alloc for %d B allocated.", bufsize);
	} else {
    	LOG_WRN("Mem pool alloc for %d B failed.", bufsize);
	}
#else
	this->buffer = new u8_t[bufsize];
	if (!this->buffer) {
		LOG_WRN("Failed to allocate GFX buffer %d x %d pixels.", w, h);
	}
#endif
	this->bx = x;
	this->by = y;
	this->bw = w;
	this->bh = h;
}

void GFX::flushBuffer() {
	write_buffer(display_dev, this->bx, this->by, this->bw, this->bh,
		this->buffer);
#ifdef GFX_MEMPOOL
	k_free(this->buffer);
#else
	delete[] this->buffer;
#endif
	this->buffer = NULL;
}

/**
 * GFX Factory
 */
GFX* createGFX(struct device* display_dev)
{
	struct display_capabilities capabilities;
	display_get_capabilities(display_dev, &capabilities);

	int sw = capabilities.x_resolution; 
	int sh = capabilities.y_resolution; 

	return new GFX(display_dev, sw, sh);
}


