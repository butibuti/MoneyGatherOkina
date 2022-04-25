#pragma once

#include"../Device/DescriptorHeapManager.h"
#include"../GameParts/GraphicDevice_Dx12.h"
namespace ButiEngine {
	template <class T>
	class CBuffer_Dx12 :public CBuffer<T>,public GPUResource
	{
	public:
	CBuffer_Dx12(const std::uint32_t arg_slot) {
		size = 0;
		this->slot = arg_slot;
	}
	CBuffer_Dx12(){}
	~CBuffer_Dx12() {
		if (this->instance) {

			if (vwp_heapManager.lock()) {
				vwp_heapManager.lock()->Release(BlankSpace{ index,size / 0x100 });
			}
			this->instance = nullptr;
		}
	}
	T& Get() const override
	{
		return *instance.get();
	}
	void Initialize() {
		size = (sizeof(T) + 255) & ~255;
		if(!this->instance)
			this->instance=(make_value<T>());
		
	}
	void CreateBuffer(const bool arg_mapKeep=true)override {
		if (index != 0&&!this->IsCereal()) {
			return;
		}
		mapKeep = arg_mapKeep;
		auto out= vwp_heapManager.lock()->CreateConstantBufferView(instance.get(), mapKeep, sizeof(T));
		index= out.index;
		gpuDescriptorHandle = out.GPUHandle;
	}
	void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override{
		vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
		vwp_heapManager = vwp_graphicDevice.lock()->GetDescriptorHeapManager();
	}
	void PreInitialize() {

	}
	void Attach(const std::uint32_t slotOffset)const override {
		vwp_graphicDevice.lock()->GetCommandList().SetGraphicsRootDescriptorTable(slotOffset + this->slot, gpuDescriptorHandle);
	}
	void Update()const override {

		vwp_heapManager.lock()->ConstantBufferUpdate(instance.get(), index, sizeof(T));
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(this->slot);
		archive(size);
		archive(index);
		archive(mapKeep);
		archive(this->exName);
		archive(instance);
	}
	bool OnShowUI()override {
		return instance->ShowUI();
	}
	Value_ptr<ICBuffer> Clone()override {
		auto output = ObjectFactory::Create<CBuffer_Dx12<T>>(this->slot);

		output->exName =this-> exName;
		*(output->instance) = (*instance);

		return output;
	}
private:
	void  UpdateResourceRelease()override {

	}
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescriptorHandle;
	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	Value_weak_ptr<DescriptorHeapManager> vwp_heapManager;
	Value_ptr<T> instance ;
	std::uint32_t size;
	std::uint32_t index = 0;
	bool mapKeep=true;
	};

}
