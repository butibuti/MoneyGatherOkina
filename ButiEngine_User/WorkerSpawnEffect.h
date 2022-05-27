#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class WorkerSpawnEffect :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "WorkerSpawnEffect";
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
	private:
		Value_ptr<Timer> m_lifeTimer;

		Vector3 m_startScale;
		Vector3 m_targetScale;
	};

}

BUTI_REGIST_GAMECOMPONENT(WorkerSpawnEffect, true);