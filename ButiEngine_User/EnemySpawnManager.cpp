#include "stdafx_u.h"
#include "EnemySpawnManager.h"
#include "InputManager.h"

void ButiEngine::EnemySpawnManager::OnUpdate()
{
}

void ButiEngine::EnemySpawnManager::OnSet()
{
    m_startMaxSpawnFrame = 0;
    m_endMaxSpawnFrame = 0;
    m_startMinSpawnFrame = 0;
    m_endMinSpawnFrame = 0;
    m_startWaitFrame = 0;
    m_lastIntervalReachFrame = 0;
}

void ButiEngine::EnemySpawnManager::Start()
{
}

void ButiEngine::EnemySpawnManager::OnShowUI()
{
    GUI::BulletText("EnemyType(0-3), StageNum(1-3)");
    GUI::Text("EnemyType(fly:0,stalker:1,kiba:2,volcano:3)");
    GUI::InputInt2("##StageData", m_volAndStageNum);

    GUI::BulletText("StartWaitFrame");
    GUI::DragFloat("##StartWaitFrame", m_startWaitFrame, 10, -10, 12000);

    GUI::BulletText("StartMaxSpawnFrame");
    GUI::DragFloat("##StartMaxSpawnFrame", m_startMaxSpawnFrame, 10, 0, 12000);

    GUI::BulletText("EndMaxSpawnFrame");
    GUI::DragFloat("##EndMaxSpawnFrame", m_endMaxSpawnFrame, 10, 0, 12000);

    GUI::BulletText("StartMinSpawnFrame");
    GUI::DragFloat("##StartMinSpawnFrame", m_startMinSpawnFrame, 10, 0, 12000);

    GUI::BulletText("EndMinSpawnFrame");
    GUI::DragFloat("##EndMinSpawnFrame", m_endMinSpawnFrame, 10, 0, 12000);

    GUI::BulletText("LastIntervalReachFrame");
    GUI::DragFloat("##LastIntervalReachFrame", m_lastIntervalReachFrame, 10, 0, 12000);


    if (GUI::Button("Output"))
    {
        EnemySpawnData outputDatas;
        outputDatas.m_startWaitFrame = m_startWaitFrame;
        outputDatas.m_startMaxSpawnFrame = m_startMaxSpawnFrame;
        outputDatas.m_endMaxSpawnFrame = m_endMaxSpawnFrame;
        outputDatas.m_startMinSpawnFrame = m_startMinSpawnFrame;
        outputDatas.m_endMinSpawnFrame = m_endMinSpawnFrame;
        outputDatas.m_lastIntervalReachFrame = m_lastIntervalReachFrame;

        std::string outputFileName = "EnemyData/" + std::to_string(m_volAndStageNum[0]) + "_" + std::to_string(m_volAndStageNum[1]) + ".enemyData";
        OutputCereal(outputDatas, outputFileName);
    }
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EnemySpawnManager::Clone()
{
	return ObjectFactory::Create<EnemySpawnManager>();
}

void ButiEngine::OutputCereal(const EnemySpawnData& arg_vec_data, const std::string& arg_fileName)
{
    std::stringstream stream;

    cereal::PortableBinaryOutputArchive binOutArchive(stream);
    binOutArchive(arg_vec_data);

    std::ofstream outputFile(GlobalSettings::GetResourceDirectory() + arg_fileName, std::ios::binary);

    outputFile << stream.str();

    outputFile.close();
    stream.clear();
}

void ButiEngine::InputCereal(EnemySpawnData& arg_ref_output_vec_data, const std::string& arg_fileName)
{
    std::stringstream stream;

    std::ifstream inputFile(GlobalSettings::GetResourceDirectory() + arg_fileName, std::ios::binary);

    stream << inputFile.rdbuf();

    cereal::PortableBinaryInputArchive binInputArchive(stream);

    binInputArchive(arg_ref_output_vec_data);
}
