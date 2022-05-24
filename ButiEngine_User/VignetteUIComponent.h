#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class VignetteUIComponent :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "VignetteUIComponent";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void StartAlphaAnimation();
		void SetIsKeepAlpha(const bool arg_isKeepAlpha) { m_isKeepAlpha = arg_isKeepAlpha; }

	private:
		Value_ptr<Timer> m_vlp_timer;

		float m_alpha;
		float m_moveAlpha;

		bool m_isChange;
		bool m_isKeepAlpha;
	};

}

BUTI_REGIST_GAMECOMPONENT(VignetteUIComponent, true);

