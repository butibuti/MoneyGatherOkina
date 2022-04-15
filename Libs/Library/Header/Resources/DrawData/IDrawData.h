#pragma once
#include"stdafx.h"
#include"ButiRendering_Dx12/Vertex.h"
#include"IDrawObject.h"
#include"../../Common/CArrayBuffer.h"
namespace ButiEngine {

	

	struct DrawInformation :public IObject {
		DrawInformation() {}
		DrawInformation(const DrawSettings arg_DrawSettings) :drawSettings(arg_DrawSettings) {}
		DrawSettings drawSettings;
		bool isAlpha = false;
		bool isDepth = true;
		std::uint32_t layer = 0;
		void Initialize()override {}
		void PreInitialize()override {}
		std::vector<Value_ptr<ICBuffer>> vec_exCBuffer;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isAlpha);
			archive(isDepth);
			archive(layer);
			archive(drawSettings);
			archive(vec_exCBuffer);
		}
		bool IsContainExCBuffer(const std::string& arg_cbufferName) {
			for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); itr++) {
				if ((*itr)->GetExName() == arg_cbufferName) {
					return true;
				}
			}
			return false;
		}
		void RemoveExCBuffer(const std::string& arg_cbufferName) {
			for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); ) {
				if ((*itr)->GetExName() == arg_cbufferName) {
					vec_exCBuffer.erase(itr);
					return;
				}
				else
					itr++;
			}
		}
		Value_ptr<ICBuffer> GetExCBuffer(const std::string& arg_cbufferName) {
			for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); ) {
				if ((*itr)->GetExName() == arg_cbufferName) {
					return *itr;
				}
				else {
					itr++;
				}
			}
			return nullptr;
		}

		Value_ptr< DrawInformation > Clone() {
			auto output = ObjectFactory::Create<DrawInformation>();
			output->layer = layer;
			output->drawSettings = drawSettings;
			output->isAlpha = isAlpha;
			output->isDepth = isDepth;
			for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); itr++) {
				output->vec_exCBuffer.push_back((*itr)->Clone());
			}

			return output;
		}
	};
	struct DrawData {
		

		void SetBlendMode(const BlendMode& arg_BlendMode);
		void MatrixUpdate();
		Matrix4x4 transform;
		Value_ptr<Transform> shp_transform;

		inline float GetMaxZ(const Matrix4x4& arg_viewMatrix){
			
			auto viewPos =transform.GetPosition_Transpose()  * arg_viewMatrix;
			//viewZ = viewPos.z;
			return viewPos.z;
		}
		//float viewZ;
		ModelTag modelTag;
		
		MeshTag meshTag;
		
		ShaderTag shaderTag;

		std::vector < MaterialTag> vec_materialTags;
		std::vector<std::uint32_t> subset;

		Value_ptr< DrawInformation >shp_drawInfo;

		Value_ptr<IRenderer> shp_renderer;
		virtual Value_ptr<ICBuffer> AddICBuffer(Value_ptr<ICBuffer> arg_cbuffer) {
			shp_drawInfo-> vec_exCBuffer.push_back(arg_cbuffer);
			return arg_cbuffer;
		}

		virtual inline Value_ptr<ICBuffer> GetICBuffer(const std::string& arg_bufferName) {

			for (auto itr =shp_drawInfo-> vec_exCBuffer.begin(); itr !=shp_drawInfo-> vec_exCBuffer.end(); itr++) {
				if ((*itr)->GetExName() == arg_bufferName) {
					return *itr;
				}
			}

			return nullptr;
		}

		template <class T>
		inline Value_ptr<CBuffer<T>> GetCBuffer(const std::string& arg_bufferName) {

			auto out = GetICBuffer(arg_bufferName);
			if (out && out->IsThis<CBuffer<T>>()) {
				return out->GetThis<CBuffer<T>>();
			}

			return nullptr;
		}
		template <class T>
			inline Value_ptr<CArrayBuffer<T>> GetCArrayBuffer(const std::string& arg_bufferName) {

			auto out = GetICBuffer(arg_bufferName);
			if (out && out->IsThis<CArrayBuffer<T>>()) {
				return out->GetThis<CArrayBuffer<T>>();
			}

			return nullptr;
		}
	protected:

		std::uint32_t cBufferCount = 0;
		Value_ptr < CBuffer<ShaderVariable>> cbuffer;
		Value_ptr<Collision::CollisionPrimitive>shp_primitive;
		std::uint32_t* collisionRegistPtr;
	};

	struct MeshDrawData:public DrawData,public IDrawObject,public IObject {
		void PreInitialize() override{}

		virtual void SetTransform(Value_ptr<Transform>& arg_transform) = 0;
		virtual bool IsCreated()=0;
		Value_ptr<Collision::CollisionPrimitive_Box_AABB> GetMeshAABB()override;
		Value_ptr<Collision::CollisionPrimitive_Box_OBB> GetMeshOBB()override;

		void SetPrimitive(Value_ptr<Collision::CollisionPrimitive>arg_prim) override;
		void SetOctRegistPtr(std::uint32_t* arg_ptr) override;
		Value_ptr<Collision::CollisionPrimitive> GetPrimitive() override;
		std::uint32_t* GetOctRegistPtr() override;
	};
	struct DualDrawData :public MeshDrawData{
		void Initialize()override;
		void PreInitialize() override;
		void Draw()override;
		void DrawBefore() override;
		float GetZ(const Matrix4x4& arg_vpMatrix)override;
		void SetInfo() override;
		void SetInfo_WithoutCommand() override;
		void BufferUpdate() override;
		void CommandSet() override;
		void CommandExecute() override;
		void SetTransform(Value_ptr<Transform>& arg_transform)override;
		Value_ptr<Collision::CollisionPrimitive_Box_AABB> GetMeshAABB() override;
		Value_ptr<Collision::CollisionPrimitive_Box_OBB> GetMeshOBB() override;
		void SetPrimitive(Value_ptr<Collision::CollisionPrimitive>arg_prim) override;
		void SetOctRegistPtr(std::uint32_t* arg_ptr) override;
		Value_ptr<Collision::CollisionPrimitive> GetPrimitive() override;
		bool IsCreated()override {
			return shp_afterDrawObj->IsCreated() && shp_befDrawObj->IsCreated();
		}
		std::uint32_t* GetOctRegistPtr() override;

		inline Value_ptr<ICBuffer> GetICBuffer(const std::string& arg_bufferName)override {
			auto end = shp_befDrawObj->shp_drawInfo->vec_exCBuffer.end();
			for (auto itr = shp_befDrawObj-> shp_drawInfo->vec_exCBuffer.begin(); itr != end; itr++) {
				if ((*itr)->GetExName() == arg_bufferName) {
					return *itr;
				}
			}

			return nullptr;
		}

		Value_ptr<MeshDrawData> shp_befDrawObj;
		Value_ptr<MeshDrawData> shp_afterDrawObj;
	};


	struct ModelDrawData
	{
		Value_ptr<IBoneObject> shp_bone;
		Value_ptr< CArrayBuffer<Matrix4x4>> cbuffer_bone;
	};
}

