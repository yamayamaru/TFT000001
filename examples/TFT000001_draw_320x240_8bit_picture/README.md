# TFT000001_draw_8bit_picture

TFT000001.h、TFT000001.cpp、TFT000001_config.h、pio_8bit_parallel.pio、  
TFT000001_draw_320x240_8bit_picture.cpp、CMakeLists.txtを同じフォルダに置き  
pico-sdkでビルドしてください。
ビルド前にTFT000001_config.hを編集して8BIT PARALLELかSPIか、  
ILI9486かILI9341かの設定を必ずしてください。  

https://github.com/yamayamaru/TFT000001/blob/main/img/raspberrypipicopinout.jpg

ソースコードに書かれてるTFT_MOSI、TFT_SCK、TFT_CS、TFT_MISOやTFT_RST、TFT_DCの番号はGPIOの番号です  
上記画像で緑で書かれてる番号になります。    
SPIでのソースコードの例では実際のピン番号は下記のようになります。  
#define TFT_MOSI 19    // ピン番号25  
#define TFT_SCK  18    // ピン番号24  
#define TFT_CS   17    // ピン番号22  
#define TFT_MISO 16    // ピン番号21  
#define TFT_RST  22    // ピン番号29  
#define TFT_DC   20    // ピン番号26  

画像からGP19、GP18、GP17、GP16のSPIはSPI0なので  
ソースコードの例ではSPIのポート番号は下記のようにspi0を指定してます  
#define TFT_SPI_PORT  spi0  
  
また、aitendoのM035C9486LB3の場合のLCD側のSCKはWRになります。  
(データシートで確認した所、ILI9486ではSCKとWRが共通のピンでした)  
DCピンはRSになります。  
  

8BIT_PARALELLでのソースコードの例では実際のピン番号は下記のようになります。
この他にLCD側のRDピンはVCCにCSピンはGNDにつないでください。  
この他にLCD側のRDピンはVCCにCSピンはGNDにつないでください。  
(8BIT_PARALELLではTFT_D0からTFT_D7は必ず連番のGPIOの番号を指定してください)  
  #define TFT_DC   14    // ピン番号19  
  #define TFT_RST  15    // ピン番号20  
  #define TFT_WR   22    // ピン番号29  

  #define TFT_D0    6    // ピン番号9  
  #define TFT_D1    7    // ピン番号10  
  #define TFT_D2    8    // ピン番号11  
  #define TFT_D3    9    // ピン番号12  
  #define TFT_D4   10    // ピン番号14  
  #define TFT_D5   11    // ピン番号15  
  #define TFT_D6   12    // ピン番号16  
  #define TFT_D7   13    // ピン番号17  
