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
		void ShakeStart(const float arg_amplitude);
		void ShakeStop();
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:

	};

}

BUTI_REGIST_GAMECOMPONENT(ShakeComponent, true);

