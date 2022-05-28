#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;
	class Worker;

	class WingAnimation :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "WingAnimation";
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

		void SetParent(Value_weak_ptr<GameObject> arg_parent) { m_vwp_parent = arg_parent; }
	private:
		float m_flapSpeed;
		float m_minFlapSpeed;
		float m_maxFlapSpeed;
		float m_theta;

		bool m_isPlayer;
		bool m_isWorker;

		Value_weak_ptr<Player> m_vwp_player;
		Value_weak_ptr<Worker> m_vwp_worker;

		Value_weak_ptr<GameObject> m_vwp_parent;
		Value_weak_ptr<Transform> m_vwp_leftWing;
		Value_weak_ptr<Transform> m_vwp_rightWing;
	};

}

BUTI_REGIST_GAMECOMPONENT(WingAnimation, true);