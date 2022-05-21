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
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void ExplosionParticles(const Vector3& arg_position);
		void SparkParticle(const Vector3& arg_position);
		void TrajectoryParticles(const Vector3& arg_position);
		void PachiPachiParticles(const Vector3& arg_position);
		void CatchParticles(const Vector3& arg_position, Value_ptr<ButiEngine::Transform> arg_vlp_parentTransform);
		void ExplosionPolygonParticles(const Vector3& arg_position, const bool arg_isBig);
		void AttackFlashParticles(const Vector3& arg_position, const float arg_size, const Vector4& arg_color);

	private:
		void Flickering();

		Value_weak_ptr<ImmediateParticleController> m_vwp_immediateParticleController;
		Value_weak_ptr<SpriteParticleController> m_vwp_spriteParticleController;

		float m_accel;

		std::int8_t m_flickeringFrame;
		bool m_isChangeColor;
	};
}

BUTI_REGIST_GAMECOMPONENT(ParticleGenerater, true);
