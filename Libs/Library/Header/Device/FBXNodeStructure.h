#pragma once
#include"../ButiMath/ButiMath.h"
#include"ButiRendering_Dx12/Header/ModelAnimation.h"
namespace ButiEngine {
namespace FBXAnalyze {
enum class FBXPropertyDataType {
	Short = 'Y', Bool = 'C', Int = 'I', Float = 'F', Double = 'D', Long = 'L',
	FloatArray = 'f', DoubleArray = 'd', LongArray = 'l', IntArray = 'i', BoolArray = 'b', StringArray = 's', ShortArray = 'y',
	String = 'S', RawData = 'R',
};
struct IFBXProperty :IObject {
};

template<typename T>
static FBXPropertyDataType GetFBXPropertyType();
struct FBXGlobalSettings {
	char upAxis = 1;
	char upAxisSign = 1;
	char frontAxis = 2;
	char frontAxisSign = -1;
	char coordAxis = 0;
	char coordAxisSign = -1;
	float frameRate = 30.0f;
	std::int32_t timeMode = 0;
	std::int64_t timeSpanStart = 0;
	std::int64_t timeSpanStop = 0;
	std::int64_t timeStep = 0;
};
struct FBXNodeStructure :public IObject
{
public:
	std::unordered_multimap<std::string, Value_ptr< FBXNodeStructure>> multimap_childNodes;
	std::string recordName;
	std::uint32_t endOffset;
	std::uint32_t propertyListLen;
	std::uint32_t propertyCount;
	bool isEmpty = false;
	bool isParent = false;

	void PreInitialize()override {}
	bool NullRecordCheck();
	Vector3 GetVector3(const FBXGlobalSettings& settings);
	Vector3 GetRawVector3();
	Vector2 GetVector2();
	virtual void NodeInitialize() {}
	float GetFloat();
	virtual std::int64_t GetNodeTag();
	Value_weak_ptr< FBXNodeStructure>parent;
	std::multimap<FBXPropertyDataType, Value_ptr<IFBXProperty>> multimap_properties;
	List < Value_ptr<FBXNodeStructure>> searchChildNode(const std::string& arg_nodeName);
	List < Value_ptr<FBXNodeStructure>> searchChildNodeWithStringProperty(const std::string& arg_nodeName, const std::string& arg_stringProp, const std::uint32_t arg_propertyIndex = 0);
	Value_ptr<FBXNodeStructure> GetChildNode(const std::string& arg_nodeName, const std::uint32_t arg_index = 0);
	Value_ptr<FBXNodeStructure> GetChildNodeWithStringProperty(const std::string& arg_nodeName, const std::string& arg_stringProp, const std::uint32_t arg_propertyIndex, const std::uint32_t arg_index = 0);
	Value_ptr< FBXNodeStructure> GetChildPropertyNode(const std::string& arg_nodeName, const std::string& arg_propertyName);
	template <typename T>
	inline Value_ptr<T> GetProperty(const std::uint32_t arg_index = 0) {

		FBXPropertyDataType type = GetFBXPropertyType< T>();
		if (!multimap_properties.count(type)) {
			return nullptr;
		}
		auto itrs = multimap_properties.find(type);


		for (std::uint32_t i = 0; i < arg_index; i++) {
			itrs++;
		}


		return (itrs)->second->GetThis<T>();
	}

private:

};

enum class LinkType {
	OO, OP, PP
};
enum class AnimationType {
	rotate, translate, scaling
};

struct FBXNodeLinkInfo :public IObject {
	Value_ptr<FBXNodeStructure> serverNode;
	Value_ptr<FBXNodeStructure> resieverNode;
	std::string info = "none prop";
	LinkType linkType;
};
struct FBXConnectionNode :public FBXNodeStructure {
public:
	template<class T>
	Value_ptr< FBXNodeStructure>GetServeNode(List< Value_ptr< T>>& arg_toNode);

	template<class T>
	Value_ptr< FBXNodeStructure> GetResieveNode(List<Value_ptr< T>>& arg_byNodes);

