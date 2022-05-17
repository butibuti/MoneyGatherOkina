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

		float GetWeight() { return m_weight; }
		void SetWeight(const float arg_weight) { m_weight = arg_weight; }
	private:
		float m_weight = 0.0f;

		void Exclusion(Value_weak_ptr<GameObject> arg_vwp_other, Value_ptr<SphereExclusion> arg_vlp_exclusion);
	};

}

BUTI_REGIST_GAMECOMPONENT(SphereExclusion, true);