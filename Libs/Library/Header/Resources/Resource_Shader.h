#pragma once
#include"stdafx.h"
namespace ButiEngine {
	class GraphicDevice;
	class Resource_PixelShader :public IResource_PixelShader
	{
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		virtual void Attach()const = 0;
		virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
		virtual std::vector<std::int32_t>& GetFormats() = 0;
	};
	class Resource_VertexShader :public IResource_VertexShader
	{
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		virtual void Attach()const = 0;
		virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
	};
	class Resource_GeometryShader :public IResource_GeometryShader
	{
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		virtual void Attach()const = 0;
		virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
	};

	class Resource_Shader :public IResource_Shader
	{
	public:
		Resource_Shader(Value_weak_ptr<IResource_VertexShader> arg_wkp_vertexShader, Value_weak_ptr<IResource_PixelShader> arg_wkp_pixelShader, Value_weak_ptr<IResource_GeometryShader> arg_wkp_geometryShader, const std::string& arg_shaderName);
		void Initialize()override {}
		void PreInitialize()override {}
		virtual void Attach()const;

		bool GetIsContainGeometryShader()const;
		void ShowGUI()override;
		std::string GetShaderName()const;

		Value_weak_ptr<IResource_VertexShader> GetVertexShader();
		Value_weak_ptr<IResource_PixelShader> GetPixelShader();
		Value_weak_ptr<IResource_GeometryShader> GetGeometryShader();
	private:
		Value_weak_ptr<IResource_VertexShader> wkp_vertexShader;
		Value_weak_ptr<IResource_PixelShader> wkp_pixelShader;
		Value_weak_ptr<IResource_GeometryShader> wkp_geometryShader;
		std::string shaderName;
	};
}