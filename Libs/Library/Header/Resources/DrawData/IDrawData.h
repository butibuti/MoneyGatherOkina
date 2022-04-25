#pragma once
#include"stdafx.h"
#include"ButiRendering_Dx12/Header/MeshPrimitive.h"
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
class DrawData {
public:
	void SetBlendMode(const BlendMode& arg_BlendMode);
	void MatrixUpdate();
	Matrix4x4 transform;
	Value_ptr<Transform> vlp_transform;

	inline float GetMaxZ(const Matrix4x4& arg_viewMatrix) {
		auto viewPos = transform.GetPosition_Transpose() * arg_viewMatrix;
		return viewPos.z;
	}
	std::vector<std::uint32_t> subset;
	Value_ptr< DrawInformation >vlp_drawInfo;
	Value_ptr<IRenderer> vlp_renderer;
	inline Value_ptr<ICBuffer> AddICBuffer(Value_ptr<ICBuffer> arg_cbuffer) {
		vlp_drawInfo->vec_exCBuffer.push_back(arg_cbuffer);
		return arg_cbuffer;
	}
	inline Value_ptr<ICBuffer> GetICBuffer(const std::string& arg_bufferName) {

		for (auto itr = vlp_drawInfo->vec_exCBuffer.begin(); itr != vlp_drawInfo->vec_exCBuffer.end(); itr++) {
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
	Value_ptr<Collision::CollisionPrimitive>& GetPrimitive() { return vlp_primitive; }
	void SetPrimitive(const Value_ptr<Collision::CollisionPrimitive>& arg_vlp_prim) { vlp_primitive = arg_vlp_prim; }
	Value_ptr < CBuffer<ShaderVariable>>& GetCBuffer() { return cbuffer; }
	void SetCbuffer(const Value_ptr < CBuffer<ShaderVariable>>& arg_cBuffer) { cbuffer = arg_cBuffer; }
	std::uint32_t* GetCollisionIndex() { return p_collisionRegistIndex; }
	void SetCollisionIndex(std::uint32_t* arg_p_collisionIndex) { p_collisionRegistIndex = arg_p_collisionIndex; }
	std::uint32_t GetCBufferCount()const { return cBufferCount; }
	void SetCBufferCount(const std::uint32_t arg_count) { cBufferCount = arg_count; }

	Value_weak_ptr<IResource_Model> GetModel() const{return vwp_model; }
	Value_weak_ptr<IResource_Mesh> GetMesh()const { return vwp_mesh; }
	Value_weak_ptr<IResource_Shader> GetShader()const { return vwp_shader; }
	List< Value_weak_ptr<IResource_Material>>& GetMaterial() { return list_vwp_material; }
	const List< Value_weak_ptr<IResource_Material>>& GetMaterial() const{ return list_vwp_material; }
	void SetModel(Value_weak_ptr<IResource_Model> arg_vwp_model) { vwp_model = arg_vwp_model; }
	void SetMesh(Value_weak_ptr<IResource_Mesh> arg_vwp_mesh) { vwp_mesh= arg_vwp_mesh; }
	void SetShader(Value_weak_ptr<IResource_Shader> arg_vwp_shader){vwp_shader = arg_vwp_shader;}
	void SetMaterial(const List< Value_weak_ptr<IResource_Material>>& arg_list_vwp_material) { list_vwp_material = arg_list_vwp_material; }
private:
	std::uint32_t cBufferCount = 0;
	Value_ptr < CBuffer<ShaderVariable>> cbuffer;
	Value_ptr<Collision::CollisionPrimitive>vlp_primitive;
	std::uint32_t* p_collisionRegistIndex;
	Value_weak_ptr<IResource_Model> vwp_model;
	Value_weak_ptr<IResource_Mesh> vwp_mesh;
	Value_weak_ptr<IResource_Shader> vwp_shader;
	List< Value_weak_ptr<IResource_Material>> list_vwp_material;
};

class DrawObject :public IDrawObject{
public:
	virtual void SetTransform(const Value_ptr<Transform>& arg_transform) = 0;
	virtual bool IsCreated() = 0;
	Value_ptr<Collision::CollisionPrimitive_Box_AABB> GetMeshAABB()override;
	Value_ptr<Collision::CollisionPrimitive_Box_OBB> GetMeshOBB()override;

	void SetPrimitive(Value_ptr<Collision::CollisionPrimitive>arg_prim) override;
	void SetOctRegistPtr(std::uint32_t* arg_ptr) override;
	Value_ptr<Collision::CollisionPrimitive> GetPrimitive() override;
	std::uint32_t* GetOctRegistPtr() override;
	DrawData& GetDrawData() { return drawData; }
	const DrawData& GetDrawData()const { return drawData; }
	virtual Value_ptr<ICBuffer> AddICBuffer(Value_ptr<ICBuffer> arg_cbuffer) {
		return drawData.AddICBuffer(arg_cbuffer);
	}
	virtual inline Value_ptr<ICBuffer> GetICBuffer(const std::string& arg_bufferName) {
		return drawData.GetICBuffer(arg_bufferName);
	}
protected:
	DrawData drawData;
};
class DualDrawObject :public DrawObject {
public:
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
	void SetTransform(const Value_ptr<Transform>& arg_transform)override;
	Value_ptr<Collision::CollisionPrimitive_Box_AABB> GetMeshAABB() override;
	Value_ptr<Collision::CollisionPrimitive_Box_OBB> GetMeshOBB() override;
	void SetPrimitive(Value_ptr<Collision::CollisionPrimitive>arg_prim) override;
	void SetOctRegistPtr(std::uint32_t* arg_ptr) override;
	Value_ptr<Collision::CollisionPrimitive> GetPrimitive() override;
	bool IsCreated()override {
		return vlp_afterDrawObj->IsCreated() && vlp_befDrawObj->IsCreated();
	}
	std::uint32_t* GetOctRegistPtr() override;
	inline Value_ptr<ICBuffer> GetICBuffer(const std::string& arg_bufferName)override {
		auto end = vlp_befDrawObj->GetDrawData(). vlp_drawInfo->vec_exCBuffer.end();
		for (auto itr = vlp_befDrawObj->GetDrawData().vlp_drawInfo->vec_exCBuffer.begin(); itr != end; itr++) {
			if ((*itr)->GetExName() == arg_bufferName) {
				return *itr;
			}
		}

		return nullptr;
	}
	Value_ptr<DrawObject> vlp_befDrawObj;
	Value_ptr<DrawObject> vlp_afterDrawObj;
};
class IModelObject {
public:
	virtual Value_ptr<IBoneObject>& GetModelBone()=0 ;
	virtual Value_ptr< CArrayBuffer<Matrix4x4>>&  GetBoneCBuffer()=0;
};
}

