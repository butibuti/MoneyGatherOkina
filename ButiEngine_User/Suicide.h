#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Suicide :public GameComponent
	{
	public:
		Suicide(const std::uint32_t arg_frame) : m_suicideFrame(arg_frame){}
		Suicide() : m_suicideFrame(0){}
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
	private:
		Value_ptr<Timer> m_vlp_timer;
		std::uint32_t m_suicideFrame;
	};

}

BUTI_REGIST_GAMECOMPONENT(Suicide, true);