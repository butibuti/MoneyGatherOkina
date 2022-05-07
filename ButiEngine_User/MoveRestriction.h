#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RigidBodyComponent;

	class MoveRestriction :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "MoveRestriction";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		float GetOutLength(const Vector3& arg_pos);
		bool IsOutField(const Vector3& arg_pos);
	private:
		void SetNewPosition();

		Value_weak_ptr<GameObject> m_vwp_field;
		Value_ptr<RigidBodyComponent> m_vlp_rigidBody;
		float m_outLength;
	};

}

BUTI_REGIST_GAMECOMPONENT(MoveRestriction, true);