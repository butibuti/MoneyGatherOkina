#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Suicide :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Suicide";
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

		void SetFrame(const std::uint32_t arg_frame) { m_vlp_timer->ChangeCountFrame(arg_frame); }

		void StartTimer() { m_vlp_timer->Start(); }
	private:
		Value_ptr<Timer> m_vlp_timer;
	};

}

BUTI_REGIST_GAMECOMPONENT(Suicide, true);