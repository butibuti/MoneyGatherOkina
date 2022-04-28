#ifndef GamePartInterface
#define GamePartInterface

#include"ButiUtil/ButiUtil/ObjectFactory.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
namespace ButiBullet {
class RigidBody;
class PhysicsWorld;
class PhysicsManager;
}

namespace ButiEngine 
{
	namespace Collision {
		class CollisionPrimitive;
	}

	class GlobalSettings {
	public:

		static std::string GetResourceDirectory() {
			return resourceDirectory;
		}
		static void SetResourceDirectory(const std::string& arg_dir) {
			resourceDirectory = arg_dir;

		}
#ifdef _EDITORBUILD
		static std::int64_t GetEditorWindowHandle() { 
			return editorWindowHandle;
		}
		static void SetEditorWindowHandle(std::int64_t arg_handle) {
			editorWindowHandle = arg_handle;
		}
#endif // _EDITORBUILD

	private:
		static std::string resourceDirectory;
#ifdef _EDITORBUILD
		static std::int64_t editorWindowHandle;
#endif
		
	};
	enum class WindowPopType {
		normal = SW_SHOWNORMAL, max = SW_SHOWMAXIMIZED
	};
	namespace ButiRendering {
	class IDrawObject;
	template<typename T>
	class CBuffer;
	}
	class IScene;
	class IApplication;
	class SceneInformation;
	class GameObjectManager;
	namespace ImageFileIO {
	class TextureResourceData;
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�Փ˃��C���[�̊Ǘ��C���^�[�t�F�[�X
	/// </summary>
	class ICollisionManager :public IObject
	{
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		/// <summary>
		/// �t�@�C������̕���
		/// </summary>
		/// <param name="arg_path">�t�@�C���p�X</param>
		virtual void InputLoadData(const std::string& arg_path) = 0;
		/// <summary>
		/// �t�@�C���ւ̏o��
		/// </summary>
		/// <param name="arg_path">�t�@�C���p�X</param>
		virtual void OutputLoadData(const std::string& arg_path) = 0;

		/// <summary>
		/// ���C���[�̍�蒼��
		/// </summary>
		virtual void ReCreateLayers() = 0;

		virtual void Update()= 0;
		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̏Փ˓o�^
		/// </summary>
		/// <param name="index">���C���[�̔ԍ�</param>
		/// <param name="arg_prim">�`��</param>
		/// <param name="arg_registObj">�Q�[���I�u�W�F�N�g</param>
		virtual void RegistCollisionObject(const std::int32_t index,Value_ptr< Collision::CollisionPrimitive>arg_prim, Value_ptr<GameObject> arg_registObj)= 0;
		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̏Փ˓o�^����
		/// </summary>
		/// <param name="index">���C���[�̔ԍ�</param>
		/// <param name="arg_registObj">�Q�[���I�u�W�F�N�g</param>
		virtual void UnRegistCollisionObject(const std::int32_t index,  Value_ptr<GameObject> arg_registObj)= 0;
		/// <summary>
		/// ���C���[�̐�
		/// </summary>
		/// <returns></returns>
		virtual std::uint32_t GetLayerCount()= 0;
		/// <summary>
		/// ���C���[�̒ǉ�
		/// </summary>
		/// <param name="size">���C���[�̑傫��</param>
		/// <param name="level">�����؋�Ԃ̕������x��</param>
		/// <param name="isCheckSame">�������C���[�ɓo�^����Ă���I�u�W�F�N�g�ƏՓ˂��邩</param>
		virtual void AddLayer(const Vector3& size, const std::int32_t level, bool isCheckSame)= 0;
		/// <summary>
		/// ���C���[�̍폜
		/// </summary>
		/// <param name="index">���C���[�̔ԍ�</param>
		virtual void RemoveLayer(const std::int32_t index)= 0;
		/// <summary>
		/// ���C���[�o�^�O�̃I�u�W�F�N�g�Ƃ̏Փ˔���
		/// </summary>
		/// <param name="arg_prim">���肷��`��</param>
		/// <param name="index">���C���[�̃C���f�b�N�X</param>
		/// <returns>true�ŏՓ˂��Ă���</returns>
		virtual bool IsWillHit(Value_ptr< Collision::CollisionPrimitive>arg_prim,const std::int32_t index) = 0;
		/// <summary>
		/// ���C���[�o�^�O�̃I�u�W�F�N�g�Ƃ̏Փ˔���
		/// </summary>
		/// <param name="arg_prim">���肷��`��</param>
		/// <param name="index">���C���[�̃C���f�b�N�X</param>
		/// <returns>�Փ˂��Ă���I�u�W�F�N�g</returns>
		virtual Value_ptr<GameObject> GetWillHitObject(Value_ptr< Collision::CollisionPrimitive>arg_prim,const std::int32_t index) = 0;
		/// <summary>
		/// ���C���[�o�^�O�̃I�u�W�F�N�g�Ƃ̏Փ˔���
		/// </summary>
		/// <param name="arg_prim">���肷��`��</param>
		/// <param name="index">���C���[�̃C���f�b�N�X</param>
		/// <returns>�Փ˂��Ă���I�u�W�F�N�g��vector</returns>
		virtual std::vector<Value_ptr<GameObject>> GetWillHitObjects(Value_ptr< Collision::CollisionPrimitive>arg_prim,const std::int32_t index)= 0;
		/// <summary>
		/// GUI�Ăяo��
		/// </summary>
		virtual void ShowGUI()= 0;
		virtual void Start() = 0;
		virtual void End()   = 0;
	};

	namespace Collision {
		template<typename T>
		class CollisionLayer;
		class CollisionPrimitive_Box_OBB;
	}

	class IResourceContainer;

	/// <summary>
	/// �E�B���h�E����C���^�[�t�F�[�X
	/// </summary>
	class IWindow
	{
	public:
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="arg_windowName"�E�B���h�E�̖��O></param>
		/// <param name="arg_popType">�o���^�C�v</param>
		/// <param name="isFullScreen">�t���X�N���[���\��</param>
		/// <param name="width">��</param>
		/// <param name="height">����</param>
		virtual void Initialize(const std::string arg_windowName, const WindowPopType arg_popType,const bool isFullScreen,const std::uint32_t width = 0,const std::uint32_t height = 0)=0;
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~IWindow() {}
		/// <summary>
		/// �E�B���h�E�n���h���̎擾
		/// </summary>
		/// <returns>�E�B���h�E�n���h��</returns>
		virtual HWND GetHandle()=0;
		/// <summary>
		/// �T�C�Y�̎擾
		/// </summary>
		/// <returns></returns>
		virtual UInt2 GetSize()=0;
		/// <summary>
		/// �E�B���h�E�̒��S�ʒu�̎擾
		/// </summary>
		/// <returns>�E�B���h�E�̒��S�ʒu</returns>
		virtual Vector2 GetWindowCenterPosition()=0;
		/// <summary>
		/// �T�C�Y�̐ݒ�
		/// </summary>
		/// <param name="width">��</param>
		/// <param name="height">����</param>
		virtual void SetSize(const std::int32_t width,const std::int32_t height)=0;
		virtual bool Update()=0;
		/// <summary>
		/// �E�B���h�E�̍���[�̎擾
		/// </summary>
		/// <returns>�E�B���h�E�̍���[�̍��W</returns>
		virtual Vector2 GetWindowPosition()=0;
		/// <summary>
		/// �������
		/// </summary>
		virtual void Release()=0;
		/// <summary>
		/// �I����Ԃ�
		/// </summary>
		/// <returns>true->�I�𒆁@false->�I����ԂłȂ�</returns>
		virtual bool IsSelected() = 0;
		/// <summary>
		/// �I����Ԃɐ؂�ւ�����t���[����
		/// </summary>
		/// <returns>true->�I����Ԃɐ؂�ւ�����t���[���ł���@false->�I����Ԃɐ؂�ւ�����t���[���łȂ�</returns>
		virtual bool TriggerSelected() = 0;
#ifdef _EDITORBUILD
		/// <summary>
		/// �G�f�B�^�̃E�B���h�E�̑I����Ԃ��Z�b�g����(EditorBuild�̂�)
		/// </summary>
		/// <param name="arg_isActive">�G�f�B�^�̑I�����</param>
		/// <returns>���݂̑I�����</returns>
		virtual bool SetCurrentSelect(const bool arg_isSelect) = 0;
#endif // _EDITORBUILD

	private:

	};

	/// <summary>
	/// SE�ABGM�Ǘ��C���^�[�t�F�[�X
	/// </summary>
	class ISoundManager :public IObject {

	public:
		void PreInitialize()override {}

		virtual void Update() = 0;
		/// <summary>
		/// SE���Đ�����
		/// </summary>
		/// <param name="tag">SE�̃^�O</param>
		/// <param name="volume">����</param>
		virtual void PlaySE(const SoundTag tag,const float volume) = 0;
		/// <summary>
		/// BGM���Đ�����
		/// </summary>
		/// <param name="tag">BGM�̃^�O</param>
		/// <param name="volume">����</param>
		virtual void PlayBGM(const SoundTag tag, const  float volume) = 0;
		/// <summary>
		/// �ォ�特�ʂ���������SE���Đ�����
		/// </summary>
		/// <param name="tag">SE�̃^�O</param>
		/// <param name="index">SE�̔ԍ�(0�`64)</param>
		/// <param name="volume">����</param>
		/// <param name="isLoop">���[�v���邩�ǂ���</param>
		virtual void PlayControllableSE(const SoundTag tag, const std::uint32_t index, const float volume,const bool isLoop)=0;
		/// <summary>
		/// SE�̉��ʂ�ύX����
		/// </summary>
		/// <param name="index">SE�̔ԍ�(0�`64)</param>
		/// <param name="volume">����</param>
		virtual void SetControllableSEVolume(const std::uint32_t index, const float volume) = 0;
		/// <summary>
		/// SE�̃��[�v�𔲂���
		/// </summary>
		/// <param name="index">SE�̔ԍ�(0�`64)</param>
		virtual void ExitControllableSELoop(const std::uint32_t index) = 0;
		/// <summary>
		/// SE���I��������
		/// </summary>
		/// <param name="index">SE�̔ԍ�(0�`64)</param>
		virtual void DestroyControllableSE(const std::uint32_t index) = 0;
		/// <summary>
		/// SE���J�n����
		/// </summary>
		/// <param name="index">SE�̔ԍ�(0�`64)</param>
		virtual void StartontrollableSE(const std::uint32_t index) = 0;
		/// <summary>
		/// SE���~����
		/// </summary>
		/// <param name="index"></param>
		virtual void StopControllableSE(const std::uint32_t index) = 0;
		/// <summary>
		/// �S�Ă�SE���~����
		/// </summary>
		virtual void StopSE() = 0;
		/// <summary>
		/// BGM���~����
		/// </summary>
		virtual void StopBGM() = 0;
		/// <summary>
		/// SE���J�n����
		/// </summary>
		virtual void RestartSE() = 0;
		/// <summary>
		/// BGM���J�n����
		/// </summary>
		virtual void RestartBGM() = 0;
		/// <summary>
		/// SE���I��������
		/// </summary>
		virtual void DestroySE() = 0;
		/// <summary>
		/// BGM���I��������
		/// </summary>
		virtual void DestroyBGM() = 0;
		/// <summary>
		/// BGM�̉��ʂ�ݒ肷��
		/// </summary>
		/// <param name="volume"></param>
		virtual void SetBGMVolume(const float volume) = 0;
		/// <summary>
		/// �������
		/// </summary>
		virtual void Release() = 0;
		/// <summary>
		/// ���ݍĐ����Ă���BGM�̎擾
		/// </summary>
		/// <returns>���ݍĐ����Ă���BGM</returns>
		virtual SoundTag GetCurrentPlayBGM()const = 0;
		/// <summary>
		/// BGM�̉��ʂ̎擾
		/// </summary>
		/// <returns>����</returns>
		virtual float GetBGMVolume()const = 0;
		/// <summary>
		/// SE�̉��ʂ̎擾
		/// </summary>
		/// <param name="index">SE�̔ԍ�(0�`64)</param>
		/// <returns>����<returns>
		virtual float GetControllableSEVolume(const std::int32_t index)const = 0;
	};

	/// <summary>
	/// �V�[���Ǘ��C���^�[�t�F�[�X
	/// </summary>
	class ISceneManager :public IObject {

	public:

		virtual ~ISceneManager() {}
		virtual inline void Update() = 0;
		/// <summary>
		/// �V�[���̐ݒ�
		/// </summary>
		/// <param name="arg_sceneName">�V�[����</param>
		/// <param name="vlp_scene">�V�[���̃C���X�^���X</param>
		virtual void SetScene(const std::string& arg_sceneName, Value_ptr<IScene> vlp_scene) = 0;
		/// <summary>
		/// �Q�[���J�n���̃W�[���ݒ�
		/// </summary>
		/// <param name="arg_sceneName">�V�[����</param>
		/// <param name="vlp_scene">�V�[���̃C���X�^���X</param>
		virtual void SetScene_Init(const std::string& arg_sceneName, Value_ptr<IScene> vlp_scene ) = 0;
		/// <summary>
		/// �V�[���̃��[�h
		/// </summary>
		/// <param name="arg_sceneName">�V�[����</param>
		/// <param name="vlp_scene">�V�[���̃C���X�^���X(null�Ȃ疼�O���琶��)</param>
		virtual void LoadScene(const std::string& arg_sceneName, Value_ptr<SceneInformation> vlp_scene = nullptr) = 0;
		/// <summary>
		/// �V�[���̃��[�h(�񓯊�)
		/// </summary>
		/// <param name="arg_sceneName">�V�[����</param>
		virtual void LoadScene_async(const std::string& arg_sceneName) = 0;
		/// <summary>
		/// �Q�[���J�n���̃V�[���̃��[�h
		/// </summary>
		/// <param name="arg_sceneName">�V�[����</param>
		/// <param name="vlp_scene">�V�[���̃C���X�^���X(null�Ȃ疼�O���琶��)</param>
		virtual void LoadScene_Init(const std::string& arg_sceneName, Value_ptr<SceneInformation> vlp_scene = nullptr) = 0;
		/// <summary>
		/// �V�[���̍폜
		/// </summary>
		/// <param name="arg_sceneName">�V�[����</param>
		virtual void RemoveScene(const std::string& arg_sceneName) = 0;
		/// <summary>
		/// �V�[���̐؂�ւ�
		/// </summary>
		/// <param name="arg_sceneName">�V�[����</param>
		/// <param name="sceneChangeDalay">�؂�ւ��Ɋ|��t���[��</param>
		virtual void ChangeScene(const std::string& arg_sceneName, float sceneChangeDalay = 0) = 0;
		/// <summary>
		/// �V�����V�[���ւ̍X�V
		/// </summary>
		virtual void RenewalScene() = 0;
		/// <summary>
		/// �V�[���̃����[�h
		/// </summary>
		virtual void ReloadScene() = 0;
		/// <summary>
		/// �V�[���̃����[�h
		/// </summary>
		/// <param name="arg_sceneName">�����[�h����V�[����</param>
		virtual void ReloadScene(const std::string& arg_sceneName)=0;
		/// <summary>
		/// ���݂̃V�[���̎擾
		/// </summary>
		/// <returns>���݂̃V�[��</returns>
		virtual inline Value_ptr<IScene> GetCurrentScene() = 0;
		/// <summary>
		/// �A�v���P�[�V�����̎擾
		/// </summary>
		/// <returns>�A�v���P�[�V����</returns>
		virtual Value_ptr<IApplication> GetApplication() = 0;
		/// <summary>
		/// �������
		/// </summary>
		virtual void Release() = 0;
		/// <summary>
		/// �V�[���̃��[�h����
		/// </summary>
		/// <returns>true=���[�h��/false=���[�h���Ă��Ȃ�</returns>
		virtual bool IsLoading() = 0;
#ifdef _EDITORBUILD
		/// <summary>
		/// �V�[���̋N���A��~
		/// </summary>
		/// <param name="arg_isActive">true=�N��/false=��~</param>
		/// <returns>���݂̏��</returns>
		virtual bool SetActive(const bool arg_isActive) = 0;
		virtual void ReloadButtonPerform() = 0;
		virtual void SaveButtonPerform() = 0;
		/// <summary>
		/// �V�[���̕`��ɂ����������Ԃ̎擾
		/// </summary>
		/// <returns>�V�[���̕`��ɂ�����������(�~���b)</returns>
		virtual std::int32_t GetDrawMilliSecond()const = 0;
		/// <summary>
		/// �V�[���̍X�V�ɂ����������Ԃ̎擾
		/// </summary>
		/// <returns>�V�[���̍X�V�ɂ�����������(�~���b)</returns>
		virtual std::int32_t GetUpdateMilliSecond()const = 0;
		/// <summary>
		/// �V�[���̕`��ɂ����������Ԃ̎擾(60�t���[���̕���)
		/// </summary>
		/// <returns>����60�t���[���ɂ�����V�[���̕`��ɂ����������Ԃ̕���</returns>
		virtual float GetDrawMilliSecondAverage()const = 0;
		/// <summary>
		/// �V�[���̍X�V�ɂ����������Ԃ̎擾(60�t���[���̕���)
		/// </summary>
		/// <returns>����60�t���[���ɂ�����V�[���̍X�V�ɂ����������Ԃ̕���(�~���b)</returns>
		virtual float GetUpdateMilliSecondAverage()const = 0;
		virtual void SceneChangeButtonPerform(const std::string& arg_sceneName) = 0;
#endif

	};


	struct ShaderLoadInfo {
		std::string shaderName;
		VertexShaderTag vertexShaderTag;
		PixelShaderTag pixelShaderTag;
		GeometryShaderTag geometryShaderTag;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(shaderName);
			archive(vertexShaderTag);
			archive(pixelShaderTag);
			archive(geometryShaderTag);
		}
	};
	struct MaterialLoadInfo {
		MaterialLoadInfo(std::string arg_fileName) {
			fileName = arg_fileName;
		}
		MaterialLoadInfo() {}
		std::string materialName;
		std::string fileName = "none";
		ButiRendering::MaterialValue var;
		std::vector<TextureTag> vec_texture;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(materialName);
			archive(fileName);
			archive(var);
			archive(vec_texture);
		}

	};

	struct ResourceLoadData
	{
		//backupdata
		std::vector<std::string> vec_filePathAndDirectory_tex;
		std::vector<std::string> vec_filePathAndDirectory_ps;
		std::vector<std::string> vec_filePathAndDirectory_vs;
		std::vector<std::string> vec_filePathAndDirectory_gs;
		std::vector<std::string> vec_filePathAndDirectory_cs;
		std::vector<std::string> vec_filePathAndDirectory_font;
		std::vector<std::string> vec_filePathAndDirectory_sound;
		std::vector<std::string> vec_filePathAndDirectory_model;
		std::vector<std::string> vec_filePathAndDirectory_motion;
		std::vector<std::string> vec_filePathAndDirectory_script;
		std::vector<std::string> vec_filePathAndDirectory_gameObject;
		std::vector<ShaderLoadInfo> vec_shaderNames;
		std::vector<MaterialLoadInfo> vec_materialLoadInfos;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(vec_filePathAndDirectory_tex);
			archive(vec_filePathAndDirectory_ps);
			archive(vec_filePathAndDirectory_vs);
			archive(vec_filePathAndDirectory_gs);
			archive(vec_filePathAndDirectory_cs);
			archive(vec_filePathAndDirectory_font);
			archive(vec_filePathAndDirectory_sound);
			archive(vec_filePathAndDirectory_model);
			archive(vec_filePathAndDirectory_motion);
			archive(vec_filePathAndDirectory_script);
			archive(vec_filePathAndDirectory_gameObject);
			archive(vec_shaderNames);
			archive(vec_materialLoadInfos);
		}
	};

