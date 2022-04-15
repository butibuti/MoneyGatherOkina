#pragma once
#ifndef DEFAULTGAMECOMPONENT_H_
#define DEFAULTGAMECOMPONENT_H_

#include "../../GameComponentHeader.h"
#include"Header/Common/CollisionPrimitive.h"
#include"ButiRendering_Dx12/MeshPrimitive.h"
#include"../../Resources/DrawData/IDrawData.h"
namespace ButiScript {
class CompiledData;
class VirtualMachine;
}
namespace ButiEngine {
enum class TextJustified {
	left,right,center
};
class Resource_RealTimeMesh;
class CameraMan :public  GameComponent {
public:
	void Start() override;
	void OnUpdate()override;
	std::string GetGameComponentName()const override {
		return "CameraMan";
	}
	void ShakeVartical(const float power);
	void ShakeHorizontal(const float power);
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(cameraName);
	}
	Value_ptr<GameComponent> Clone()override {
		return ObjectFactory::Create<CameraMan>();
	}
	void OnShowUI()override;
private:
	float vertQuake;
	static float vertQuakeMax;
	float horizonQuake;
	static float horizonQuakeMax;
	Value_ptr<Transform> child;
	std::string cameraName = "main";
};
namespace Collision {
enum class CollisionPrimType {
	cube, sphere, capsule
};
class ColliderComponent :public GameComponent
{
public:
	ColliderComponent(Value_ptr<CollisionPrimitive> arg_shp_collisionPrim, const std::uint32_t arg_layerNum = 0);
	ColliderComponent() {}
	void Initialize()override;
	void OnSet()override;
	void OnUpdate()override;
	void OnRemove()override;
	void Start()override;
	void CollisionStart();
	void CollisionStop();
	std::string GetGameComponentName()const override {
		return "ColliderComponent";
	}
	Value_ptr<GameComponent> Clone()override;
	Value_ptr<CollisionPrimitive> GetCollisionPrimitive();
	void SetCollisionPrimitive(Value_ptr<CollisionPrimitive> arg_shp_collisionPrim);
	void OnShowUI()override;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(shp_collisionPrim);
		archive(layerNum);
		archive(isActive);
	}
private:
	std::uint32_t layerNum = 0;
	Value_ptr<CollisionPrimitive> shp_collisionPrim;
};
}


class TransformAnimation :public GameComponent
{
public:
	std::string GetGameComponentName()const override {
		return "TransformAnimation";
	}
	void OnUpdate()override;
	Value_ptr<Transform> GetInitTransform();
	Value_ptr<Transform> GetTargetTransform();
	virtual void SetInitTransform(Value_ptr<Transform> arg_shp_InitTransform);
	void SetEaseType(const Easing::EasingType type);
	void SetSpeed(const float arg_speed);
	void SetTime(const float arg_time);
	void SetReverse(const bool isReverse);
	Value_ptr<GameComponent> Clone()override;

	virtual void _cdecl SetTargetTransform(Value_ptr<Transform> arg_shp_targetTransform);
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(speed);
		archive(t);
		archive(shp_targetTransform);
		archive(shp_initTransform);
		archive(easeType);
		archive(isReverse);
	}
	void OnShowUI()override;
	virtual void PositionSet();
	void OnSet()override;
protected:
	std::int32_t direction = 1;
	float t = 0;
	Value_ptr<Transform> shp_targetTransform;
	Quat initRotate;
	Quat targetRotate;
	Value_ptr<Transform> shp_initTransform;
	float speed = 0.002f;
	Easing::EasingType easeType = Easing::EasingType::EaseIn;
	bool isReverse = false;
};
class UIAnimation :public TransformAnimation
{
public:
	std::string GetGameComponentName()const override {
		return "UIAnimation";
	}
	void SetInitTransform(Value_ptr<Transform> arg_shp_InitTransform)override;
	void SetTargetTransform(Value_ptr<Transform> arg_shp_targetTransform)override;
	Value_ptr<GameComponent> Clone()override;
	void PositionSet()override;
	void OnShowUI()override;
	void OnSet()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(speed);
		archive(t);
		archive(shp_targetTransform);
		archive(shp_initTransform);
		archive(easeType);
		archive(isReverse);
	}
protected:
	Vector2 size;
};
class CubeTransformAnimation :public TransformAnimation
{
public:
	std::string GetGameComponentName()const override {
		return "CubeTransformAnimation";
	}
	void SetXEaseType(const Easing::EasingType type);
	void SetYEaseType(const Easing::EasingType type);
	void SetZEaseType(const Easing::EasingType type);
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(speed);
		archive(t);
		archive(shp_targetTransform);
		archive(shp_initTransform);
		archive(easeType);
		archive(xEaseType);
		archive(yEaseType);
		archive(zEaseType);
		archive(isReverse);
	}
	void OnShowUI()override;
	void PositionSet()override;
