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




void print_double(double a);
void print_double_ln(double a);

void mandelbrot(int width, int height);
void mandel(double xcorner, double ycorner, double length, int xdot, int ydot, int depth);
int mand(double a1, double b1, int depth1);
void set_param(void);
struct _param01 {
    double xcorner;
    double ycorner;
    double length;
    int    depth;
} param01[28];

void loop(void) {
    set_param();

    mandelbrot(tft.width(), tft.height());

    sleep_ms(20000);
}

void mandelbrot(int width, int height) {

    double    xcorner, ycorner, length;
    int       xdot, ydot, depth;
    int       i, ii;

    xdot = width;
    ydot = height;

    for (ii = 0; ii < 28; ii++) {
        i = ii % 28;
        mandel(param01[i].xcorner, param01[i].ycorner, param01[i].length, xdot, ydot, param01[i].depth);
    }
}

void mandel(double xcorner, double ycorner, double length, int xdot, int ydot, int depth) {
    double xgap, ygap;
    double xx, yy;
    int    i, j;
    int    col01, colr, colg, colb;

    // xgap = length / xdot;
    // ygap = length / xdot;
    xgap = length / ydot;
    ygap = length / ydot;
    //  x軸を240ドットから320ドットに広げる
    xcorner = xcorner - (xdot - ydot) / 2 * xgap;
    printf("xdot = %d    ydot = %d\r\n", xdot, ydot);
//    printf("xcorner = %.15e\r\n", xcorner);
//    printf("ycorner = %.15e\r\n", ycorner);
//    printf("length  = %.15e\r\n", length);
    printf("xcorner = ");
    print_double_ln(xcorner);
    printf("ycorner = ");
    print_double_ln(ycorner);
    printf("length  = ");
    print_double_ln(length);

    printf("depth   = %d\r\n", depth);
    printf("\r\n");

    xx = xcorner;
    yy = ycorner;
    for(j = 0; j < ydot; j++) {
        for(i = 0; i < xdot; i++) {
          col01 = (short) mand( xx, yy, depth);
            if (col01 == 0){
                colr = 0;
                colg = 0;
                colb = 0;
            } else {
                colr = (col01 % 64) * 2 + 129;
                colg = (col01 % 64) * 4 + 3;
                colb = (col01 % 64) * 4 + 3;
            }
            tft.fillRect(i, j, 2, 1, tft.color(colr, colg, colb));

          xx = xx + xgap;
        }
        yy = yy + ygap;
        xx = xcorner;
    }
}

int mand(double a1, double b1, int depth1) {
    int     i1;
    double  x1, y1, tmp1, x2, y2;

    i1 = 0;
    x1 = 0; y1 = 0;
    x2 = x1 * x1;   y2 = y1 * y1;
    while ((i1 < depth1) && (x2+y2 <=4)) {
        tmp1 = x2 - y2 + a1;
        y1 = 2 * x1 * y1 + b1;
        x1 = tmp1;
        x2 = x1 * x1;  y2 = y1 * y1;
        i1++;
    }

    if (i1 == depth1) i1 = 0;
    else            i1++;
    return i1;
}


