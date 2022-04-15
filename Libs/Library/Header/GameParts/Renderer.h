#pragma once
#include"stdafx.h"
#include"../Resources/DrawData/IDrawObject.h"
namespace ButiEngine {
Value_ptr<IRenderer> CreateRenderer(Value_weak_ptr<IScene> arg_wkp_iscene);
}