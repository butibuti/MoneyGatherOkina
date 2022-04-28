#pragma once

namespace ButiEngine {
namespace ButiRendering {
class MotionKeyFrameData;
}
class ModelFileConverter
{
public:
	static void FBXtoB3M(const std::string& arg_fbxFileName, const std::string& arg_b3mFileName);
	static void PMXtoB3M(const std::string& arg_pmxFileName, const std::string& arg_b3mFileName);
	static void VMDtoBMD(const std::string& arg_vmdFileName, const std::string& arg_bmdFileName);
	static void CreateBMD(std::map<std::string, List<ButiRendering:: MotionKeyFrameData>>& arg_map_keyFrames, const std::string& arg_bmdFileName, const std::string& arg_fileDirectory = "");
private:

	struct Byte64 {
		unsigned char data[64];
	};

};

}