void set_param(void) {
    param01[0].xcorner =  -2.4;
    param01[0].ycorner =  -1.7;
    param01[0].length  =   3.2;
    param01[0].depth   =  1000;

    param01[1].xcorner = -6.1099999999999965e-001;
    param01[1].ycorner =  6e-001;
    param01[1].length  =  1.0000000000000001e-001;
    param01[1].depth   =  1000;

    param01[2].xcorner = -7.512120844523107e-001;
    param01[2].ycorner = 2.939359283447132e-002;
    param01[2].length = 2.441406250000000e-005;
    param01[2].depth = 1000;

    param01[3].xcorner = -1.7878664118448890e+000;
    param01[3].ycorner = -4.6994043985999932e-003;
    param01[3].length  =  1.5258789062500001e-006;
    param01[3].depth   =  1000;

    param01[4].xcorner = -1.255058807618605e+000;
    param01[4].ycorner =  3.834129844820908e-001;
    param01[4].length  =  5.960464477539063e-009;
    param01[4].depth   =  1000;

    param01[5].xcorner = -7.424999999999979e-002;
    param01[5].ycorner = -6.523749999999998e-001;
    param01[5].length = 3.125000000000000e-003;
    param01[5].depth   =  1000;

    param01[6].xcorner = -7.366145833333310e-002;
    param01[6].ycorner = -6.500052083333332e-001;
    param01[6].length = 3.125000000000000e-003;
    param01[6].depth   =  1000;

    param01[7].xcorner = -7.451562499999977e-002;
    param01[7].ycorner = -6.500117187500000e-001;
    param01[7].length = 7.812500000000000e-004;
    param01[7].depth   =  1000;

    param01[8].xcorner = -7.409765624999977e-002;
    param01[8].ycorner = -6.494752604166667e-001;
    param01[8].length = 1.953125000000000e-004;
    param01[8].depth   =  1000;

    param01[9].xcorner = -1.408903645833333e+000;
    param01[9].ycorner = -1.342989908854166e-001;
    param01[9].length = 2.441406250000000e-005;
    param01[9].depth   =  1000;

    param01[10].xcorner = -1.023473307291662e-001;
    param01[10].ycorner = 9.571370442708340e-001;
    param01[10].length = 4.882812500000000e-005;
    param01[10].depth   =  1000;

    param01[11].xcorner = -1.023286539713537e-001;
    param01[11].ycorner = 9.571538899739589e-001;
    param01[11].length = 1.220703125000000e-005;
    param01[11].depth   =  1000;

    param01[12].xcorner = -7.424999999999979e-002;
    param01[12].ycorner = -6.523749999999998e-001;
    param01[12].length = 3.125000000000000e-003;
    param01[12].depth   =  1000;

    param01[13].xcorner = -7.366145833333310e-002;
    param01[13].ycorner = -6.500052083333332e-001;
    param01[13].length = 3.125000000000000e-003;
    param01[13].depth   =  1000;

    param01[14].xcorner = -7.451562499999977e-002;
    param01[14].ycorner = -6.500117187500000e-001;
    param01[14].length = 7.812500000000000e-004;
    param01[14].depth   =  1000;

    param01[15].xcorner = -7.409765624999977e-002;
    param01[15].ycorner = -6.494752604166667e-001;
    param01[15].length = 1.953125000000000e-004;
    param01[15].depth   =  1000;

    param01[16].xcorner = -1.408903645833333e+000;
    param01[16].ycorner = -1.342989908854166e-001;
    param01[16].length = 2.441406250000000e-005;
    param01[16].depth   =  1000;

    param01[17].xcorner = -1.408903645833333e+000;
    param01[17].ycorner = -1.342989908854166e-001;
    param01[17].length = 2.441406250000000e-005;
    param01[17].depth   =  1000;

    param01[18].xcorner = -1.408886164585749e+000;
    param01[18].ycorner = -1.342802622318267e-001;
    param01[18].length = 1.192092895507813e-008;
    param01[18].depth   =  1000;

    param01[19].xcorner = -1.408886160294215e+000;
    param01[19].ycorner = -1.342802576224008e-001;
    param01[19].length = 2.980232238769531e-009;
    param01[19].depth   =  1000;

    param01[20].xcorner = -1.023473307291662e-001;
    param01[20].ycorner = 9.571370442708340e-001;
    param01[20].length = 4.882812500000000e-005;
    param01[20].depth   =  1000;

    param01[21].xcorner = -1.023286539713537e-001;
    param01[21].ycorner = 9.571538899739589e-001;
    param01[21].length = 1.220703125000000e-005;
    param01[21].depth   =  1000;

    param01[22].xcorner = -1.165292968750000e+000;
    param01[22].ycorner = 2.393867187500003e-001;
    param01[22].length = 3.906250000000000e-004;
    param01[22].depth   =  1000;

    param01[23].xcorner = -1.164973597208659e+000;
    param01[23].ycorner = 2.394546101888024e-001;
    param01[23].length = 1.525878906250000e-006;
    param01[23].depth   =  1000;

    param01[24].xcorner = -6.703997395833329e-001;
    param01[24].ycorner = -4.582591145833326e-001;
    param01[24].length = 3.906250000000000e-004;
    param01[24].depth   =  1000;

    param01[25].xcorner = -6.702213948567705e-001;
    param01[25].ycorner = -4.580732421874992e-001;
    param01[25].length = 2.441406250000000e-005;
    param01[25].depth   =  1000;

    param01[26].xcorner = -6.702324136098221e-001;
    param01[26].ycorner = -4.580734767913810e-001;
    param01[26].length = 1.907348632812500e-007;
    param01[26].depth   =  1000;

    param01[27].xcorner = -6.702323307991023e-001;
    param01[27].ycorner = -4.580733914375297e-001;
    param01[27].length = 2.384185791015625e-008;
    param01[27].depth   =  1000;
}

void print_double(double a) {
    double exp01, exp02, a01, b01, c01, c_exp01, d01;
    int sign01 = 0;
    int digits01 = 15 - 1;

    a01 = a;
    if (a01 < 0) {
        sign01 = 1;
        a01 = -a01;
    }

    if (a01 > 5.0e-324) {
        b01 = log(a01)/log(10.0);
        c_exp01 = floor(b01);
        c01 = pow(10.0, (b01 - c_exp01));
    } else {
        c01 = 0.0;
        c_exp01 = 0.0;
    }

    c01 = c01 + 0.5 * pow(10.0, (double)(-digits01));

    if (sign01) printf("-");
    d01 = floor(c01);
    c01 = c01 - d01;
    printf("%1d.", (int)d01);
    for (int i = 0; i < digits01; i++) {
        c01 = c01 * 10.0;
        d01 = floor(c01);
        c01 = c01 - d01;
        printf("%1d", (int)d01);
    }
    if (c_exp01 >= 0) {
        printf("e+");
    } else {
        printf("e-");
        c_exp01 = -c_exp01;
    }

    printf("%03d", (int)c_exp01);
}

void print_double_ln(double a) {
    print_double(a);
    printf("\r\n");
}
