#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class TiltMotionObjectComponent :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "TiltMotionObjectComponent";
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

		void SetParent(Value_weak_ptr<GameObject> arg_vwp_parent) { m_vwp_parent = arg_vwp_parent; }
		Value_weak_ptr<GameObject> GetParent() { return m_vwp_parent; }
	private:
		Value_weak_ptr<GameObject> m_vwp_parent;
	};

}

BUTI_REGIST_GAMECOMPONENT(TiltMotionObjectComponent, true);

