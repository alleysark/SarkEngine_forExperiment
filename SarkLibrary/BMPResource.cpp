#include <fstream>
#include "BMPResource.h"

namespace sark{

	BMPResource::BMPResource(int32 width, int32 height, uint16 bitCount, IBMPPIXELFORMAT* pPixels){
		mWidth = width;
		mHeight = height;
		mBitCount = bitCount;
		mpPixels = pPixels;
	}

	BMPResource::~BMPResource(){
	}

	void BMPResource::Unload(){
		if (mpPixels != NULL){
			delete[] mpPixels;
			mpPixels = NULL;
		}
	}

	BMPResource* BMPResource::LoadImp(std::string& name){
		BITMAPFILEHEADER bmpFileHead;
		BITMAPINFOHEADER bmpInfoHead;

		std::ifstream stream;
		stream.open(name, std::ios::binary | std::ios::beg);
		if (!stream.is_open())
			return NULL;

		stream.read((char*)&bmpFileHead, sizeof(BITMAPFILEHEADER));
		stream.read((char*)&bmpInfoHead, sizeof(BITMAPINFOHEADER));

		IBMPPIXELFORMAT* pPixels = NULL;
		if (bmpInfoHead.biBitCount == 24){
			pPixels = new BMPPIXELFORMAT24[bmpInfoHead.biWidth * bmpInfoHead.biHeight];

			long width_step = sizeof(BMPPIXELFORMAT24)* bmpInfoHead.biWidth;
			if (width_step % 4 != 0){
				width_step += (4 - width_step % 4);
			}

			for (long i = 0; i < bmpInfoHead.biHeight; i++){
				stream.seekg(bmpFileHead.bfOffBits + width_step*i, std::ios::beg);
				stream.read((char*)&pPixels[bmpInfoHead.biWidth*i], sizeof(BMPPIXELFORMAT24)*bmpInfoHead.biWidth);
			}
		}
		else if (bmpInfoHead.biBitCount == 32){
			pPixels = new BMPPIXELFORMAT32[bmpInfoHead.biWidth * bmpInfoHead.biHeight];

			stream.seekg(bmpFileHead.bfOffBits, std::ios::beg);
			for (long i = 0; i < bmpInfoHead.biHeight; i++){
				stream.read((char*)&pPixels[bmpInfoHead.biWidth*i], sizeof(BMPPIXELFORMAT32)*bmpInfoHead.biWidth);
			}
		}
		else{
			return NULL;
		}

		BMPResource* bmpResc = new BMPResource(bmpInfoHead.biWidth, bmpInfoHead.biHeight, bmpInfoHead.biBitCount, pPixels);
		return bmpResc;
	}

}