
#include "../../GameComponentHeader.h"
namespace ButiEngine {

	class RotationAnimation :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "RotationAnimation";
		}
		void OnUpdate()override;
		Vector3 GetInitRotate()const;
		Vector3 GetTargetRotate()const;
		Quat GetInitRotate_Quat()const;
		Quat GetTargetRotate_Quat()const;
		Matrix4x4 GetInitRotate_Matrix()const;
		Matrix4x4 GetTargetRotate_Matrix()const;
		virtual void SetInitRotate(const Vector3& arg_rotate);
		virtual void _cdecl SetTargetRotate(const Vector3& arg_rotate);
		virtual void SetInitRotate(const Matrix4x4& arg_rotate);
		virtual void _cdecl SetTargetRotate(const Matrix4x4& arg_rotate);
		virtual void SetInitRotate(const Quat& arg_rotate);
		virtual void _cdecl SetTargetRotate(const Quat& arg_rotate);
		void SetEaseType(const Easing::EasingType type);
		void SetSpeed(const float arg_speed);
		void SetTime(const float arg_time);
		void SetReverse(const bool arg_isReverse);
		bool GetIsReverse()const;
		Value_ptr<GameComponent> Clone()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(speed);
			archive(t);
			archive(targetRotate);
			archive(initRotate);
			archive(easeType);
			archive(isReverse);
		}
		void OnShowUI()override;
		virtual void RotateSet();
		void OnSet()override;
	protected:
		std::int32_t direction = 1;
		float t = 0;
		Quat targetRotate;
		Quat initRotate;
		float speed = 0.002f;;
		Easing::EasingType easeType = Easing::EasingType::EaseIn;
		bool isReverse = false;
	};

}
BUTI_REGIST_GAMECOMPONENT(RotationAnimation,true)