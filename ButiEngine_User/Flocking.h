#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;
	class FlockingLeader;
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
		void SetViewRadius();
		void CalculateAveragePos();
		void CalculateMoveSpeed();
		void CalculateGatherVec();
		void CalculateCohesionVec();
		void CalculateALignmentVec();
		void CalculateSeparationVec();
		void CalculateAvoidPlayerVec();
		void CalculateSurroundVec();
		void Move();

		bool IsNearLeader(const Vector3& arg_pos);

		static std::vector<Value_ptr<GameObject>> m_vec_workers;

		static float m_gatherWeight;
		static float m_cohesionWeight;
		static float m_alignmentWeight;
		static float m_separationWeight;
		static float m_avoidPlayerWeight;
		static float m_surroundWeight;
		static float m_viewRadius;
		static float m_minViewRadius;
		static float m_maxViewRadius;
		static float m_nearBorder;
		static float m_playerNearBorder;
		static float m_leaderNearBorder;

		Value_weak_ptr<GameObject> m_vwp_player;
		Value_weak_ptr<GameObject> m_vwp_leader;
		Value_ptr<LookAtComponent> m_vlp_lookAt;
		Value_ptr<Player> m_vlp_playerComponent;
		Value_ptr<FlockingLeader> m_vlp_flockingLeader;
		float m_maxRotationSpeed;
		float m_rotationSpeed;
		float m_maxMoveSpeed;
		float m_moveSpeed;

		Vector3 m_velocity;

		Vector3 m_averagePos;

		Vector3 m_gatherVec;
		Vector3 m_cohesionVec;
		Vector3 m_alignmentVec;
		Vector3 m_separationVec;
		Vector3 m_avoidPlayerVec;
		Vector3 m_surroundVec;
	};

}

BUTI_REGIST_GAMECOMPONENT(Flocking, true);