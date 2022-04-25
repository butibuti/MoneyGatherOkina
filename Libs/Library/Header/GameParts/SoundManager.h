#pragma once
#include"stdafx.h"
#include"../Resources/WaveDatas.h"
namespace ButiEngine {

	Value_ptr<ISoundManager> CreateSoundManager(Value_weak_ptr<IApplication> arg_vwp_app);

	
}

