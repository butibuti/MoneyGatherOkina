#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpawnEffect :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "SpawnEffect";
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

		void SetColor(const Vector4& arg_color);
	private:
		Value_ptr<Timer> m_lifeTimer;

		Vector3 m_startScale;
		Vector3 m_targetScale;

		Vector4 color;
	};

}

BUTI_REGIST_GAMECOMPONENT(SpawnEffect, true);