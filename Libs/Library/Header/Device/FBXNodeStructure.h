#pragma once
#include"../ButiMath/ButiMath.h"
#include"Header/Resources/ModelAnimation.h"
namespace ButiEngine {
	namespace FBXAnalyze {
		enum class FBXPropertyDataType {
			Short = 'Y', Bool = 'C', Int = 'I', Float = 'F', Double = 'D',Long='L',
			FloatArray = 'f', DoubleArray = 'd', LongArray = 'l', IntArray = 'i', BoolArray = 'b',
			String = 'S', RawData='R',
		};
		struct FBXProperty :IObject {
			void Initialize()override {};
			void PreInitialize()override {};
		};
		struct FBXGlobalSettings {
			char upAxis = 1;
			char upAxisSign = 1;
			char frontAxis = 2;
			char frontAxisSign = -1;
			char coordAxis = 0;
			char coordAxisSign = -1;
			float frameRate=30.0f;
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
			bool isParent=false;
			void Initialize()override {}
			void PreInitialize()override{}
			bool NullRecordCheck();
			Vector3 GetVector3(const FBXGlobalSettings& settings);
			Vector3 GetRawVector3();
			Vector2 GetVector2();
			virtual void NodeInitialize() {}
			float GetFloat();
			virtual std::int64_t GetNodeTag();
			Value_weak_ptr< FBXNodeStructure>parent;
			std::multimap<FBXPropertyDataType,Value_ptr<FBXProperty>> multimap_properties;
			std::vector < Value_ptr<FBXNodeStructure>> searchChildNode(const std::string& arg_nodeName);
			std::vector < Value_ptr<FBXNodeStructure>> searchChildNodeWithStringProperty(const std::string& arg_nodeName, const std::string& arg_stringProp, const std::uint32_t arg_propertyIndex=0);
			Value_ptr<FBXNodeStructure> GetChildNode(const std::string& arg_nodeName, const std::uint32_t arg_index = 0);
			Value_ptr<FBXNodeStructure> GetChildNodeWithStringProperty(const std::string& arg_nodeName, const std::string& arg_stringProp, const std::uint32_t arg_propertyIndex, const std::uint32_t arg_index = 0);
			Value_ptr< FBXNodeStructure> GetChildPropertyNode(const std::string& arg_nodeName, const std::string& arg_propertyName);
			template <typename T>
			inline Value_ptr<T> GetProperty(const std::uint32_t arg_index=0) {

				FBXPropertyDataType type = T::GetType();
				if (!multimap_properties.count(type)) {
					return nullptr;
				}
				auto itrs= multimap_properties.find(type);

				
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
			rotate,translate,scaling
		};

		struct FBXNodeLinkInfo :public IObject {
			void Initialize()override {}
			void PreInitialize()override {}
			Value_ptr<FBXNodeStructure> serverNode;
			Value_ptr<FBXNodeStructure> resieverNode;
			std::string info="none prop";
			LinkType linkType;
		};
		struct FBXConnectionNode :public FBXNodeStructure {
		public:
			template<class T>
			Value_ptr< FBXNodeStructure>GetServeNode(std::vector< Value_ptr< T>>& arg_toNode);

			template<class T>
			Value_ptr< FBXNodeStructure> GetResieveNode(std::vector<Value_ptr< T>>& arg_byNodes);
			
			template<class T,class U>
			Value_ptr<FBXNodeLinkInfo> GetPairNode(std::vector<Value_ptr< T>>& arg_byNodes, std::vector<Value_ptr<U>>& arg_toNodes);
			
		};
		
