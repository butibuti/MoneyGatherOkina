#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Pocket;

	class Stick :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Stick";
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

		void SetPocket(Value_weak_ptr<GameObject> arg_vwp_pocket);
	private:
		Value_weak_ptr<GameObject> m_vwp_pocket;
	};

}

BUTI_REGIST_GAMECOMPONENT(Stick, true);