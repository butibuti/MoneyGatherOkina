#include"stdafx.h"
#include"CBuffer_Dx12.h"
namespace ButiEngine {

	class Camera_Dx12 :public Camera
	{
	public:
		Camera_Dx12(const std::string& cameraName, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice);
		Camera_Dx12(const CameraProjProperty& arg_cameraViewProp,const std::string& cameraName, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice);
		~Camera_Dx12();
		void Initialize()override;
		void PreInitialize()override {}
		void Start()override;
		void Stop() const override;
		void SetProjectionTexture(const TextureTag& arg_tag)override;
		void SetDepthStencilView(const TextureTag& arg_tag)override;
		void ShowUI()override;
		void End()override;
	private:

		Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> drawCommandList;

		D3D12_RECT     scissorRect;
		D3D12_VIEWPORT    viewport;
		std::vector< Value_ptr<IRenderTarget>> vec_vlp_renderTarget;
		Value_ptr<IDepthStencil> vlp_depthStencil;
	};
}