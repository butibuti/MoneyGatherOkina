#pragma once

#include "../GameObjects/GameComponent.h"
namespace ButiEngine {
using makeComponentFunction = Value_ptr<GameComponent>(*)();

class ComponentsLoader :public IObject {
public:

	static void CreateInstance();

	static  Value_ptr< ComponentsLoader> GetInstance();

	void Initialize() {}
	void PreInitialize() {}
	void Release();
	void AddGameComponent(Value_ptr<GameComponent> arg_gamecomponent);

	void RemoveComponent(const std::string& arg_name);

	Value_ptr<GameComponent> ShowAddGameComponentUI();

	template <typename T>
	void AddGameComponent() {
		AddGameComponent(ObjectFactory::Create<T>());
	}


	template <typename T>
	void RemoveComponent() {
		RemoveComponent(ObjectFactory::Create<T>()->GetGameComponentName());
	}


private:

	std::map<std::string, Value_ptr<GameComponent>> map_addGameComponents;
	Value_ptr<GameComponent> vlp_currentSelectObj;

	static Value_ptr< ComponentsLoader> instance;
};
}