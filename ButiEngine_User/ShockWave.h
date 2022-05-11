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

		void Disappear();
		void SetScale(const float arg_vibrationPower);

	private:
		Vector3 m_maxScale;
		Vector3 m_calcScale;

	};

}

BUTI_REGIST_GAMECOMPONENT(ShockWave, true);