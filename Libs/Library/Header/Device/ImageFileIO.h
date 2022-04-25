#ifndef IMAGEFILEIO_H
#define IMAGEFILEIO_H
#include<string>
#include"ButiFont/ButiFont.h"
namespace ButiEngine {
namespace ImageFileIO {
struct TextureResourceData:public IObject {
	~TextureResourceData();
	std::int32_t width=0;
	std::int32_t height=0;
	std::int32_t pixSize=0;
	std::int32_t rowPitch = 0;
	std::int32_t slicePitch =0;
	unsigned char* rawData=nullptr;
	char loadType=2;//0=stb 1= ButiFont 2=other 
};
struct FontResourceData :public IObject {
	~FontResourceData();
	Value_ptr<TextureResourceData> vlp_texResData;
	ButiFont::IFontInformation* p_fontInfo;
};


extern void OutputPNG(const std::string& arg_filePath,const void* arg_data, const std::int32_t width, const std::int32_t height, const std::int32_t stride, const std::int32_t size);
extern void OutputJPG(const std::string& arg_filePath,const void* arg_data, const std::int32_t width, const std::int32_t height, const std::int32_t stride, const std::int32_t size);
extern void OutputBMP(const std::string& arg_filePath,const void* arg_data, const std::int32_t width, const std::int32_t height, const std::int32_t stride, const std::int32_t size);

extern Value_ptr<TextureResourceData >InputPNG(const std::string& arg_filePath);
extern Value_ptr<FontResourceData >InputTTF(const std::string& arg_filePath, const std::int32_t size,ButiFont::FontLanguage arg_lang);


}
}
#endif // !IMAGEFILEIO_H

