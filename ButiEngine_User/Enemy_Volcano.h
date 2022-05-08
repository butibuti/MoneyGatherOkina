#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Enemy_Volcano :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Enemy_Volcano";
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
	private:
		void ShotVolcanoRock();

		std::int32_t m_rockShotCount;
		std::int32_t m_rockShotRate;

		bool m_isHeatUp;

	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy_Volcano, true);

