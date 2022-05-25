#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpriteParticleGenerator;

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
		bool IsGather() { return m_isGather; }
	private:
		Vector3 m_prevPos;
		Vector3 m_pos;

		bool m_isGather;
		
		Value_weak_ptr<SpriteParticleGenerator> m_vwp_spriteParticleGenerator;
	};

}

BUTI_REGIST_GAMECOMPONENT(FlockingLeader, true);