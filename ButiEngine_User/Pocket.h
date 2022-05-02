#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Worker;

	class Pocket :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Pocket";
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

		void SetWorker(Value_weak_ptr<GameObject> arg_worker) { m_vwp_worker = arg_worker; }
		bool ExistWorker() { return m_vwp_worker.lock(); }
	private:
		Value_weak_ptr<GameObject> m_vwp_worker;
	};

}

BUTI_REGIST_GAMECOMPONENT(Pocket, true);