#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class WorkerSpawner :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "WorkerSpawner";
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
		Value_weak_ptr<GameObject> m_vwp_player;
	};

}

BUTI_REGIST_GAMECOMPONENT(WorkerSpawner, true);