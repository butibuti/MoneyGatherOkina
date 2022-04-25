#include"stdafx.h"

#include <D3dx12.h>
namespace ButiEngine
{
	class GraphicDevice_Dx12;
	class Resource_Shader;

	class PipelineState :public IObject {
	public:
		PipelineState(Microsoft::WRL::ComPtr<ID3D12PipelineState> arg_pipelineState):cmp_dx12pipelineState(arg_pipelineState){}
		void Initialize()override{}
		void PreInitialize()override{}

		std::int32_t GetPSOutputCount() const { return psoutputCount; }
		Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPielineState() { return cmp_dx12pipelineState; }

	private:
		std::int32_t psoutputCount;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> cmp_dx12pipelineState;
	};

	class PipelineStateManager :public IObject {
	public:
		PipelineStateManager(Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice);
		void Release();
		void ClearPipelineState();
		void Initialize() {}
		void PreInitialize() {}
		std::int32_t GetPipelineStatePSOutputCount(const std::string& arg_pipeLineState);
		Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState(const std::string& arg_pipeLineState);
		Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState(const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature, const D3D12_ROOT_SIGNATURE_DESC rootdesc, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc, D3D12_RASTERIZER_DESC& arg_rasteriserDesc, Value_ptr<IResource_Shader> arg_shader, const BlendMode arg_BlendMode, const TopologyType arg_topologyType,const bool arg_isDepth);
	private:
		std::unordered_map<std::string,Value_ptr<PipelineState> > map_pipeLineState;
		Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	};
}