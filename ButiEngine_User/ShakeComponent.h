#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class ShakeComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "ShakeComponent";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		void ShakeStart();
		void ShakeStop();
		void SetShakePower(const float arg_amplitude);
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		Vector3 m_currentRotate;
		Vector3 m_moveRotate;
		Vector3 m_currentPos;
		Vector3 m_movePos;
		float m_amplitude;
		bool m_isShake;
		bool m_isMove;
	};

}

BUTI_REGIST_GAMECOMPONENT(ShakeComponent, true);

