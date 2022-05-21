#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class MoveAnimationComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "MoveAnimationComponent";
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

		void SetStartPosition(const Vector3& arg_position) 
		{ 
			m_startPosition = arg_position; 
			m_currentPosition = arg_position;
		}
		void SetEndPosition(const Vector3& arg_position) { m_endPosition = arg_position; }
		void SetIsActive(const bool arg_isActive) { m_isActive = arg_isActive; }
		void SetSpeed(const float arg_speed) { m_speed = arg_speed; }
		void SetIsShake(const bool arg_isShake) { m_isShake = arg_isShake; }

	private:
		Vector3 m_startPosition;
		Vector3 m_endPosition;
		Vector3 m_currentPosition;

		float m_borderDistance;
		float m_speed;

		bool m_isActive;
		bool m_isShake;
	};

}

BUTI_REGIST_GAMECOMPONENT(MoveAnimationComponent, true);