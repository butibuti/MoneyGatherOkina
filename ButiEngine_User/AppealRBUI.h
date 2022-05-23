#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ScaleAnimation;
	class SpriteAnimationComponent;

	class AppealRBUI :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "AppealRBUI";
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

		void Appear();
		void Disappear();
	private:
		void SpriteAnimation();
		void AddScaleAnimation(const Vector3& arg_targetScale);

		Value_ptr<Timer> m_vlp_scaleAnimationTimer;
		Value_ptr<Timer> m_vlp_spriteAnimationTimer;
		Value_weak_ptr<ScaleAnimation> m_vwp_scaleAnimation;
		Value_weak_ptr<SpriteAnimationComponent> m_vwp_spriteAnimation;
		Vector3 m_targetScale;
		bool m_isAppear;
	};

}

BUTI_REGIST_GAMECOMPONENT(AppealRBUI, true);