	/// <summary>
	/// ���\�[�X�Ǘ��C���^�[�t�F�[�X
	/// </summary>
	class IResourceContainer :public IObject
	{
	public:

		virtual void InputReloadData()=0;
		virtual void OutputReloadData()=0;

		/// <summary>
		/// �`��f�o�C�X�̐ݒ�
		/// </summary>
		/// <param name="arg_vlp_graphicDevice">�`��f�o�C�X</param>
		virtual void SetGraphicDevice(Value_weak_ptr<ButiRendering::GraphicDevice> arg_vlp_graphicDevice)=0;

		/// <summary>
		/// GUI�̌Ăяo��
		/// </summary>
		virtual void ShowGUI()=0;


		/// <summary>
		/// �N�����̃��b�V���̓ǂݍ���
		/// </summary>
		virtual void PreLoadMeshLoad() = 0;
		/// <summary>
		/// ���\�[�X�����ǂݍ��ݎ��̃��b�V���ǂݍ���
		/// </summary>
		virtual void InitMeshLoad() = 0;
		/// <summary>
		/// ���b�V���ǂݍ���
		/// </summary>
		virtual MeshTag LoadMesh(const std::string& filePath, const Value_ptr< ButiRendering::MeshPrimitiveBase>& arg_vlp_inputMeshData) = 0;
		virtual MeshTag LoadMesh(const std::string& filePath, const List< Value_ptr< ButiRendering::MeshPrimitiveBase>>& arg_list_vlp_inputMeshData) = 0;

