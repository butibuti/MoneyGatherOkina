#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class EnemySpawnManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "EnemySpawnManager";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

	private:
		std::int32_t m_stageAndWaveNum[2];
	};

	struct EnemySpawnData {
		std::string m_enemyName;
		Value_ptr<Transform> m_vlp_enemyTransform;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(m_enemyName);
			archive(m_vlp_enemyTransform);
		}
	};

	void OutputCereal(const std::vector<EnemySpawnData>& arg_vec_data, const std::string& arg_fileName);
	void InputCereal(std::vector<EnemySpawnData>& arg_ref_output_vec_data, const std::string& arg_fileName);

}

BUTI_REGIST_GAMECOMPONENT(EnemySpawnManager, true);

