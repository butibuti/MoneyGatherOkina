#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Worker :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Worker";
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
	private:
		void OnCollisionEnemy(Value_weak_ptr<GameObject> arg_vwp_enemy);
		Value_weak_ptr<GameObject> m_vwp_player;

		static float m_nearBorder;
	};

}

BUTI_REGIST_GAMECOMPONENT(Worker, true);