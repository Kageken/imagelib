#include<iostream>
#include"bmp.hpp"

void twoTone(BitMapProcessor *bmp);

int main(void){
	std::cout << "<<This is sample_bmp.cpp>>" << std::endl;

	testMess();

	BitMapProcessor bmp;

	bmp.loadData("../images/yukina.bmp");
	bmp.dispBmpInfo();

	bmp.writeData("../images/yukina_copy.bmp");

	twoTone(&bmp);
	bmp.writeData("../images/yukina_2tone.bmp");
	bmp.restore();

	return 0;
}

void twoTone(BitMapProcessor *bmp){
	for(int i = 0; i < bmp->height(); i++){
		for(int j = 0; j < bmp->width(); j++){
			int ave = 0;
			ave += bmp->getColor(i, j).r;
			ave += bmp->getColor(i, j).g;
			ave += bmp->getColor(i, j).b;
			ave /= 3;
			Color color;
			color.r = ave;
			color.g = ave;
			color.b = ave;
			bmp->setColor(i, j, color);
		}
	}
}