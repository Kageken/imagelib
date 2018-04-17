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
	std::vector<uint8_t> img;  // 作業用ビットマップデータ
	std::vector<uint8_t> org;  // 復元用ビットマップデータ
	FileHeader fHeader;  // ファイルヘッダ
	InfoHeader iHeader;  // 情報ヘッダ
	
	public:
		BitMapProcessor(){  // コンストラクタ
			bmp = NULL;
			img.clear();
			img.shrink_to_fit();
			org.clear();
			org.shrink_to_fit();
		}

		~BitMapProcessor(){  // デストラクタ
			fclose(bmp);
			img.clear();
			img.shrink_to_fit();
			org.clear();
			org.shrink_to_fit();
		}

		void loadData(string filename);  // ファイルのロード
		void dispBmpInfo();  // 画像情報の表示
		void writeData();  // データの書き込み
		Color getColor(int row, int col);  // 色の取得
		void setColor(int row, int col, Color clr);  // 色をセット
		void restore();  // データを編集前へ復元
		int height();  // 画像高を取得
		int width();  // 画像幅を取得

	private:
		void readFileHeader();  // ファイルヘッダを読み込み
		void readInfoHeader();  // 情報ヘッダを読み込み
		void readBmpData();  // 画像データを読み込み
}

// 関数群
inline int bitToInteger(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4){  // 4bit情報を整数値に変換
	return b1 +
		   b2 * 256 +
		   b3 * 256 * 256 +
		   b4 * 256 * 256 * 256;
}

void BitMapProcessor::loadData(string filename){
	if(bmp != NULL){
		fclose(bmp);
	}
	bmp = fopen(filename.c_str(), "rb");
	if(bmp == NULL){
		std::cout << "ファイルオープンに失敗しました" << std::endl;
	}
	readFileHeader();
	readInfoHeader();
	readBmpData();
}

void testMess(void){  // テスト用
	std::cout << "<<this is bmp.hpp>>" << std::endl;
}