		/// <summary>
		/// �l�𒼐ڎw�肵�ă}�e���A������
		/// </summary>
		/// <param name="arg_resourceMaterial">�}�e���A�����</param>
		/// <param name="arg_textureTag">�e�N�X�`�����</param>
		/// <param name="arg_materialName">�}�e���A���̖��O</param>
		/// <returns>�ǂݍ��񂾃}�e���A���̃^�O</returns>
		virtual MaterialTag LoadMaterial(const ButiRendering::MaterialValue& arg_resourceMaterial, const std::vector< TextureTag>& arg_textureTag, const std::string& arg_materialName) =0;
		/// <summary>
		/// �}�e���A�����t�@�C������ǂݍ���
		/// </summary>
		/// <param name="arg_fileName">�t�@�C���̖��O</param>
		/// <param name="arg_fileDirectory">�t�@�C���̃f�B���N�g��</param>
		/// <returns>�ǂݍ��񂾃}�e���A���̃^�O</returns>
		virtual MaterialTag LoadMaterial(const std::string& arg_fileName, const std::string& arg_fileDirectory )=0;
		/// <summary>
		/// �}�e���A�����t�@�C������ǂݍ���
		/// </summary>
		/// <param name="arg_fileName">�t�@�C���̖��O</param>
		/// <returns>�ǂݍ��񂾃}�e���A���̃^�O</returns>
		inline MaterialTag LoadMaterial(const std::string& arg_fileName) {
			static std::string dir = "";
			return LoadMaterial(arg_fileName, dir);
		}
		/// <summary>
		/// �}�e���A���̈ꊇ�ǂݍ���
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">�ǂݍ��݃t�@�C���p�X��vector</param>
		/// <returns>�ǂݍ��񂾃}�e���A���̃^�O��vector</returns>
		virtual std::vector < MaterialTag> LoadMaterial(const std::vector<std::string>& arg_vec_filePathAndDirectory) = 0;
		/// <summary>
		/// �}�e���A���̈ꊇ�ǂݍ���
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">�ǂݍ��ݏ���vector</param>
		/// <returns>�ǂݍ��񂾃}�e���A���̃^�O��vector</returns>
		virtual std::vector < MaterialTag> LoadMaterial(const std::vector<MaterialLoadInfo>& arg_vec_loadInfo)=0;
		/// <summary>
		/// �}�e���A���̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C���̖��O</param>
		/// <param name="arg_fileDirectory">�t�@�C���f�B���N�g��</param>
		/// <returns>�ǂݍ��񂾃}�e���A���̃^�O</returns>
		virtual MaterialTag LoadMaterial(const std::wstring& arg_filePath, const std::string& arg_fileDirectory )=0;
		/// <summary>
		/// �e�N�X�`���ǂݍ���
		/// </summary>
		/// <param name="arg_fileName">�e�N�X�`���̖��O</param>
		/// <param name="arg_fileDirectory">�e�N�X�`���̃f�B���N�g��</param>
		/// <returns></returns>
		virtual TextureTag LoadTexture(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// �e�N�X�`���ǂݍ���
		/// </summary>
		/// <param name="arg_fileName">�e�N�X�`���̖��O</param>
		/// <returns></returns>
		inline TextureTag LoadTexture(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadTexture(arg_filePath, dir);
		}
		/// <summary>
		/// �e�N�X�`���쐬
		/// </summary>
		/// <param name="arg_texName">�e�N�X�`���̌����Ɏg�p���閼�O</param>
		/// <param name="arg_p_pixelSrc">�摜�f�[�^</param>
		/// <param name="arg_width">���T�C�Y</param>
		/// <param name="arg_height">�c�T�C�Y</param>
		/// <param name="arg_pixelSize">��s�N�Z���ӂ�̑傫��</param>
		/// <param name="arg_format">�t�H�[�}�b�g</param>
		/// <returns></returns>
		virtual TextureTag LoadTexture(const std::string& arg_texName, void* arg_p_pixelSrc, const std::int32_t arg_width, const std::int32_t arg_height, const std::int32_t arg_pixelSize, const std::int32_t arg_format) = 0;
		/// <summary>
		/// �e�N�X�`���쐬
		/// </summary>
		/// <param name="arg_texName">�e�N�X�`���̌����Ɏg�p���閼�O</param>
		/// <param name="arg_texResourceData">�e�N�X�`���̏��</param>
		/// <returns></returns>
		virtual TextureTag LoadTexture(const std::string& arg_texName,Value_ptr<ImageFileIO::TextureResourceData> arg_texResourceData) = 0;
		/// <summary>
		/// �e�N�X�`���̈ꊇ�ǂݍ���
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">�e�N�X�`���p�X��vector</param>
		/// <returns>�ǂݍ��񂾃e�N�X�`���̃^�O��vector</returns>
		virtual std::vector < TextureTag> LoadTexture(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;
		/// <summary>
		/// �s�N�Z���V�F�[�_�̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <param name="arg_fileDirectory">�t�@�C���̃f�B���N�g��</param>
		/// <returns>�ǂݍ��񂾃V�F�[�_�̃^�O</returns>
		virtual PixelShaderTag LoadPixelShader(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// �s�N�Z���V�F�[�_�̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <returns>�ǂݍ��񂾃V�F�[�_�̃^�O</returns>
		inline PixelShaderTag LoadPixelShader(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadPixelShader(arg_filePath, dir);
		}
		/// <summary>
		/// �s�N�Z���V�F�[�_�̈ꊇ�ǂݍ���
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">�t�@�C���p�X��vector</param>
		/// <returns>�ǂݍ��񂾃V�F�[�_�̃^�O��vector</returns>
		virtual std::vector < PixelShaderTag> LoadPixelShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		/// <summary>
		/// ���_�V�F�[�_�̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <param name="arg_fileDirectory">�t�@�C���̃f�B���N�g��</param>
		/// <returns>�ǂݍ��񂾃V�F�[�_�̃^�O</returns>
		virtual VertexShaderTag LoadVertexShader(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// ���_�V�F�[�_�̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <returns>�ǂݍ��񂾃V�F�[�_�̃^�O</returns>
		inline VertexShaderTag LoadVertexShader(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadVertexShader(arg_filePath, dir);
		}
		/// <summary>
		/// ���_�V�F�[�_�̈ꊇ�ǂݍ���
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">�t�@�C���p�X��vector</param>
		/// <returns>�ǂݍ��񂾃V�F�[�_�̃^�O��vector</returns>
		virtual std::vector < VertexShaderTag> LoadVertexShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		/// <summary>
		/// �W�I���g���V�F�[�_�̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <param name="arg_fileDirectory">�t�@�C���̃f�B���N�g��</param>
		/// <returns>�ǂݍ��񂾃V�F�[�_�̃^�O</returns>
		virtual GeometryShaderTag LoadGeometryShader(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// �W�I���g���V�F�[�_�̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <returns>�ǂݍ��񂾃V�F�[�_�̃^�O</returns>
		inline GeometryShaderTag LoadGeometryShader(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadGeometryShader(arg_filePath, dir);
		}
		/// <summary>
		/// �W�I���g���V�F�[�_�̈ꊇ�ǂݍ���
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">�t�@�C���p�X��vector</param>
		/// <returns>�ǂݍ��񂾃V�F�[�_�̃^�O��vector</returns>
		virtual std::vector < GeometryShaderTag> LoadGeometryShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		/// <summary>
		/// �V�F�[�_�̐���
		/// </summary>
		/// <param name="arg_shaderInfos">�V�F�[�_�̏��</param>
		/// <returns>���������V�F�[�_�̃^�O</returns>
		virtual ShaderTag LoadShader(const  ShaderLoadInfo& arg_shaderInfos)=0;
		/// <summary>
		/// �V�F�[�_�̈ꊇ����
		/// </summary>
		/// <param name="arg_vec_shaderInfos">�V�F�[�_����vector</param>
		/// <returns>���������V�F�[�_��vector</returns>
		virtual std::vector < ShaderTag> LoadShader(const std::vector<ShaderLoadInfo>& arg_vec_shaderInfos)=0;
		/// <summary>
		/// �����̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <param name="arg_fileDirectory">�t�@�C���̃f�B���N�g��</param>
		/// <returns>�ǂݍ��񂾉����̃^�O</returns>
		virtual SoundTag LoadSound(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// �����̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <returns>�ǂݍ��񂾉����̃^�O</returns>
		inline SoundTag LoadSound(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadSound(arg_filePath, dir);
		}
		/// <summary>
		/// �����̈ꊇ�ǂݍ���
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">�t�@�C���p�X��vector</param>
		/// <returns>�ǂݍ��񂾉�����vector</returns>
		virtual std::vector < SoundTag> LoadSound(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		/// <summary>
		/// ���f���f�[�^�̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <param name="arg_fileDirectory">�t�@�C���̃f�B���N�g��</param>
		/// <returns>�ǂݍ��񂾃��f���̃^�O</returns>
		virtual ModelTag LoadModel(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// ���f���f�[�^�̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <returns>�ǂݍ��񂾃��f���̃^�O</returns>
		inline ModelTag LoadModel(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadModel(arg_filePath, dir);
		}
		/// <summary>
		/// ���f���f�[�^�̈ꊇ�ǂݍ���
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">�t�@�C���p�X��vector</param>
		/// <returns>�ǂݍ��񂾃��f���̃^�O��vector</returns>
		virtual std::vector<ModelTag> LoadModel(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		/// <summary>
		/// ���[�V�����f�[�^�̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <param name="arg_fileDirectory">�t�@�C���f�B���N�g��</param>
		/// <returns>�ǂݍ��񂾃��[�V�����̃^�O</returns>
		virtual MotionTag LoadMotion(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// ���[�V�����f�[�^�̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <returns>�ǂݍ��񂾃��[�V�����̃^�O</returns>
		inline MotionTag LoadMotion(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadMotion(arg_filePath, dir);
		}
		/// <summary>
		/// ���[�V�����f�[�^�̈ꊇ�ǂݍ���
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">�t�@�C���p�X��vector</param>
		/// <returns>�ǂݍ��񂾃��[�V�����̃^�O��vector</returns>
		virtual std::vector<MotionTag> LoadMotion(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;
		/// <summary>
		/// �R���p�C���ς݃X�N���v�g�̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <param name="arg_fileDirectory">�t�@�C���f�B���N�g��</param>
		/// <returns>�ǂݍ��񂾃X�N���v�g�̃^�O</returns>
		virtual ScriptTag LoadScript(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// �R���p�C���ς݃X�N���v�g�̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <returns>�ǂݍ��񂾃X�N���v�g�̃^�O</returns>
		inline ScriptTag LoadScript(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadScript(arg_filePath, dir);
		}
		/// <summary>
		/// �X�N���v�g�̈ꊇ�ǂݍ���
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">�t�@�C���p�X��vector</param>
		/// <returns>�ǂݍ��񂾃X�N���v�g�̃^�O��vector</returns>
		virtual std::vector<ScriptTag> LoadScript(const std::vector<std::string>& arg_vec_filePathAndDirectory) = 0;


		/// <summary>
		/// TrueTypeFont�̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <param name="arg_fileDirectory">�t�@�C���f�B���N�g��</param>
		/// <returns>�ǂݍ��񂾃t�H���g�̃^�O</returns>
		virtual FontTag LoadFont(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// TrueTypeFont�̓ǂݍ���
		/// </summary>
		/// <param name="arg_filePath">�t�@�C����</param>
		/// <returns>�ǂݍ��񂾃t�H���g�̃^�O</returns>
		inline FontTag LoadFont(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadFont(arg_filePath, dir);
		}
		/// <summary>
		/// �t�H���g�̈ꊇ�ǂݍ���
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">�t�@�C���p�X��vector</param>
		/// <returns>�ǂݍ��񂾃t�H���g�̃^�O��vector</returns>
		virtual std::vector < FontTag> LoadFont(const std::vector<std::string>& arg_vec_filePathAndDirectory) = 0;
		/// <summary>
		/// �e�L�X�g�`��p���b�V���̐���
		/// </summary>
		/// <param name="arg_glyphMesh">�X�V�p�̖��O</param>
		/// <param name="glyphCount">������</param>
		/// <returns></returns>
		virtual MeshTag CreateTextMesh(const std::string& arg_glyphMesh, const std::int32_t glyphCount) = 0;
		/// <summary>
		/// �X�N���v�g�̃\�[�X�t�@�C�����X�V����Ă��Ȃ����`�F�b�N
		/// </summary>
		virtual void ScriptEditCheck() = 0;

		virtual std::vector<TextureTag> GetTextureTags()const = 0;
		virtual std::vector<ModelTag> GetModelTags()const = 0;
		virtual std::vector<MeshTag> GetMeshTags()const = 0;
		virtual std::vector<ScriptTag> GetScriptTags()const = 0;
		virtual std::vector<SoundTag> GetSoundTags()const = 0;
		virtual std::vector<MotionTag> GetMotionTags()const = 0;
		virtual std::vector<FontTag> GetFontTags()const = 0;
		virtual std::vector<VertexShaderTag> GetVertexShaderTags()const = 0;
		virtual std::vector<GeometryShaderTag> GetGeometryShaderTags()const = 0;
		virtual std::vector<PixelShaderTag> GetPixelShaderTags()const = 0;
		virtual std::vector<ShaderTag> GetShaderTags()const = 0;
		virtual std::vector<MaterialTag> GetMaterialTags()const = 0;

		virtual std::vector<std::string> GetLoadedTextureNames()const = 0;
		virtual std::vector<std::string> GetLoadedModelNames()const = 0;
		virtual std::vector<std::string> GetLoadedMeshNames()const = 0;
		virtual std::vector<std::string> GetLoadedScriptNames()const = 0;
		virtual std::vector<std::string> GetLoadedSoundNames()const = 0;
		virtual std::vector<std::string> GetLoadedMotionNames()const = 0;
		virtual std::vector<std::string> GetLoadedFontNames()const = 0;
		virtual std::vector<std::string> GetLoadedVertexShaderNames()const = 0;
		virtual std::vector<std::string> GetLoadedGeometryShaderNames()const = 0;
		virtual std::vector<std::string> GetLoadedPixelShaderNames()const = 0;
		virtual std::vector<std::string> GetLoadedShaderNames()const = 0;
		virtual std::vector<std::string> GetLoadedMaterialNames()const = 0;

		virtual const std::vector<MaterialLoadInfo> & GetLoadedMaterialInfos()const = 0;
		virtual const std::vector<ShaderLoadInfo> & GetLoadedShaderInfos()const = 0;

		/// <summary>
		/// ���b�V���̍폜
		/// </summary>
		/// <param name="arg_meshTag">�폜����^�O</param>
		virtual void UnLoad(MeshTag arg_meshTag)=0;
		/// <summary>
		/// �e�N�X�`���̍폜
		/// </summary>
		/// <param name="arg_textureTag">�폜����^�O</param>
		virtual void UnLoad(TextureTag arg_textureTag)=0;
		/// <summary>
		/// �V�F�[�_�̍폜
		/// </summary>
		/// <param name="arg_shaderTag">�폜����^�O</param>
		virtual void UnLoad(ShaderTag arg_shaderTag)=0;
		/// <summary>
		/// �s�N�Z���V�F�[�_�̍폜
		/// </summary>
		/// <param name="arg_shaderTag">�폜����^�O</param>
		virtual void UnLoad(PixelShaderTag arg_shaderTag)=0;
		/// <summary>
		/// ���_�V�F�[�_�̍폜
		/// </summary>
		/// <param name="arg_shaderTag">�폜����^�O</param>
		virtual void UnLoad(VertexShaderTag arg_shaderTag)=0;
		/// <summary>
		/// �W�I���g���V�F�[�_�̍폜
		/// </summary>
		/// <param name="arg_shaderTag">�폜����^�O</param>
		virtual void UnLoad(GeometryShaderTag arg_shaderTag)=0;
		/// <summary>
		/// �����̍폜
		/// </summary>
		/// <param name="arg_soundTag">�폜����^�O</param>
		virtual void UnLoad(SoundTag arg_soundTag)=0;
		/// <summary>
		/// ���f���̍폜
		/// </summary>
		/// <param name="arg_modelTag">�폜����^�O</param>
		virtual void UnLoad(ModelTag arg_modelTag)=0;
		/// <summary>
		/// �}�e���A���̍폜
		/// </summary>
		/// <param name="arg_materialTag">�폜����^�O</param>
		virtual void UnLoad(MaterialTag arg_materialTag)=0;
		/// <summary>
		/// ���[�V�����̍폜
		/// </summary>
		/// <param name="arg_motionTag">�폜����^�O</param>
		virtual void UnLoad(MotionTag arg_motionTag) = 0;
		/// <summary>
		/// �X�N���v�g�̍폜
		/// </summary>
		/// <param name="arg_motionTag">�폜����^�O</param>
		virtual void UnLoad(ScriptTag arg_scriptTag) = 0;
		/// <summary>
		/// �t�H���g�̍폜
		/// </summary>
		/// <param name="arg_motionTag">�폜����^�O</param>
		virtual void UnLoad(FontTag arg_fontTag) = 0;

		/// <summary>
		/// �������
		/// </summary>
		virtual void Release()=0;

		/// <summary>
		/// �}�e���A���̍X�V
		/// </summary>
		virtual void MaterialUpdate()=0;

		/// <summary>
		/// ���b�V���̎擾
		/// </summary>
		/// <param name="arg_key">�����^�O</param>
		/// <returns>���b�V���̃��\�[�X</returns>
		virtual Value_weak_ptr<ButiRendering::IResource_Mesh> GetMesh(const  MeshTag& arg_key)=0;
		/// <summary>
		/// �e�N�X�`���̎擾
		/// </summary>
		/// <param name="arg_key">�����^�O</param>
		/// <returns>�e�N�X�`���̃��\�[�X</returns>
		virtual Value_weak_ptr<ButiRendering::IResource_Texture> GetTexture(const TextureTag& arg_key)=0;
		/// <summary>
		/// �V�F�[�_�̎擾
		/// </summary>
		/// <param name="arg_key">�����^�O</param>
		/// <returns>�V�F�[�_�̃��\�[�X</returns>
		virtual Value_weak_ptr<ButiRendering::IResource_Shader> GetShader(const ShaderTag& arg_key)=0;
		/// <summary>
		/// �����̎擾
		/// </summary>
		/// <param name="arg_key">�����^�O</param>
		/// <returns>�����̃��\�[�X</returns>
		virtual Value_weak_ptr<IResource_Sound> GetSound(const SoundTag& arg_key)=0;
		/// <summary>
		/// �}�e���A���̎擾
		/// </summary>
		/// <param name="arg_key">�����^�O</param>
		/// <returns>�}�e���A���̃��\�[�X</returns>
		virtual Value_weak_ptr<ButiRendering::IResource_Material>GetMaterial(const MaterialTag& arg_key)=0;
		/// <summary>
		/// �x���V�F�[�f�B���O�p�̃}�e���A�����X�g�̎擾
		/// </summary>
		/// <returns>�x���V�F�[�f�B���O�p�̃}�e���A���p�����[�^��vector</returns>
		virtual std::vector<ButiRendering::MaterialValue_Deferred> CreateDeferredMaterialList() = 0;
		/// <summary>
		/// ���f���̎擾
		/// </summary>
		/// <param name="arg_key">�����^�O</param>
		/// <returns>���f���̃��\�[�X</returns>
		virtual Value_weak_ptr<ButiRendering::IResource_Model>GetModel(const ModelTag& arg_key)=0;
		/// <summary>
		/// ���[�V�����̎擾
		/// </summary>
		/// <param name="arg_key">�����^�O</param>
		/// <returns>���[�V�����̃��\�[�X</returns>
		virtual Value_ptr<ButiRendering::ModelAnimation>GetMotion(const MotionTag& arg_key)=0;
		/// <summary>
		/// �R���p�C���ς݃X�N���v�g�̎擾
		/// </summary>
		/// <param name="arg_key">�����^�O</param>
		/// <returns>�X�N���v�g�̃��\�[�X</returns>
		virtual Value_weak_ptr<IResource_Script>GetScript(const ScriptTag& arg_key) = 0;
		/// <summary>
		/// �t�H���g�̎擾
		/// </summary>
		/// <param name="arg_key">�����^�O</param>
		/// <returns>�t�H���g�̃��\�[�X</returns>
		virtual Value_weak_ptr<IResource_Font>GetFont(const FontTag& arg_key) = 0;

		virtual bool Exist(const TextureTag& arg_tag)const = 0;
		virtual bool Exist(const MeshTag& arg_tag)const = 0;
		virtual bool Exist(const ModelTag& arg_tag)const = 0;
		virtual bool Exist(const MaterialTag& arg_tag)const = 0;
		virtual bool Exist(const ScriptTag& arg_tag)const = 0;
		virtual bool Exist(const SoundTag& arg_tag)const = 0;
		virtual bool Exist(const ShaderTag& arg_tag)const = 0;
		virtual bool Exist(const VertexShaderTag& arg_tag)const = 0;
		virtual bool Exist(const PixelShaderTag& arg_tag)const = 0;
		virtual bool Exist(const GeometryShaderTag& arg_tag)const = 0;
		virtual bool Exist(const MotionTag& arg_tag)const = 0;
		virtual bool Exist(const FontTag& arg_tag)const = 0;

		/// <summary>
		/// �ēǂݍ���
		/// </summary>
		virtual void Reload() = 0;
		/// <summary>
		/// �ēǂݍ���(�񓯊�)
		/// </summary>
		virtual void Reload_async() = 0;

		/// <summary>
		/// ���ݓǂݍ��ݒ����ǂ���
		/// </summary>
		/// <returns></returns>
		virtual bool IsLoading() = 0;
	};

	/// <summary>
	/// �A�v���P�[�V�����̃C���^�[�t�F�[�X
	/// </summary>
	class IApplication :public IObject
	{
	public:
		//�A�v���P�[�V�����������p�\����
		struct ApplicationInitData {
			std::string appWindowName;
			std::string initSceneName ;
			WindowPopType popType ;
			std::int32_t windowWidth ;
			std::int32_t windowHeight;
			bool isFullScreen;
		};
		struct FrameRateInformation {
			float averageFPS;
			float currentFPS;
			std::int32_t drawMillSec;
			std::int32_t updateMillSec;
		};
		/// <summary>
		/// �E�B���h�E�̎擾
		/// </summary>
		/// <returns></returns>
		virtual std::unique_ptr<IWindow>& GetWindow()=0;
		/// <summary>
		/// �V�[���}�l�[�W���[�̎擾
		/// </summary>
		/// <returns></returns>
		virtual Value_ptr<ISceneManager> GetSceneManager()=0;
		/// <summary>
		/// �`��f�o�C�X�̎擾
		/// </summary>
		/// <returns></returns>
		virtual Value_ptr<ButiRendering::GraphicDevice> GetGraphicDevice()=0;
		/// <summary>
		/// ���\�[�X�Ǘ��C���^�[�t�F�[�X�̎擾
		/// </summary>
		/// <returns>���\�[�X�Ǘ��C���^�[�t�F�[�X</returns>
		virtual Value_ptr<IResourceContainer> GetResourceContainer()=0;
		/// <summary>
		/// �Q�[���I�u�W�F�N�g�^�O�Ǘ��C���X�^���X�̎擾
		/// </summary>
		/// <returns>�Q�[���I�u�W�F�N�g�^�O�Ǘ��C���X�^���X</returns>
		virtual Value_ptr<GameObjectTagManager> GetGameObjectTagManager()=0;
		/// <summary>
		/// �����Ǘ��C���X�^���X�̎擾
		/// </summary>
		/// <returns>�����Ǘ��C���X�^���X</returns>
		virtual Value_ptr<ISoundManager> GetSoundManager() = 0;
		/// <summary>
		/// �X�V����
		/// </summary>
		/// <returns></returns>
		virtual bool Update()=0;
		/// <summary>
		/// ���s
		/// </summary>
		/// <returns></returns>
		virtual std::int32_t Run()=0;
		/// <summary>
		/// �ŏ��ɓǂݍ��ރ��\�[�X�̃��[�h
		/// </summary>
		virtual void PreLoadResources() = 0;
		/// <summary>
		/// ���\�[�X�R���e�i�ɋL�^����Ă��郊�\�[�X�̓ǂݍ���
		/// </summary>
		virtual void InitLoadResources() = 0;
		/// <summary>
		/// ���\�[�X�R���e�i�ɋL�^����Ă��郊�\�[�X�̓ǂݍ���(�񓯊�)
		/// </summary>
		virtual void InitLoadResources_async() = 0;
		/// <summary>
		/// �I������
		/// </summary>
		virtual void Exit()=0;
		/// <summary>
		/// �O��̃t���[���̏��v����(�~���b)
		/// </summary>
		/// <returns>�O��̃t���[���̏��v����(�~���b)</returns>
		virtual std::int64_t GetBefFrame()const = 0;
		virtual const ApplicationInitData* GetAppInitData()const=0;

#ifdef _EDITORBUILD
		virtual const FrameRateInformation& GetFrameInformation()const=0;
#endif
	};

	class SceneInformation;
	class SceneChangeInformation;
	class SceneRenderingInformation;
	/// <summary>
	/// �V�[���̃C���^�[�t�F�[�X
	/// </summary>
	class IScene :public IObject
	{
	public:
		/// <summary>
		/// �X�V����
		/// </summary>
		virtual void Update() = 0;
		/// <summary>
		/// UI�X�V
		/// </summary>
		virtual void UIUpdate() = 0;
		/// <summary>
		/// �`��O����
		/// </summary>
		virtual void BefDraw() = 0;
		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̓o�^
		/// </summary>
		virtual void RegistGameObjects() = 0;
		/// <summary>
		/// �`��
		/// </summary>
		virtual void Draw() = 0;
		/// <summary>
		/// �ҏW���[�h�ł̃J�����̑���
		/// </summary>
		virtual void EditCameraUpdate() = 0;
		/// <summary>
		/// �Փ˔���N���X�̋N��
		/// </summary>
		/// <param name="arg_layerCount">�Փ˔��背�C���[�̐�</param>
		virtual void ActiveCollision(const std::uint32_t arg_layerCount) = 0;
		/// <summary>
		/// �J�����̎擾
		/// </summary>
		/// <param name="arg_camName">�J������</param>
		/// <returns> �J�����̎擾</returns>
		virtual Value_ptr<ButiRendering::ICamera> GetCamera(const std::string& arg_camName) = 0;
		/// <summary>
		///  �J�����̎擾
		/// </summary>
		/// <param name="arg_camNum">�`��p�X�̃C���f�b�N�X</param>
		/// <returns>�J�����̎擾</returns>
		virtual Value_ptr<ButiRendering::ICamera> GetCamera(const std::uint32_t arg_camNum = 0) = 0;
		/// <summary>
		/// �`��p�X�̒ǉ�
		/// </summary>
		/// <param name="arg_path">�`��p�X</param>
		/// <returns>�ǉ����ꂽ�`��p�X</returns>
		virtual Value_ptr<ButiRendering::IRenderingPath> AddRenderingPath(Value_ptr<ButiRendering::IRenderingPath> arg_path) = 0;
		/// <summary>
		/// �ՓˊǗ��C���X�^���X�̎擾
		/// </summary>
		/// <returns>�ՓˊǗ��C���X�^���X</returns>
		virtual Value_weak_ptr<ICollisionManager> GetCollisionManager() = 0;
		/// <summary>
		/// �`��p�X�̍폜
		/// </summary>
		/// <param name="arg_pathName">�g�p����L�[</param>
		virtual void RemoveRenderingPath(const std::string& arg_pathName) = 0;
		/// <summary>
		/// �`��p�X�̍폜
		/// </summary>
		/// <param name="arg_pathIndex">�`��p�X�̃C���f�b�N�X</param>
		virtual void RemoveRenderingPath(const std::uint32_t arg_pathIndex) = 0;
		/// <summary>
		/// �`��Ǘ��C���X�^���X�̎擾
		/// </summary>
		/// <returns>�`��Ǘ��C���X�^���X</returns>
		virtual Value_ptr<ButiRendering::IRenderer> GetRenderer() = 0;
		/// <summary>
		/// ���\�[�X�Ǘ��C���X�^���X�̎擾
		/// </summary>
		/// <returns>���\�[�X�Ǘ��C���X�^���X</returns>
		virtual Value_ptr<IResourceContainer> GetResourceContainer() = 0;
		/// <summary>
		/// �V�[���Ǘ��C���X�^���X�̎擾
		/// </summary>
		/// <returns>�V�[���Ǘ��C���X�^���X</returns>
		virtual Value_weak_ptr< ISceneManager> GetSceneManager() = 0;
		/// <summary>
		/// �E�B���h�E�Ǘ��C���X�^���X�̎擾
		/// </summary>
		/// <returns>�E�B���h�E�Ǘ��C���X�^���X</returns>
		virtual std::unique_ptr<IWindow>& GetWindow() = 0;
		/// <summary>
		/// �������
		/// </summary>
		virtual void Release() = 0;
		/// <summary>
		/// �V�[�����̎擾
		/// </summary>
		/// <returns>�V�[�����</returns>
		virtual Value_ptr< SceneInformation> GetSceneInformation() = 0;
		/// <summary>
		/// �V�[���ύX���̎擾
		/// </summary>
		/// <returns>�V�[���ύX���</returns>
		virtual Value_ptr< SceneChangeInformation> GetSceneChangeInformation() = 0;
		/// <summary>
		/// �V�[���`����̎擾
		/// </summary>
		/// <returns>�V�[���`����</returns>
		virtual Value_ptr< SceneRenderingInformation> GetSceneRenderingInformation() = 0;
		/// <summary>
		/// �V�[�����̎擾
		/// </summary>
		virtual void Save() = 0;
		/// <summary>
		/// �V�[�����s���̃J�����̋N��
		/// </summary>
		virtual void CameraActivation() = 0;
		/// <summary>
		/// �V�[���ҏW���̃J�����̋N��
		/// </summary>
		virtual void CameraEditActivation() = 0;
		/// <summary>
		/// �J�n
		/// </summary>
		virtual void Start() = 0;
		/// <summary>
		/// �I��
		/// </summary>
		virtual void End() = 0;
		/// <summary>
		/// GUI�o�^
		/// </summary>
		virtual void GUIRegist() = 0;
		/// <summary>
		/// GUI�ւ̓o�^����
		/// </summary>
		virtual void GUIRelease() = 0;
		/// <summary>
		/// GameObjectManager�̎擾
		/// </summary>
		virtual Value_ptr<GameObjectManager> GetGameObjectManager() = 0;
		/// <summary>
		/// �����V���~���[�V�����Ǘ��N���X�ւ̃A�N�Z�X
		/// </summary>
		/// <returns></returns>
		virtual Value_ptr<ButiBullet::PhysicsManager> GetPhysicsManager()const = 0;
	};


	void OutputCereal(const ResourceLoadData& v, const std::string& arg_fileName);
	void InputCereal(ResourceLoadData& v, const std::string& arg_fileName);
}



#endif // !GamePartInterface