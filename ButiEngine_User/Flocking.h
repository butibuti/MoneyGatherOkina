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
		void OnRemove()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		void CalcAveragePos(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalcMoveSpeed(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalcGatherVec();
		void CalcCohesionVec(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalcALignmentVec(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalcSeparationVec(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalcAvoidPlayerVec(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalcSurroundVec(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void Move();

		static float m_gatherWeight;
		static float m_cohesionWeight;
		static float m_alignmentWeight;
		static float m_separationWeight;
		static float m_avoidPlayerWeight;
		static float m_surroundWeight;
		static float m_viewRadius;
		static float m_nearBorder;

		Value_weak_ptr<GameObject> m_vwp_player;
		float m_rotationSpeed;
		float m_moveSpeed;

		Vector3 m_averagePos;

		Vector3 m_gatherDir;
		Vector3 m_cohesionVec;
		Vector3 m_alignmentVec;
		Vector3 m_separationVec;
		Vector3 m_avoidPlayerVec;
		Vector3 m_surroundVec;
	};

}

BUTI_REGIST_GAMECOMPONENT(Flocking, true);