#pragma once
#include"stdafx.h"
#include"Bone.h"
#include"Morph.h"
namespace ButiEngine {


	class Resource_Model:public IResource_Model
	{
	public:
		Resource_Model();

		void Initialize()override {};
		void PreInitialize()override {};

		void SetMesh(const Value_weak_ptr< IResource_Mesh>& arg_vwp_mesh) override;
		void SetMaterial(const List<Value_weak_ptr< IResource_Material>>& arg_list_vwp_material) override;
		void SetName(const std::string & arg_name) override;
		void SetEngName(const std::string & arg_engName) override;
		void SetModelName(const std::string & arg_modelName) override;
		void SetEngModelName(const std::string & arg_engModelName) override;
		void SetComment(const std::string & arg_comment) override;
		void SetEngComment(const std::string & arg_engComment) override;
		void AddBone(Bone& arg_bone) override;
		void AddMorph(Value_ptr<Morph::Morph> arg_morph) override;
		void SetVersion(const float arg_version) override;
		void SetSubset(const std::vector<std::uint32_t>& arg_subset) override;
		std::vector<std::uint32_t> GetSubset() override;
		const std::string & GetName() override;
		const std::string & GetEngName() override;
		const std::string & GetModelName() override;
		const std::string & GetEngModelName() override;
		const std::string & GetComment() override;
		const std::string & GetEngComment() override;
		std::vector<Value_ptr<Bone>> GetBone() override;

		Value_weak_ptr<IResource_Mesh> GetMesh()const override;
		const List<Value_weak_ptr<IResource_Material>>& GetMaterial()const override;
		List<Value_weak_ptr<IResource_Material>>& GetMaterial() override;
	private:
		std::string modelName;
		std::string modelNameEng;
		std::string comment;
		std::string commentEng;
		std::string name;
		std::string nameEng;
		std::vector<Value_ptr< Morph::Morph>> vec_morphs;

		std::vector<std::uint32_t> subset;

		Value_weak_ptr<IResource_Mesh> vwp_mesh;
		List<Value_weak_ptr<IResource_Material>> list_vwp_material;

		std::vector<Bone> vec_bone;

		float version;
	};

}