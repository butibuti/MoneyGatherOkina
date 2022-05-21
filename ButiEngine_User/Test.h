#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
class TestBehavior :public GameComponent
{
public:

	std::string GetGameComponentName()const override {
		return "TestBehavior";
	}
	void OnUpdate()override;
	void Start()override;
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);

	}
private:

	Value_ptr<GameObject> vlp_UIObject;
};

}

BUTI_REGIST_GAMECOMPONENT(TestBehavior, true);