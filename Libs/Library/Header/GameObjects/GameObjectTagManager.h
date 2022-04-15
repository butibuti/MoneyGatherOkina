#pragma once
#include"../Library/ButiUtil/ButiUtil/ID.h"
namespace ButiEngine {
	class GameObject;
	using GameObjectTag= ID<GameObject>;

	class GameObjectTagManager
	{
	public:
		GameObjectTag CreateGameObjectTag(std::string arg_tagName);
		GameObjectTag GetObjectTag(std::string arg_tagName);
		std::string GetTagName(const GameObjectTag& arg_gameObj);
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(idManager);
		}
	private:
		IDManager<GameObject> idManager;
	};

	void OutputCereal(const Value_ptr<GameObjectTagManager>& v, const std::string& arg_filePath);

	void InputCereal(Value_ptr<GameObjectTagManager>& v,const std::string& arg_filePath );
}


namespace std {

	template<>
	struct hash<ButiEngine::GameObjectTag> {
	public:
		size_t operator()(const ButiEngine::GameObjectTag& data)const {

			std::size_t seed = 0;
			ButiEngine::hash_combine(seed, data.GetID());


			return seed;

		}
	};
}