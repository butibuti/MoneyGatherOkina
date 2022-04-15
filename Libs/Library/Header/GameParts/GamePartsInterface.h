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
	class IDrawObject;
	class IScene;
	class IApplication;
	template<typename T>
	class CBuffer;
	class SceneInformation;
	class GameObjectManager;
	namespace ImageFileIO {
	class TextureResourceData;
	}

	/// <summary>
	/// ゲームオブジェクト衝突レイヤーの管理インターフェース
	/// </summary>
	class ICollisionManager :public IObject
	{
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		/// <summary>
		/// ファイルからの復旧
		/// </summary>
		/// <param name="arg_path">ファイルパス</param>
		virtual void InputLoadData(const std::string& arg_path) = 0;
		/// <summary>
		/// ファイルへの出力
		/// </summary>
		/// <param name="arg_path">ファイルパス</param>
		virtual void OutputLoadData(const std::string& arg_path) = 0;

		/// <summary>
		/// レイヤーの作り直し
		/// </summary>
		virtual void ReCreateLayers() = 0;

		virtual void Update()= 0;
		/// <summary>
		/// ゲームオブジェクトの衝突登録
		/// </summary>
		/// <param name="index">レイヤーの番号</param>
		/// <param name="arg_prim">形状</param>
		/// <param name="arg_registObj">ゲームオブジェクト</param>
		virtual void RegistCollisionObject(const std::int32_t index,Value_ptr< Collision::CollisionPrimitive>arg_prim, Value_ptr<GameObject> arg_registObj)= 0;
		/// <summary>
		/// ゲームオブジェクトの衝突登録解除
		/// </summary>
		/// <param name="index">レイヤーの番号</param>
		/// <param name="arg_registObj">ゲームオブジェクト</param>
		virtual void UnRegistCollisionObject(const std::int32_t index,  Value_ptr<GameObject> arg_registObj)= 0;
		/// <summary>
		/// レイヤーの数
		/// </summary>
		/// <returns></returns>
		virtual std::uint32_t GetLayerCount()= 0;
		/// <summary>
		/// レイヤーの追加
		/// </summary>
		/// <param name="size">レイヤーの大きさ</param>
		/// <param name="level">八分木空間の分割レベル</param>
		/// <param name="isCheckSame">同じレイヤーに登録されているオブジェクトと衝突するか</param>
		virtual void AddLayer(const Vector3& size, const std::int32_t level, bool isCheckSame)= 0;
		/// <summary>
		/// レイヤーの削除
		/// </summary>
		/// <param name="index">レイヤーの番号</param>
		virtual void RemoveLayer(const std::int32_t index)= 0;
		/// <summary>
		/// レイヤー登録外のオブジェクトとの衝突判定
		/// </summary>
		/// <param name="arg_prim">判定する形状</param>
		/// <param name="index">レイヤーのインデックス</param>
		/// <returns>trueで衝突している</returns>
		virtual bool IsWillHit(Value_ptr< Collision::CollisionPrimitive>arg_prim,const std::int32_t index) = 0;
		/// <summary>
		/// レイヤー登録外のオブジェクトとの衝突判定
		/// </summary>
		/// <param name="arg_prim">判定する形状</param>
		/// <param name="index">レイヤーのインデックス</param>
		/// <returns>衝突しているオブジェクト</returns>
		virtual Value_ptr<GameObject> GetWillHitObject(Value_ptr< Collision::CollisionPrimitive>arg_prim,const std::int32_t index) = 0;
		/// <summary>
		/// レイヤー登録外のオブジェクトとの衝突判定
		/// </summary>
		/// <param name="arg_prim">判定する形状</param>
		/// <param name="index">レイヤーのインデックス</param>
		/// <returns>衝突しているオブジェクトのvector</returns>
		virtual std::vector<Value_ptr<GameObject>> GetWillHitObjects(Value_ptr< Collision::CollisionPrimitive>arg_prim,const std::int32_t index)= 0;
		/// <summary>
		/// GUI呼び出し
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
	class ICamera;

	/// <summary>
	/// 描画オブジェクトレイヤーのインターフェース
	/// </summary>
	class IDrawLayer :public IObject {
	public:

		void Initialize()override {};
		void PreInitialize()override {}

		/// <summary>
		/// 中身の削除
		/// </summary>
		virtual void Clear() = 0;
		/// <summary>
		/// レンダリング処理の前に行う処理を走らせる
		/// </summary>
		virtual void BefRendering() = 0;
		/// <summary>
		/// 描画オブジェクトを登録
		/// </summary>
		/// <param name="arg_wkp_drawObject">描画オブジェクト</param>
		/// <param name="arg_isAfterRendering">遅らせて描画するか(半透明オブジェクト等)</param>
		/// <param name="arg_ret_pim">描画オブジェクトの形状</param>
		/// <param name="arg_isShadow">影を生成するオブジェクトか</param>
		virtual void Regist(Value_ptr< IDrawObject> arg_wkp_drawObject, const bool arg_isAfterRendering, Value_ptr<Collision::CollisionPrimitive_Box_OBB> arg_ret_pim = nullptr, const bool arg_isShadow = false) = 0;
		/// <summary>
		/// 描画オブジェクトの登録解除
		/// </summary>
		/// <param name="arg_shp_drawObject">描画オブジェクト</param>
		/// <param name="arg_isAfterRendering">描画を遅らせるか</param>
		/// <param name="arg_isShadow">影を生成するオブジェクトか</param>
		virtual void UnRegist(Value_ptr< IDrawObject> arg_shp_drawObject, const bool arg_isAfterRendering,  const bool arg_isShadow = false) = 0;
		/// <summary>
		/// シャドウ撮影用カメラの登録
		/// </summary>
		/// <param name="arg_shadowCamera"></param>
		virtual void SetShadowCamera(Value_ptr<ICamera> arg_shadowCamera) = 0;
		/// <summary>
		/// シャドウ読み込みテクスチャの登録
		/// </summary>
		/// <param name="arg_textureTag"></param>
		virtual void SetShadowTexture(TextureTag arg_textureTag) = 0;
		/// <summary>
		/// レンダリングの実行
		/// </summary>
		virtual void Rendering() = 0;
		/// <summary>
		/// 描画コマンドの構築
		/// </summary>
		virtual void CommandSet() = 0;
		/// <summary>
		/// 影のレンダリング
		/// </summary>
		virtual void ShadowRendering() = 0;
		/// <summary>
		/// 衝突レイヤーの取得(現在使用不可)
		/// </summary>
		/// <returns></returns>
		virtual Value_ptr<Collision::CollisionLayer<IDrawObject>> GetCollisionLayer() = 0;
		/// <summary>
		/// シャドウ用カメラの取得
		/// </summary>
		/// <returns></returns>
		virtual Value_ptr<ICamera> GetShadowCamera() = 0;
		/// <summary>
		/// シャドウ用テクスチャの取得
		/// </summary>
		/// <returns></returns>
		virtual TextureTag GetShadowTexture() = 0;
	};
	/// <summary>
	/// 影用の描画レイヤー
	/// </summary>
	class IDrawLayer_Shadow {
	public:
		/// <summary>
		/// 影を書き込んだかの取得
		/// </summary>
		/// <returns></returns>
		virtual bool IsShadowDrawed() = 0;
	};

	class IResourceContainer;

	/// <summary>
	/// シーンが所持する描画用クラス
	/// </summary>
	class IRenderer :public IObject {
	public:
		void PreInitialize()override {}
		virtual void Update() = 0;

		/// <summary>
		/// 全体のレンダリング開始
		/// </summary>
		virtual void RenderingStart() = 0;
		/// <summary>
		/// レンダリング開始前の処理
		/// </summary>
		virtual void BefRendering() = 0;
		/// <summary>
		/// 描画レイヤの追加
		/// </summary>
		virtual void AddLayer() = 0;
		/// <summary>
		/// レイヤーの数の取得
		/// </summary>
		/// <returns>レイヤー数</returns>
		virtual std::uint32_t GetLayerCount() = 0;
		/// <summary>
		/// 描画レイヤーのレンダリング
		/// </summary>
		/// <param name="arg_layerIndex">描画するレイヤーの番号</param>
		virtual void Rendering(const std::uint32_t arg_layerIndex) = 0;
		/// <summary>
		/// 描画レイヤーの影のレンダリング
		/// </summary>
		/// <param name="arg_layerIndex">描画するレイヤーの番号</param>
		virtual void ShadowRendering(const std::uint32_t arg_layerIndex) = 0;
		/// <summary>
		/// 全体のレンダリング終了
		/// </summary>
		virtual void RenderingEnd() = 0;
		/// <summary>
		/// 描画オブジェクトの解放
		/// </summary>
		virtual void ClearDrawObjects() = 0;
		//所属しているシーンの取得
		virtual Value_ptr<IScene> GetScene() = 0;
		/// <summary>
		/// 描画レイヤーに対するシャドウ用テクスチャの設定
		/// </summary>
		/// <param name="arg_layerIndex">設定するレイヤーの番号</param>
		/// <param name="arg_shadowTex">設定するシャドウ用テクスチャ</param>
		virtual void SetShadowTexture(const std::uint32_t arg_layerIndex, TextureTag arg_shadowTex) = 0;
		/// <summary>
		/// 描画レイヤーが使用するシャドウ用テクスチャの取得
		/// </summary>
		/// <param name="arg_layerIndex">レイヤーの番号</param>
		/// <returns>使用しているシャドウ用テクスチャ</returns>
		virtual TextureTag GetShadowTexture(const std::uint32_t arg_layerIndex) = 0;
		/// <summary>
		/// 描画オブジェクトの登録
		/// </summary>
		/// <param name="arg_shp_drawObject">描画オブジェクト</param>
		/// <param name="arg_afterDraw">遅らせて描画するか(半透明オブジェクト等)</param>
		/// <param name="arg_layerIndex">登録する描画レイヤーの番号</param>
		/// <param name="isShadow">影を落とすか</param>
		virtual void RegistDrawObject(Value_ptr< IDrawObject> arg_shp_drawObject, const bool arg_afterDraw, const std::uint32_t arg_layerIndex = 0, const bool isShadow = false) = 0;
		/// <summary>
		/// 描画オブジェクトの登録解除
		/// </summary>
		/// <param name="arg_shp_drawObject">描画オブジェクト</param>
		/// <param name="arg_afterDraw">遅らせて描画するか(半透明オブジェクト等)</param>
		/// <param name="arg_layerIndex">登録する描画レイヤーの番号</param>
		/// <param name="isShadow">影を落とすか</param>
		virtual void UnRegistDrawObject(Value_ptr< IDrawObject> arg_shp_drawObject, const bool arg_afterDraw, const std::uint32_t arg_layerIndex = 0, const bool isShadow = false) = 0;
		/// <summary>
		/// GUIの呼び出し
		/// </summary>
		virtual void ShowUI() = 0;
		/// <summary>
		/// 解放処理
		/// </summary>
		virtual void Release() = 0;
		/// <summary>
		/// Renderer制御の定数バッファ更新
		/// </summary>
		/// <param name="arg_param">更新する値</param>
		virtual void UpdateRendererCBuffer(const RenderingSceneInfo& arg_param) = 0;
		/// <summary>
		/// Renderer制御の定数バッファ解放
		/// </summary>
		virtual void ReleaseRendererCBuffer() = 0;
		/// <summary>
		/// Renderer制御の定数バッファ取得
		/// </summary>
		/// <returns>Renderer制御の定数バッファ</returns>
		virtual Value_ptr<CBuffer<RenderingSceneInfo>> GetRendererCBuffer() = 0;
		/// <summary>
		/// シャドウ用カメラの設定
		/// </summary>
		/// <param name="arg_layer">設定するレイヤーの番号</param>
		/// <param name="arg_shp_camera">設定するカメラ</param>
		virtual void SetShadowCamera(const std::uint32_t arg_layer,Value_ptr<ICamera> arg_shp_camera)=0;
		/// <summary>
		/// シャドウ用カメラの取得
		/// </summary>
		/// <param name="arg_layer">取得するレイヤーの番号</param>
		/// <returns>シャドウ用カメラ</returns>
		virtual Value_ptr<ICamera> GetShadowCamera(const std::uint32_t arg_layer) = 0;
		/// <summary>
		/// 描画オブジェクトとの衝突判定（現在使用不可）
		/// </summary>
		/// <param name="arg_prim">衝突判定に使う形状</param>
		/// <param name="arg_layer">判定するレイヤーの番号</param>
		/// <returns>衝突している描画オブジェクトのvector</returns>
		virtual std::vector< Value_ptr<IDrawObject>> GetHitDrawObjects(Value_ptr<Collision::CollisionPrimitive> arg_prim, const std::int32_t arg_layer)=0;
		/// <summary>
		/// 使用しているResourceContainerの取得
		/// </summary>
		/// <returns>使用しているResourceContainer</returns>
		virtual Value_ptr<IResourceContainer> GetResourceContainer()=0;
	};

	/// <summary>
	/// ウィンドウ制御インターフェース
	/// </summary>
	class IWindow
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="arg_windowName"ウィンドウの名前></param>
		/// <param name="arg_popType">出現タイプ</param>
		/// <param name="isFullScreen">フルスクリーン表示</param>
		/// <param name="width">幅</param>
		/// <param name="height">高さ</param>
		virtual void Initialize(const std::string arg_windowName, const WindowPopType arg_popType,const bool isFullScreen,const std::uint32_t width = 0,const std::uint32_t height = 0)=0;
		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~IWindow() {}
		/// <summary>
		/// ウィンドウハンドルの取得
		/// </summary>
		/// <returns>ウィンドウハンドル</returns>
		virtual HWND GetHandle()=0;
		/// <summary>
		/// サイズの取得
		/// </summary>
		/// <returns></returns>
		virtual UInt2 GetSize()=0;
		/// <summary>
		/// ウィンドウの中心位置の取得
		/// </summary>
		/// <returns>ウィンドウの中心位置</returns>
		virtual Vector2 GetWindowCenterPosition()=0;
		/// <summary>
		/// サイズの設定
		/// </summary>
		/// <param name="width">幅</param>
		/// <param name="height">高さ</param>
		virtual void SetSize(const std::int32_t width,const std::int32_t height)=0;
		virtual bool Update()=0;
		/// <summary>
		/// ウィンドウの左上端の取得
		/// </summary>
		/// <returns>ウィンドウの左上端の座標</returns>
		virtual Vector2 GetWindowPosition()=0;
		/// <summary>
		/// 解放処理
		/// </summary>
		virtual void Release()=0;
		/// <summary>
		/// 選択状態か
		/// </summary>
		/// <returns>true->選択中　false->選択状態でない</returns>
		virtual bool IsSelected() = 0;
		/// <summary>
		/// 選択状態に切り替わったフレームか
		/// </summary>
		/// <returns>true->選択状態に切り替わったフレームである　false->選択状態に切り替わったフレームでない</returns>
		virtual bool TriggerSelected() = 0;
#ifdef _EDITORBUILD
		/// <summary>
		/// エディタのウィンドウの選択状態をセットする(EditorBuildのみ)
		/// </summary>
		/// <param name="arg_isActive">エディタの選択状態</param>
		/// <returns>現在の選択状態</returns>
		virtual bool SetCurrentSelect(const bool arg_isSelect) = 0;
#endif // _EDITORBUILD

	private:

	};

	/// <summary>
	/// SE、BGM管理インターフェース
	/// </summary>
	class ISoundManager :public IObject {

	public:
		void PreInitialize()override {}

		virtual void Update() = 0;
		/// <summary>
		/// SEを再生する
		/// </summary>
		/// <param name="tag">SEのタグ</param>
		/// <param name="volume">音量</param>
		virtual void PlaySE(const SoundTag tag,const float volume) = 0;
		/// <summary>
		/// BGMを再生する
		/// </summary>
		/// <param name="tag">BGMのタグ</param>
		/// <param name="volume">音量</param>
		virtual void PlayBGM(const SoundTag tag, const  float volume) = 0;
		/// <summary>
		/// 後から音量を換えられるSEを再生する
		/// </summary>
		/// <param name="tag">SEのタグ</param>
		/// <param name="index">SEの番号(0～64)</param>
		/// <param name="volume">音量</param>
		/// <param name="isLoop">ループするかどうか</param>
		virtual void PlayControllableSE(const SoundTag tag, const std::uint32_t index, const float volume,const bool isLoop)=0;
		/// <summary>
		/// SEの音量を変更する
		/// </summary>
		/// <param name="index">SEの番号(0～64)</param>
		/// <param name="volume">音量</param>
		virtual void SetControllableSEVolume(const std::uint32_t index, const float volume) = 0;
		/// <summary>
		/// SEのループを抜ける
		/// </summary>
		/// <param name="index">SEの番号(0～64)</param>
		virtual void ExitControllableSELoop(const std::uint32_t index) = 0;
		/// <summary>
		/// SEを終了させる
		/// </summary>
		/// <param name="index">SEの番号(0～64)</param>
		virtual void DestroyControllableSE(const std::uint32_t index) = 0;
		/// <summary>
		/// SEを開始する
		/// </summary>
		/// <param name="index">SEの番号(0～64)</param>
		virtual void StartontrollableSE(const std::uint32_t index) = 0;
		/// <summary>
		/// SEを停止する
		/// </summary>
		/// <param name="index"></param>
		virtual void StopControllableSE(const std::uint32_t index) = 0;
		/// <summary>
		/// 全てのSEを停止する
		/// </summary>
		virtual void StopSE() = 0;
		/// <summary>
		/// BGMを停止する
		/// </summary>
		virtual void StopBGM() = 0;
		/// <summary>
		/// SEを開始する
		/// </summary>
		virtual void RestartSE() = 0;
		/// <summary>
		/// BGMを開始する
		/// </summary>
		virtual void RestartBGM() = 0;
		/// <summary>
		/// SEを終了させる
		/// </summary>
		virtual void DestroySE() = 0;
		/// <summary>
		/// BGMを終了させる
		/// </summary>
		virtual void DestroyBGM() = 0;
		/// <summary>
		/// BGMの音量を設定する
		/// </summary>
		/// <param name="volume"></param>
		virtual void SetBGMVolume(const float volume) = 0;
		/// <summary>
		/// 解放処理
		/// </summary>
		virtual void Release() = 0;
		/// <summary>
		/// 現在再生しているBGMの取得
		/// </summary>
		/// <returns>現在再生しているBGM</returns>
		virtual SoundTag GetCurrentPlayBGM()const = 0;
		/// <summary>
		/// BGMの音量の取得
		/// </summary>
		/// <returns>音量</returns>
		virtual float GetBGMVolume()const = 0;
		/// <summary>
		/// SEの音量の取得
		/// </summary>
		/// <param name="index">SEの番号(0～64)</param>
		/// <returns>音量<returns>
		virtual float GetControllableSEVolume(const std::int32_t index)const = 0;
	};

	/// <summary>
	/// シーン管理インターフェース
	/// </summary>
	class ISceneManager :public IObject {

	public:

		virtual ~ISceneManager() {}
		virtual inline void Update() = 0;
		/// <summary>
		/// シーンの設定
		/// </summary>
		/// <param name="arg_sceneName">シーン名</param>
		/// <param name="shp_scene">シーンのインスタンス</param>
		virtual void SetScene(const std::string& arg_sceneName, Value_ptr<IScene> shp_scene) = 0;
		/// <summary>
		/// ゲーム開始時のジーン設定
		/// </summary>
		/// <param name="arg_sceneName">シーン名</param>
		/// <param name="shp_scene">シーンのインスタンス</param>
		virtual void SetScene_Init(const std::string& arg_sceneName, Value_ptr<IScene> shp_scene ) = 0;
		/// <summary>
		/// シーンのロード
		/// </summary>
		/// <param name="arg_sceneName">シーン名</param>
		/// <param name="shp_scene">シーンのインスタンス(nullなら名前から生成)</param>
		virtual void LoadScene(const std::string& arg_sceneName, Value_ptr<SceneInformation> shp_scene = nullptr) = 0;
		/// <summary>
		/// シーンのロード(非同期)
		/// </summary>
		/// <param name="arg_sceneName">シーン名</param>
		virtual void LoadScene_async(const std::string& arg_sceneName) = 0;
		/// <summary>
		/// ゲーム開始時のシーンのロード
		/// </summary>
		/// <param name="arg_sceneName">シーン名</param>
		/// <param name="shp_scene">シーンのインスタンス(nullなら名前から生成)</param>
		virtual void LoadScene_Init(const std::string& arg_sceneName, Value_ptr<SceneInformation> shp_scene = nullptr) = 0;
		/// <summary>
		/// シーンの削除
		/// </summary>
		/// <param name="arg_sceneName">シーン名</param>
		virtual void RemoveScene(const std::string& arg_sceneName) = 0;
		/// <summary>
		/// シーンの切り替え
		/// </summary>
		/// <param name="arg_sceneName">シーン名</param>
		/// <param name="sceneChangeDalay">切り替えに掛るフレーム</param>
		virtual void ChangeScene(const std::string& arg_sceneName, float sceneChangeDalay = 0) = 0;
		/// <summary>
		/// 新しいシーンへの更新
		/// </summary>
		virtual void RenewalScene() = 0;
		/// <summary>
		/// シーンのリロード
		/// </summary>
		virtual void ReloadScene() = 0;
		/// <summary>
		/// シーンのリロード
		/// </summary>
		/// <param name="arg_sceneName">リロードするシーン名</param>
		virtual void ReloadScene(const std::string& arg_sceneName)=0;
		/// <summary>
		/// 現在のシーンの取得
		/// </summary>
		/// <returns>現在のシーン</returns>
		virtual inline Value_ptr<IScene> GetCurrentScene() = 0;
		/// <summary>
		/// アプリケーションの取得
		/// </summary>
		/// <returns>アプリケーション</returns>
		virtual Value_ptr<IApplication> GetApplication() = 0;
		/// <summary>
		/// 解放処理
		/// </summary>
		virtual void Release() = 0;
		/// <summary>
		/// シーンのロード中か
		/// </summary>
		/// <returns>true=ロード中/false=ロードしていない</returns>
		virtual bool IsLoading() = 0;
#ifdef _EDITORBUILD
		/// <summary>
		/// シーンの起動、停止
		/// </summary>
		/// <param name="arg_isActive">true=起動/false=停止</param>
		/// <returns>現在の状態</returns>
		virtual bool SetActive(const bool arg_isActive) = 0;
		virtual void ReloadButtonPerform() = 0;
		virtual void SaveButtonPerform() = 0;
		/// <summary>
		/// シーンの描画にかかった時間の取得
		/// </summary>
		/// <returns>シーンの描画にかかった時間(ミリ秒)</returns>
		virtual std::int32_t GetDrawMilliSecond()const = 0;
		/// <summary>
		/// シーンの更新にかかった時間の取得
		/// </summary>
		/// <returns>シーンの更新にかかった時間(ミリ秒)</returns>
		virtual std::int32_t GetUpdateMilliSecond()const = 0;
		/// <summary>
		/// シーンの描画にかかった時間の取得(60フレームの平均)
		/// </summary>
		/// <returns>直近60フレームにおけるシーンの描画にかかった時間の平均</returns>
		virtual float GetDrawMilliSecondAverage()const = 0;
		/// <summary>
		/// シーンの更新にかかった時間の取得(60フレームの平均)
		/// </summary>
		/// <returns>直近60フレームにおけるシーンの更新にかかった時間の平均(ミリ秒)</returns>
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
		MaterialValue var;
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
	/// リソース管理インターフェース
	/// </summary>
	class IResourceContainer :public IObject
	{
	public:

		virtual void InputReloadData()=0;
		virtual void OutputReloadData()=0;

		/// <summary>
		/// 描画デバイスの設定
		/// </summary>
		/// <param name="arg_shp_graphicDevice">描画デバイス</param>
		virtual void SetGraphicDevice(Value_weak_ptr<GraphicDevice> arg_shp_graphicDevice)=0;

		/// <summary>
		/// GUIの呼び出し
		/// </summary>
		virtual void ShowGUI()=0;


		/// <summary>
		/// 起動時のメッシュの読み込み
		/// </summary>
		virtual void PreLoadMeshLoad() = 0;
		/// <summary>
		/// リソース初期読み込み時のメッシュ読み込み
		/// </summary>
		virtual void InitMeshLoad() = 0;

		/// <summary>
		/// 値を直接指定してマテリアル生成
		/// </summary>
		/// <param name="arg_resourceMaterial">マテリアル情報</param>
		/// <param name="arg_textureTag">テクスチャ情報</param>
		/// <param name="arg_materialName">マテリアルの名前</param>
		/// <returns>読み込んだマテリアルのタグ</returns>
		virtual MaterialTag LoadMaterial(const MaterialValue& arg_resourceMaterial, const std::vector< TextureTag>& arg_textureTag, const std::string& arg_materialName) =0;
		/// <summary>
		/// マテリアルをファイルから読み込み
		/// </summary>
		/// <param name="arg_fileName">ファイルの名前</param>
		/// <param name="arg_fileDirectory">ファイルのディレクトリ</param>
		/// <returns>読み込んだマテリアルのタグ</returns>
		virtual MaterialTag LoadMaterial(const std::string& arg_fileName, const std::string& arg_fileDirectory )=0;
		/// <summary>
		/// マテリアルをファイルから読み込み
		/// </summary>
		/// <param name="arg_fileName">ファイルの名前</param>
		/// <returns>読み込んだマテリアルのタグ</returns>
		inline MaterialTag LoadMaterial(const std::string& arg_fileName) {
			static std::string dir = "";
			return LoadMaterial(arg_fileName, dir);
		}
		/// <summary>
		/// マテリアルの一括読み込み
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">読み込みファイルパスのvector</param>
		/// <returns>読み込んだマテリアルのタグのvector</returns>
		virtual std::vector < MaterialTag> LoadMaterial(const std::vector<std::string>& arg_vec_filePathAndDirectory) = 0;
		/// <summary>
		/// マテリアルの一括読み込み
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">読み込み情報のvector</param>
		/// <returns>読み込んだマテリアルのタグのvector</returns>
		virtual std::vector < MaterialTag> LoadMaterial(const std::vector<MaterialLoadInfo>& arg_vec_loadInfo)=0;
		/// <summary>
		/// マテリアルの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイルの名前</param>
		/// <param name="arg_fileDirectory">ファイルディレクトリ</param>
		/// <returns>読み込んだマテリアルのタグ</returns>
		virtual MaterialTag LoadMaterial(const std::wstring& arg_filePath, const std::string& arg_fileDirectory )=0;
		/// <summary>
		/// テクスチャ読み込み
		/// </summary>
		/// <param name="arg_fileName">テクスチャの名前</param>
		/// <param name="arg_fileDirectory">テクスチャのディレクトリ</param>
		/// <returns></returns>
		virtual TextureTag LoadTexture(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// テクスチャ読み込み
		/// </summary>
		/// <param name="arg_fileName">テクスチャの名前</param>
		/// <returns></returns>
		inline TextureTag LoadTexture(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadTexture(arg_filePath, dir);
		}
		/// <summary>
		/// テクスチャ作成
		/// </summary>
		/// <param name="arg_texName">テクスチャの検索に使用する名前</param>
		/// <param name="arg_p_pixelSrc">画像データ</param>
		/// <param name="arg_width">横サイズ</param>
		/// <param name="arg_height">縦サイズ</param>
		/// <param name="arg_pixelSize">一ピクセル辺りの大きさ</param>
		/// <param name="arg_format">フォーマット</param>
		/// <returns></returns>
		virtual TextureTag LoadTexture(const std::string& arg_texName, void* arg_p_pixelSrc, const std::int32_t arg_width, const std::int32_t arg_height, const std::int32_t arg_pixelSize, const std::int32_t arg_format) = 0;
		/// <summary>
		/// テクスチャ作成
		/// </summary>
		/// <param name="arg_texName">テクスチャの検索に使用する名前</param>
		/// <param name="arg_texResourceData">テクスチャの情報</param>
		/// <returns></returns>
		virtual TextureTag LoadTexture(const std::string& arg_texName,Value_ptr<ImageFileIO::TextureResourceData> arg_texResourceData) = 0;
		/// <summary>
		/// テクスチャの一括読み込み
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">テクスチャパスのvector</param>
		/// <returns>読み込んだテクスチャのタグのvector</returns>
		virtual std::vector < TextureTag> LoadTexture(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;
		/// <summary>
		/// ピクセルシェーダの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <param name="arg_fileDirectory">ファイルのディレクトリ</param>
		/// <returns>読み込んだシェーダのタグ</returns>
		virtual PixelShaderTag LoadPixelShader(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// ピクセルシェーダの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <returns>読み込んだシェーダのタグ</returns>
		inline PixelShaderTag LoadPixelShader(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadPixelShader(arg_filePath, dir);
		}
		/// <summary>
		/// ピクセルシェーダの一括読み込み
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">ファイルパスのvector</param>
		/// <returns>読み込んだシェーダのタグのvector</returns>
		virtual std::vector < PixelShaderTag> LoadPixelShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		/// <summary>
		/// 頂点シェーダの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <param name="arg_fileDirectory">ファイルのディレクトリ</param>
		/// <returns>読み込んだシェーダのタグ</returns>
		virtual VertexShaderTag LoadVertexShader(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// 頂点シェーダの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <returns>読み込んだシェーダのタグ</returns>
		inline VertexShaderTag LoadVertexShader(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadVertexShader(arg_filePath, dir);
		}
		/// <summary>
		/// 頂点シェーダの一括読み込み
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">ファイルパスのvector</param>
		/// <returns>読み込んだシェーダのタグのvector</returns>
		virtual std::vector < VertexShaderTag> LoadVertexShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		/// <summary>
		/// ジオメトリシェーダの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <param name="arg_fileDirectory">ファイルのディレクトリ</param>
		/// <returns>読み込んだシェーダのタグ</returns>
		virtual GeometryShaderTag LoadGeometryShader(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// ジオメトリシェーダの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <returns>読み込んだシェーダのタグ</returns>
		inline GeometryShaderTag LoadGeometryShader(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadGeometryShader(arg_filePath, dir);
		}
		/// <summary>
		/// ジオメトリシェーダの一括読み込み
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">ファイルパスのvector</param>
		/// <returns>読み込んだシェーダのタグのvector</returns>
		virtual std::vector < GeometryShaderTag> LoadGeometryShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		/// <summary>
		/// シェーダの生成
		/// </summary>
		/// <param name="arg_shaderInfos">シェーダの情報</param>
		/// <returns>生成したシェーダのタグ</returns>
		virtual ShaderTag LoadShader(const  ShaderLoadInfo& arg_shaderInfos)=0;
		/// <summary>
		/// シェーダの一括生成
		/// </summary>
		/// <param name="arg_vec_shaderInfos">シェーダ情報のvector</param>
		/// <returns>生成したシェーダのvector</returns>
		virtual std::vector < ShaderTag> LoadShader(const std::vector<ShaderLoadInfo>& arg_vec_shaderInfos)=0;
		/// <summary>
		/// 音声の読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <param name="arg_fileDirectory">ファイルのディレクトリ</param>
		/// <returns>読み込んだ音声のタグ</returns>
		virtual SoundTag LoadSound(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// 音声の読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <returns>読み込んだ音声のタグ</returns>
		inline SoundTag LoadSound(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadSound(arg_filePath, dir);
		}
		/// <summary>
		/// 音声の一括読み込み
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">ファイルパスのvector</param>
		/// <returns>読み込んだ音声のvector</returns>
		virtual std::vector < SoundTag> LoadSound(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		/// <summary>
		/// モデルデータの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <param name="arg_fileDirectory">ファイルのディレクトリ</param>
		/// <returns>読み込んだモデルのタグ</returns>
		virtual ModelTag LoadModel(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// モデルデータの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <returns>読み込んだモデルのタグ</returns>
		inline ModelTag LoadModel(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadModel(arg_filePath, dir);
		}
		/// <summary>
		/// モデルデータの一括読み込み
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">ファイルパスのvector</param>
		/// <returns>読み込んだモデルのタグのvector</returns>
		virtual std::vector<ModelTag> LoadModel(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		/// <summary>
		/// モーションデータの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <param name="arg_fileDirectory">ファイルディレクトリ</param>
		/// <returns>読み込んだモーションのタグ</returns>
		virtual MotionTag LoadMotion(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// モーションデータの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <returns>読み込んだモーションのタグ</returns>
		inline MotionTag LoadMotion(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadMotion(arg_filePath, dir);
		}
		/// <summary>
		/// モーションデータの一括読み込み
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">ファイルパスのvector</param>
		/// <returns>読み込んだモーションのタグのvector</returns>
		virtual std::vector<MotionTag> LoadMotion(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;
		/// <summary>
		/// コンパイル済みスクリプトの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <param name="arg_fileDirectory">ファイルディレクトリ</param>
		/// <returns>読み込んだスクリプトのタグ</returns>
		virtual ScriptTag LoadScript(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// コンパイル済みスクリプトの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <returns>読み込んだスクリプトのタグ</returns>
		inline ScriptTag LoadScript(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadScript(arg_filePath, dir);
		}
		/// <summary>
		/// スクリプトの一括読み込み
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">ファイルパスのvector</param>
		/// <returns>読み込んだスクリプトのタグのvector</returns>
		virtual std::vector<ScriptTag> LoadScript(const std::vector<std::string>& arg_vec_filePathAndDirectory) = 0;


		/// <summary>
		/// TrueTypeFontの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <param name="arg_fileDirectory">ファイルディレクトリ</param>
		/// <returns>読み込んだフォントのタグ</returns>
		virtual FontTag LoadFont(const std::string& arg_filePath, const std::string& arg_fileDirectory) = 0;
		/// <summary>
		/// TrueTypeFontの読み込み
		/// </summary>
		/// <param name="arg_filePath">ファイル名</param>
		/// <returns>読み込んだフォントのタグ</returns>
		inline FontTag LoadFont(const std::string& arg_filePath) {
			static std::string dir = "";
			return LoadFont(arg_filePath, dir);
		}
		/// <summary>
		/// フォントの一括読み込み
		/// </summary>
		/// <param name="arg_vec_filePathAndDirectory">ファイルパスのvector</param>
		/// <returns>読み込んだフォントのタグのvector</returns>
		virtual std::vector < FontTag> LoadFont(const std::vector<std::string>& arg_vec_filePathAndDirectory) = 0;
		/// <summary>
		/// テキスト描画用メッシュの生成
		/// </summary>
		/// <param name="arg_glyphMesh">更新用の名前</param>
		/// <param name="glyphCount">文字数</param>
		/// <returns></returns>
		virtual MeshTag CreateTextMesh(const std::string& arg_glyphMesh, const std::int32_t glyphCount) = 0;
		/// <summary>
		/// スクリプトのソースファイルが更新されていないかチェック
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
		/// メッシュの削除
		/// </summary>
		/// <param name="arg_meshTag">削除するタグ</param>
		virtual void UnLoad(MeshTag arg_meshTag)=0;
		/// <summary>
		/// テクスチャの削除
		/// </summary>
		/// <param name="arg_textureTag">削除するタグ</param>
		virtual void UnLoad(TextureTag arg_textureTag)=0;
		/// <summary>
		/// シェーダの削除
		/// </summary>
		/// <param name="arg_shaderTag">削除するタグ</param>
		virtual void UnLoad(ShaderTag arg_shaderTag)=0;
		/// <summary>
		/// ピクセルシェーダの削除
		/// </summary>
		/// <param name="arg_shaderTag">削除するタグ</param>
		virtual void UnLoad(PixelShaderTag arg_shaderTag)=0;
		/// <summary>
		/// 頂点シェーダの削除
		/// </summary>
		/// <param name="arg_shaderTag">削除するタグ</param>
		virtual void UnLoad(VertexShaderTag arg_shaderTag)=0;
		/// <summary>
		/// ジオメトリシェーダの削除
		/// </summary>
		/// <param name="arg_shaderTag">削除するタグ</param>
		virtual void UnLoad(GeometryShaderTag arg_shaderTag)=0;
		/// <summary>
		/// 音声の削除
		/// </summary>
		/// <param name="arg_soundTag">削除するタグ</param>
		virtual void UnLoad(SoundTag arg_soundTag)=0;
		/// <summary>
		/// モデルの削除
		/// </summary>
		/// <param name="arg_modelTag">削除するタグ</param>
		virtual void UnLoad(ModelTag arg_modelTag)=0;
		/// <summary>
		/// マテリアルの削除
		/// </summary>
		/// <param name="arg_materialTag">削除するタグ</param>
		virtual void UnLoad(MaterialTag arg_materialTag)=0;
		/// <summary>
		/// モーションの削除
		/// </summary>
		/// <param name="arg_motionTag">削除するタグ</param>
		virtual void UnLoad(MotionTag arg_motionTag) = 0;
		/// <summary>
		/// スクリプトの削除
		/// </summary>
		/// <param name="arg_motionTag">削除するタグ</param>
		virtual void UnLoad(ScriptTag arg_scriptTag) = 0;
		/// <summary>
		/// フォントの削除
		/// </summary>
		/// <param name="arg_motionTag">削除するタグ</param>
		virtual void UnLoad(FontTag arg_fontTag) = 0;

		/// <summary>
		/// 解放処理
		/// </summary>
		virtual void Release()=0;

		/// <summary>
		/// マテリアルの更新
		/// </summary>
		virtual void MaterialUpdate()=0;

		/// <summary>
		/// メッシュの取得
		/// </summary>
		/// <param name="arg_key">検索タグ</param>
		/// <returns>メッシュのリソース</returns>
		virtual Value_weak_ptr<IResource_Mesh> GetMesh(const  MeshTag& arg_key)=0;
		/// <summary>
		/// テクスチャの取得
		/// </summary>
		/// <param name="arg_key">検索タグ</param>
		/// <returns>テクスチャのリソース</returns>
		virtual Value_weak_ptr<IResource_Texture> GetTexture(const TextureTag& arg_key)=0;
		/// <summary>
		/// シェーダの取得
		/// </summary>
		/// <param name="arg_key">検索タグ</param>
		/// <returns>シェーダのリソース</returns>
		virtual Value_weak_ptr<IResource_Shader> GetShader(const ShaderTag& arg_key)=0;
		/// <summary>
		/// 音声の取得
		/// </summary>
		/// <param name="arg_key">検索タグ</param>
		/// <returns>音声のリソース</returns>
		virtual Value_weak_ptr<IResource_Sound> GetSound(const SoundTag& arg_key)=0;
		/// <summary>
		/// マテリアルの取得
		/// </summary>
		/// <param name="arg_key">検索タグ</param>
		/// <returns>マテリアルのリソース</returns>
		virtual Value_weak_ptr<IResource_Material>GetMaterial(const MaterialTag& arg_key)=0;
		/// <summary>
		/// 遅延シェーディング用のマテリアルリストの取得
		/// </summary>
		/// <returns>遅延シェーディング用のマテリアルパラメータのvector</returns>
		virtual std::vector<MaterialValue_Deferred> CreateDeferredMaterialList() = 0;
		/// <summary>
		/// モデルの取得
		/// </summary>
		/// <param name="arg_key">検索タグ</param>
		/// <returns>モデルのリソース</returns>
		virtual Value_weak_ptr<IResource_Model>GetModel(const ModelTag& arg_key)=0;
		/// <summary>
		/// モーションの取得
		/// </summary>
		/// <param name="arg_key">検索タグ</param>
		/// <returns>モーションのリソース</returns>
		virtual Value_ptr<ModelAnimation>GetMotion(const MotionTag& arg_key)=0;
		/// <summary>
		/// コンパイル済みスクリプトの取得
		/// </summary>
		/// <param name="arg_key">検索タグ</param>
		/// <returns>スクリプトのリソース</returns>
		virtual Value_weak_ptr<IResource_Script>GetScript(const ScriptTag& arg_key) = 0;
		/// <summary>
		/// フォントの取得
		/// </summary>
		/// <param name="arg_key">検索タグ</param>
		/// <returns>フォントのリソース</returns>
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
		/// 再読み込み
		/// </summary>
		virtual void Reload() = 0;
		/// <summary>
		/// 再読み込み(非同期)
		/// </summary>
		virtual void Reload_async() = 0;

		/// <summary>
		/// 現在読み込み中かどうか
		/// </summary>
		/// <returns></returns>
		virtual bool IsLoading() = 0;
	};

	/// <summary>
	/// アプリケーションのインターフェース
	/// </summary>
	class IApplication :public IObject
	{
	public:
		//アプリケーション初期化用構造体
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
		/// ウィンドウの取得
		/// </summary>
		/// <returns></returns>
		virtual std::unique_ptr<IWindow>& GetWindow()=0;
		/// <summary>
		/// シーンマネージャーの取得
		/// </summary>
		/// <returns></returns>
		virtual Value_ptr<ISceneManager> GetSceneManager()=0;
		/// <summary>
		/// 描画デバイスの取得
		/// </summary>
		/// <returns></returns>
		virtual Value_ptr<GraphicDevice> GetGraphicDevice()=0;
		/// <summary>
		/// リソース管理インターフェースの取得
		/// </summary>
		/// <returns>リソース管理インターフェース</returns>
		virtual Value_ptr<IResourceContainer> GetResourceContainer()=0;
		/// <summary>
		/// ゲームオブジェクトタグ管理インスタンスの取得
		/// </summary>
		/// <returns>ゲームオブジェクトタグ管理インスタンス</returns>
		virtual Value_ptr<GameObjectTagManager> GetGameObjectTagManager()=0;
		/// <summary>
		/// 音声管理インスタンスの取得
		/// </summary>
		/// <returns>音声管理インスタンス</returns>
		virtual Value_ptr<ISoundManager> GetSoundManager() = 0;
		/// <summary>
		/// 更新処理
		/// </summary>
		/// <returns></returns>
		virtual bool Update()=0;
		/// <summary>
		/// 実行
		/// </summary>
		/// <returns></returns>
		virtual std::int32_t Run()=0;
		/// <summary>
		/// 最初に読み込むリソースのロード
		/// </summary>
		virtual void PreLoadResources() = 0;
		/// <summary>
		/// リソースコンテナに記録されているリソースの読み込み
		/// </summary>
		virtual void InitLoadResources() = 0;
		/// <summary>
		/// リソースコンテナに記録されているリソースの読み込み(非同期)
		/// </summary>
		virtual void InitLoadResources_async() = 0;
		/// <summary>
		/// 終了処理
		/// </summary>
		virtual void Exit()=0;
		/// <summary>
		/// 前回のフレームの所要時間(ミリ秒)
		/// </summary>
		/// <returns>前回のフレームの所要時間(ミリ秒)</returns>
		virtual std::int64_t GetBefFrame()const = 0;
		virtual const ApplicationInitData* GetAppInitData()const=0;

#ifdef _EDITORBUILD
		virtual const FrameRateInformation& GetFrameInformation()const=0;
#endif
	};

	struct CameraProjProperty {
		CameraProjProperty() {};
		CameraProjProperty(const std::uint32_t arg_widthScale, const std::uint32_t arg_heightScale, const std::uint32_t arg_x, const std::uint32_t arg_y, const bool arg_isPararell = false, std::uint32_t arg_layer = 0);

		std::int32_t currentWidth = 0, currentHeight = 0,width,height, left = 0, top = 0;
		float front = 0.0f, angle = 60.0f, farClip = 50.0f, nearClip = 0.1f;
		bool isPararell = false;
		std::uint32_t layer = 0;
		std::int32_t renderingInfo = 0;
		std::vector< TextureTag >projectionTexture;
		TextureTag depthStencilTexture;
		std::string cameraName;
		Vector4 clearColor;
		bool isInitActive = true, isEditActive = false, isShadow = false;
		std::vector< TextureTag >vec_shadowTextures;
		std::vector< TextureTag >vec_staticShadowTextures;


		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(width);
			archive(height);
			archive(left);
			archive(top);
			archive(front);
			archive(angle);
			archive(farClip);
			archive(nearClip);
			archive(isPararell);
			archive(layer);
			archive(front);
			archive(renderingInfo);
			archive(projectionTexture);
			archive(depthStencilTexture);
			archive(cameraName);
			archive(clearColor);
			archive(isInitActive);
			archive(isEditActive);
			archive(isShadow);
			archive(vec_shadowTextures);
			archive(vec_staticShadowTextures);
		}

	}; 

	namespace Geometry {
		class Box_AABB;
	}
	/// <summary>
	/// カメラのインターフェース
	/// </summary>
	class ICamera :public IObject {
	public:

		Value_ptr<Transform> shp_transform = ObjectFactory::Create<Transform>(Vector3(0, 0, 0));

		/// <summary>
		/// 描画開始
		/// </summary>
		virtual void Start() = 0;
		/// <summary>
		/// 描画停止
		/// </summary>
		virtual void Stop()const = 0;

		virtual void Initialize()override {}
		virtual void PreInitialize()override {}
		/// <summary>
		/// 名前の取得
		/// </summary>
		/// <returns>名前</returns>
		virtual const std::string& GetName() const = 0;
		/// <summary>
		/// 名前の設定
		/// </summary>
		/// <param name="arg_name">名前</param>
		virtual void SetName(const std::string& arg_name) = 0;
		/// <summary>
		/// 実行するかの設定
		/// </summary>
		/// <param name="arg_active"></param>
		virtual void SetActive(const bool arg_active) = 0;
		/// <summary>
		/// 実行するかの取得
		/// </summary>
		/// <returns></returns>
		virtual bool GetActive()const = 0;
		/// <summary>
		/// 描画処理
		/// </summary>
		virtual void Draw() = 0;
		/// <summary>
		/// GUI呼び出し
		/// </summary>
		virtual void ShowUI() = 0;
		/// <summary>
		/// カメラ設定の取得
		/// </summary>
		/// <returns></returns>
		virtual CameraProjProperty& GetCameraProperty() = 0;
		/// <summary>
		/// カメラに形状が写っているかの取得
		/// </summary>
		/// <param name="arg_AABB">形状</param>
		/// <returns></returns>
		virtual std::int32_t IsContaineVisibility(Value_ptr<Geometry::Box_AABB>arg_AABB) = 0;
		/// <summary>
		/// 終了処理
		/// </summary>
		virtual void End() = 0;
		/// <summary>
		/// 描画結果を出力するレンダーターゲットテクスチャの設定
		/// </summary>
		/// <param name="arg_tag">描画結果を出力するレンダーターゲットテクスチャ</param>
		virtual void SetProjectionTexture(const TextureTag& arg_tag) = 0;
		/// <summary>
		/// 参照、出力する深度テクスチャの設定
		/// </summary>
		/// <param name="arg_tag">参照、出力する深度テクスチャ</param>
		virtual void SetDepthStencilView(const TextureTag& arg_tag) = 0;
		/// <summary>
		/// 描画前処理
		/// </summary>
		virtual void BefDraw() = 0;
		/// <summary>
		/// VP行列の取得
		/// </summary>
		/// <returns>VP行列</returns>
		virtual Matrix4x4 GetViewProjectionMatrix() = 0;
		/// <summary>
		/// V行列の取得
		/// </summary>
		/// <returns>V行列</returns>
		virtual Matrix4x4 GetViewMatrix() = 0;
		/// <summary>
		/// P行列の取得
		/// </summary>
		/// <returns>P行列</returns>
		virtual Matrix4x4 GetProjectionMatrix() = 0;
		/// <summary>
		/// カメラ位置の取得
		/// </summary>
		/// <returns>カメラ位置</returns>
		virtual Vector3 GetPosition() = 0;
	protected:
	};
	class RenderingPathInfo;
	/// <summary>
	/// 描画パスのインターフェース
	/// </summary>
	class IRenderingPath:public IObject {
	public:
		void Initialize()override{}
		void PreInitialize()override{}
		/// <summary>
		/// フレームの開始時の処理
		/// </summary>
		virtual void BefExecute() = 0;
		/// <summary>
		/// 実行
		/// </summary>
		virtual void Execute()=0;
		/// <summary>
		/// フレームの終了時の処理
		/// </summary>
		virtual void End() = 0;
		/// <summary>
		/// 解放処理
		/// </summary>
		virtual void Release() = 0;
		/// <summary>
		/// GUI呼び出し
		/// </summary>
		virtual void OnShowGUI() = 0;
		/// <summary>
		/// ゲームシーン実行中に有効な描画パスなら起動、違ったら停止
		/// </summary>
		virtual void SetPlayActive() = 0;
		/// <summary>
		/// ゲームシーン編集中に有効な描画パスなら起動、違ったら停止
		/// </summary>
		virtual void SetEditActive() = 0;
		/// <summary>
		/// 名前の設定
		/// </summary>
		/// <param name="arg_name">名前</param>
		virtual void SetName(const std::string& arg_name) = 0;
		/// <summary>
		/// 名前の取得
		/// </summary>
		/// <returns>名前</returns>
		virtual const std::string& GetName()const=0;
		/// <summary>
		/// 描画パスの保存データの取得
		/// </summary>
		/// <returns>描画パスの保存データ</returns>
		virtual Value_ptr<RenderingPathInfo> GetRenderingPathInfo() = 0;
	};

	class SceneInformation;
	class SceneChangeInformation;
	class SceneRenderingInformation;
	/// <summary>
	/// シーンのインターフェース
	/// </summary>
	class IScene :public IObject
	{
	public:
		/// <summary>
		/// 更新処理
		/// </summary>
		virtual void Update() = 0;
		/// <summary>
		/// UI更新
		/// </summary>
		virtual void UIUpdate() = 0;
		/// <summary>
		/// 描画前処理
		/// </summary>
		virtual void BefDraw() = 0;
		/// <summary>
		/// ゲームオブジェクトの登録
		/// </summary>
		virtual void RegistGameObjects() = 0;
		/// <summary>
		/// 描画
		/// </summary>
		virtual void Draw() = 0;
		/// <summary>
		/// 編集モードでのカメラの操作
		/// </summary>
		virtual void EditCameraUpdate() = 0;
		/// <summary>
		/// 衝突判定クラスの起動
		/// </summary>
		/// <param name="arg_layerCount">衝突判定レイヤーの数</param>
		virtual void ActiveCollision(const std::uint32_t arg_layerCount) = 0;
		/// <summary>
		/// カメラの取得
		/// </summary>
		/// <param name="arg_camName">カメラ名</param>
		/// <returns> カメラの取得</returns>
		virtual Value_ptr<ICamera> GetCamera(const std::string& arg_camName) = 0;
		/// <summary>
		///  カメラの取得
		/// </summary>
		/// <param name="arg_camNum">描画パスのインデックス</param>
		/// <returns>カメラの取得</returns>
		virtual Value_ptr<ICamera> GetCamera(const std::uint32_t arg_camNum = 0) = 0;
		/// <summary>
		/// 描画パスの追加
		/// </summary>
		/// <param name="arg_path">描画パス</param>
		/// <returns>追加された描画パス</returns>
		virtual Value_ptr<IRenderingPath> AddRenderingPath(Value_ptr<IRenderingPath> arg_path) = 0;
		/// <summary>
		/// 衝突管理インスタンスの取得
		/// </summary>
		/// <returns>衝突管理インスタンス</returns>
		virtual Value_weak_ptr<ICollisionManager> GetCollisionManager() = 0;
		/// <summary>
		/// 描画パスの削除
		/// </summary>
		/// <param name="arg_pathName">使用するキー</param>
		virtual void RemoveRenderingPath(const std::string& arg_pathName) = 0;
		/// <summary>
		/// 描画パスの削除
		/// </summary>
		/// <param name="arg_pathIndex">描画パスのインデックス</param>
		virtual void RemoveRenderingPath(const std::uint32_t arg_pathIndex) = 0;
		/// <summary>
		/// 描画管理インスタンスの取得
		/// </summary>
		/// <returns>描画管理インスタンス</returns>
		virtual Value_ptr<IRenderer> GetRenderer() = 0;
		/// <summary>
		/// リソース管理インスタンスの取得
		/// </summary>
		/// <returns>リソース管理インスタンス</returns>
		virtual Value_ptr<IResourceContainer> GetResourceContainer() = 0;
		/// <summary>
		/// シーン管理インスタンスの取得
		/// </summary>
		/// <returns>シーン管理インスタンス</returns>
		virtual Value_weak_ptr< ISceneManager> GetSceneManager() = 0;
		/// <summary>
		/// ウィンドウ管理インスタンスの取得
		/// </summary>
		/// <returns>ウィンドウ管理インスタンス</returns>
		virtual std::unique_ptr<IWindow>& GetWindow() = 0;
		/// <summary>
		/// 解放処理
		/// </summary>
		virtual void Release() = 0;
		/// <summary>
		/// シーン情報の取得
		/// </summary>
		/// <returns>シーン情報</returns>
		virtual Value_ptr< SceneInformation> GetSceneInformation() = 0;
		/// <summary>
		/// シーン変更情報の取得
		/// </summary>
		/// <returns>シーン変更情報</returns>
		virtual Value_ptr< SceneChangeInformation> GetSceneChangeInformation() = 0;
		/// <summary>
		/// シーン描画情報の取得
		/// </summary>
		/// <returns>シーン描画情報</returns>
		virtual Value_ptr< SceneRenderingInformation> GetSceneRenderingInformation() = 0;
		/// <summary>
		/// シーン情報の取得
		/// </summary>
		virtual void Save() = 0;
		/// <summary>
		/// シーン実行中のカメラの起動
		/// </summary>
		virtual void CameraActivation() = 0;
		/// <summary>
		/// シーン編集中のカメラの起動
		/// </summary>
		virtual void CameraEditActivation() = 0;
		/// <summary>
		/// 開始
		/// </summary>
		virtual void Start() = 0;
		/// <summary>
		/// 終了
		/// </summary>
		virtual void End() = 0;
		/// <summary>
		/// GUI登録
		/// </summary>
		virtual void GUIRegist() = 0;
		/// <summary>
		/// GUIへの登録解除
		/// </summary>
		virtual void GUIRelease() = 0;
		/// <summary>
		/// GameObjectManagerの取得
		/// </summary>
		virtual Value_ptr<GameObjectManager> GetGameObjectManager() = 0;
		/// <summary>
		/// 物理シュミレーション管理クラスへのアクセス
		/// </summary>
		/// <returns></returns>
		virtual Value_ptr<ButiBullet::PhysicsManager> GetPhysicsManager()const = 0;
	};


	void OutputCereal(const ResourceLoadData& v, const std::string& arg_fileName);
	void InputCereal(ResourceLoadData& v, const std::string& arg_fileName);
}



#endif // !GamePartInterface