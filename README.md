# TFT000001

TFT000001はRaspberry Pi PicoのC/C++開発環境のpico-sdk用のグラフィックスライブラリです。  
プログラム全体がC++で書かれていて、C++用のライブラリになってます。  
TFT000001はILI9486とILI9341搭載のLCDに対応しています。  
  
文字表示するには別途print_TFT000001も使ってください。  
https://github.com/yamayamaru/print_TFT000001  

ビルドする前に必ず、TFT000001_config.hを編集して8BIT PARALLELなのかSPIなのか  
ILI9486なのかILI9341なのかの設定を行ってください。  

TFT000001はAdafruit-GFX-LibraryとHumblesoft_ILI9341とTFT_eSPIを元に作られています  
    <https://github.com/adafruit/Adafruit-GFX-Library>  
    <https://github.com/h-nari/Humblesoft_ILI9341>  
    <https://github.com/Bodmer/TFT_eSPI>  

まだ、試作段階なのでご了承ください。

ILI9486のLCDはaitendoのM035C9486LB3で8bit PARALLEL、SPIともに動作確認してます。  
ILI9486で8bit PARALLELの場合、16bitカラー、18bitカラー両方に対応してます。  
ILI9486でSPI接続の場合、18bitカラーのみになります。  
ILI9486はSPI接続でも動作しますが、お勧めは8bit PARALLELです。  
aitendoのM035C9486LB3はデフォルトの半田のジャンパー設定が8bit PARALLELなので尚更8bit PARALLELがオススメです。  
ILI9341のLCDはSPIのみの対応です。  
ILI9341は16bitカラーのみです。  

## 変更履歴
* 0.0.1          2022/03/21 初版アップ  
* 0.0.2          2022/06/22 yama_2_GFX_H_beta::color(), yama_2_GFX_H_beta::colorRGB24()を修正  
                            TFT00000::setRotation()の追加  
