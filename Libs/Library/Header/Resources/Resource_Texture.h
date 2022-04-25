#pragma once
#include"stdafx.h"
namespace ButiEngine {
	class GraphicDevice;


	class Resource_Texture :public IResource_Texture, public GPUResource
	{
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
		virtual void Attach(std::int32_t slot) = 0;
		void SetFilePath(const std::string& arg_textureFilePath); 
		const std::string& GetTexturePath() const override;
		const std::string& GetTagName()const override;
	protected:
		std::string textureFilePath;
	};

}