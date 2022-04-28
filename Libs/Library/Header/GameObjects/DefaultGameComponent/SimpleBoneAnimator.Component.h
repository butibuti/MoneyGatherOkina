#pragma once
#include"MeshDrawComponent.h"
namespace ButiEngine {
namespace ButiRendering {
class ModelAnimation;
}

class IModelObject;
class SimpleBoneAnimatorComponent :public GameComponent {
public:
	SimpleBoneAnimatorComponent() {}
	void OnUpdate()override;
	void AddAnimation(Value_ptr<ButiRendering::ModelAnimation> arg_vlp_animation);
	void AddAnimation(MotionTag arg_motionTag);
	void SetLoop(const bool arg_isLoop);
	void OnShowUI()override;
	void Start()override;
	void OnSet()override;
	void SetModelData();
	std::string GetGameComponentName()const override {
		return "SimpleBoneAnimatorComponent";
	}
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(tag);
		archive(isLoop);
		archive(pase);
	}
	Value_ptr<GameComponent> Clone()override;
private:
	Value_ptr<ButiRendering::ModelAnimation> vlp_animation;
	Value_ptr<ButiRendering::IModelObject > vlp_modelData;
	MotionTag tag;
	bool isLoop;
	float pase;
};
}


BUTI_REGIST_GAMECOMPONENT(SimpleBoneAnimatorComponent,true)