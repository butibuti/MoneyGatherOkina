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
		std::int32_t m_volAndStageNum[2];
		float m_startMaxSpawnFrame;
		float m_endMaxSpawnFrame;
		float m_startMinSpawnFrame;
		float m_endMinSpawnFrame;
		float m_startWaitFrame;
		float m_lastIntervalReachFrame;
	};

	struct EnemySpawnData {
		float m_startMaxSpawnFrame;
		float m_endMaxSpawnFrame;
		float m_startMinSpawnFrame;
		float m_endMinSpawnFrame;
		float m_startWaitFrame;
		float m_lastIntervalReachFrame;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(m_startMaxSpawnFrame);
			archive(m_endMaxSpawnFrame);
			archive(m_startMinSpawnFrame);
			archive(m_endMinSpawnFrame);
			archive(m_startWaitFrame);
			archive(m_lastIntervalReachFrame);
		}
	};

	void OutputCereal(const EnemySpawnData& arg_vec_data, const std::string& arg_fileName);
	void InputCereal(EnemySpawnData& arg_ref_output_vec_data, const std::string& arg_fileName);

}

BUTI_REGIST_GAMECOMPONENT(EnemySpawnManager, true);

