#pragma once
#ifndef _Scene_H_
#define _Scene_H_
#include"stdafx.h"
namespace ButiEngine {

	Value_ptr<IScene> CreateScene(Value_weak_ptr<ISceneManager> arg_vwp_sceneManager, Value_ptr< SceneInformation> argSceneInformation);

}
#endif // !_Scene_H_

