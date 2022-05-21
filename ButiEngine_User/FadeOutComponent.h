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

		void SetIsFade(const bool arg_isFade) { m_isFade = arg_isFade; }
		void SetMoveAlpha(const float arg_moveAlpha) { m_moveAlpha = arg_moveAlpha; }

	private:
		bool m_isFade;
		float m_alpha;
		float m_moveAlpha;
	};
}

BUTI_REGIST_GAMECOMPONENT(FadeOutComponent, true);

