#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H
#include"../GameComponent.h"
#include"ButiUtil/ButiUtil/Flag.h"
#include"ButiBulletWrap/ButiBulletWrap/Common.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
namespace ButiEngine {
class RigidBodyComponent:public GameComponent,public ButiBullet::PhysicsDetail::IPhysicsObjectEventListener {
public:
	struct RigidBodyParameter {
		std::int32_t groupMask= 0x0000FFFF;
		std::int32_t group= 0x00000001;
		bool isKinematics=false;
		bool isAdditionalDamping=true;
		float mass=1.0f;
		float friction = 0.5f;
		float restitution=0.0f;
		float linearDamping=0.0f;
		float angularDamping=0.0f; 
		Flags<ButiBullet::RigidBodyLimitFlags> flg_linearLimits=ButiBullet::RigidBodyLimitFlags::None;
		Flags<ButiBullet::RigidBodyLimitFlags> flg_angularLimits = ButiBullet::RigidBodyLimitFlags::None;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(groupMask);
			archive(group);
			archive(isKinematics);
			archive(isAdditionalDamping);
			archive(mass);
			archive(friction);
			archive(restitution);
			archive(linearDamping);
			archive(angularDamping);
			archive(flg_linearLimits);
			archive(flg_angularLimits);
		}
	};
	class ShapeParam {
	public:
		virtual Value_ptr<ButiBullet::CollisionShape> CreateShape(Value_ptr<GameObject> arg_ownerObject)const=0;
		virtual bool ShowGUI() = 0;
	};
	class SphereShapeParam :public ShapeParam {
	public:
		Value_ptr<ButiBullet::CollisionShape> CreateShape(Value_ptr<GameObject> arg_ownerObject)const;
		bool ShowGUI();

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(radius);
		}
	private:
		float radius=0.5f;
	};
	class BoxShapeParam :public ShapeParam {
	public:
		Value_ptr<ButiBullet::CollisionShape> CreateShape(Value_ptr<GameObject> arg_ownerObject)const;
		bool ShowGUI();
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(size);
		}
	private:
		Vector3 size=Vector3(1.0f,1.0f,1.0f);
	};

	class PlaneShapeParam :public ShapeParam {
	public:
		Value_ptr<ButiBullet::CollisionShape> CreateShape(Value_ptr<GameObject> arg_ownerObject)const;
		bool ShowGUI();
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(upAxis);
		}
	private:
		Vector3 upAxis = Vector3(0, 1, 0);
	};
	class CylinderShapeParam :public ShapeParam {
	public:
		Value_ptr<ButiBullet::CollisionShape> CreateShape(Value_ptr<GameObject> arg_ownerObject)const;
		bool ShowGUI();
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(size);
		}
	private:
		Vector3 size= Vector3(1, 1, 1);
	};
	class ConeShapeParam :public ShapeParam {
	public:
		Value_ptr<ButiBullet::CollisionShape> CreateShape(Value_ptr<GameObject> arg_ownerObject)const;
		bool ShowGUI();
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(height);
			archive(radius);
		}
	private:
		float height=1.0f;
		float radius = 0.5f;
	};
	class CapsuleShapeParam :public ShapeParam {
	public:
		Value_ptr<ButiBullet::CollisionShape> CreateShape(Value_ptr<GameObject> arg_ownerObject)const;
		bool ShowGUI();
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(radius);
			archive(height);
		}
	private:
		float radius=0.5f;
		float height=2.0f;
	};
	class MeshShapeParam :public ShapeParam {
	public:
		MeshShapeParam()  {}
		Value_ptr<ButiBullet::CollisionShape> CreateShape(Value_ptr<GameObject> arg_ownerObject)const;
		bool ShowGUI();
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(meshTag);
		}
	private:
		MeshTag meshTag;
	};
	std::string GetGameComponentName()const override {
		return "RigidBodyComponent";
	}

	void OnUpdate()override;
	void OnSet()override;
	void Start()override;
	void OnRemove()override;
	Value_ptr<GameComponent> Clone() override;

	void OnBeforeStepSimulation_Deprecated() override;
	void OnAfterStepSimulation() override;
	void OnCollisionEnter(ButiBullet::PhysicsObject* otherObject, ButiBullet::ContactPoint* contact)override;
	void OnCollisionLeave(ButiBullet::PhysicsObject* otherObject, ButiBullet::ContactPoint* contact)override;
	void OnCollisionStay(ButiBullet::PhysicsObject* otherObject, ButiBullet::ContactPoint* contact) override;

	void OnShowUI()override;

	const RigidBodyParameter& GetRigidBodyParameter()const { return param; }

	void SetIsKinematics(const bool arg_isKinematics);
	void SetIsAdditionalDamping(const bool arg_isAdditionalDamping);
	void SetMass(const float arg_mass);
	void SetFriction(const float arg_friction);
	void SetRestitution(const float arg_restitution);
	void SetLinerDamping(const float arg_linerDamping);
	void SetAngularDamping(const float arg_angularDamping); 
	void SetGroup(std::int32_t arg_group);
	void SetGroupMask(std::int32_t arg_groupMask);
	
	Value_ptr<ButiBullet::IRigidBody> GetRigidBody()const;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(param);
		archive(vlp_shapeParam);
		archive(isAffectedForGravity);
	}


private:
	void CreateRigidBody();
	void ReleaseRigidBody();
	void RecreateRigidBody() {
		ReleaseRigidBody();
		CreateRigidBody();
	}
	Value_ptr<ButiBullet::RigidBody> vlp_rigidBody;
	Value_ptr<ShapeParam> vlp_shapeParam;
	RigidBodyParameter param;
	bool isAffectedForGravity=true;
};

}

BUTI_REGIST_GAMECOMPONENT(RigidBodyComponent, true)
CEREAL_REGISTER_TYPE(ButiEngine::RigidBodyComponent::SphereShapeParam);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::RigidBodyComponent::ShapeParam, ButiEngine::RigidBodyComponent::SphereShapeParam);
CEREAL_REGISTER_TYPE(ButiEngine::RigidBodyComponent::BoxShapeParam);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::RigidBodyComponent::ShapeParam, ButiEngine::RigidBodyComponent::BoxShapeParam);
CEREAL_REGISTER_TYPE(ButiEngine::RigidBodyComponent::PlaneShapeParam);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::RigidBodyComponent::ShapeParam, ButiEngine::RigidBodyComponent::PlaneShapeParam);
CEREAL_REGISTER_TYPE(ButiEngine::RigidBodyComponent::ConeShapeParam);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::RigidBodyComponent::ShapeParam, ButiEngine::RigidBodyComponent::ConeShapeParam);
CEREAL_REGISTER_TYPE(ButiEngine::RigidBodyComponent::CylinderShapeParam);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::RigidBodyComponent::ShapeParam, ButiEngine::RigidBodyComponent::CylinderShapeParam);
CEREAL_REGISTER_TYPE(ButiEngine::RigidBodyComponent::CapsuleShapeParam);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::RigidBodyComponent::ShapeParam, ButiEngine::RigidBodyComponent::CapsuleShapeParam);
CEREAL_REGISTER_TYPE(ButiEngine::RigidBodyComponent::MeshShapeParam);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::RigidBodyComponent::ShapeParam, ButiEngine::RigidBodyComponent::MeshShapeParam);

#endif // !RIGIDBODYCOMPONENT_H
