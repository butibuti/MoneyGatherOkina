#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class FlockingLeader :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FlockingLeader";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnRemove()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		float GetMoveSpeed() { return (m_pos - m_prevPos).GetLength(); }
	private:
		Vector3 m_prevPos;
		Vector3 m_pos;
	};

}

BUTI_REGIST_GAMECOMPONENT(FlockingLeader, true);