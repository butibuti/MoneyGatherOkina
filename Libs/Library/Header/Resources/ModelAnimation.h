#pragma once
#include<map>
#include"Bone.h"
namespace ButiEngine {
	struct LarpData {
		LarpData(BYTE* arg_data);
		LarpData();

		CubicBezierCurve2D rotationBezier;
		CubicBezierCurve2D translationXBezier;
		CubicBezierCurve2D translationYBezier;
		CubicBezierCurve2D translationZBezier;
	};
	struct Pose {
		Pose(){}
		Pose(const Vector3& arg_position, const Vector3& arg_rotation,const Vector3& arg_scale);
		Vector3 position;
		Quat rotation;
		Vector3 scale;
	};

	struct IKActive {
		std::wstring boneName;
		bool isActive;
	};

	struct MotionKeyFrameData {
		MotionKeyFrameData(){}
		MotionKeyFrameData( const std::uint32_t arg_endFrame, const Vector3& arg_position, const Vector3& arg_rotation,const Vector3& arg_scale=Vector3(1,1,1));
		MotionKeyFrameData(const std::uint32_t arg_endFrame, const Pose& arg_pose);

		std::uint32_t endFrame=0;
		Pose pose;
		LarpData larp;
	};


	class IMotionTimeLine :public IObject {
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		virtual void Start() = 0;
		virtual void FrameSet(const float) = 0;
		virtual void Reset() = 0;
		virtual std::uint32_t GetEndFrame() = 0;
		virtual void PreStart() = 0;
		virtual void SetBone(Value_ptr<Bone> arg_vlp_bone) = 0;
		virtual void SetMotionData(const MotionKeyFrameData& arg_motionData) = 0;
		virtual void SetMotionData(const std::vector<MotionKeyFrameData>& arg_motionDatas)=0;
		virtual void LocalPoseSet(Value_ptr<Transform> arg_parentBone) = 0;
		virtual std::string GetContentsName() = 0;
	};
	class BoneMotionTimeLine :public IMotionTimeLine {
	public:
		BoneMotionTimeLine();
		~BoneMotionTimeLine();
		void SetBone(Value_ptr<Bone> arg_vlp_bone)override;
		void PreStart()override;
		void Reset()override;
		void SetMotionData(const MotionKeyFrameData& arg_motionData) override;
		void SetMotionData(const std::vector<MotionKeyFrameData>& arg_motionDatas) override;
		void Start() override;
		void FrameSet(const float frame)override;
		std::uint32_t GetEndFrame() override;
		void LocalPoseSet(Value_ptr<Transform> arg_parentBone)override;
		void SetBoneName(const std::string& arg_name);
		std::string GetContentsName()override;
	private:
		bool isActive=false;
		Quat initRotate;
		Vector3 initPosition;
		std::vector< MotionKeyFrameData > vec_data;
		std::vector< MotionKeyFrameData >::iterator currentMotionItr;
		std::vector< MotionKeyFrameData >::iterator befMotionItr;
		Value_ptr<Bone> bone;
		std::string boneName;
	};
	class ModelAnimation :public IObject
	{
	public:
		void Initialize() override {}
		void PreInitialize() override {}
		void Update(const float arg_frame);
		void IKTest();
		void Reset();
		void PreMotionStart(Value_ptr<Transform> arg_parentBoneTransform);
		Value_ptr<IMotionTimeLine> AddMotionTimeLine( Value_ptr<IMotionTimeLine> arg_motion);
		bool SetLoop(const bool arg_isLoop);
		void SetBoneDrawObj(Value_ptr<IBoneObject> arg_vlp_boneDrawObj);
		float GetFrame()const;
	private:
		bool isActive = true;
		bool isRoop = false;
		float frame = 0.0f;
		std::uint32_t endFrame = 0;
		

		std::vector<Value_ptr<IMotionTimeLine>> vec_timeLines;
		Value_ptr<IBoneObject> vlp_boneDrawObj;
	};

}