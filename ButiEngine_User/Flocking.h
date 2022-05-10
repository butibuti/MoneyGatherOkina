#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;
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
		void CalculateAveragePos(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalculateMoveSpeed(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalculateGatherVec();
		void CalculateCohesionVec(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalculateALignmentVec(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalculateSeparationVec(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalculateAvoidPlayerVec(std::vector<Value_ptr<GameObject>> arg_vec_workers);
		void CalculateSurroundVec(std::vector<Value_ptr<GameObject>> arg_vec_workers);
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
		Value_ptr<LookAtComponent> m_vlp_lookAt;
		Value_ptr<Player> m_vlp_playerComponent;
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