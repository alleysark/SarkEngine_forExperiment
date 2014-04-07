#include <fstream>
#include <GL/glew.h>
#include "BMPResource.h"
#include "Debug.h"

namespace sark{

	BMPResource::BMPResource()
		: mWidth(0), mHeight(0), mPixels(NULL)
	{}
	BMPResource::BMPResource(const BMPResource& bmp)
		: mWidth(bmp.mWidth), mHeight(bmp.mHeight)
	{
		integer size = bmp.mWidth * bmp.mHeight * 3;
		mPixels = new uint8[size];
		memcpy(mPixels, bmp.mPixels, size);
	}

	BMPResource::~BMPResource(){
		if (mPixels != NULL){
			delete[] mPixels;
		}
	}

	const integer BMPResource::GetWidth() const{
		return mWidth;
	}

	const integer BMPResource::GetHeight() const{
		return mHeight;
	}

	const integer BMPResource::GetDepth() const{
		return 0;
	}

	ITextureResource::Format BMPResource::GetPixelFormat() const{
		return ITextureResource::Format::RGB;
	}

	ITextureResource::PixelType BMPResource::GetPixelType() const{
		return ITextureResource::PixelType::UINT8;
	}

	const void* BMPResource::GetPixels() const{
		return mPixels;
	}

	BMPResource* BMPResource::LoadImp(const std::string& name){
		BITMAPFILEHEADER bmpFileHead;
		BITMAPINFOHEADER bmpInfoHead;
		memset(&bmpFileHead, 0, sizeof(BITMAPFILEHEADER));
		memset(&bmpInfoHead, 0, sizeof(BITMAPINFOHEADER));

		std::ifstream stream;
		stream.open(name, std::ios::binary | std::ios::beg);
		if (!stream.is_open()){
			LogWarn(name + " would not open");
			return NULL;
		}

		stream.read((char*)&bmpFileHead, sizeof(BITMAPFILEHEADER));
		if (((char*)&bmpFileHead)[0] != 'B' || ((char*)&bmpFileHead)[1] != 'M'){
			LogWarn(name + " has invalid bitmap magic-code");
			return NULL;
		}

		stream.read((char*)&bmpInfoHead, sizeof(BITMAPINFOHEADER));
		if (bmpInfoHead.biSize != 40){
			LogWarn("it only support BITMAPINFOHEADER format bitmap");
			return NULL;
		}

		if (bmpInfoHead.biBitCount != 24 && bmpInfoHead.biBitCount != 32){
			LogWarn(name + " has unsupported bit-count")
				return NULL;
		}

		BMPResource* bmp = new BMPResource();
		bmp->mWidth = bmpInfoHead.biWidth;
		bmp->mHeight = bmpInfoHead.biHeight;
		bmp->mPixels = new uint8[bmp->mWidth * bmp->mHeight * 3];

		stream.seekg(bmpFileHead.bfOffBits, std::ios::beg);
		if (bmpInfoHead.biBitCount == 24){
			// 24bit bmp has dummy pixels in each rows
			integer dumbyte = ((bmp->mWidth) % 4);
			dumbyte = (dumbyte == 0 ? 0 : (4 - dumbyte) * 3);

			int64 i, k;
			for (i = 0; i < bmp->mHeight; i++){
				for (k = 0; k < bmp->mWidth; k++){
					// read bgr order
					int64 pix_idx = i*bmp->mWidth * 3 + k*3;
					stream.read((char*)&bmp->mPixels[pix_idx], 3);

					uint8 t = bmp->mPixels[pix_idx];
					bmp->mPixels[pix_idx] = bmp->mPixels[pix_idx + 2];
					bmp->mPixels[pix_idx + 2] = t;
				}
				stream.seekg(dumbyte, std::ios::cur);
			}
		}
		else if (bmpInfoHead.biBitCount == 32){
			uint8 rgba[4];
			int64 i, k;
			for (i = 0; i < bmp->mHeight; i++){
				for (k = 0; k < bmp->mWidth; k++){
					// read rgba order
					stream.read((char*)rgba, 4);

					int64 pix_idx = i*bmp->mWidth * 3 + k * 3;
					bmp->mPixels[pix_idx] = rgba[0];
					bmp->mPixels[pix_idx+1] = rgba[1];
					bmp->mPixels[pix_idx+2] = rgba[2];
					// alpha value is discarded
				}
			}
		}
		return bmp;
	}

}