#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ShockWave :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "ShockWave";
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

		void Appear();
		void Disappear();
	private:
		Value_weak_ptr<GameObject> m_vwp_player;

		Vector3 m_maxScale;
	};

}

BUTI_REGIST_GAMECOMPONENT(ShockWave, true);