#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class DrawObject :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "DrawObject";
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
	};

}

BUTI_REGIST_GAMECOMPONENT(DrawObject, true);

