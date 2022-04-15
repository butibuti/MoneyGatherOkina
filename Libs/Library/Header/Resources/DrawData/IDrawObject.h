
#include"stdafx.h"
#include"../Bone.h"

#ifndef IDRAWOBJ_H
#define IDRAWOBJ_H

namespace ButiEngine {


	namespace Collision {
		class CollisionPrimitive;
		class CollisionPrimitive_Box_AABB;
		class CollisionPrimitive_Box_OBB;
	}

	class IDrawObject {
	public:
		virtual void Draw() = 0;
		virtual void DrawBefore() {}
		virtual float GetZ(const Matrix4x4& arg_vpMatrix) = 0;
		virtual void SetInfo() {}
		virtual void SetInfo_WithoutCommand(){}
		virtual void BufferUpdate() {}
		virtual void CommandSet() {}
		virtual void CommandExecute() {}
		virtual void ShowZ(){}

		virtual Value_ptr<Collision::CollisionPrimitive_Box_AABB> GetMeshAABB() = 0;
		virtual Value_ptr<Collision::CollisionPrimitive_Box_OBB> GetMeshOBB() = 0;
		virtual void SetPrimitive(Value_ptr<Collision::CollisionPrimitive>arg_prim) = 0;
		virtual void SetOctRegistPtr(std::uint32_t* arg_ptr) = 0;
		virtual Value_ptr<Collision::CollisionPrimitive> GetPrimitive() = 0;
		virtual std::uint32_t* GetOctRegistPtr() = 0;
	};
	class IBoneObject :public  IObject{
	public:
		std::vector<Value_ptr<Bone>> vec_bone;
		std::vector<Value_ptr<Bone>> vec_IKBone;
		std::vector<Value_ptr<Bone>> vec_addBone;
		void Initialize()override {}
		void PreInitialize()override{}
		void InverseKinematic() {

			for (auto itr = vec_IKBone.begin(); itr != vec_IKBone.end(); itr++) {
				(*itr)->CCDInverseKinematic();
			}
		}
		inline void BonePowerAdd() {

			for (auto itr = vec_addBone.begin(); itr != vec_addBone.end(); itr++) {
				(*itr)->AddBonePower();
			}
		}

		std::vector<Value_ptr<Bone>>& GetBones() {
			return vec_bone;
		}
		std::vector<Value_ptr<Bone>>& GetIKBones() {
			return vec_IKBone;
		}
		void SetIKBone() {
			vec_IKBone.clear();
			for (auto itr = vec_bone.begin(); itr != vec_bone.end(); itr++) {
				if ((*itr)->isIK) {
					vec_IKBone.push_back(*itr);
				}
			}
		}
		void SetAddBone() {
			vec_addBone.clear();
			for (auto itr = vec_bone.begin(); itr != vec_bone.end(); itr++) {
				if ((*itr)->addBoneIndex >= 0) {
					vec_addBone.push_back(*itr);
				}
			}
		}
		inline Value_ptr<Bone> searchBoneByName(const std::string& arg_boneName) {
			for (auto itr = vec_bone.begin(); itr != vec_bone.end(); itr++) {
				if ((*itr)->boneName == arg_boneName) {
					return (*itr);
				}
			}
			return nullptr;
		}
		inline Value_ptr<Bone> searchBoneByEngName(const std::string& arg_engBoneName) {
			for (auto itr = vec_bone.begin(); itr != vec_bone.end(); itr++) {
				if ((*itr)->boneNameEng == arg_engBoneName) {
					return (*itr);
				}
			}
			return nullptr;
		}
		inline Value_ptr<Bone> searchBoneByIndex(const std::int32_t arg_index) {
			if (arg_index < 0 || arg_index>vec_bone.size() - 1) {
				return nullptr;
			}
			else
			{
				return vec_bone.at(arg_index);
			}
		}
	};

}

#endif // !IDRAWOBJ_H
