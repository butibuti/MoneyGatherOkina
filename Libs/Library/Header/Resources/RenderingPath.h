#ifndef define RENDERINGPATH_H
#define RENDERINGPATH_H


namespace ButiEngine {

	struct ForwardCameraRenderingPathInfo :public RenderingPathInfo {
		Value_ptr<Transform> vlp_cameraTrnasform;
		Value_ptr<CameraProjProperty> vlp_cameraProp;
		Value_ptr<IRenderingPath> CreateRenderingPath(Value_ptr<IRenderer> arg_vlp_renderer, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(vlp_cameraTrnasform);
			archive(vlp_cameraProp);
			archive(order);
		}
	};
	struct DeferredCameraRenderingPathInfo :public RenderingPathInfo {
		Value_ptr<Transform> vlp_cameraTrnasform;
		Value_ptr<CameraProjProperty> vlp_cameraProp;
		std::string forwardCameraPathName;
		Value_ptr<IRenderingPath> CreateRenderingPath(Value_ptr<IRenderer> arg_vlp_renderer, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(vlp_cameraTrnasform);
			archive(vlp_cameraProp);
			archive(forwardCameraPathName);
			archive(order);
		}
	};

	class CameraRenderingPath :public IRenderingPath {
	public:
		Value_ptr<ICamera> GetCamera()const;
		void SetName(const std::string& arg_name);
		const std::string& GetName()const;
		void SetPlayActive()override;
		void SetEditActive()override;
	protected:
		Value_ptr<ICamera> vlp_camera;
		Value_ptr<IScene> vlp_scene;
	};

	class ForwardCameraRenderingPath :public CameraRenderingPath {
	public:
		ForwardCameraRenderingPath(Value_ptr<ICamera> arg_vlp_camera, Value_ptr<IScene> arg_vlp_scene);
		Value_ptr<RenderingPathInfo> GetRenderingPathInfo()override;
		void BefExecute()override;
		void Execute() override;
		void End() override;
		void Release() override;
		void OnShowGUI() override;
	private:
		Value_ptr<IRenderer> vlp_renderer;
	};

	class DeferredCameraRenderingPath :public CameraRenderingPath {
	public:
		DeferredCameraRenderingPath(Value_ptr<ICamera> arg_vlp_camera, Value_ptr<IScene> arg_vlp_scene, std::string arg_forwardCamName);
		Value_ptr<RenderingPathInfo> GetRenderingPathInfo()override;
		void BefExecute()override;
		void Execute() override;
		void End() override;
		void Release() override;
		void OnShowGUI() override;
	private:
		Value_ptr<ICamera> vlp_forwardCamera;
		Value_ptr<IRenderer> vlp_renderer;
		std::string forwardCamName;
	};

}

CEREAL_REGISTER_TYPE(ButiEngine::RenderingPathInfo);
CEREAL_REGISTER_TYPE(ButiEngine::ForwardCameraRenderingPathInfo);
CEREAL_REGISTER_TYPE(ButiEngine::DeferredCameraRenderingPathInfo);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::RenderingPathInfo, ButiEngine::ForwardCameraRenderingPathInfo);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::RenderingPathInfo, ButiEngine::DeferredCameraRenderingPathInfo);
#endif // !define RENDERINGPATH_H

