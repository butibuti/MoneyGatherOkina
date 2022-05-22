#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class WarningMark :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "WarningMark";
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

		void SetParent(Value_weak_ptr<GameObject> arg_parent) { m_parent = arg_parent; }

		void Dead();
		void Appear();
		void Disappear();
	private:
		void AddScaleAnimation(const Vector3& arg_targetScale);

		Value_weak_ptr<GameObject> m_parent;
		bool m_isAppear;
	};

}

BUTI_REGIST_GAMECOMPONENT(WarningMark, true);