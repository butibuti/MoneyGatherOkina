#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class FloatMotionParent :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FloatMotionParent";
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

	private:
		Vector3 m_currentPosition;
		Vector3 m_previousPosition;

		std::int16_t m_moveChangeCount;

		bool m_moveChangeFlag;
	};

}

BUTI_REGIST_GAMECOMPONENT(FloatMotionParent, true);

