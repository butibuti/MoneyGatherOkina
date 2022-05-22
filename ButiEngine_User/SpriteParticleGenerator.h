#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class SpriteParticleGenerator : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "SpriteParticleGenerator";
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

		void AttackFlashParticles(const Vector3& arg_position, const float arg_spawnRadius, const float arg_size, const Vector4& arg_color);
		void ChargeParticles(const Vector3& arg_position, const Vector4& arg_color);

	private:
		Value_weak_ptr<SpriteParticleController> m_vwp_spriteParticleController;
	};
}

BUTI_REGIST_GAMECOMPONENT(SpriteParticleGenerator, true);