		enum class NodeAttributeType {
			eUnknown,eNull,	eMarker,eSkeleton,eMesh,
			eNurbs,	ePatch,	eCamera,eCameraStereo,eCameraSwitcher,
			eLight,	eOpticalReference,eOpticalMarker,eNurbsCurve,eTrimNurbsSurface,
			eBoundary,eNurbsSurface,eShape,eLODGroup,eSubDiv,eCachedEffect,eLine,
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
			std::vector< Value_weak_ptr<FBXGeometryNode>> geometryNodes; 
			Value_weak_ptr<FBXTextureNode> diffuseTextureNode;
			std::vector< Value_weak_ptr<FBXModelNode>>  modelNodes;
		};
		struct FBXModelNode :public FBXNodeStructure {
			std::vector< std::pair<std::int32_t, std::uint32_t>> vec_materialNodes;
			Value_weak_ptr<FBXGeometryNode> geometryNode;
			Matrix4x4 GetTransform(const FBXGlobalSettings& settings);
		};
		struct PoseNode :public FBXNodeStructure {
			std::int64_t GetNodeTag()override;
			Matrix4x4 GetMatrix();
		};
		struct FBXBoneNode :public FBXNodeStructure {
			std::string GetName();
			Vector3 GetPosition( const FBXGlobalSettings& settings);
			Vector3 GetRotation( const FBXGlobalSettings& settings);
			Matrix4x4 GetMatrix(const std::vector<Value_ptr<FBXBoneNode>>& arg_bones, const FBXGlobalSettings& settings);
			Matrix4x4 GetMatrix();
			Matrix4x4 GetLocalMatrix(const std::vector<Value_ptr<FBXBoneNode>>& arg_bones, const FBXGlobalSettings& settings);
			std::int32_t parentBoneIndex = -1;
			std::int32_t boneIndex = -1;
			Value_ptr<PoseNode> shp_poseNode;
		};
		struct FBXDeformerNode :public FBXNodeStructure {
			std::vector<double> GetWeight();
			std::vector<std::int32_t> GetBoneIndex();
			std::int32_t boneIndex = -1;
		};
		struct FBXPolygonVertexIndexNode;

		struct FBXGeometryNode :public FBXNodeStructure {

			Value_weak_ptr<FBXModelNode> modelNode;
			Value_ptr< FBXPolygonVertexIndexNode> GetPolygonVertexIndexNode();
		};

		struct FBXPolygonVertexIndexNode :public FBXNodeStructure {
		private:
			std::vector<std::int32_t> polygonIndex;
			std::vector<std::int32_t> triPolygonIndex;
		public:
			void NodeInitialize()override;
			std::vector<std::int32_t>& GetPolygonIndex() {
				return polygonIndex;
			}
			std::vector<std::int32_t>& GetTriPolygonIndex() {
				return triPolygonIndex;
			}
			std::vector<std::int32_t> GetTriPolygonIndex(const std::int32_t offset);

			std::vector<std::int32_t>& CreateTriPolygonIndexByPolyMaterial(const std::vector<std::int32_t>& arg_vec_materialIndex, const std::uint32_t arg_materialCount, std::vector<std::int32_t>& arg_materialIndexSize);
			std::vector<std::int32_t>& CreateTriPolygonIndexByMonoMaterial();
		};


		struct FBXAnimationCurveData :FBXNodeStructure {
			void GetTimes(std::vector<std::int32_t>& arg_ref_vec_time,const std::int64_t arg_timeStep);
		};
		struct FBXAnimationCurveNode :FBXNodeStructure {
			Value_ptr< FBXBoneNode> shp_boneNode;
			Value_ptr<FBXAnimationCurveData> shp_animCurve_x;
			Value_ptr<FBXAnimationCurveData> shp_animCurve_y;
			Value_ptr<FBXAnimationCurveData> shp_animCurve_z;
			void GetCurveData(std::vector<Vector3*>& arg_vec_p_value,const FBXGlobalSettings& arg_globalSettings,const std::int32_t arg_localStop);
			AnimationType type;
		};
		struct TRSCurves {
			Value_ptr< FBXAnimationCurveNode> shp_transCurve;
			Value_ptr< FBXAnimationCurveNode> shp_rotateCurve;
			Value_ptr< FBXAnimationCurveNode> shp_scaleCurve;
		};
		struct FBXAnimationLayer :FBXNodeStructure {

