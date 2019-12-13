#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "hilbert_gen.h"

typedef struct {
    uint8_t* data;
    int32_t n_cols;
    int32_t n_rows;
} BinaryImage;

static inline void bimg_set_pixel(BinaryImage img, int32_t row, int32_t col) {
    uint32_t k = row * img.n_cols + col;
    img.data[k >> 3] |= 1 << (k & 0x7);
}

int main(int argc, char* argv[]) {
    uint8_t curve_order = 10;
    // minimal command parsing
    printf("first parameter argument specifies order of the curve to generate\n");
    if (argc > 1) {
        int input = atoi(argv[1]);
        if (input > 0 && input < 14) {
            curve_order = input;
        }
    }

    printf("selected hilbert curve of order %d, generating ...\n", curve_order);
    // create an empty buffer to store the curve
    uint32_t curve_length = hilbert_curve_length(curve_order);
    uint8_t* curve = malloc(curve_length / 4);
    assert(curve);
    printf("allocated size %dB buffer for the curve\n", curve_length / 4);

    // generate the hilbert curve
    hilbert_curve_generate(curve, curve_order);

    // create a binary image to draw the curve
    uint32_t bimg_width = 1 << (curve_order + 1);
    BinaryImage bimg = {calloc(bimg_width * bimg_width / 8, 1), bimg_width, bimg_width};
    assert(bimg.data);
    printf("allocated size %dB buffer for the image\n", bimg_width * bimg_width / 8);

    int32_t last_x = 0;
    int32_t last_y = 0;
    // follow the curve
    HILBERT_CURVE_FOR_EACH_XY(curve, curve_order) {
        // draw draw in binary image
        bimg_set_pixel(bimg, y + last_y, x + last_x);
        bimg_set_pixel(bimg, 2 * y, 2 * x);
        last_x = x;
        last_y = y;
    }

    FILE* fp = fopen("hilbert.pbm", "w+");
    fprintf(fp, "P4\n%d %d\n", bimg.n_cols, bimg.n_rows);
    fwrite(bimg.data, 1, bimg.n_cols * bimg.n_rows / 8, fp);
    fclose(fp);

    free(curve);
    free(bimg.data);
    return 0;
}
