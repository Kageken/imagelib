#include<iostream>
#include<cstdio>
#include<cstring>

// マクロ
#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40

// 構造体宣言
typedef struct FileHeader{  // ファイルヘッダ構造体
	uint8_t data[FILE_HEADER_SIZE];  // 仮データ置き場
	std::string fileType;  // ファイルタイプ
	int fileSize;  // ファイルサイズ
	int reserve1, reserve2;  // 予約領域
	int offset;  // 画像データまでのオフセット
}FileHeader;

typedef struct InfoHeader{  // 情報ヘッダ構造体
	uint8_t data[INFO_HEADER_SIZE];  // 仮データ置き場
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
	
	public:
		BitMapProcessor(){  // コンストラクタ
			bmp = NULL;
			img = NULL;
			org = NULL;
		}

		~BitMapProcessor(){  // デストラクタ
			fclose(bmp);
			delete []img;
			delete []org;
		}

		void loadData(std::string filename);  // ファイルのロード
		void dispBmpInfo();  // 画像情報の表示
		void writeData(std::string filename);  // データの書き込み
		Color getColor(int row, int col);  // 色の取得
		void setColor(int row, int col, Color color);  // 色をセット
		void restore();  // データを編集前へ復元
		int height();  // 画像高を取得
		int width();  // 画像幅を取得

	private:
		void readFileHeader();  // ファイルヘッダを読み込み
		void readInfoHeader();  // 情報ヘッダを読み込み
		void readBmpData();  // 画像データを読み込み
};

// 関数群
inline int bitToInteger(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4){  // 4bit情報を整数値に変換
	return b1 +
		   b2 * 256 +
		   b3 * 256 * 256 +
		   b4 * 256 * 256 * 256;
}

void BitMapProcessor::loadData(std::string filename){  // ファイル読み込み
	if(bmp != NULL){
		fclose(bmp);
	}
	bmp = fopen(filename.c_str(), "rb");
	if(bmp == NULL){
		std::cout << "loadData() : ファイルオープンに失敗しちゃった" << std::endl;
	}
	readFileHeader();
	readInfoHeader();
	readBmpData();
}

void BitMapProcessor::dispBmpInfo(){  // 画像情報の表示
	std::cout << "* File Header Info *" << std::endl;
	std::cout << "File Type : " << fHeader.fileType << std::endl;
	std::cout << "File Size : " << fHeader.fileSize << std::endl;

	std::cout << "* Info Header Info *" << std::endl;
	std::cout << "Info Header Size : " << iHeader.infoHeaderSize << std::endl;
	std::cout << "Width : " << iHeader.width << std::endl;
	std::cout << "Height : " << iHeader.height << std::endl;
	std::cout << "Color / Pixel : " << iHeader.clrPerPix << std::endl;
	std::cout << "Image Data Size : " << iHeader.dataSize << std::endl;
}

void BitMapProcessor::writeData(std::string filename){  // ファイル書き出し
	FILE *out = fopen(filename.c_str(), "wb");

	if(out == NULL){
		std::cout << "writeData() : 書き出し先のファイルが開けないよ" << std::endl;
	}

	fwrite(fHeader.data, sizeof(uint8_t), FILE_HEADER_SIZE, out);
	fwrite(iHeader.data, sizeof(uint8_t), INFO_HEADER_SIZE, out);
	fwrite(img, sizeof(uint8_t), iHeader.dataSize, out);

	fclose(out);
}

Color BitMapProcessor::getColor(int row, int col){  // 色の取得
	if(row < 0 || row >= iHeader.height){
		std::cout << "getColor() : rowが範囲外" << std::endl;
	}
	if(col < 0 || col >= iHeader.width){
		std::cout << "getColor() : colが範囲外" << std::endl;
	}

	int width = 3 * iHeader.width;
	while(width % 4){
		++width;
	}

	int bPos = row * width + 3 * col;
	int gPos = bPos + 1;
	int rPos = bPos + 2;

	Color color;
	color.r = img[rPos];
	color.g = img[gPos];
	color.b = img[bPos];

	return color;
}

void BitMapProcessor::setColor(int row, int col, Color color){  // 色を設定
	if(row < 0 || row >= iHeader.height){
		std::cout << "setColor() : rowが範囲外" << std::endl;
	}
	if(col < 0 || col >= iHeader.width){
		std::cout << "setColor() : colが範囲外" << std::endl;
	}

	int width = 3 * iHeader.width;
	while(width % 4){
		++width;
	}

	int bPos = row * width + 3 * col;
	int gPos = bPos + 1;
	int rPos = bPos + 2;

	img[rPos] = color.r;
	img[gPos] = color.g;
	img[bPos] = color.b;
}

void BitMapProcessor::restore(){  // データを加工前に
	memcpy(img, org, iHeader.dataSize);
}

void BitMapProcessor::readFileHeader(){  // ファイルヘッダ読み込み
	uint8_t data[FILE_HEADER_SIZE];
	size_t size = fread(data, sizeof(uint8_t), FILE_HEADER_SIZE, bmp);

	memcpy(fHeader.data, data, sizeof(data));
	fHeader.fileType = "";
	fHeader.fileType += data[0];
	fHeader.fileType += data[1];
	fHeader.fileSize = bitToInteger(data[2], data[3], data[4], data[5]);
	fHeader.reserve1 = bitToInteger(data[6], data[7], 0, 0);
	fHeader.reserve2 = bitToInteger(data[8], data[9], 0, 0);
	fHeader.offset = bitToInteger(data[10], data[11], data[12], data[13]);
}

void BitMapProcessor::readInfoHeader(){  // 情報ヘッダ読み込み
	uint8_t data[INFO_HEADER_SIZE];
	size_t size = fread(data, sizeof(uint8_t), INFO_HEADER_SIZE, bmp);

	memcpy(iHeader.data, data, sizeof(data));
	iHeader.infoHeaderSize = bitToInteger(data[0], data[1], data[2], data[3]);
	iHeader.width = bitToInteger(data[4], data[5], data[6], data[7]);
	iHeader.height = bitToInteger(data[8], data[9], data[10], data[11]);
	iHeader.plain = bitToInteger(data[12], data[13], 0, 0);
	iHeader.clrPerPix = bitToInteger(data[14], data[15], 0, 0);
	iHeader.compress = bitToInteger(data[16], data[17], data[18], data[19]);
	iHeader.dataSize = bitToInteger(data[20], data[21], data[22], data[23]);
	iHeader.rowResol = bitToInteger(data[24], data[25], data[26], data[27]);
	iHeader.colResol = bitToInteger(data[28], data[29], data[30], data[31]);
	iHeader.clrPallete = bitToInteger(data[32], data[33], data[34], data[35]);
	iHeader.pallete = bitToInteger(data[36], data[37], data[38], data[39]);
}

void BitMapProcessor::readBmpData(){  // 画像データ読み込み
	if(img != NULL){
		delete []img;
	}
	int sz = iHeader.dataSize;
	img = new uint8_t [sz];
	size_t size = fread(img, sizeof(uint8_t), sz, bmp);
	if(size != sz){
		std::cout << "readBmpData() : 画像データの読み込みサイズが矛盾しちょる" << std::endl;
	}

	if(org != NULL){
		delete []org;
	}
	org = new uint8_t [sz];
	memcpy(org, img, sz);
}

void testMess(void){  // テスト用
	std::cout << "<<this is bmp.hpp>>" << std::endl;
}