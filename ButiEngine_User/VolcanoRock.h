#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ParticleGenerater;
	class SoundPlayerComponent;

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
		Value_ptr<Timer> m_vlp_lifeTimer;
		Value_weak_ptr<SoundPlayerComponent> m_vwp_soundPlayerComponent;

		Vector3 m_velocity;
		Vector3 m_rotateSpeed;
		float m_gravity;

	};

}

BUTI_REGIST_GAMECOMPONENT(VolcanoRock, true);

