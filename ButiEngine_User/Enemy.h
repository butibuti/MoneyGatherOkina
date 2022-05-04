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

		void SetVibrationCapacity(const float arg_capacity) { m_vibrationCapacity = arg_capacity; }
		void SetVibrationResistance(const float arg_resistance) { m_vibrationResistance = arg_resistance; }
		Value_weak_ptr<GameObject> GetNearFreePocket(const Vector3& arg_pos, float arg_border);
		void CreatePocket(const std::uint8_t arg_pocketCount);
	private:
		std::uint8_t GetExistWorkerPocketCount();
		void RemoveAllPocket();

		//êUìÆÇÃå∏è≠ó 
		static float m_vibrationDecrease;

		Value_weak_ptr<GameObject> m_vwp_player;
		std::vector<Value_weak_ptr<GameObject>> m_vec_pockets;
		std::uint8_t m_pocketCount;
		std::int32_t m_testPocketCount;

		float m_vibrationCapacity;
		float m_vibrationResistance;


	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy, true);