#pragma once
#include<mutex>
namespace ButiEngine {
class IScene;
class GameObjectManager :public IObject
{
	friend class GameObject;
public:
	GameObjectManager(Value_weak_ptr<IScene> arg_vwp_scene);
	GameObjectManager() {}
	void SetScene(Value_weak_ptr<IScene> arg_vwp_scene);
	void Update();
	void RegistNewGameObject();
	void RegistGameObject();
	void Initialize()override;
	void PreInitialize()override;
	void ShowGUI();
	void Release();
	void Initialize_cereal();

	Value_weak_ptr<GameObject> GetSelectedUI();

	Value_weak_ptr<GameObject> AddObject(Value_ptr<Transform> arg_transform, std::string arg_objectName = "GameObject");
	Value_weak_ptr<GameObject> AddObject(Value_ptr<GameObject> arg_gameObject);
	Value_weak_ptr<GameObject>AddObjectFromCereal(std::string filePath, Value_ptr<Transform> arg_transform = nullptr);
	Value_weak_ptr<GameObject>AddObjectFromCereal_Insert(std::string filePath, Value_ptr<Transform> arg_transform = nullptr);

	std::string ReNameGameObject(const std::string& arg_ObjectName, const std::string& arg_befObjectName);

	Value_weak_ptr<GameObject> GetGameObject(const std::string& arg_objectName);
	Value_weak_ptr<GameObject> GetGameObject(const GameObjectTag& arg_objectTag);
	std::vector<Value_ptr<GameObject>> GetGameObjects(const GameObjectTag& arg_objectTag);

	void Start();
	void End();

	Value_weak_ptr<IScene> GetScene() {
		return vwp_scene;
	}

	Value_weak_ptr<IApplication> GetApplication();

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(vec_gameObjects);
		archive(map_gameObjects);
	}
	void CreateViewGameObjectVector(const std::string& arg_serchStr, const std::vector<ButiEngine::GameObjectTag>& arg_vec_serchTags);
	std::vector<std::string> GetGameObjectList()const;
	void SetSelectedGameObject(Value_weak_ptr<GameObject> arg_obj) { selectedGameObject = arg_obj; }
private:
	void UnRegistGameObject(Value_ptr<GameObject> arg_gameObject);
	void CreateViewGameObjectVector();
	std::mutex mtx_addGameObject;
	std::vector<Value_ptr<GameObject>> vec_gameObjects;
	Value_ptr<std::vector<Value_ptr<GameObject>>> vlp_vec_viewGameObjects;
	std::vector<Value_ptr<GameObject>> vec_newGameObjects;
	std::unordered_map<std::string, Value_weak_ptr<GameObject>> map_gameObjects;
	Value_weak_ptr<IScene> vwp_scene;
	Value_weak_ptr<GameObject> selectedGameObject;
	std::string eventRegistKey;
};

void OutputCereal(const Value_ptr<GameObjectManager>& v, const std::string& path);

void InputCereal(Value_ptr<GameObjectManager>& v, const std::string& path);
}