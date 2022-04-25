#pragma once
#include"stdafx.h"
#include"WaveDatas.h"
namespace ButiEngine {
	class Resource_Sound :public IResource_Sound
	{
	public:
		void Initialize()override;
		void PreInitialize()override;
		Value_ptr<WaveDatas> GetWavDatas();
	protected:
		Value_ptr<WaveDatas> vlp_wavData;
	};

}