protected:
	Easing::EasingType xEaseType = Easing::EasingType::none;
	Easing::EasingType yEaseType = Easing::EasingType::none;
	Easing::EasingType zEaseType = Easing::EasingType::none;
};

class FPSViewBehavior :public GameComponent
{
public:
	void Start()override;
	void OnUpdate() override;
	void OnSet()override;
	std::string GetGameComponentName()const override {
		return "FPSViewBehavior";
	};
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
	}
private:
	bool isCenter = true;
};
class ChaseComponent :public GameComponent
{
public:
	ChaseComponent(Value_ptr<Transform> arg_shp_target, const float arg_speed = 0.1f);
	ChaseComponent() {}
	void OnUpdate() override;
	void OnSet()override;
	std::string GetGameComponentName()const override {
		return "ChaseComponent";
	};
	void OnShowUI()override;

	Value_ptr<GameComponent> Clone()override;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(shp_target);
		archive(speed);
		archive(isActive);
	}

private:
	Value_ptr<Transform> shp_target;
	float speed;
};
class IKComponent :public GameComponent
{
public:
	IKComponent() {}
	std::string GetGameComponentName()const override {
		return "IKComponent";
	}
	void OnRemove()override;
	void OnSet()override;
	void Start()override;
	Value_ptr<GameComponent> Clone()override;

	void OnShowUI()override;

	void OnUpdate()override;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(vec_ikData);
	}


protected:
	void RegistIK();
private:
	Value_ptr<ModelDrawData> shp_modelData;
	std::vector<Value_ptr<Bone>> vec_endBones;
	std::vector<Value_ptr< IKData>> vec_ikData;
};


struct Particle2D {
	Vector3 position;
	Vector4 color = Vector4(1, 1, 1, 1);
	float size = 1.0f;
	Vector3 velocity;
	Vector3 force;
	float accelation;
	float life = 60;
	Vector4 colorPase;
	float sizePase = 0;
};
struct Particle3D {
	Vector3 position = Vector3();
	Vector3 axis = Vector3(1, 0, 0);

	Vector4 color = Vector4(1, 1, 1, 1);
	float size = 5.0f;
	float angle = 0.0f;
	float anglePase = 0.0f;
	Vector3 velocity = Vector3();
	Vector3 force = Vector3();
	float accelation = 0;
	float life = 60;
	Vector4 colorPase = Vector4();
	float sizePase = 0;
};


class Resource_RealTimeMesh;


class ImmediateParticleController :public GameComponent
{
public:

	void OnUpdate()override;
	void OnSet()override;
	void Start();
	std::string GetGameComponentName()const override {
		return "ImmediateParticleController";
	}
	void AddParticle(const Particle3D& arg_particle);
	Value_ptr<GameComponent> Clone()override;
	void OnRemove()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(meshTag);
	}
private:
	std::vector<Particle3D> vec_particleInfo;
	MeshTag meshTag;
	Value_ptr<ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Color>> shp_backUp;
	Value_ptr<Resource_RealTimeMesh> shp_mesh;
	std::int32_t addParticleCount = 0;
};
class LookAtComponent :public GameComponent
{
public:
	LookAtComponent(Value_ptr<Transform> arg_shp_lookTarget);
	LookAtComponent() {}

	void OnUpdate()override;
	void OnSet()override;
	void Detach();
	std::string GetGameComponentName()const override {
		return "LookAtComponent";
	}
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(shp_lookTarget);
		archive(isActive);
	}
	void OnShowUI();
private:
	Value_ptr<Transform> shp_lookTarget;
};
class PostEffectParamUpdater :public GameComponent
{
public:
	PostEffectParamUpdater(const Vector4& pase);
	PostEffectParamUpdater();
	void OnUpdate()override;
	std::string GetGameComponentName()const override {
		return "PostEffectParamUpdater";
	}
	void OnSet();
	void Start();
	Value_ptr<GameComponent> Clone()override;
	void OnShowUI()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(pase);
		archive(isActive);
	}
private:
	Vector4 pase;
	Value_ptr<CBuffer<ObjectInformation>> shp_param;
};
class SplineCurveMover :public GameComponent
{
public:
	std::string GetGameComponentName()const override {
		return "SplineCurveMover";
	}
	void OnUpdate()override;
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(speed);
		archive(t);
		archive(splineCurve);
	}
	void OnShowUI()override;
	void PositionSet();
	void SetSplineCurve(const SplineCurve& arg_curve);
	void SetSpeed(const float spped);
