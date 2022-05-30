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

		void SetParent(Value_weak_ptr<GameObject> arg_parent) { m_vwp_parent = arg_parent; }
		Value_weak_ptr<GameObject> GetParent() { return m_vwp_parent; }

		void Disappear();
		void SetScale(const float arg_vibrationPower);

	private:
		Value_weak_ptr<GameObject> m_vwp_parent;
		Vector3 m_maxScale;
		Vector3 m_calcScale;

	};

}

BUTI_REGIST_GAMECOMPONENT(ShockWave, true);