#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Pocket;

	class Enemy :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Enemy";
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

		Value_weak_ptr<GameObject> GetFreePocket();
		void CreatePocket(std::uint8_t arg_pocketCount);
	private:
		std::uint8_t GetExistWorkerPocketCount();
		void RemoveAllPocket();

		Value_weak_ptr<GameObject> m_vwp_player;
		std::vector<Value_weak_ptr<GameObject>> m_vec_pockets;
		std::uint8_t m_pocketCount;
		std::int32_t m_testPocketCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy, true);