#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Flocking :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Flocking";
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
	private:
		void CalcMoveSpeed(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalcGatherDir();
		void CalcCohesionVec(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalcALignmentVec(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalcSeparationVec(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void Move();

		static float m_rotationSpeed;
		static float m_gatherWeight;
		static float m_cohesionWeight;
		static float m_alignmentWeight;
		static float m_separationWeight;

		Value_weak_ptr<GameObject> m_vwp_player;
		float m_viewRadius;
		float m_nearBorder;
		float m_moveSpeed;
		Vector3 m_gatherDir;
		Vector3 m_cohesionVec;
		Vector3 m_alignmentVec;
		Vector3 m_separationVec;
	};

}

BUTI_REGIST_GAMECOMPONENT(Flocking, true);