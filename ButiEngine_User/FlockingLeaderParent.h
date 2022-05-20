#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class FlockingLeaderParent :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FlockingLeaderParent";
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

	private:
		void SetLookAtParameter();

		Value_weak_ptr<GameObject> m_vwp_player;
		Value_ptr<LookAtComponent> m_vlp_lookAt;
	};

}

BUTI_REGIST_GAMECOMPONENT(FlockingLeaderParent, true);