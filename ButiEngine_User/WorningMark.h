#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class WorningMark :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "WorningMark";
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

		void SetWaitFrame(const std::uint32_t arg_waitFrame) { m_vlp_waitTimer->ChangeCountFrame(arg_waitFrame); }
		void SetAddObjectName(const std::string& arg_addObjectName) { m_addObjectName = arg_addObjectName; }
	private:
		void AddScaleAnimation();

		Value_ptr<Timer> m_vlp_waitTimer;
		std::string m_addObjectName;
	};

}

BUTI_REGIST_GAMECOMPONENT(WorningMark, true);