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
		void OnRemove()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		static float GetVibrationForce() { return m_vibrationForce; }
	private:
		void OnCollisionEnemy(Value_weak_ptr<GameObject> arg_vwp_enemy);

		Value_weak_ptr<GameObject> m_vwp_player;
		Value_weak_ptr<GameObject> m_vwp_drawObject;

		static float m_nearBorder;
		static float m_vibrationForce;
	};

}

BUTI_REGIST_GAMECOMPONENT(Worker, true);