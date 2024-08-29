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

void mexican_hat05();

void loop() {
    mexican_hat05();

    sleep_ms(5000);
}


double d[160];
char col2r[161];
char col2g[161];
char col2b[161];

void mexican_hat05() {
    int       i, x1, y1, zz, px, py, f, sxi;
    double    dr, r, z, sx, sy, x, y;
    int       xmax, ymax;

    xmax = tft.width();
    ymax = tft.height();

    tft.fillRect(0, 0, tft.width(), tft.height(), tft.color(TFT000001_24BIT_COLOR_BLACK));

    for (i = 0; i <= 160; i++) {
        col2r[i] = 0;
        col2g[i] = 0;
        col2b[i] = 0;
        if (i <= 31) {
            col2b[i] = i;
        }
        if ((i > 32) && (i <= 63)) {
            col2b[i] = (63 - i);
            col2r[i] = (i -33);
        }
        if (i == 64) {
            col2r[i] = (i - 33);
        }
        if ((i > 64) && (i <= 95)) {
            col2r[i] = (95 - i);
            col2g[i] = (i - 65);
        }
        if (i == 96) {
            col2g[i] = (i - 65);
        }
        if ((i > 96) && (i <= 127)) {
            col2g[i] = ((127 - i) + (i - 97));
            col2b[i] = (i - 97);
        }
        if (i == 128) {
            col2g[i] = (i - 97);
            col2b[i] = (i - 97);
        }
        if ((i > 128) && (i <= 159)) {
            col2g[i] = ((159 - i) + (i - 129));
            col2b[i] = ((159 - i) + (i - 129));
            col2r[i] = (i - 129);
        }
        if (i >= 160) {
            col2g[i] = 31;
            col2b[i] = 31;
            col2r[i] = 31;
        }
    }


    for (i = 0; i < 160; ++i) {
        d[i] = 100.0;
    }

    dr = 3.141592653589793 / 180.0;
    for (y1 = -1800; y1 <= 1800; y1 += 10) {
        y = y1 / 10.0;
        for (x1 = -1800; x1 < 1800; x1 += 2) {
            x = x1 / 10.0;
            r = dr * sqrt((double)x * (double)x + (double)y * (double)y);
            z = 100 * cos(r) - 30 * cos(3*r);
            sx = 80 + x / 3 - y / 6;
            sy = 40 - y / 6 - z / 4;
            sxi = (int)sx;
            if ((sxi < 0) || (sxi >= 160)) continue;
            if (d[sxi] <= sy) continue;

            zz = (int)((z + 100) * 0.035 / 6 * 159) + 1;

            if (zz > 160) zz = 160;
            px = (int)(sx * 2.0 * xmax / 320.0);
            py = (int)(sy * 2.0 * ymax / 200.0);

            tft.drawPixel(px, py, tft.color((col2r[zz] << 3), (col2g[zz] << 3), (col2b[zz] << 3)));

            d[sxi] = sy;
        }
    }
}
