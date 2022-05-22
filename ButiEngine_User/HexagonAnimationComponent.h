#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class HexagonAnimationComponent : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "HexagonAnimationComponent";
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

		bool IsAnimation() { return m_isAnimation; }
		void SetNumber(const std::int8_t arg_myNumber) { m_myNumber = arg_myNumber; }
		void StartAnimation(const bool arg_isChange);
		void Hide();
		void Apparent();
	private:
		void ScaleAnimation();
		void AnimationTime();

		Vector3 m_defaultScale;
		Vector3 m_scale;

		std::int8_t m_myNumber;
		std::int16_t m_animationTimer;

		bool m_isShow;
		bool m_isAnimation;
	};
}

BUTI_REGIST_GAMECOMPONENT(HexagonAnimationComponent, true);
