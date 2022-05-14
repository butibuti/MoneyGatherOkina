#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class PolygonParticleGenerater : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "PolygonParticleGenerater";
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

		void ExplosionPolygonParticles(const Vector3& arg_position);

	private:
		void Flickering();

		Value_weak_ptr<ImmediateParticleController> m_vwp_immediateParticleController;

		std::int8_t m_flickeringFrame;
		bool m_isChangeColor;
	};
}

BUTI_REGIST_GAMECOMPONENT(PolygonParticleGenerater, true);
