#include"stdafx.h"
#include"DrawData_Dx12.h"
#include"../../Common/CArrayBuffer_Dx12.h"
namespace ButiEngine {
	struct ModelDrawData_Dx12 :public ModelDrawData,public DrawData_Dx12 {
		ModelDrawData_Dx12(const ModelTag& arg_model, const ShaderTag& arg_shader, Value_ptr<IRenderer> arg_shp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice, Value_ptr< DrawInformation >arg_shp_drawInfo, Value_ptr<Transform>arg_shp_transform, Value_ptr<IBoneObject> arg_shp_bone);
		ModelDrawData_Dx12(const ModelTag& arg_model, const ShaderTag& arg_shader, const std::vector<MaterialTag>& arg_materialTag,Value_ptr<IRenderer> arg_shp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice,  Value_ptr< DrawInformation >arg_shp_drawInfo,Value_ptr<Transform>arg_shp_transform,Value_ptr<IBoneObject> arg_shp_bone);
		
		inline float GetZ(const Matrix4x4& arg_vpMatrix)override {
			return  GetMaxZ( arg_vpMatrix);
		}
		void Initialize()override;

		void PreInitialize()override {};

		void DrawBefore()override;

		void SetTransform(Value_ptr<Transform>& arg_transform)override {
			transform = arg_transform->GetMatrix();
		}

	
		void Draw()override;

		void  ModelUpdate() ;
	};
}