#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Pocket :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Pocket";
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

		bool HasWorker() { return m_hasWorker; }
	private:
		bool m_hasWorker;
	};

}

BUTI_REGIST_GAMECOMPONENT(Pocket, true);