#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>

// マクロ
#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40

// 構造体宣言
typedef struct FileHeader{
	std::vector<uint8_t> data;  // 仮データ置き場
	string ftype;  // ファイルタイプ
	int fsize;  // ファイルサイズ
	int reserve1, reserve2;  // 予約領域
	int offset;  // 画像データまでのオフセット
}FileHeader;

typedef struct InfoHeader{
	std::vector<uint8_t> data;  // 仮データ置き場
}

// 関数宣言群
void testMess(void);

// 関数群
void testMess(void){
	std::cout << "<<this is bmp.hpp>>" << std::endl;
}