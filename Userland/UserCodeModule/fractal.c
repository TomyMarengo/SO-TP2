#include <stdint.h>
#include <fractal.h>
#include <syscalls.h>
#include <color.h>

#define MAX_ITERATIONS 300

typedef struct {
    double x;
    double y;
} Vec2;

static void CMult(const Vec2* a, const Vec2* b, Vec2* res) {
    res->x = a->x*b->x - a->y*b->y;
    res->y = a->x*b->y + a->y*b->x;
}

static void CSquare(const Vec2* a, Vec2* res) {
    res->x = a->x*a->x - a->y*a->y;
    res->y = a->x*a->y + a->y*a->x;
}

typedef struct {
    uint8_t isFinalized;
    Vec2 v;
} PixelCell;

static void memclr(void* m, uint64_t count) {
    for (; count; count--)
        *((uint8_t*)(m++)) = 0;
}

static Color computeColor(uint64_t iteration) {
    iteration *= 256;
    uint64_t b = iteration / 50;
    uint64_t g = iteration / 150;
    uint64_t r = iteration / 100;
    Color currentColor = {
        ((b > 255) ? 255 : (uint8_t)b),
        ((g > 255) ? 255 : (uint8_t)g),
        ((r > 255) ? 255 : (uint8_t)r)
    };
    return currentColor;
}

static void setInitialState(PixelCell* cells, uint32_t width, uint32_t height, uint64_t pixelcount) {
    const double minX = 0.2513415;
    const double maxX = 0.5031265;
    const double centerY = 0.105178981;
    double pixelDelta = (maxX - minX) / width;
    double minY = centerY - pixelDelta*height/2.0;
    
    do {
        pixelcount--;
        Vec2 v = { minX + pixelDelta*(pixelcount % width), minY + pixelDelta*(pixelcount / width) };
        cells->v = v;
        cells++;
    } while (pixelcount != 0);
}

static const Vec2 c = { 0.2680149, 0.0 };
static const Vec2 b = { 0.1, -0.5 };

static void step(Vec2* v) {
    Vec2 v_squared, b_times_v;
    CSquare(v, &v_squared);
    CMult(&b, v, &b_times_v);
    v->x = v_squared.x + b_times_v.x + c.x;
    v->y = v_squared.y + b_times_v.y + c.y;
}

#define WIDTH 1024
#define HEIGHT 768
#define PIXELCOUNT (WIDTH*HEIGHT)
static PixelCell cells[PIXELCOUNT]; // 12.75MBs of RAM. Ouch.

void frc_run(void) {
    sys_clearscreen();
    sys_writeat("Press ESC at any moment to abort.", 33, 0, 0, white);
    memclr((void*)cells, PIXELCOUNT * sizeof(PixelCell));
    setInitialState(cells, WIDTH, HEIGHT, PIXELCOUNT);

    uint64_t readlen;
    uint8_t readbuf[32];

    // Run simulation and draw fractal.
    for (uint64_t iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        Color currentColor = computeColor(iteration);

        PixelCell* cell = cells;
        for (uint64_t i = 0; i < PIXELCOUNT; i++, cell++) {
            if (!cell->isFinalized) {
                step(&(cell->v));
                cell->isFinalized = (cell->v.x > 2.0 || cell->v.y > 2.0);
                if (cell->isFinalized)
                    sys_drawpoint(i % WIDTH, i / WIDTH, currentColor);
            }
        }

        // Check if should exit
        if ((readlen = sys_pollread(KBDIN, (char*)readbuf, sizeof(readbuf), 0)) != 0) {
            do {
                readlen--;
                if (readbuf[readlen] == (uint8_t)1) { // check for scancode for pressing down the ESC key.
                    iteration = MAX_ITERATIONS; // Stop the for loop.
                    readlen = 0;
                }
            } while (readlen != 0);
        }
    }

    // Show "done" message.
    sys_writeat("Done. Press ESC to exit.", 24, 50, 50, white);

    // Wait until we read the scancode for pressing down the ESC key.
    do {
        sys_read(KBDIN, (char*)readbuf, 1);
    } while (*readbuf != (uint64_t)1); 
}
