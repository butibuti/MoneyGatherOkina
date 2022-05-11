#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ParticleGenerater;

	class VolcanoRock :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "VolcanoRock";
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

		void SetVelocity(const Vector3& arg_velocity);

		void Dead();
	private:
		Value_weak_ptr<ParticleGenerater> m_vwp_particleGenerater;
		Vector3 m_velocity;
		float m_gravity;
		std::int32_t m_life;

	};

}

BUTI_REGIST_GAMECOMPONENT(VolcanoRock, true);

