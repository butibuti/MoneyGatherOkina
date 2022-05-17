#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class NumberManagerComponent : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "NumberManagerComponent";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void SetNumber(const std::int32_t arg_number) { m_number = arg_number; }
		void SetDigit(const std::int32_t arg_maxDigit) { m_maxDigit = arg_maxDigit; }
		void SetScale(const Vector3& arg_scale) { m_scale = arg_scale; }
		void SetColor(const Vector4& arg_color) { m_color = arg_color; }
		void SetRotate(const Vector4& arg_rotate) { m_rotate = arg_rotate; }

	private:
		void FixNumber();
		void UpdateNumbers();
		void UpdateParams();
		std::int32_t GetDigit(const std::int32_t arg_number);

		std::vector<Value_ptr<GameObject>> m_vec_vlp_numbers;

		Vector3 m_scale;
		Vector3 m_rotate;
		Vector4 m_color;

		float m_space;

		std::int32_t m_number;
		std::int32_t m_previousNumber;
		std::int32_t m_maxDigit;

	};
}

BUTI_REGIST_GAMECOMPONENT(NumberManagerComponent, true);

