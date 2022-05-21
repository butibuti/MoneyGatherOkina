#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class ExpansionAnimationComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "ExpansionAnimationComponent";
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

		void SetScale(const Vector3& arg_scale) { m_defaultScale = arg_scale; }
		void SetIsActive(const bool arg_isActive) { m_isActive = arg_isActive; }

	private:
		Vector3 m_defaultScale;
		Vector3 m_currentScale;

		float m_borderScale;

		bool m_isActive;
	};

}

BUTI_REGIST_GAMECOMPONENT(ExpansionAnimationComponent, true);

