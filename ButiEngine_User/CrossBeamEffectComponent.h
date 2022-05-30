#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class CrossBeamEffectComponent :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "CrossBeamEffectComponent";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnRemove()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void SetPosition(const Vector3& arg_position) { m_position = arg_position; }

	private:
		Value_ptr<Timer> m_vlp_animationTimer;
		Value_weak_ptr<GameObject> m_vwp_crossFlashEffectUI[2];

		Vector3 m_position;
		std::int8_t m_animationStep;
		float m_animationScale;
		float m_animationRotate;
	};

}

BUTI_REGIST_GAMECOMPONENT(CrossBeamEffectComponent, true);