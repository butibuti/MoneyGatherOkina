#include "TestBehavior.h"
#include"Header/Common/CollisionPrimitive.h"


float gAcc = 0.98f; // 重力加速度
float deltaT = 0.00001f; // 時間刻み
float length1 = 3.0f; // 振り子1の長さ
float length2 = 2.0f; // 振り子2の長さ
float mass1 = 1.5f; // 振り子1の重さ
float mass2 = 0.5f; // 振り子2の重さ


float th1 = 3.0f; // 振り子1の角度
float th2 = -2.5f; // 振り子2の角度
float th1Dot = 0.0f; // 振り子1の角速度
float th2Dot = 0.0f; // 振り子2の角速度


void ButiEngine::TestBehavior::OnUpdate()
{
    float th1DotDot, th2DotDot;
    float a1, a2, b, d1, d2;

    // A1,A2の計算
    a1 = (mass1 + mass2) * length1 * length1;
    a2 = mass2 * length2 * length2;

    // アニメーションの1フレームあたりの処理
    for (std::int32_t skip = 0; skip < 5000; skip++) {
        // B,D1,D2の計算
        b = mass2 * length1 * length2 * cos(th1 - th2);
        d1 = -mass2 * length1 * length2 * th2Dot * th2Dot * sin(th1 - th2) - (mass1 + mass2) * gAcc * length1 * sin(th1);
        d2 = mass2 * length1 * length2 * th1Dot * th1Dot * sin(th1 - th2) - mass2 * gAcc * length2 * sin(th2);


        th1DotDot = (a2 * d1 - b * d2) / (a1 * a2 - b * b);
        th2DotDot = (a1 * d2 - b * d1) / (a1 * a2 - b * b);
        th1Dot += th1DotDot * deltaT;
        th2Dot += th2DotDot * deltaT;
        th1 += th1Dot * deltaT;
        th2 += th2Dot * deltaT;
    }
    shp_pendulum0->transform->SetWorldPosition(Vector3(length1 * sin(th1),- length1 * cos(th1), gameObject.lock()->transform->GetWorldZ()));
    shp_pendulum1->transform->SetWorldPosition(Vector3( length2 * sin(th2),- length2 * cos(th2),0)+shp_pendulum0->transform->GetWorldPosition());



}


void ButiEngine::TestBehavior::OnSet()
{
    th1 = MathHelper::ToRadian(ButiRandom::GetRandom<std::int32_t>(0, 360, 1));
    th2 = MathHelper::ToRadian(ButiRandom::GetRandom<std::int32_t>(0, 360, 1));
}

void ButiEngine::TestBehavior::Start()
{

    shp_pendulum0 = GetManager().lock()->AddObjectFromCereal("Particle", ObjectFactory::Create<Transform>(Vector3(), Vector3(), Vector3(0.8f))).lock();
    shp_pendulum1 = GetManager().lock()->AddObjectFromCereal("RedParticle", ObjectFactory::Create<Transform>(Vector3(),Vector3(),Vector3(0.8f))).lock();

}

void ButiEngine::TestBehavior::OnCollisionEnter(Value_weak_ptr<GameObject> arg_other)
{
	OnCollision(arg_other);
}

void ButiEngine::TestBehavior::OnCollision(Value_weak_ptr<GameObject> arg_other)
{

}

void ButiEngine::TestBehavior::OnRemove()
{
	
}

void ButiEngine::TestBehavior::OnShowUI()
{
    GUI::DragFloat("gAcc", gAcc, 0.01, 0, 1.0f);
    GUI::DragFloat("length1", length1, 0.1, 0, 50.0f);
    GUI::DragFloat("length2", length2, 0.1, 0, 50.0f);
    GUI::DragFloat("mass1", mass1, 0.1, 0, 50.0f);
    GUI::DragFloat("mass2", mass2, 0.1, 0, 50.0f);
    GUI::DragFloat("pendulum0", th1, 0.01f, -3.14f, 3.14f);
    GUI::DragFloat("pendulum2", th2, 0.01f, -3.14f, 3.14f);
    GUI::DragFloat("pendulum0acc", th1Dot, 0.01f, -3.14f, 3.14f);
    GUI::DragFloat("pendulum1acc", th2Dot, 0.01f, -3.14f, 3.14f);
}

void ButiEngine::TestBehavior::ShowGUI()
{

}

ButiEngine::Vector3 ButiEngine::TestBehavior::GetWorldPosition()
{
	return gameObject.lock()->transform->GetWorldPosition();
}

void ButiEngine::TestBehavior::AdditionMass(const float arg_mass)
{
	mass += arg_mass;

	if (shp_befPoint) {
		shp_befPoint->AdditionMass(mass);
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TestBehavior::Clone()
{
	auto clone = ObjectFactory::Create<TestBehavior>();
	clone->mass = mass;
	clone->acc = acc;
	clone->attenuation = attenuation;
	clone->bounce = bounce;
	clone->leaderSpeed = leaderSpeed;
	clone->speed = speed;
	return clone;
}