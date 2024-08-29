#include "pico/stdlib.h"
#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "TFT000001.h"
#include <math.h>

#ifndef TFT000001_PIO_8BIT_PARALELL
  #define TFT_MOSI 19
  #define TFT_SCK  18
  #define TFT_CS   17
  #define TFT_MISO 16
  #define TFT_SPI_PORT spi0

  #define TFT_RST  22
  #define TFT_DC   20
#endif



#ifdef TFT000001_PIO_8BIT_PARALELL
  #define TFT_DC   14
  #define TFT_RST  15
  #define TFT_WR   22

  #define TFT_D0    6
  #define TFT_D1    7
  #define TFT_D2    8
  #define TFT_D3    9
  #define TFT_D4   10
  #define TFT_D5   11
  #define TFT_D6   12
  #define TFT_D7   13
#endif



#ifdef TFT000001_PIO_8BIT_PARALELL
  #include "hardware/pio.h"
  #include "pio_8bit_parallel.pio.h"
  TFT000001 tft = TFT000001(TFT_D0, TFT_WR, TFT_DC, TFT_RST, -1);
#else
  #include "hardware/spi.h"
  TFT000001 tft = TFT000001(TFT_SPI_PORT, TFT_DC, TFT_RST, TFT_CS);
#endif


void setup();
void loop();
int main() {
    stdio_init_all();

    setup();

    while (1) {
        loop();
    }

    return 0;
}

void setup(){


#ifdef TFT000001_PIO_8BIT_PARALELL
    tft.INIT_TFT_DATA_BUS();
#endif



#ifndef TFT000001_PIO_8BIT_PARALELL
    spi_init(TFT_SPI_PORT, 40000000);
    gpio_set_function(TFT_MISO, GPIO_FUNC_SPI);
    gpio_set_function(TFT_SCK, GPIO_FUNC_SPI);
    gpio_set_function(TFT_MOSI, GPIO_FUNC_SPI);
    gpio_pull_up(TFT_MISO);
    gpio_pull_up(TFT_SCK);
    gpio_pull_up(TFT_MOSI);
    spi_set_format(TFT_SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);   // SPI MODE0, MSBFIRST
#endif

    sleep_ms(100); 
    printf("\nReset:");


    tft.init(1);  // ILI9486の場合1なら18bitカラー、0なら16bitカラー
                     // ILI9341は16bitカラーのみ

    tft.set_rotation(3);
    tft.fillRect(0, 0, tft.width(), tft.height(), tft.color(TFT000001_24BIT_COLOR_BLACK));
}

void mexican_hat02();

void loop() {
    mexican_hat02();

    sleep_ms(5000);
}


double d[160];
int color01[8];

void mexican_hat02() {
    int       i, j, x, y, zz, px, py, f, col, sxi;
    double    dr, r, z, sx, sy;
    int       xmax, ymax;

    xmax = tft.width();
    ymax = tft.height();

    color01[0] = TFT000001_24BIT_COLOR_BLACK;
    color01[1] = TFT000001_24BIT_COLOR_BLUE;
    color01[2] = TFT000001_24BIT_COLOR_GREEN;
    color01[3] = TFT000001_24BIT_COLOR_CYAN;
    color01[4] = TFT000001_24BIT_COLOR_RED;
    color01[5] = TFT000001_24BIT_COLOR_MAGENTA;
    color01[6] = TFT000001_24BIT_COLOR_YELLOW;
    color01[7] = TFT000001_24BIT_COLOR_WHITE;

    tft.fillRect(0, 0, tft.width(), tft.height(), tft.color(TFT000001_24BIT_COLOR_BLACK));

    for (i = 0; i < 160; ++i) {
        d[i] = 100.0;
    }
    dr = 3.141592653589793 / 180.0;
    for (y = -180; y <= 180; y += 6) {
        for (x = -180; x < 180; x += 4) {
            r = dr * sqrt((double)x * (double)x + (double)y * (double)y);
            z = 100 * cos(r) - 30 * cos(3*r);
            sx = 80 + x / 3 - y / 6;
            sy = 40 - y / 6 - z / 4;
            sxi = (int)sx;
            if ((sxi < 0) || (sxi >= 160)) continue;
            if (d[sxi] <= sy) continue;
            zz = (int)((z + 100) * 0.035) + 1;
            col = 0;
            f = 0;
            if ((zz == 1) || (zz == 3) || (zz == 5) || (zz == 7)) {
                col = col | 1;
                f = 1;
            }
            if ((zz == 2) || (zz == 3) || (zz >= 6)) {
                col = col | 4;
                f = 1;
            }
            if ((zz >= 4)) {
                col = col | 2;
                f = 1;
            }

            px = (int)(sx * 2.0 * xmax / 320.0);
            py = (int)(sy * 2.0 * ymax / 200.0);
            if (f == 1) {
                tft.fillRect(px, py, 3, 3, tft.color(color01[col]));
            }
            d[sxi] = sy;
        }
    }
}
