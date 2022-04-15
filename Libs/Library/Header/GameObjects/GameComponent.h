#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#pragma once
#include<memory>
namespace ButiEngine {
class GameComponent :public IObject
{
public:
	void Initialize()override {}
	void PreInitialize()override {}
	void Update();
	virtual void Start() {}
	void Set(Value_weak_ptr<GameObject> arg_wkp_gameObj);
	void SetIsRemove(bool arg_isRemove) {
		isRemove = arg_isRemove;
	}
	void SetIsActive(bool arg_isActive) {
		isActive = arg_isActive;
	}
	void ShowUI() {

		GUI::Checkbox("IsActive", &isActive);
		OnShowUI();
	}
	virtual void OnShowUI() {}
	virtual void OnSet();
	virtual void OnRemove();
	virtual void OnCollision(Value_weak_ptr<GameObject> arg_other) {}
	virtual void OnCollisionEnter(Value_weak_ptr<GameObject> arg_other) {}
	virtual void OnCollisionEnd(Value_weak_ptr<GameObject> arg_other) {}
	virtual std::string GetGameComponentName()const = 0;
	virtual Value_ptr<GameComponent> Clone() = 0;
	/// <summary>
	/// 型+名前でゲームコンポーネントの検索をするときに使う
	/// </summary>
	virtual bool CheckByName(const std::string&)const { return false; }

	Value_weak_ptr<GameObject> AddObject(Value_ptr<Transform> arg_transform, std::string arg_objectName = "GameObject");
	Value_weak_ptr<GameObject> AddObject(Value_ptr<GameObject> arg_gameObject);
	Value_weak_ptr<GameObject>AddObjectFromCereal(std::string filePath, Value_ptr<Transform> arg_transform = nullptr);
	bool IsRemove() {
		return isRemove;
	}
	bool IsActive() {
		return isActive;
	}
	bool IsOverrideOnUpdate();
protected:
	virtual void OnUpdate(){}
	Value_ptr<ICamera> GetCamera();
	Value_ptr<ICamera> GetCamera(const std::uint32_t arg_camNum);
	Value_ptr<ICamera> GetCamera(const std::string& arg_camName);
	Value_weak_ptr<GameObjectManager> GetManager();
	Value_weak_ptr<ICollisionManager> GetCollisionManager();
	Value_ptr<GameObjectTagManager> GetTagManager();
	Value_weak_ptr<GameObject> gameObject;

	bool isActive = true;
	bool isRemove = false;
private:
};

struct GameComponentRegister {
public:
	GameComponentRegister(Value_ptr<GameComponent> arg_gameComponent,const bool isOverrideOnUpdate);
};

void RegistGameComponentOverrideOnUpdate(const std::string& arg_componentName,const bool isOverride);

}
#endif