	template<class T, class U>
	Value_ptr<FBXNodeLinkInfo> GetPairNode(List<Value_ptr< T>>& arg_byNodes, List<Value_ptr<U>>& arg_toNodes);

};

enum class NodeAttributeType {
	eUnknown, eNull, eMarker, eSkeleton, eMesh,
	eNurbs, ePatch, eCamera, eCameraStereo, eCameraSwitcher,
	eLight, eOpticalReference, eOpticalMarker, eNurbsCurve, eTrimNurbsSurface,
	eBoundary, eNurbsSurface, eShape, eLODGroup, eSubDiv, eCachedEffect, eLine,
};

struct FBXNode_End :public FBXNodeStructure {

};


struct FBXAttributeNode :public FBXNodeStructure {
	NodeAttributeType attributeType;
	void SetAttributeType();
};
struct FBXGeometryNode;
struct FBXModelNode;
struct FBXGlobalSettingsNode :public FBXNodeStructure {
	FBXGlobalSettings GetSettings();
};

struct FBXTextureNode :public FBXNodeStructure {

};
struct FBXMaterialNode :public FBXNodeStructure {

	std::int32_t index;
	List< Value_weak_ptr<FBXGeometryNode>> geometryNodes;
	Value_weak_ptr<FBXTextureNode> diffuseTextureNode;
	List< Value_weak_ptr<FBXModelNode>>  modelNodes;
};
struct FBXModelNode :public FBXNodeStructure {
	List< std::pair<std::int32_t, std::uint32_t>> vec_materialNodes;
	Value_weak_ptr<FBXGeometryNode> geometryNode;
	Matrix4x4 GetTransform(const FBXGlobalSettings& settings);
};
struct PoseNode :public FBXNodeStructure {
	std::int64_t GetNodeTag()override;
	Matrix4x4 GetMatrix();
};
struct FBXBoneNode :public FBXNodeStructure {
	std::string GetName();
	Vector3 GetPosition(const FBXGlobalSettings& settings);
	Vector3 GetRotation(const FBXGlobalSettings& settings);
	Matrix4x4 GetMatrix(const List<Value_ptr<FBXBoneNode>>& arg_bones, const FBXGlobalSettings& settings);
	Matrix4x4 GetMatrix();
	Matrix4x4 GetLocalMatrix(const List<Value_ptr<FBXBoneNode>>& arg_bones, const FBXGlobalSettings& settings);
	std::int32_t parentBoneIndex = -1;
	std::int32_t boneIndex = -1;
	Value_ptr<PoseNode> vlp_poseNode;
};
struct FBXDeformerNode :public FBXNodeStructure {
	List<double> GetWeight();
	List<std::int32_t> GetBoneIndex();
	std::int32_t boneIndex = -1;
};
struct FBXPolygonVertexIndexNode;

struct FBXGeometryNode :public FBXNodeStructure {

	Value_weak_ptr<FBXModelNode> modelNode;
	Value_ptr< FBXPolygonVertexIndexNode> GetPolygonVertexIndexNode();
};

struct FBXPolygonVertexIndexNode :public FBXNodeStructure {
private:
	List<std::int32_t> polygonIndex;
	List<std::int32_t> triPolygonIndex;
public:
	void NodeInitialize()override;
	List<std::int32_t>& GetPolygonIndex() {
		return polygonIndex;
	}
	List<std::int32_t>& GetTriPolygonIndex() {
		return triPolygonIndex;
	}
	List<std::int32_t> GetTriPolygonIndex(const std::int32_t offset);

