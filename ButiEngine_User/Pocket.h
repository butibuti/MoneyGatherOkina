#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Pocket :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Pocket";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnRemove()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void SetEnemy(Value_weak_ptr<GameObject> arg_vwp_enemy) { m_vwp_enemy = arg_vwp_enemy; }
		void SetWorker(Value_weak_ptr<GameObject> arg_vwp_worker) { m_vwp_worker = arg_vwp_worker; }
		Value_weak_ptr<GameObject> GetEnemy() { return m_vwp_enemy; }
		Value_weak_ptr<GameObject> GetWorker() { return m_vwp_worker; }

		bool ExistStickWorker() { return m_vwp_worker.lock(); }
		void Dead();
	private:
		Value_weak_ptr<GameObject> m_vwp_enemy;
		Value_weak_ptr<GameObject> m_vwp_worker;
	};

}

BUTI_REGIST_GAMECOMPONENT(Pocket, true);