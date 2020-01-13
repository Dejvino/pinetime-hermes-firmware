#include "GFX.h"
#include "Adafruit-GFX-Library/Adafruit_GFX.h"

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
	u8_t b[2];
	b[0] = (color >> 8);
	b[1] = (color >> 0);
	display_write(display_dev, x, y, &this->pixelBufferDesc, &b);
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
	this->buffer = new u8_t[w*h*color_size];
	this->bx = x;
	this->by = y;
	this->bw = w;
	this->bh = h;
}

void GFX::flushBuffer() {
	write_buffer(display_dev, this->bx, this->by, this->bw, this->bh,
		this->buffer);
	delete this->buffer;
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