private:

	SplineCurve splineCurve;
	float t = 0;
	float speed = 0.0025f;;
};
class SucideComponent :public GameComponent
{
public:
	SucideComponent(const float arg_count);
	SucideComponent();
	void OnUpdate()override;
	std::string GetGameComponentName()const override {
		return "SucideComponent";
	}
	Value_ptr<GameComponent> Clone()override;
	void OnShowUI()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(count);
		archive(shp_timer);
		archive(isActive);
	}
private:
	Value_ptr<RelativeTimer> shp_timer;
	float count;
};
class UIComponent :public GameComponent
{
public:
	std::string GetGameComponentName()const override {
		return "UIComponent";
	}
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(relativePos);
		archive(relativeScale);
	}
	void OnShowUI()override;
	void OnUpdate()override {}
	void OnSet()override;
	void Start()override;
	void SetPosision(const Vector2& arg_relativePos);
	void SetScalse(const Vector2& arg_relativeScale);
protected:
	void SetRelativeTransform();
	Vector2 relativePos;
	Vector2 relativeScale = Vector2(1, 1);
};
class ModelAnimation;

class ModelDrawData;
class MeshDrawComponent :public GameComponent
{
public:
	MeshDrawComponent(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const MaterialTag& arg_materialTag, Value_ptr< DrawInformation >arg_shp_drawInfo = nullptr, Value_ptr<Transform> arg_shp_transform = nullptr);
	MeshDrawComponent(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const std::vector< MaterialTag>& arg_materialTag, Value_ptr< DrawInformation >arg_shp_drawInfo = nullptr, Value_ptr<Transform> arg_shp_transform = nullptr);
	MeshDrawComponent(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, Value_ptr< DrawInformation >arg_shp_drawInfo = nullptr, Value_ptr<Transform> arg_shp_transform = nullptr);
	MeshDrawComponent(const MeshTag& arg_meshTag, const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, const std::vector< MaterialTag>& arg_materialTag, Value_ptr< DrawInformation >arg_shp_drawInfo = nullptr, Value_ptr<Transform> arg_shp_transform = nullptr);
	MeshDrawComponent() {}
	std::string GetGameComponentName()const override {
		return "MeshDraw";
	}
	void OnUpdate()override;
	void OnSet()override;
	void OnRemove() override;
	void SetBlendMode(const BlendMode& arg_blendMode);
	void SetMaterialTag(MaterialTag  arg_materialTag, const std::int32_t arg_index);
	void SetMeshTag(MeshTag  arg_meshTag);
	void SetModelTag(ModelTag  arg_modelTag);
	void SetShaderTag(ShaderTag  arg_shaderTag);
	const MeshTag& GetMeshTag();
	const ShaderTag& GetShaderTag();
	const ModelTag& GetModelTag();
	const std::vector<MaterialTag>& GetMaterialTag();
	Value_ptr<GameComponent> Clone()override;
	virtual void Regist();
	void ReRegist();
	virtual void UnRegist();
	Value_ptr< DrawInformation > GetDrawInformation();
	void OnShowUI()override;
	Value_ptr<Transform> GetTransform();

	template <class T>
	Value_ptr<CBuffer< T>> CreateCBuffer(const std::string& arg_cBufferName, const std::uint32_t arg_slot, Value_weak_ptr<GraphicDevice> arg_wkp_graphicDevice);
	template <class T>
	Value_ptr<CBuffer<T>> GetCBuffer(const std::string& arg_cBufferName) {
		return data->GetCBuffer<T>(arg_cBufferName);
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(meshTag);
		archive(shaderTag);
		archive(modelTag);
		archive(materialTag);
		archive(isActive);
		archive(shp_transform);
		archive(shp_drawInfo);
	}
protected:
	void ShowDrawSettingsUI(Value_ptr< DrawInformation >shp_arg_drawInfo = nullptr, const std::string& arg_settingsName = "");
	void ShowExCBufferUI(Value_ptr< DrawInformation >shp_arg_drawInfo = nullptr, const std::string& arg_settingsName = "");
	Value_ptr< MeshDrawData > data;
	virtual void CreateData();
	MeshTag meshTag;
	ShaderTag shaderTag;
	ModelTag modelTag;
	Value_ptr<Transform> shp_transform;
	Value_ptr< DrawInformation >shp_drawInfo = nullptr;
	std::vector<MaterialTag> materialTag;

