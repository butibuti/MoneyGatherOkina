#include"ButiUtil/ButiUtil/ID.h"
#include"ButiRendering_Dx12/Header/ResourceInterface.h"
namespace ButiScript {
class CompiledData;
class Compiler;
}
namespace ButiFont {
class IFontInformation;
}
namespace ButiEngine {

class IResource_Script;
class IResource_Sound;

using ScriptTag = ID<IResource_Script>;
using SoundTag = ID<IResource_Sound>;

struct WaveDatas;
namespace ImageFileIO {
class IFontInformation;
}

class IResource_Sound :public IObject {
public:
	virtual Value_ptr<WaveDatas> GetWavDatas() = 0;
};


class IResource_Script :public IObject {
public:
	virtual void SetCompiler(ButiScript::Compiler* arg_compiler) = 0;
	virtual Value_ptr<ButiScript::CompiledData> GetCompiledData() = 0;
	/// <summary>
	/// �ăR���p�C��
	/// </summary>
	/// <returns>0������/1�����s</returns>
	virtual std::int32_t Recompile() = 0;
	virtual void Reload() = 0;
	/// <summary>
	/// �V�[����̑S�Ẳ��z�}�V�����z�b�g�����[�h
	/// </summary>
	virtual void HotReload() = 0;
	virtual const std::string& GetScriptName()const=0;
	/// <summary>
	/// �\�[�X�t�@�C���ɕύX�����������`�F�b�N�A�ύX����Ȃ炻�̂܂܍ăR���p�C��
	/// </summary>
	/// <returns>0���ύX�Ȃ�/1���ύX����</returns>
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