#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SphereExclusion :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "SphereExclusion";
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
		}

		float GetMass() { return m_mass; }
		void SetMass(const float arg_mass) { m_mass = arg_mass; }
	private:
		float m_mass = 0.0f;

		void Exclusion(Value_weak_ptr<GameObject> arg_vwp_other, Value_ptr<SphereExclusion> arg_vlp_exclusion);
	};

}

BUTI_REGIST_GAMECOMPONENT(SphereExclusion, true);