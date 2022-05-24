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
	void CreateParticle(const Vector3& arg_position);
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(m_range);
		archive(m_density);
		archive(m_fluctuation);
		archive(m_color);
		archive(m_minLife);
		archive(m_maxLife);
		archive(m_minAngleSpeed);
		archive(m_maxAngleSpeed);
		archive(m_sizePase);
		archive(m_size);
	}
private:
	float m_size=5.0f,m_minLife=150.0f,m_maxLife=600.0f,m_maxLifeOffset,m_minAngleSpeed,m_maxAngleSpeed,m_sizePase=0.1f, m_sizePaseDiv;
	std::int32_t controlIndex = 0;
	Vector4 m_color = Vector4(1, 0, 0.86, 1.0f);
	Vector3 m_range, m_density=Vector3(1.0f,1.0f,1.0f), m_fluctuation;
	Value_weak_ptr<ImmediateParticleController> m_vwp_particleController;
};

}

BUTI_REGIST_GAMECOMPONENT(FieldParticleEmitter, true);