	List<std::int32_t>& CreateTriPolygonIndexByPolyMaterial(const List<std::int32_t>& arg_vec_materialIndex, const std::uint32_t arg_materialCount, List<std::int32_t>& arg_materialIndexSize);
	List<std::int32_t>& CreateTriPolygonIndexByMonoMaterial();
};


struct FBXAnimationCurveData :FBXNodeStructure {
	void GetTimes(List<std::int32_t>& arg_ref_vec_time, const std::int64_t arg_timeStep);
};
struct FBXAnimationCurveNode :FBXNodeStructure {
	Value_ptr< FBXBoneNode> vlp_boneNode;
	Value_ptr<FBXAnimationCurveData> vlp_animCurve_x;
	Value_ptr<FBXAnimationCurveData> vlp_animCurve_y;
	Value_ptr<FBXAnimationCurveData> vlp_animCurve_z;
	void GetCurveData(List<Vector3*>& arg_vec_p_value, const FBXGlobalSettings& arg_globalSettings, const std::int32_t arg_localStop);
	AnimationType type;
};
struct TRSCurves {
	Value_ptr< FBXAnimationCurveNode> vlp_transCurve;
	Value_ptr< FBXAnimationCurveNode> vlp_rotateCurve;
	Value_ptr< FBXAnimationCurveNode> vlp_scaleCurve;
};
struct FBXAnimationLayer :FBXNodeStructure {

