#ifndef SIMPLE_GRAPHICS_HPP
#define SIMPLE_GRAPHICS_HPP

#include <cassert>
#include <functional>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cstdint>
#include <io.h>
#include <queue>
#include <vector>

#define HW_GRAPHICS

class SimpleGraphics {
public:
    typedef uint16_t rgba_t;
    
    static constexpr rgba_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return (convert_8bit(b, pixel_bits))
            | (convert_8bit(g, pixel_bits) << pixel_bits / 4)
            | (convert_8bit(r, pixel_bits) << (2 * pixel_bits / 4))
            | (convert_8bit(a, pixel_bits) << (3 * pixel_bits / 4));
    }

    inline void draw_pixel(rgba_t color, unsigned x, unsigned y) {

        rgba_t *addr = m_buffer_base + (y * m_width + x);

        while (!commands.empty()) usleep(1);

        // Ignore invalid writes
        if (addr < m_max_addr)
            IOWR_16DIRECT(addr, 0, color);
    }

    inline rgba_t read_pixel(unsigned x, unsigned y) {
		rgba_t *addr = m_buffer_base + (y * m_width + x);

		// Ignore invalid writes
		if (addr < m_max_addr)
			return IORD_16DIRECT(addr, 0);

		return 0;
	}

    void draw_rect(rgba_t color, unsigned x1, unsigned y1, unsigned x2, unsigned y2);

    void draw_x(rgba_t color, unsigned x, unsigned y, int radius);

    void draw_char(rgba_t color, unsigned x, unsigned y, char c);

    void draw_string(rgba_t color, unsigned x, unsigned y, std::string str);

    void startIRQ();

    SimpleGraphics(char *graphics_base, int ic_id, int irq_id, 
            rgba_t *buffer_base, unsigned int width, unsigned int height) :
        m_graphics_base(graphics_base),
        m_ic_id(ic_id),
        m_irq_id(irq_id),
        m_buffer_base(buffer_base),
        m_max_addr(buffer_base + width * height),
        m_width(width),
        m_height(height),
        m_pending_irq(false)
    { 
        assert(ic_id != -1);
        assert(irq_id != -1);
    }



private:
    char *const m_graphics_base;
    alt_u32 m_ic_id; 
    alt_u32 m_irq_id;
    rgba_t *const m_buffer_base;
    rgba_t *const m_max_addr;
    const unsigned int m_width, m_height;
    volatile bool m_pending_irq;

    typedef struct {
    	unsigned X1;
    	unsigned X2;
    	unsigned Y1;
    	unsigned Y2;
    	rgba_t COLOR;
    	unsigned COMMAND;
    } CMD;
    std::queue<CMD> commands;

    static constexpr unsigned pixel_bits = sizeof(rgba_t) * 8;
    static constexpr uint32_t convert_8bit(uint8_t val, unsigned int pixel_bits) {
        return ((val >> (8 - pixel_bits / 4)) & ((1 << pixel_bits / 4) - 1));
    }

	static void graphics_interrupt(void *graphicsctrl);

	static constexpr unsigned DRAW_PIXEL = 1;
	static constexpr unsigned DRAW_RECT = 2;

	static constexpr unsigned X1 = 0;
	static constexpr unsigned X2 = 4;
	static constexpr unsigned Y1 = 8;
	static constexpr unsigned Y2 = 12;
	static constexpr unsigned COLOUR = 20;
	static constexpr unsigned COMMAND = 24;
	static constexpr unsigned IRQ = 28;
	static constexpr unsigned IRQ_DISABLE = 32;

    static constexpr unsigned queue_max = 64;
};


#endif
