#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class ParticleGenerater : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "ParticleGenerater";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnShowUI()override;
		void Start()override;
		void ExplosionParticles(const Vector3& arg_position);
		void SparkParticle(const Vector3& arg_position);
		void TrajectoryParticles(const Vector3& arg_position);
		void PachiPachiParticles(const Vector3& arg_position);
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

	private:
		void Flickering();

		Value_weak_ptr<ImmediateParticleController> m_vwp_immediateParticleController;

		std::int8_t m_flickeringFrame;
		bool m_isChangeColor;
	};
}

BUTI_REGIST_GAMECOMPONENT(ParticleGenerater, true);
