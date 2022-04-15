#pragma once

#include"stdafx.h"

namespace ButiEngine {
namespace Collision {
struct CollisionLayerInfo {

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(size);
		archive(offset);
		archive(level);
		archive(isCheckSame);
		archive(vec_checkOtherLayerIndex);
	}
	Vector3 size;
	Vector3 offset;
	std::int32_t level;
	bool isCheckSame;
	std::vector<std::int32_t> vec_checkOtherLayerIndex;
};
struct ColiisionLayerLoadData {
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(vec_collisionLayerInfo);
	}
	std::vector<CollisionLayerInfo> vec_collisionLayerInfo;
};

Value_ptr<ICollisionManager> CreateCollisionManager(const std::int32_t arg_layerCount);

}


void OutputCereal(const Collision:: ColiisionLayerLoadData& v,  const std::string& path);

void InputCereal(Collision::ColiisionLayerLoadData& v, const std::string& path);
}