	void CreateMotionData(const List<Value_ptr<FBXBoneNode>>& arg_bones, std::map<std::string, List< ButiRendering::MotionKeyFrameData>>& arg_ref_map_motionDatas, const FBXGlobalSettings& arg_ref_globalSettings, const std::int64_t arg_localStop, const std::int64_t arg_refStop);
	std::map<std::string, TRSCurves> map_curves;
};

struct FBXAnimationStack :FBXNodeStructure {
	std::string GetAnimationName();
	std::int64_t GetLocalStop();
	std::int64_t GetRefStop();
	List<Value_ptr<FBXAnimationLayer>> vec_vlp_animLayer;
};
template<typename T>
struct FBXProperty :public IFBXProperty {
	FBXProperty() {}
	FBXProperty(const T& arg_prop) :nodeProperty(arg_prop) {}
	T nodeProperty;
};
template<typename T>
struct FBXProperty<List<T>> :public IFBXProperty {
	FBXProperty() {}
	FBXProperty(const List<T>& arg_prop) :nodeProperty(arg_prop) {}
	List<T> nodeProperty;
};
template<typename T> 
static FBXPropertyDataType GetFBXPropertyType() {
	if constexpr (std::is_same_v<T,FBXProperty<std::int32_t>>) { return FBXPropertyDataType::Int; }
	if constexpr (std::is_same_v<T,FBXProperty<std::int16_t>>) { return FBXPropertyDataType::Short; }
	if constexpr (std::is_same_v<T,FBXProperty<std::int64_t>>) { return FBXPropertyDataType::Long; }
	if constexpr (std::is_same_v<T,FBXProperty<std::string>>) { return FBXPropertyDataType::String; }
	if constexpr (std::is_same_v<T,FBXProperty<float>>) { return FBXPropertyDataType::Float; }
	if constexpr (std::is_same_v<T,FBXProperty<double>>) { return FBXPropertyDataType::Double; }
	if constexpr (std::is_same_v<T,FBXProperty<bool>>) { return FBXPropertyDataType::Bool; }
	if constexpr (std::is_same_v<T,FBXProperty<List<std::int32_t>>>) { return FBXPropertyDataType::IntArray; }
	if constexpr (std::is_same_v<T,FBXProperty<List<std::int16_t>>>) { return FBXPropertyDataType::ShortArray; }
	if constexpr (std::is_same_v<T,FBXProperty<List<std::int64_t>>>) { return FBXPropertyDataType::LongArray; }
	if constexpr (std::is_same_v<T,FBXProperty<List<std::string>>>) { return FBXPropertyDataType::StringArray; }
	if constexpr (std::is_same_v<T,FBXProperty<List<float>>>) { return FBXPropertyDataType::FloatArray; }
	if constexpr (std::is_same_v<T,FBXProperty<List<double>>>) { return FBXPropertyDataType::DoubleArray; }
	if constexpr (std::is_same_v<T,FBXProperty<List<bool>>>) { return FBXPropertyDataType::BoolArray; }
	if constexpr (std::is_same_v<T,FBXProperty<List<char>>>) { return FBXPropertyDataType::RawData; }
	assert(0 && "不明なプロパティ");
	return FBXPropertyDataType::Int;
}
using FBXNode_IntProperty = FBXProperty<std::int32_t>;
using FBXNode_LongProperty = FBXProperty<std::int64_t>;
using FBXNode_FloatProperty = FBXProperty<float>;
using FBXNode_StringProperty = FBXProperty<std::string>;


using FBXNode_DoubleProperty = FBXProperty<double>;
using FBXNode_ShortProperty = FBXProperty<std::int16_t>;
using FBXNode_BoolProperty = FBXProperty<bool>;
using FBXNode_RawDataProperty = FBXProperty<List<char>>;

using FBXNode_ShortArrayProperty = FBXProperty<List<std::int16_t>>;
using FBXNode_BoolArrayProperty = FBXProperty<List<bool>>;

using FBXNode_IntArrayProperty = FBXProperty<List<std::int32_t>>;
using FBXNode_LongArrayProperty = FBXProperty<List<std::int64_t>>;
using FBXNode_FloatArrayProperty = FBXProperty<List<float>>;
using FBXNode_DoubleArrayProperty = FBXProperty<List<double>>;

extern List< std::int32_t> GetPolygonVertexIndex(const FBXNode_IntArrayProperty& arg_prop);
extern List< std::int32_t> GetUVIndex(const FBXNode_IntArrayProperty& arg_prop);

extern List< Vector4> CreateVector4(const FBXNode_FloatArrayProperty& arg_prop);
extern List< Vector3> CreateVector3(const FBXNode_FloatArrayProperty& arg_prop);
extern List< Vector2> CreateVector2(const FBXNode_FloatArrayProperty& arg_prop);

extern List< Vector4> CreateVector4(const FBXNode_DoubleArrayProperty& arg_prop);
extern List< Vector3> CreateVector3(const FBXNode_DoubleArrayProperty& arg_prop, const FBXGlobalSettings& settings, const Matrix4x4* arg_transform = nullptr);
template <class T>
static List<T> CreateVertex(const FBXNode_DoubleArrayProperty& arg_prop,const FBXGlobalSettings& settings, const Matrix4x4* arg_transform = nullptr) {
	List<T> out;
	std::int32_t size = arg_prop. nodeProperty.GetSize() / 3;
	out.Reserve(size);

	for (std::int32_t i = 0; i < size; i++) {
		T vertex;
		vertex.position = Vector3(static_cast<float>(-1 * settings.coordAxisSign * arg_prop.nodeProperty.At(i * 3 + settings.coordAxis)),static_cast<float>(settings.upAxisSign * arg_prop.nodeProperty.At(i * 3 + settings.upAxis)), settings.frontAxisSign * static_cast<float>(arg_prop.nodeProperty.At(i * 3 + settings.frontAxis)));
		if (arg_transform) {
			vertex.position *= *arg_transform;
		}

		out.Add(vertex);
	}

	return out;

}
extern List< Vector3> CreateVector3Half_Latter(const FBXNode_DoubleArrayProperty& arg_prop);
extern List< Vector2> CreateVector2(const FBXNode_DoubleArrayProperty& arg_prop);
extern List< Vector2> CreateVector2Half_Latter(const FBXNode_DoubleArrayProperty& arg_prop);

class FBXScene :public IObject
{
public:
	std::unordered_multimap<std::string, Value_ptr< FBXNodeStructure>>& GetVec_NodeRecords() {
		return multimap_nodeRecords;
	}
	void SetNodeRecords(std::unordered_multimap<std::string, Value_ptr< FBXNodeStructure>> arg_nodeRecords) {
		multimap_nodeRecords = arg_nodeRecords;
	}
	List < Value_ptr< FBXNodeStructure>> searchNode(const std::string& arg_searchNodeName);
	List < Value_ptr< FBXNodeStructure>> searchNode(const std::string& arg_searchNodeName, const std::uint32_t searchPropertyCount);
	List < Value_ptr<FBXNodeStructure>> searchNodeWithStringProperty(const std::string& arg_nodeName, const std::string& arg_stringProp, const std::uint32_t arg_propertyIndex = 0);

