#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class EnemyScaleAnimationComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "EnemyScaleAnimationComponent";
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

		void SetScaleRate(const float arg_scaleRate) { m_lerpScale = arg_scaleRate; }
		void SetAnotherScale(const Vector3& arg_scale) { m_anotherScale = arg_scale; }
		void SetMaxPlusScale(const float arg_scaleRate) { m_maxPlusScale = m_defaultScale * arg_scaleRate; }
		Vector3 GetScale() { return m_defaultScale; }

	private:
		Vector3 m_defaultScale;
		Vector3 m_anotherScale;
		Vector3 m_maxPlusScale;
		Vector3 m_previousScale;

		float m_lerpScale;
	};

}

BUTI_REGIST_GAMECOMPONENT(EnemyScaleAnimationComponent, true);

