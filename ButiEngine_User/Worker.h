#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class VibrationEffectComponent;

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

		void SetVibration(const bool arg_isVibration) { m_isVibration = arg_isVibration; }

		void Dead();

	private:
		void OnCollisionStalker(Value_weak_ptr<GameObject> arg_vwp_other);
		void OnCollisionEnemy(Value_weak_ptr<GameObject> arg_vwp_enemy);

		void StopVibrationEffect();

		void SetLookAtParameter();

		static float m_nearBorder;
		static float m_vibrationForce;

		Value_weak_ptr<GameObject> m_vwp_vibrationEffect;
		Value_weak_ptr<VibrationEffectComponent> m_vwp_vibrationEffectComponent;
		Value_ptr<LookAtComponent> m_vlp_lookAt;

		Vector3 m_defaultScale;

		bool m_isVibration;
	};

}

BUTI_REGIST_GAMECOMPONENT(Worker, true);