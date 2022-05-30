#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class TutorialCrystalSpawner :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "TutorialCrystalSpawner";
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

		std::int8_t GetCrystalCount() { return m_crystalCount; }

		void RemoveCrystal() { m_crystalCount--; }
	private:
		void CreateCrystals();

		std::int8_t m_initCrystalCount;
		std::int8_t m_crystalCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(TutorialCrystalSpawner, true);

