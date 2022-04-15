#include"ButiUtil/ButiUtil/ID.h"
namespace ButiScript {
class CompiledData;
class Compiler;
}
namespace ButiFont {
class IFontInformation;
}
namespace ButiEngine {

namespace ButiRendering {
class MeshPrimitiveBase;
}
namespace Vertex {
enum class VertexType;
}

class IResource_Mesh;
class IResource_Motion;
class IResource_VertexShader;
class IResource_PixelShader;
class IResource_GeometryShader;
class IResource_Shader;
class IResource_Sound;
class IResource_Texture;
class IResource_Model;
class IResource_Material;
class IResource_Script;
class IRenderer;


using MeshTag = ID<IResource_Mesh>;
using MotionTag = ID<IResource_Motion>;

using VertexShaderTag = ID<IResource_VertexShader>;
using PixelShaderTag = ID<IResource_PixelShader>;
using GeometryShaderTag = ID<IResource_GeometryShader>;
using ShaderTag = ID<IResource_Shader>;
using SoundTag = ID<IResource_Sound>;
using TextureTag = ID<IResource_Texture>;
using ModelTag = ID<IResource_Model>;

using MaterialTag = ID<IResource_Material>;
using ScriptTag = ID<IResource_Script>;

class ModelAnimation;
struct MotionKeyFrameData;
struct Bone;
struct WaveDatas;
namespace Morph{
class Morph;
}
namespace ImageFileIO {
class IFontInformation;
}

class IRenderTarget {
public:
	virtual void SetRenderTarget(Vector4& arg_clearColor) = 0;
	virtual void DisSetRenderTarget() = 0;
	virtual void SetIsCleared(bool arg_isClear) = 0;
	virtual Vector2 GetSize() = 0;
};
class IDepthStencil {
public:
	virtual void SetDepthStencilView() = 0;
	virtual void DisSetDepthStencil() = 0;
	virtual void SetIsCleared(bool arg_isClear) = 0;
	virtual Vector2 GetSize() = 0;
};

class IResource_Motion :public IObject {
public:
	virtual Value_ptr<ModelAnimation> GetAnimation() = 0;
	virtual void AddKeyFrame(const std::string& arg_boneName, const MotionKeyFrameData& arg_data) = 0;
	virtual void AddKeyFrameLane(const std::string& arg_boneName, const std::vector<MotionKeyFrameData>& arg_datas) = 0;
};
class IResource_VertexShader :public IObject {
public:
	virtual void Attach()const = 0;
	virtual std::string GetName()const = 0;
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
	virtual std::uint32_t GetInputVertexType()const=0;
};
class IResource_PixelShader :public IObject {
public:
	virtual void Attach()const = 0;
	virtual std::string GetName()const = 0;
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
	virtual std::vector<std::uint32_t> GetOutputPixelFormat()const = 0;
};
class IResource_GeometryShader :public IObject {
public:
	virtual void Attach()const = 0;
	virtual std::string GetName()const = 0;
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
};

class IResource_Shader :public IObject {
public:
	virtual void Attach()const = 0;

	virtual bool GetIsContainGeometryShader()const = 0;

	virtual std::string GetShaderName()const = 0;

