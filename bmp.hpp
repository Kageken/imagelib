#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>

// マクロ
#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40

// 構造体宣言
typedef struct FileHeader{  // ファイルヘッダ構造体
	std::vector<uint8_t> data;  // 仮データ置き場
	string fileType;  // ファイルタイプ
	int fileSize;  // ファイルサイズ
	int reserve1, reserve2;  // 予約領域
	int offset;  // 画像データまでのオフセット
}FileHeader;

typedef struct InfoHeader{  // 情報ヘッダ構造体
	std::vector<uint8_t> data;  // 仮データ置き場
	int infoHeaderSize;  // 情報ヘッダサイズ
	int width, height;  // 画像の幅，高さ
	int plain;  // プレーン数
	int clrPerPix;  // 画素あたりの色数
	int compress;  // 圧縮形式
	int dataSize;  // 画像データサイズ
	int rowResol, colResol;  // 水平解像度，垂直解像度
	int clrPallete;  // パレットの色数
	int pallete;  // 重要パレットのインデックス
}InfoHeader;

typedef struct Color{  // 色構造体
	int r, g, b;
}Color;

// 関数宣言群
int bitToInteger(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
void testMess(void);

// クラス定義
class BitMapProcessor{
	FILE *bmp;  // ファイルポインタ
	uint8_t *img;  // 作業用ビットマップデータ
	uint8_t *org;  // 復元用ビットマップデータ
	FileHeader fHeader;  // ファイルヘッダ
	InfoHeader iHeader;  // 情報ヘッダ
	
}

// 関数群
void testMess(void){
	std::cout << "<<this is bmp.hpp>>" << std::endl;
}