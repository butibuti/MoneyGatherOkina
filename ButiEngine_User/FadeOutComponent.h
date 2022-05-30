#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class FadeOutComponent : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "FadeOutComponent";
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
		/// <summary>
		/// true‚ÅFadeIn
		/// </summary>
		/// <param name="arg_isFadeIn"></param>
		void SetIsFade(const bool arg_isFadeIn) { m_isFadeIn = arg_isFadeIn; }
		void SetMoveAlpha(const float arg_moveAlpha) { m_moveAlpha = arg_moveAlpha; }
		void SetMaxAlpha(const float arg_maxAlpha) { m_maxAlpha = arg_maxAlpha; }
		void SetPositionZ(const float arg_positionZ);
		void SetColor(const Vector3& arg_color);
		bool IsFadeAnimation()const;
	private:
		bool m_isFadeIn;
		float m_alpha;
		float m_moveAlpha;
		float m_maxAlpha;
	};
}

BUTI_REGIST_GAMECOMPONENT(FadeOutComponent, true);

