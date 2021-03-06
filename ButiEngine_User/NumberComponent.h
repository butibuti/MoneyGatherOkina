#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class NumberComponent : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "NumberComponent";
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
		void SetScale(const Vector3& arg_scale) { m_scale = arg_scale; }
		void SetColor(const Vector4& arg_color) { m_color = arg_color; }
		void SetRotate(const Vector3& arg_rotate) { m_rotate = arg_rotate; }

		void UpdateColor();

	private:
		void FixNumber();

		std::vector<Value_ptr<MeshDrawComponent>> m_vec_vlp_meshDrawComponents;

		Vector3 m_scale;
		Vector3 m_rotate;
		Vector4 m_color;

		std::int32_t m_number;
		std::int32_t m_previousNumber;

	};
}

BUTI_REGIST_GAMECOMPONENT(NumberComponent, true);

