#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class OutsideCrystal :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "OutsideCrystal";
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
		void Dead();
	private:
		void SetEnemyParameter();

		static std::int32_t m_progressPoint;
		static std::int32_t m_pocketCount;
		static float m_createPocketRadius;
		static float m_vibrationCapacity;
		static float m_vibrationResistance;
	};

}

BUTI_REGIST_GAMECOMPONENT(OutsideCrystal, true);