			void CreateMotionData(const std::vector<Value_ptr<FBXBoneNode>>& arg_bones, std::map<std::wstring, std::vector< MotionKeyFrameData>>& arg_ref_map_motionDatas, const FBXGlobalSettings& arg_ref_globalSettings, const std::int64_t arg_localStop, const std::int64_t arg_refStop);
			std::map<std::string, TRSCurves> map_curves;
		};

		struct FBXAnimationStack :FBXNodeStructure {
			std::string GetAnimationName();
			std::int64_t GetLocalStop();
			std::int64_t GetRefStop();
			std::vector<Value_ptr<FBXAnimationLayer>> vec_shp_animLayer;
		};

		struct FBXNode_IntProperty :public FBXProperty {
			std::int32_t nodeProperty = 0;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::Int;
			}
		};
		struct FBXNode_LongProperty :public FBXProperty {
			std::int64_t nodeProperty = 0;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::Long;
			}
		};
		struct FBXNode_FloatProperty :public FBXProperty {
			float nodeProperty = 0.0f;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::Float;
			}
		};
		struct FBXNode_StringProperty :public FBXProperty {
			FBXNode_StringProperty(const std::string& propertyString) {
				nodeProperty = propertyString;
			}
			FBXNode_StringProperty() {}
			std::string nodeProperty = "";
			static FBXPropertyDataType GetType() {
				return FBXPropertyDataType::String;
			}
		};


		struct FBXNode_DoubleProperty :public FBXProperty {
			double nodeProperty = 0.0;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::Double;
			}
		};
		struct FBXNode_ShortProperty :public FBXProperty {
			std::int16_t nodeProperty = 0;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::Short;
			}
		};
		struct FBXNode_BoolProperty :public FBXProperty {
			bool nodeProperty = 0;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::Bool;
			}
		};
		struct FBXNode_RawDataProperty :public FBXProperty {
			std::vector<char> nodeProperty;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::RawData;
			}
		};

		struct FBXNode_IntArrayProperty :public FBXProperty {
			std::vector< std::int32_t> nodeProperty;
			std::vector< std::int32_t> GetPolygonVertexIndex();
			std::vector< std::int32_t> GetUVIndex();
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::IntArray;
			}
		};
		struct FBXNode_LongArrayProperty :public FBXProperty {
			std::vector<std::int64_t> nodeProperty;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::LongArray;
			}
		};
		struct FBXNode_FloatArrayProperty :public FBXProperty {
			std::vector<float> nodeProperty;
			std::vector< Vector4> CreateVector4();
			std::vector< Vector3> CreateVector3();
			std::vector< Vector2> CreateVector2();
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::FloatArray;
			}
		};
		struct FBXNode_DoubleArrayProperty :public FBXProperty {
			std::vector< double> nodeProperty;
			std::vector< Vector4> CreateVector4();
			std::vector< Vector3> CreateVector3(const FBXGlobalSettings& settings, const Matrix4x4* arg_transform = nullptr);
			template <class T>
			std::vector<T> CreateVertex(const FBXGlobalSettings& settings, const Matrix4x4* arg_transform = nullptr) {
				std::vector<T> out;
				std::int32_t size = nodeProperty.size() / 3;
				out.reserve(size);

				for (std::int32_t i = 0; i < size; i++) {
					T vertex;
					vertex.position = Vector3((float)-1 * settings.coordAxisSign * nodeProperty.at(i * 3 + settings.coordAxis), (float)settings.upAxisSign * nodeProperty.at(i * 3 + settings.upAxis), settings.frontAxisSign * (float)nodeProperty.at(i * 3 + settings.frontAxis));
					if (arg_transform) {
						vertex.position *= *arg_transform;
					}

					out.push_back(vertex);
				}

				return out;

			}
			std::vector< Vector3> CreateVector3Half_Latter();
			std::vector< Vector2> CreateVector2();
			std::vector< Vector2> CreateVector2Half_Latter();
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::DoubleArray;
			}
		};
		struct FBXNode_ShortArrayProperty :public FBXProperty {
			std::vector< std::int16_t> nodeProperty;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::Short;
			}
		};
		struct FBXNode_BoolArrayProperty :public FBXProperty {
			std::vector< bool> nodeProperty;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::BoolArray;
			}
		};
		class FBXScene:public IObject
		{
		public:
			void Initialize()override {}
			void PreInitialize()override{}
			std::unordered_multimap<std::string, Value_ptr< FBXNodeStructure>>& GetVec_NodeRecords() {
				return multimap_nodeRecords;
			}
			void SetNodeRecords(std::unordered_multimap<std::string, Value_ptr< FBXNodeStructure>> arg_nodeRecords) {
				multimap_nodeRecords = arg_nodeRecords;
			}
			std::vector < Value_ptr< FBXNodeStructure>> searchNode(const std::string& arg_searchNodeName);
			std::vector < Value_ptr< FBXNodeStructure>> searchNode(const std::string& arg_searchNodeName,const std::uint32_t searchPropertyCount);
			std::vector < Value_ptr<FBXNodeStructure>> searchNodeWithStringProperty(const std::string& arg_nodeName, const std::string& arg_stringProp, const std::uint32_t arg_propertyIndex = 0);

			template <typename T>
			std::vector < Value_ptr< T>> searchNode(const std::string& arg_searchNodeName) {
				std::vector<Value_ptr<T>> output;
				auto search = searchNode(arg_searchNodeName);
				output.reserve(search.size());

				auto endItr = search.end();
				for (auto itr = search.begin(); itr != endItr; itr++) {
					output.push_back((*itr)->GetThis<T>());
				}
				return output;
			}
			template <typename T>
			std::vector < Value_ptr< T>> searchNode(const std::string& arg_nodeName, const std::string& arg_stringProp, const std::uint32_t arg_propertyIndex = 0) {
				std::vector<Value_ptr<T>> output;
				auto search = searchNodeWithStringProperty(arg_nodeName,arg_stringProp,arg_propertyIndex);
				output.reserve(search.size());

				auto endItr = search.end();
				for (auto itr = search.begin(); itr != endItr; itr++) {
					output.push_back((*itr)->GetThis<T>());
				}
				return output;
			}

		private:
			std::unordered_multimap<std::string, Value_ptr< FBXNodeStructure>> multimap_nodeRecords;
		};

		namespace VertexIndexUtill {

			inline std::vector<std::int32_t> ConvertTriPolygon(std::vector<std::int32_t>& arg_polygonVertex, std::int32_t offset = 0) {
				std::vector<std::int32_t> out;
				out.reserve(arg_polygonVertex.size() * 2);
				std::vector<std::int32_t> buffer;
				buffer.resize(6);

				std::int32_t currentPolygonNum = 0;
				std::int32_t currentOffset = 0;
				for (auto itr = arg_polygonVertex.begin(); itr != arg_polygonVertex.end(); itr++) {
					buffer.at(currentPolygonNum) = currentOffset;
					currentPolygonNum++;
					if (*itr < 0) {
						switch (currentPolygonNum)
						{
						case 3:
							out.push_back(buffer.at(2) + offset);
							out.push_back(buffer.at(1) + offset);
							out.push_back(buffer.at(0) + offset);
							break;
						case 4:
							out.push_back(buffer.at(3) + offset);
							out.push_back(buffer.at(1) + offset);
							out.push_back(buffer.at(0) + offset);


							out.push_back(buffer.at(3) + offset);
							out.push_back(buffer.at(2) + offset);
							out.push_back(buffer.at(1) + offset);
							//std::cout << "Quad polygon is out of support." << std::endl;
							break;
						default:
							break;
						}


						currentPolygonNum = 0;
					}
					currentOffset++;
				}
				out.shrink_to_fit();
				return out;
			}

			inline std::vector<std::int32_t> ConvertTriPolygon(std::vector<std::int32_t>& arg_polygonVertex,  const std::vector<std::int32_t>& arg_vec_materialIndex, const std::uint32_t arg_materialCount ,  std::vector<std::int32_t>& arg_materialIndexSize,std::int32_t offset=0) {
				std::vector<std::int32_t> out;
				std::vector<std::int32_t> buffer;
				buffer.resize(6);
				std::int32_t currentPolygonNum = 0;
				std::int32_t currentOffset=0;

				auto matItr = arg_vec_materialIndex.begin();
				std::vector<std::vector<std::uint32_t>> materialIndices;
				materialIndices.resize(arg_materialCount);

				for (std::int32_t i = 0; i < materialIndices.size(); i++) {
					materialIndices.at(i).reserve(arg_polygonVertex.size() * 3);
				}

				for (auto itr = arg_polygonVertex.begin(); itr != arg_polygonVertex.end(); itr++) {
					buffer.at(currentPolygonNum) = currentOffset;
					currentPolygonNum++;
					if (*itr < 0) {
						switch (currentPolygonNum)
						{
						case 3:
							materialIndices.at(*matItr) .push_back(buffer.at(2)+offset);
							materialIndices.at(*matItr) .push_back(buffer.at(1)+offset);
							materialIndices.at(*matItr) .push_back(buffer.at(0)+offset);
							matItr++;
							break;
						case 4:
							materialIndices.at(*matItr) .push_back(buffer.at(3)+offset);
							materialIndices.at(*matItr) .push_back(buffer.at(1)+offset);
							materialIndices.at(*matItr) .push_back(buffer.at(0)+offset);


							materialIndices.at(*matItr) .push_back(buffer.at(3)+offset);
							materialIndices.at(*matItr) .push_back(buffer.at(2)+offset);
							materialIndices.at(*matItr) .push_back(buffer.at(1)+offset);
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

				out.reserve(currentOffset);

				arg_materialIndexSize.reserve(materialIndices.size());

				for (std::int32_t i = 0; i < materialIndices.size(); i++) {
					arg_materialIndexSize.push_back(materialIndices.at(i).size());
					std::copy(materialIndices.at(i).begin(), materialIndices.at(i).end(), std::back_inserter(out));
				}

				out.shrink_to_fit();
				return out;
			}
		}

		namespace UVUTill {

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

template<class T>
inline ButiEngine::Value_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure> ButiEngine::FBXAnalyze::FBXConnectionNode::GetServeNode(std::vector<Value_ptr<T>>& arg_toNode)
{
	std::int64_t key = GetProperty<FBXNode_LongProperty>()->nodeProperty;
	for (auto itr = arg_toNode.begin(); itr != arg_toNode.end(); itr++) {
		if ((*itr)->GetNodeTag()== key) {
			return (*itr);
		}
	}

	return nullptr;
}

template<class T>
ButiEngine::Value_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure> ButiEngine::FBXAnalyze::FBXConnectionNode::GetResieveNode(std::vector<Value_ptr<T>>& arg_byNodes)
{
	std::int64_t key = GetProperty<FBXNode_LongProperty>(1)->nodeProperty;
	for (auto itr = arg_byNodes.begin(); itr != arg_byNodes.end(); itr++) {
		if ((*itr)->GetNodeTag() == key) {
			return (*itr);
		}
	}

	return nullptr;
}

template<class T, class U>
inline ButiEngine::Value_ptr<ButiEngine::FBXAnalyze::FBXNodeLinkInfo> ButiEngine::FBXAnalyze::FBXConnectionNode::GetPairNode(std::vector<Value_ptr<T>>& arg_byNodes, std::vector<Value_ptr<U>>& arg_toNodes)
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