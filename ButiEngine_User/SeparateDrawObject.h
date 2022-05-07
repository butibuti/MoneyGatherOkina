#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SeparateDrawObject :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "SeparateDrawObject";
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

		Value_ptr<Transform> GetTransform()
		{ 
			if (!m_vwp_drawObject.lock()) { return nullptr; }
			return m_vwp_drawObject.lock()->transform; 
		}
		Value_weak_ptr<GameObject> CreateDrawObject(const std::string& arg_objectName);
	private:
		Value_weak_ptr<GameObject> m_vwp_drawObject;
	};

}

BUTI_REGIST_GAMECOMPONENT(SeparateDrawObject, true);