#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;
	class WaveManager;
	class VibrationEffectComponent;

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

		void SetNearBorder(const float arg_nearBorder);
		void SetVibrationCapacity(const float arg_capacity) { m_vibrationCapacity = arg_capacity; }
		void SetVibrationResistance(const float arg_resistance) { m_vibrationResistance = arg_resistance; }
		void SetExplosionScale(const float arg_scale) { m_explosionScale = arg_scale; }
		void SetIsNearPlayer(const bool arg_isNearPlayer) { m_isNearPlayer = arg_isNearPlayer; }

		Value_weak_ptr<GameObject> GetPlayer() { return m_vwp_player; }
		Value_weak_ptr<GameObject> GetNearFreePocket(const Vector3& arg_pos, float arg_border);

		std::vector<Value_weak_ptr<GameObject>> GetStickWorkers();

		bool IsVibrate();

		void CreatePocket(const std::uint8_t arg_pocketCount);
		void RemovePocket(const std::uint8_t arg_pocketNum);
	private:
		void IncreaseVibration();
		void DecreaseVibration();
		void VibrationStickWoker();

		void Explosion();

		//êUìÆó ÇÃè„è∏ílÇåvéZÇ∑ÇÈ
		void CalculateVibrationIncrease();
		std::uint8_t GetStickWorkerCount();
		void RemoveAllPocket();
		void SubDeadCount();
		void StopVibrationEffect();

		//êUìÆÇÃå∏è≠ó 
		static float m_vibrationDecrease;

		Value_weak_ptr<GameObject> m_vwp_player;
		Value_weak_ptr<WaveManager> m_vwp_waveManager;
		Value_weak_ptr<GameObject> m_vwp_vibrationEffect;
		Value_weak_ptr<VibrationEffectComponent> m_vwp_vibrationEffectComponent;
		Value_ptr<Player> m_vlp_playerComponent;
		Value_weak_ptr<GameObject> m_vwp_playerSensor;

		std::vector<Value_weak_ptr<GameObject>> m_vec_pockets;
		std::int32_t m_testPocketCount;

		bool m_isNearPlayer;
		bool m_isHitShockWave;
		Vector3 m_defaultScale;

		float m_vibration;
		float m_vibrationIncrease;
		float m_vibrationCapacity;
		float m_vibrationResistance;

		float m_explosionScale;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy, true);