#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Explosion :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Explosion";
		}
		void OnUpdate()override;
		void OnSet()override;
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

BUTI_REGIST_GAMECOMPONENT(Explosion, true);