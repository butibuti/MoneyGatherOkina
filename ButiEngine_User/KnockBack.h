#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class KnockBack :public GameComponent
	{
	public:
		KnockBack(const Vector3& arg_dir, const float arg_force, const bool arg_isGravity, const std::int32_t arg_knockBackFrame);
		KnockBack(){}
		std::string GetGameComponentName()const override {
			return "KnockBack";
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
		Value_ptr<Timer> m_vlp_timer;
		Vector3 m_velocity;
		bool m_isGravity;
		float m_velocityY;
		Vector3 m_startVelocity;
	};

}

BUTI_REGIST_GAMECOMPONENT(KnockBack, true);