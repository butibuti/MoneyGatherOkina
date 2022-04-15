#pragma once
#include"WaveDatas.h"

namespace ButiEngine {

	class SoundHelper
	{
	public:
		~SoundHelper();

		static Value_ptr<WaveDatas> CreateFromFile(const std::string& filePath);
	private:

		SoundHelper();
	};
}