	bool isCereal = true;
};
class MeshDrawComponent_Static :public MeshDrawComponent
{
public:
	MeshDrawComponent_Static(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const MaterialTag& arg_materialTag, Value_ptr< DrawInformation >arg_shp_drawInfo = nullptr, Value_ptr<Transform> arg_shp_transform = nullptr);
	MeshDrawComponent_Static(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const std::vector< MaterialTag>& arg_materialTag, Value_ptr< DrawInformation >arg_shp_drawInfo = nullptr, Value_ptr<Transform> arg_shp_transform = nullptr);
	MeshDrawComponent_Static(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, Value_ptr< DrawInformation >arg_shp_drawInfo = nullptr, Value_ptr<Transform> arg_shp_transform = nullptr);
	MeshDrawComponent_Static(const MeshTag& arg_meshTag, const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, const std::vector< MaterialTag>& arg_materialTag, Value_ptr< DrawInformation >arg_shp_drawInfo = nullptr, Value_ptr<Transform> arg_shp_transform = nullptr);
	MeshDrawComponent_Static() {}
	std::string GetGameComponentName()const override {
		return "MeshDraw_Static";
	}
	void OnUpdate()override;
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(meshTag);
		archive(shaderTag);
		archive(modelTag);
		archive(materialTag);
		archive(isActive);
		archive(shp_transform);
		archive(shp_drawInfo);
	}
protected:
};
class TextDrawComponent :public MeshDrawComponent
{
public:
	TextDrawComponent() {}
	std::string GetGameComponentName()const override {
		return "TextDrawComponent";
	}
	void OnSet()override;

	Value_ptr<GameComponent> Clone()override;
	void OnShowUI()override;
	const std::string& GetText()const {
		return text;
	}
	void OnRemove()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(fontTag);
		archive(text);
		archive(shaderTag);
		archive(modelTag);
		archive(materialTag);
		archive(isActive);
		archive(shp_transform);
		archive(shp_drawInfo);
		archive(map_size);
		archive(map_color);
		archive(textMeshSize);
		archive(just);
	}
	void SetText(const std::string& arg_text);
	void SetText(const std::string& arg_text,const Vector4& arg_color);

	void SetSize( const float arg_size);
	void SetSize(const std::int32_t  arg_index, const float arg_size);
	void SetSize(const std::string& arg_text, const float arg_size);
	float GetSize(const std::int32_t arg_index)const;

	void SetColor(const std::int32_t arg_index, const Vector4& arg_color);
	void SetColor(const Vector4& arg_color);
	void SetColor(const std::string& arg_text, const Vector4& arg_color);
	Vector4 GetColor(const std::int32_t arg_index)const;

	void CreateTextMesh();
protected:
	FontTag fontTag;
	std::map< std::int32_t, float > map_size;
	std::map< std::int32_t, Vector4> map_color;
	std::string text;
	std::int32_t textMeshSize = 256;
	TextJustified just=TextJustified::left;
	Value_ptr<Resource_RealTimeMesh> shp_mesh;
};




}


BUTI_REGIST_GAMECOMPONENT(MeshDrawComponent,true)
BUTI_REGIST_GAMECOMPONENT(MeshDrawComponent_Static,true)
BUTI_REGIST_GAMECOMPONENT(TextDrawComponent,false)

BUTI_REGIST_GAMECOMPONENT(UIComponent, true)

CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::MeshDrawComponent, ButiEngine::MeshDrawComponent_Static);

BUTI_REGIST_GAMECOMPONENT(Collision::ColliderComponent,true)
BUTI_REGIST_GAMECOMPONENT(SucideComponent,true)
BUTI_REGIST_GAMECOMPONENT(TransformAnimation,true)


BUTI_REGIST_GAMECOMPONENT(CameraMan,true)
BUTI_REGIST_GAMECOMPONENT(CubeTransformAnimation,true)

BUTI_REGIST_GAMECOMPONENT(FPSViewBehavior,true);

BUTI_REGIST_GAMECOMPONENT(IKComponent,true)

BUTI_REGIST_GAMECOMPONENT(ImmediateParticleController,true)
BUTI_REGIST_GAMECOMPONENT(LookAtComponent,true)
BUTI_REGIST_GAMECOMPONENT(PostEffectParamUpdater,true)
BUTI_REGIST_GAMECOMPONENT(SplineCurveMover,true)
BUTI_REGIST_GAMECOMPONENT(UIAnimation,true)
BUTI_REGIST_GAMECOMPONENT(ChaseComponent,true)


#endif // !DEFAULTGAMECOMPONENT_H_