	template <typename T>
	List < Value_ptr< T>> searchNode(const std::string& arg_searchNodeName) {
		List<Value_ptr<T>> output;
		auto search = searchNode(arg_searchNodeName);
		output.Reserve(search.GetSize());

		for (auto itr = search.begin(), endItr = search.end(); itr != endItr; itr++) {
			output.Add((*itr)->GetThis<T>());
		}
		return output;
	}
	template <typename T>
	List < Value_ptr< T>> searchNode(const std::string& arg_nodeName, const std::string& arg_stringProp, const std::uint32_t arg_propertyIndex = 0) {
		List<Value_ptr<T>> output;
		auto search = searchNodeWithStringProperty(arg_nodeName, arg_stringProp, arg_propertyIndex);
		output.Reserve(search.GetSize());

		for (auto itr = search.begin(), endItr = search.end(); itr != endItr; itr++) {
			output.Add((*itr)->GetThis<T>());
		}
		return output;
	}

private:
	std::unordered_multimap<std::string, Value_ptr< FBXNodeStructure>> multimap_nodeRecords;
};

namespace VertexIndexUtil {

inline List<std::int32_t> ConvertTriPolygon(List<std::int32_t>& arg_polygonVertex, std::int32_t offset = 0) {
	List<std::int32_t> out;
	out.Reserve(arg_polygonVertex.GetSize() * 2);
	List<std::int32_t> buffer;
	buffer.Resize(6);

	std::int32_t currentPolygonNum = 0;
	std::int32_t currentOffset = 0;
	for (auto itr = arg_polygonVertex.begin(); itr != arg_polygonVertex.end(); itr++) {
		buffer.at(currentPolygonNum) = currentOffset;
		currentPolygonNum++;
		if (*itr < 0) {
			switch (currentPolygonNum)
			{
			case 3:
				out.Add(buffer.at(2) + offset);
				out.Add(buffer.at(1) + offset);
				out.Add(buffer.at(0) + offset);
				break;
			case 4:
				out.Add(buffer.at(3) + offset);
				out.Add(buffer.at(1) + offset);
				out.Add(buffer.at(0) + offset);


				out.Add(buffer.at(3) + offset);
				out.Add(buffer.at(2) + offset);
				out.Add(buffer.at(1) + offset);
				//std::cout << "Quad polygon is out of support." << std::endl;
				break;
			default:
				break;
			}


			currentPolygonNum = 0;
		}
		currentOffset++;
	}
	return out;
}

inline List<std::int32_t> ConvertTriPolygon(List<std::int32_t>& arg_polygonVertex, const List<std::int32_t>& arg_vec_materialIndex, const std::uint32_t arg_materialCount, List<std::int32_t>& arg_materialIndexSize, std::int32_t offset = 0) {
	List<std::int32_t> out;
	List<std::int32_t> buffer;
	buffer.Resize(6);
	std::int32_t currentPolygonNum = 0;
	std::int32_t currentOffset = 0;

	auto matItr = arg_vec_materialIndex.begin();
	List<List<std::uint32_t>> materialIndices;
	materialIndices.Resize(arg_materialCount);

	for (std::int32_t i = 0; i < materialIndices.GetSize(); i++) {
		materialIndices.at(i).Reserve(arg_polygonVertex.GetSize() * 3);
	}

	for (auto itr = arg_polygonVertex.begin(); itr != arg_polygonVertex.end(); itr++) {
		buffer.at(currentPolygonNum) = currentOffset;
		currentPolygonNum++;
		if (*itr < 0) {
			switch (currentPolygonNum)
			{
			case 3:
				materialIndices.at(*matItr).Add(buffer.at(2) + offset);
				materialIndices.at(*matItr).Add(buffer.at(1) + offset);
				materialIndices.at(*matItr).Add(buffer.at(0) + offset);
				matItr++;
				break;
			case 4:
				materialIndices.at(*matItr).Add(buffer.at(3) + offset);
				materialIndices.at(*matItr).Add(buffer.at(1) + offset);
				materialIndices.at(*matItr).Add(buffer.at(0) + offset);


				materialIndices.at(*matItr).Add(buffer.at(3) + offset);
				materialIndices.at(*matItr).Add(buffer.at(2) + offset);
				materialIndices.at(*matItr).Add(buffer.at(1) + offset);
				matItr++;
				std::cout << "Quad polygon is out of support." << std::endl;
				break;
			default:
				break;
			}


			currentPolygonNum = 0;
		}
		currentOffset++;
	}

	out.Reserve(currentOffset);

	arg_materialIndexSize.Reserve(materialIndices.GetSize());

	for (std::int32_t i = 0; i < materialIndices.GetSize(); i++) {
		arg_materialIndexSize.Add(materialIndices.at(i).GetSize());
		std::copy(materialIndices.at(i).begin(), materialIndices.at(i).end(), std::back_inserter(out));
	}

	return out;
}
}

namespace UVUtil {

//true=Index_to_Direct
//false=Direct
inline bool GetUVReferenceMode(Value_ptr<FBXNodeStructure> arg_UVLayernode)
{
	std::string refType = arg_UVLayernode->GetChildNode("ReferenceInformationType")->GetProperty<FBXNode_StringProperty>()->nodeProperty;
	if (refType == "IndexToDirect")
	{
		return true;
	}
	else
	{
		return false;
	}
}



//true=Polygon_Vertex
//false=Controll_Point
inline bool GetUVMappingMode(Value_ptr<FBXNodeStructure> arg_UVLayernode) {
	std::string mapType = arg_UVLayernode->GetChildNode("MappingInformationType")->GetProperty<FBXNode_StringProperty>()->nodeProperty;
	if (mapType == "ByPolygonVertex")
	{
		return true;
	}
	else
	{
		return false;
	}
}


}

}
}

