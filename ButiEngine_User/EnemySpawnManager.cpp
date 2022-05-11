#include "stdafx_u.h"
#include "EnemySpawnManager.h"
#include "InputManager.h"

void ButiEngine::EnemySpawnManager::OnUpdate()
{
}

void ButiEngine::EnemySpawnManager::OnSet()
{
}

void ButiEngine::EnemySpawnManager::Start()
{
}

void ButiEngine::EnemySpawnManager::OnShowUI()
{
    if (GUI::Button("Add_Enemy_Fly"))
    {
        auto fly = GetManager().lock()->AddObjectFromCereal("Enemy_Fly");
        auto drawFly = GetManager().lock()->AddObjectFromCereal("DrawObject_Fly");
        drawFly.lock()->transform->SetBaseTransform(fly.lock()->transform, true);
    }
    if (GUI::Button("Add_Enemy_Kiba"))
    {
        auto kiba = GetManager().lock()->AddObjectFromCereal("Enemy_Kiba");
        auto drawKiba = GetManager().lock()->AddObjectFromCereal("DrawObject_Kiba");
        drawKiba.lock()->transform->SetBaseTransform(kiba.lock()->transform, true);
    }
    if (GUI::Button("Add_Enemy_Stalker"))
    {
        auto stalker = GetManager().lock()->AddObjectFromCereal("Enemy_Stalker");
        auto drawStalker = GetManager().lock()->AddObjectFromCereal("DrawObject_Stalker");
        drawStalker.lock()->transform->SetBaseTransform(stalker.lock()->transform, true);
    }
    if (GUI::Button("Add_Enemy_Volcano"))
    {
        auto volcano = GetManager().lock()->AddObjectFromCereal("Enemy_Volcano");
        auto drawVolcano = GetManager().lock()->AddObjectFromCereal("DrawObject_Volcano");
        drawVolcano.lock()->transform->SetBaseTransform(volcano.lock()->transform, true);
    }
    if (GUI::Button("Add_Enemy_Tutorial"))
    {
        auto tutorial = GetManager().lock()->AddObjectFromCereal("Enemy_Tutorial");
        auto drawTutorial = GetManager().lock()->AddObjectFromCereal("DrawObject_Tutorial");
        drawTutorial.lock()->transform->SetBaseTransform(tutorial.lock()->transform, true);
    }
    GUI::BulletText("StageNum : WaveNum");
    GUI::InputInt2("##StageData", m_stageAndWaveNum);

    if (GUI::Button("Output"))
    {
        auto vec_enemyGameObject = GetManager().lock()->GetGameObjects(GameObjectTag("Enemy"));

        std::vector<EnemySpawnData> vec_outputDatas;

        for (auto vlp_gameObject : vec_enemyGameObject)
        {
            //‚±‚±‚Å–¼‘O®—
            std::string gameObjectFullName = vlp_gameObject->GetGameObjectName();
            std::string gameObjectName;
            auto secondUnderScoreIndex = gameObjectFullName.rfind("_");
            if (gameObjectFullName.find("_") != secondUnderScoreIndex)
            {
                gameObjectName = gameObjectFullName.substr(0, secondUnderScoreIndex);
            }
            else
            {
                gameObjectName = gameObjectFullName;
            }

            vec_outputDatas.push_back({ gameObjectName, vlp_gameObject->transform->Clone() });
        }
        std::string outputFileName = "EnemyData/" + std::to_string(m_stageAndWaveNum[0]) + "_" + std::to_string(m_stageAndWaveNum[1]) + ".enemyData";
        OutputCereal(vec_outputDatas, outputFileName);
    }

}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EnemySpawnManager::Clone()
{
	return ObjectFactory::Create<EnemySpawnManager>();
}

void ButiEngine::OutputCereal(const std::vector<EnemySpawnData>& arg_vec_data, const std::string& arg_fileName)
{
    std::stringstream stream;

    cereal::PortableBinaryOutputArchive binOutArchive(stream);
    binOutArchive(arg_vec_data);

    std::ofstream outputFile(GlobalSettings::GetResourceDirectory() + arg_fileName, std::ios::binary);

    outputFile << stream.str();

    outputFile.close();
    stream.clear();
}

void ButiEngine::InputCereal(std::vector<EnemySpawnData>& arg_ref_output_vec_data, const std::string& arg_fileName)
{
    std::stringstream stream;

    std::ifstream inputFile(GlobalSettings::GetResourceDirectory() + arg_fileName, std::ios::binary);

    stream << inputFile.rdbuf();

    cereal::PortableBinaryInputArchive binInputArchive(stream);

    binInputArchive(arg_ref_output_vec_data);
}
