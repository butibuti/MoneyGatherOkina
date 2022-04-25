#pragma once
#include"../ButiMath/ButiMath.h"
namespace ButiEngine {
	class IResource_Texture;
	using TextureTag = ID<IResource_Texture>;
	struct GPUResource {
		virtual void  ResourceUpdate() {}
		virtual void  UpdateResourceRelease() {}
		virtual void  CopyForOutput() {}
		virtual void  DataLock() {}
		virtual void  DataUnlock() {}
		virtual void  OutputToMemory() {}
		virtual void Swap(){}
	};
	enum class CullMode {
		none =1, front = 2, back =3
	}; 

	enum class BlendMode {
		AlphaBlend = 0, Addition = 1, Subtruction = 2, Reverse = 3, NoBlend = 4
	};
	enum class BillBoardMode {
		none = 0, full = 1, x = 2, y = 3, z = 4,  
	};
	enum class DrawFixParam {
		none,removeRotation , removeDecimalPart, removeDecimalPartXY , removeDecimalPartYZ , removeDecimalPartXZ 
	};
	enum class TopologyType {
		triangleList = 4, point = 1,  line = 2, triangle = 3, pointList = 1,
	};
	enum class FillMode {
		solid= 3,wire= 2
	};

	struct DrawSettings
	{
		DrawSettings() {}
		DrawSettings(CullMode arg_cullMode) :cullMode(arg_cullMode) {}
		DrawSettings(CullMode arg_cullMode, bool arg_isFill) :cullMode(arg_cullMode) {
			if (!arg_isFill) {
				isFill = FillMode::wire;
			}
		}
		DrawSettings(BlendMode arg_blendMode) :blendMode(arg_blendMode) {}
		DrawSettings(bool arg_isFill)  {
			if (!arg_isFill) {
				isFill = FillMode::wire;
			}
		}
		DrawSettings(BlendMode arg_blendMode, CullMode arg_cullMode) :cullMode(arg_cullMode), blendMode(arg_blendMode) {}
		DrawSettings(BlendMode arg_blendMode, CullMode arg_cullMode, bool arg_isFill) :cullMode(arg_cullMode), blendMode(arg_blendMode) {
			if (!arg_isFill) {
				isFill = FillMode::wire;
			}
		}
		CullMode cullMode = CullMode::back;
		FillMode isFill = FillMode::solid;
		BlendMode blendMode = BlendMode::AlphaBlend;
		BillBoardMode billboardMode =BillBoardMode::none;
		DrawFixParam drawFixParam = DrawFixParam::none;
		std::vector<SamplerState >vec_samplerMode = { SamplerState::LinearClamp };
		TopologyType topologyType = TopologyType::triangleList;
		bool isShadowMap = false;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(cullMode);
			archive(isFill);
			archive(blendMode);
			archive(billboardMode);
			archive(drawFixParam);
			archive(vec_samplerMode);
			archive(topologyType);
			archive(isShadowMap);
		}

		inline bool operator==(const DrawSettings& other)const {

			if (cullMode == other.cullMode && isFill == other.isFill&&billboardMode==other.billboardMode) {
				return true;
			}

			return false;
		}
	};
}

namespace std {

	template<>
	struct hash<ButiEngine::DrawSettings> {
	public:
		size_t operator()(const ButiEngine::DrawSettings& data)const {

			std::size_t seed = 0;
			ButiEngine::hash_combine(seed, data.isFill);
			ButiEngine::hash_combine(seed, (std::int32_t)data.cullMode);


			return seed;

		}
	};
}

namespace ButiEngine {
	class IApplication;
	class GraphicDevice:public IObject
	{
	public:


		virtual void Present() = 0;


		virtual void Release() = 0;

		virtual void ResourceUpload() = 0;

		virtual void UploadResourceRelease() = 0;
		virtual void UploadResourceBufferMerge() = 0;

		virtual void DrawStart(){}
		virtual void SetDefaultRenderTarget() = 0;
		virtual void ClearWindow() = 0;
		virtual void ClearDepthStancil(const float arg_depth) = 0;
		Matrix4x4 GetProjectionMatrix();
		Matrix4x4 GetCameraViewMatrix();
		const Vector3& GetCameraPos();
		void SetCameraPos(const Vector3& arg_pos);
		Matrix4x4 GetRawViewMatrix();
		Value_weak_ptr<IApplication> GetApplication();
		void SetProjectionMatrix(const Matrix4x4& arg_projectionMatrix);
		void SetViewMatrix(const Matrix4x4& arg_viewMatrix);
		void SetRawViewMatrix(const Matrix4x4& arg_viewMatrix);
		void SetViewMatrix_billBoard(const Matrix4x4& arg_viewMatrix);
		void SetViewMatrix_billBoardX(const Matrix4x4& arg_viewMatrix);
		void SetViewMatrix_billBoardY(const Matrix4x4& arg_viewMatrix);
		void SetViewMatrix_billBoardZ(const Matrix4x4& arg_viewMatrix);
		virtual void ResetPipeLine()=0;
		virtual void PipelineClear() = 0;

		const Matrix4x4& GetViewMatrixBillBoard() {
			return viewMatrix_billBoard;
		}
		const Matrix4x4& GetViewMatrixBillBoardX() {
			return viewMatrix_billBoardX;
		}
		const Matrix4x4& GetViewMatrixBillBoardY() {
			return viewMatrix_billBoardY;
		}
		const Matrix4x4& GetViewMatrixBillBoardZ() {
			return viewMatrix_billBoardZ;
		}

		void SetClearColor(const Vector4& arg_clearColor);
		Vector4 GetClearColor();
		TextureTag GetDefaultRenderTarget() { return defaultRenderTarget; }
		void SetDefaultRenderTarget(TextureTag arg_renderTargetTexture) {
			defaultRenderTarget = arg_renderTargetTexture;
		}
	protected:
		TextureTag defaultRenderTarget;
		Value_weak_ptr<IApplication> vwp_application;
		Matrix4x4 projectionMatrix;

		Matrix4x4 viewMatrix;
		Matrix4x4 viewMatrix_billBoard;
		Matrix4x4 viewMatrix_billBoardX;
		Matrix4x4 viewMatrix_billBoardY;
		Matrix4x4 viewMatrix_billBoardZ;
		Matrix4x4 rawViewMatrix;

		Vector4 color = Vector4(0, 1, 0, 1);
		Vector3 cameraPos;

		float clearColor[4] = { color.x, color.y, color.z, color.w };

	};

}