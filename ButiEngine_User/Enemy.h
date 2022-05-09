#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;

	class Enemy :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Enemy";
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

		void SetNearBorder(const float arg_nearBorder) { m_nearBorder = arg_nearBorder; }
		void SetVibrationCapacity(const float arg_capacity) { m_vibrationCapacity = arg_capacity; }
		void SetVibrationResistance(const float arg_resistance) { m_vibrationResistance = arg_resistance; }

		Value_weak_ptr<GameObject> GetPlayer() { return m_vwp_player; }
		Value_weak_ptr<GameObject> GetNearFreePocket(const Vector3& arg_pos, float arg_border);

		bool IsVibrate() { return m_isVibrate; }

		void CreatePocket(const std::uint8_t arg_pocketCount);
		void RemovePocket(const std::uint8_t arg_pocketNum);
	private:
		void IncreaseVibration();
		void DecreaseVibration();
		//êUìÆó ÇÃè„è∏ílÇåvéZÇ∑ÇÈ
		void CalcVibrationIncrease();
		std::uint8_t GetStickWorkerCount();
		void RemoveAllPocket();

		//êUìÆÇÃå∏è≠ó 
		static float m_vibrationDecrease;

		Value_weak_ptr<GameObject> m_vwp_player;
		Value_ptr<Player> m_vlp_playerComponent;
		std::vector<Value_weak_ptr<GameObject>> m_vec_pockets;
		std::int32_t m_testPocketCount;

		bool m_isVibrate;
		float m_nearBorder;
		float m_vibration;
		float m_vibrationIncrease;
		float m_vibrationCapacity;
		float m_vibrationResistance;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy, true);