	virtual Value_weak_ptr<IResource_VertexShader> GetVertexShader() = 0;
	virtual Value_weak_ptr<IResource_PixelShader> GetPixelShader() = 0;
	virtual Value_weak_ptr<IResource_GeometryShader> GetGeometryShader() = 0;
};
class IResource_Sound :public IObject {
public:
	virtual Value_ptr<WaveDatas> GetWavDatas() = 0;
};


class IResource_Texture :public IObject {
public:
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
	virtual void Attach(std::int32_t slot) = 0;
	virtual UInt2 GetSize() const = 0;
	virtual std::int32_t GetDataSize() const = 0;
	virtual std::int32_t GetStride() const = 0;
	virtual std::int32_t GetFormat()const = 0;
	virtual void SetFilePath(const std::string& arg_textureFilePath) = 0;
	virtual void ToPNG(const std::string& arg_textureFilePath) {}
	virtual void* GetResourcePtr() { return nullptr; }
	virtual GUIWindowReaction ShowTextureWindow(const std::string& arg_windowName="",const std::int32_t winFrag=0) { return GUIWindowReaction(); }
	virtual const std::string& GetTexturePath() = 0;
	virtual const unsigned char* GetRawData()=0;
#ifdef _EDITORBUILD
	virtual void SetEditorViewed(const bool arg_isViewed) {}
	virtual bool GetEditorViewed()const { return false; }
	virtual void Wait(){}
#endif
};
class IResource_Material :public IObject {
public:
	virtual void Attach(const std::uint32_t slotOffset, Value_ptr<IRenderer> arg_shp_renderer) = 0;
	virtual const std::string& GetMaterialName() = 0;
	virtual const std::string& GetMaterialNameEng() = 0;
	virtual const std::string& GetMaterialComment() = 0;
	virtual void SetMaterialName(const std::string& arg_name) = 0;
	virtual void SetMaterialNameEng(const std::string& arg_name) = 0;
	virtual void SetComment(const std::string& arg_comment) = 0;
	virtual MaterialValue& GetMaterialVariable()  = 0;
	virtual Value_ptr<IResource_Texture> GetTexture(const std::int32_t index = 0) = 0;
	virtual Value_ptr<IResource_Texture> GetSphereTexture() = 0;
	virtual TextureTag GetSphereTextureTag() const= 0;
	virtual TextureTag GetTextureTag(const std::int32_t index=0)const = 0;
	virtual void Update() = 0;
	virtual std::uint32_t GetTextureCount()const =0;
	virtual bool OnShowUI()=0;
	virtual void SetMaterialIndex(const std::int32_t arg_index) = 0;
	virtual void SetTextureTag(const std::uint32_t index,TextureTag arg_tag)=0;
	virtual void SetMaterialVariable(const MaterialValue& arg_var)=0;
	virtual MaterialValue_Deferred GetMaterialDeferredValue() = 0;
	virtual std::vector< TextureTag > GetTextureTags() = 0;
};
class IResource_Mesh :public IObject {
public:
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
	virtual void SetBackupData(Value_ptr<ButiRendering::MeshPrimitiveBase> arg_vlp_meshPrimitive) = 0;
	virtual void Draw(const std::uint32_t arg_vertexType) = 0;
	virtual std::uint32_t GetVertexCount() = 0;
	virtual std::uint32_t GetIndexCount() = 0;
	virtual bool GetPosRayCast(Vector3* arg_p_pos, Vector3* arg_p_normal, const Line& arg_line)=0;
	virtual const ButiRendering::MeshPrimitiveBase* GetBackUpdata(std::uint32_t arg_vertexType)const = 0;
};
class IResource_Model :public IObject {
public:
	virtual void SetMeshTag(const MeshTag& arg_meshTag) = 0;
	virtual void SetMaterialVec(const std::vector<MaterialTag>& arg_materialTags) = 0;
	virtual void SetName(const std::wstring& arg_name) = 0;
	virtual void SetEngName(const std::wstring& arg_engName) = 0;
	virtual void SetModelName(const std::wstring& arg_modelName) = 0;
	virtual void SetEngModelName(const std::wstring& arg_engModelName) = 0;
	virtual void SetComment(const std::wstring& arg_comment) = 0;
	virtual void SetEngComment(const std::wstring& arg_engComment) = 0;
	virtual void AddBone(Bone& arg_bone) = 0;
	virtual void AddMorph(Value_ptr<Morph::Morph> arg_morph) = 0;
	virtual void AddMaterial(const MaterialTag& arg_materialTag) = 0;
	virtual void SetVersion(const float arg_version) = 0;
	virtual void SetSubset(const std::vector<std::uint32_t>& arg_subset) = 0;
	virtual const MeshTag GetMeshTag() = 0;
	virtual std::vector<MaterialTag>& GetMaterialTags() = 0;

	virtual std::vector<std::uint32_t> GetSubset() = 0;
	virtual const std::wstring& GetName() = 0;
	virtual const std::wstring& GetEngName() = 0;
	virtual const std::wstring& GetModelName() = 0;
	virtual const std::wstring& GetEngModelName() = 0;
	virtual const std::wstring& GetComment() = 0;
	virtual const std::wstring& GetEngComment() = 0;
	virtual std::vector<Value_ptr<Bone>> GetBone() = 0;
};
class IResource_Script :public IObject {
public:
	virtual void SetCompiler(ButiScript::Compiler* arg_compiler) = 0;
	virtual Value_ptr<ButiScript::CompiledData> GetCompiledData() = 0;
	/// <summary>
	/// 再コンパイル
	/// </summary>
	/// <returns>0＝成功/1＝失敗</returns>
	virtual std::int32_t Recompile() = 0;
	virtual void Reload() = 0;
	/// <summary>
	/// シーン上の全ての仮想マシンをホットリロード
	/// </summary>
	virtual void HotReload() = 0;
	virtual const std::string& GetScriptName()const=0;
	/// <summary>
	/// ソースファイルに変更があったかチェック、変更ありならそのまま再コンパイル
	/// </summary>
	/// <returns>0＝変更なし/1＝変更あり</returns>
	virtual std::int32_t SourceEditCheck() = 0;
};
class IResource_Font:public IObject {
public:
	virtual const std::string & GetFontName()const = 0;
	virtual TextureTag GetTextureTag()const = 0;
	virtual MaterialTag GetMaterialTag()const = 0;
	virtual ButiFont::IFontInformation* GetFontInformation() const= 0;
};
}