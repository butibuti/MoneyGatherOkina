#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

class SceneChangeAnimationComponent : public GameComponent
{
public:
	std::string GetGameComponentName()const override {
		return "SceneChangeAnimationComponent";
	}
	void OnUpdate()override;
	void OnSet()override;
	void Start()override;
	void OnShowUI()override;
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
	}

	void SceneStart();
	void SceneEnd();
	void HidePanel();
	void ApparentPanel();
	bool IsAnimation();

private:
	void CreateHexagonPanel();

	std::vector<Value_weak_ptr<GameObject>> m_vec_vwp_hexagons;

	Vector3 m_defaultPosition;
	Vector3 m_space;

	std::int8_t m_panelCount;
};
}

BUTI_REGIST_GAMECOMPONENT(SceneChangeAnimationComponent, true);

