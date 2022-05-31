#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class TutorialCrystalSpawner;

	class TutorialCrystal :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "TutorialCrystal";
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
		void SetIsLast(const bool arg_isLast) { m_isLast = arg_isLast; }
		Vector4 GetColor() { return m_color; }
	private:
		void SetEnemyParameter();

		static std::int32_t m_progressPoint;
		static std::int32_t m_pocketCount;
		static float m_createPocketRadius;
		static float m_vibrationCapacity;
		static float m_vibrationResistance;

		Value_weak_ptr<TutorialCrystalSpawner> m_vwp_crystalSpawner;

		bool m_isLast;
		Vector4 m_color;
	};

}

BUTI_REGIST_GAMECOMPONENT(TutorialCrystal, true);