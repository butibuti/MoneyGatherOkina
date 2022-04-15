
#include "HitChecker.h"

//BUTI_REGIST_GAMECOMPONENT(HitChecker)
void ButiEngine::HitChecker::OnUpdate()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::HitChecker::Clone()
{
    return ObjectFactory::Create<HitChecker>();
}

void ButiEngine::HitChecker::OnCollision(Value_weak_ptr<GameObject> arg_other)
{
    hitObjName = arg_other.lock()->GetGameObjectName();
}

void ButiEngine::HitChecker::ShowGUI()
{
    GUI::Begin("HitChecker");
    if (hitObjName.size() > 0) {

        GUI::BulletText("Hit:"+ hitObjName);
        hitObjName.clear();
    }
    GUI::End();

}
