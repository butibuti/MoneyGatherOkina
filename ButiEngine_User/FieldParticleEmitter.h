#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

class FieldParticleEmitter :public GameComponent
{
public:

	std::string GetGameComponentName()const override {
		return "FieldParticleEmitter";
	}
	void OnUpdate()override;
	void OnSet()override;
	void Start()override;
	void OnShowUI()override;
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(m_range);
		archive(m_density);
		archive(m_generatePase);
		archive(m_fluctuation);
	}
private:
	float m_generatePase;
	Vector3 m_range, m_density=Vector3(1.0f,1.0f,1.0f), m_fluctuation;
	Value_weak_ptr<ImmediateParticleController> m_vwp_particleController;
};

}

BUTI_REGIST_GAMECOMPONENT(FieldParticleEmitter, true);