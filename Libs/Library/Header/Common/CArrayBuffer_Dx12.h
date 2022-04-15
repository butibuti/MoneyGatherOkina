#pragma once
#include"CArrayBuffer.h"
#include"../GameParts/GraphicDevice_Dx12.h"
namespace ButiEngine {

	template <class T>
	class CArrayBuffer_Dx12:public CArrayBuffer<T>
	{
	public:
		CArrayBuffer_Dx12(const std::uint32_t arg_slot,const std::uint32_t count) {
			this->slot = arg_slot;
			size = (sizeof(T) + 255) & ~255;
			instanceSize = sizeof(T) * count;
		}
		CArrayBuffer_Dx12(){}
		~CArrayBuffer_Dx12() {
			if (instance) {
				if (wkp_heapManager.lock()) {
					wkp_heapManager.lock()->Release(BlankSpace{ index,instanceSize / 0x100 });

				}
				delete instance;
			}

		}
		T& Get(const std::uint32_t arg_index) override
		{
			return instance[arg_index];
		}

		void Initialize() override{
			instance = (T*)malloc(instanceSize);
		}


		void CreateBuffer(const bool arg_mapKeep = true) override{
			mapKeep = arg_mapKeep;
			auto out = wkp_heapManager.lock()->CreateConstantBufferView(instance, mapKeep, instanceSize);
			index = out.index;
			gpuDescriptorHandle = out.GPUHandle;
		}

		void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) {
			wkp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
			wkp_heapManager = wkp_graphicDevice.lock()->GetDescriptorHeapManager();
		}
		void PreInitialize() override{

		}
		void Attach(const std::uint32_t slotOffset)const override{
			wkp_graphicDevice.lock()->GetCommandList().SetGraphicsRootDescriptorTable(slotOffset + this->slot, gpuDescriptorHandle);
		}
		void Update()const override{

			wkp_heapManager.lock()->ConstantBufferUpdate(instance, index, instanceSize);
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(this->slot);
			archive(size);
			archive(index);
			archive(mapKeep);
			archive(this->exName);
			archive(instanceSize);
		}

		Value_ptr<ICBuffer> Clone()override {
			auto output = ObjectFactory::Create<CArrayBuffer_Dx12<T>>(this->slot, instanceSize / sizeof(T));

			output->exName = this->exName;

			return output;
		}
	private:
		Value_weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
		Value_weak_ptr<DescriptorHeapManager> wkp_heapManager;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescriptorHandle;
		T* instance=nullptr;
		std::uint32_t size=0;
		std::uint32_t index=0;
		std::uint32_t instanceSize=0;
		bool mapKeep = true;
	};
}