#pragma once

namespace ButiEngine {

	struct SceneInformation :public IObject
	{
	public:
		SceneInformation(const std::string& arg_sceneName) {
			sceneName = arg_sceneName;
		}
		~SceneInformation();
		std::string GetSceneName() {
			return sceneName;
		}
		void Initialize()override {}
		void PreInitialize()override {}
	private:
		std::string sceneName;
	};
	struct SceneChangeInformation :public IObject
	{
	public:
		SceneChangeInformation() {
		}
		void Initialize()override {}
		void PreInitialize()override {}
	private:
	};
	struct SceneRenderingInformation 
	{
	public:
		SceneRenderingInformation() {
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(vec_renderingPathInfo);
			archive(vec_shadowCameraIndex);
			archive(vec_shadowTextures);
			archive(vec_staticShadowTextures);
			archive(layerCount);
		}
		std::vector<Value_ptr<ButiRendering::RenderingPathInfo>> vec_renderingPathInfo;
		std::vector<std::vector<TextureTag>> vec_shadowTextures;
		std::vector<std::vector<TextureTag>> vec_staticShadowTextures;
		std::vector<std::int32_t> vec_shadowCameraIndex;
		std::int32_t layerCount = 1;
	private:
	};

	void OutputCereal(const Value_ptr<SceneRenderingInformation>& v, std::string arg_path);

	void InputCereal(Value_ptr<SceneRenderingInformation>& v,std::string arg_path);


}

