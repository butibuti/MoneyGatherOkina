#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

enum class MovePattern {
	Rest, ConstantVelocity, ConstantAccel, Equation, FreeFall
};

class FluidSimulation :public GameComponent
{
public:

	struct Particle
	{
		ButiEngine::Vector3 v;
		float rho=0;
		float sum=0;
		float p=0;
		ButiEngine::Vector3 f;
		ButiEngine::Value_ptr<ButiEngine::Transform> shp_objectTransform;
	};
	std::string GetGameComponentName()const override {
		return "FluidSimulation";
	}
	void OnUpdate()override;
	void OnSet()override;
	void Start()override;
	void OnCollisionEnter(Value_weak_ptr<GameObject> arg_other)override;
	void OnCollision(Value_weak_ptr<GameObject> arg_other)override;
	void OnRemove()override;
	void OnShowUI()override;
	void ShowGUI()override;
	Vector3 GetWorldPosition();
	void AdditionMass(const float arg_mass);
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(isLeader);
		archive(leaderSpeed);
		archive(speed);
		archive(bounce);
		archive(attenuation);
		archive(acc);
		archive(mass);
		archive(inertia);
		archive(shp_befPoint);

	}
private:

	float bounce, speed, mass = 1.0f, attenuation, leaderSpeed, inertia;
	bool isLeader;
	Vector3 velocity, fixVelocity, acc;

	Value_ptr<RelativeTimer>shp_timer;
	Value_ptr<IResource_Mesh> shp_stageMesh;

	std::vector<Value_ptr<GameObject>> vec_shp_gameObject;
	std::vector<ButiEngine::FluidSimulation::Particle> vec_particle;
	Value_ptr<FluidSimulation> shp_befPoint;



	//Value_ptr<Transform> shp_floorTransform;
};

}

BUTI_REGIST_GAMECOMPONENT(FluidSimulation,true);