template<typename T>
inline ButiEngine::Value_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure> ButiEngine::FBXAnalyze::FBXConnectionNode::GetServeNode(List<Value_ptr<T>>& arg_toNode)
{
	std::int64_t key = GetProperty<FBXNode_LongProperty>()->nodeProperty;
	for (auto itr = arg_toNode.begin(); itr != arg_toNode.end(); itr++) {
		if ((*itr)->GetNodeTag() == key) {
			return (*itr);
		}
	}

	return nullptr;
}

template<typename T>
ButiEngine::Value_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure> ButiEngine::FBXAnalyze::FBXConnectionNode::GetResieveNode(List<Value_ptr<T>>& arg_byNodes)
{
	std::int64_t key = GetProperty<FBXNode_LongProperty>(1)->nodeProperty;
	for (auto itr = arg_byNodes.begin(); itr != arg_byNodes.end(); itr++) {
		if ((*itr)->GetNodeTag() == key) {
			return (*itr);
		}
	}

	return nullptr;
}

template<typename T, typename U>
inline ButiEngine::Value_ptr<ButiEngine::FBXAnalyze::FBXNodeLinkInfo> ButiEngine::FBXAnalyze::FBXConnectionNode::GetPairNode(List<Value_ptr<T>>& arg_byNodes, List<Value_ptr<U>>& arg_toNodes)
{
	Value_ptr<FBXNodeStructure> resiever = GetResieveNode(arg_byNodes);
	if (!resiever)
	{
		return nullptr;
	}
	Value_ptr<FBXNodeStructure> server = GetServeNode(arg_toNodes);
	if (!server) {
		return nullptr;
	}
	auto out = ObjectFactory::Create<FBXNodeLinkInfo>();
	out->serverNode = server;
	out->resieverNode = resiever;
	std::string linkType = GetProperty<FBXNode_StringProperty>(0)->nodeProperty;
	if (linkType == "OO")
	{
		out->linkType = LinkType::OO;
	}
	else if (linkType == "OP")
	{
		out->linkType = LinkType::OP;
		out->info = GetProperty<FBXNode_StringProperty>(1)->nodeProperty;
	}
	else {
		out->linkType = LinkType::PP;
	}

	return out;
}