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
    m_spawnerIndex = 0;
    m_maxSpawnCount = 0;

    m_isHeatUp = false;
    m_maxHeatUpSpawnCount = 0;
    m_maxSpawnRate = 0;
    m_minSpawnRate = 0;
    m_changeInterval = 0;
}

void ButiEngine::EnemySpawnManager::Start()
{
}

void ButiEngine::EnemySpawnManager::OnShowUI()
{
    GUI::Text("------CommonDatas------");
    GUI::BulletText("EnemyType(0-3), StageNum(0-1)");
    GUI::Text("EnemyType(fly:0,stalker:1,kiba:2,volcano:3)");
    GUI::InputInt2("##StageData", m_volAndStageNum);

    GUI::BulletText("SpawnerIndex(0-3)");
    GUI::InputInt("##SpawnerIndex", m_spawnerIndex);

    GUI::Text("------EnemySpawnDatas------");
    GUI::BulletText("EnemyType(fly:0,stalker:1,kiba:2,volcano:3)");
    GUI::Text("EnemyType:%d, StageNum:%d", m_volAndStageNum[0], m_volAndStageNum[1]);
    GUI::Text("SpawnerIndex:%d", m_spawnerIndex);

    GUI::BulletText("MaxSpawnCount");
    GUI::InputInt("##MaxSpawnCount", m_maxSpawnCount);

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

    if (GUI::Button("Output_EnemyData"))
    {
        EnemySpawnData outputDatas;
        outputDatas.m_startWaitFrame = m_startWaitFrame;
        outputDatas.m_startMaxSpawnFrame = m_startMaxSpawnFrame;
        outputDatas.m_endMaxSpawnFrame = m_endMaxSpawnFrame;
        outputDatas.m_startMinSpawnFrame = m_startMinSpawnFrame;
        outputDatas.m_endMinSpawnFrame = m_endMinSpawnFrame;
        outputDatas.m_lastIntervalReachFrame = m_lastIntervalReachFrame;
        outputDatas.m_spawnerIndex = m_spawnerIndex;
        outputDatas.m_maxSpawnCount = m_maxSpawnCount;

        std::string outputFileName = "EnemyData/" + std::to_string(m_volAndStageNum[0]) + "_" + std::to_string(m_volAndStageNum[1]) + "_" + std::to_string(m_spawnerIndex) + ".enemyData";
        OutputCereal(outputDatas, outputFileName);
    }


    GUI::Text("------HeatUpDatas------");
    GUI::BulletText("EnemyType(fly:0,stalker:1,kiba:2,volcano:3)");
    GUI::Text("EnemyType:%d, StageNum:%d", m_volAndStageNum[0], m_volAndStageNum[1]);
    GUI::Text("SpawnerIndex:%d", m_spawnerIndex);

    GUI::BulletText("IsHeatUp");
    GUI::Checkbox("##IsHeatUp", m_isHeatUp);

    GUI::BulletText("MaxHeatUpSpawnCount");
    GUI::InputInt("##MaxHeatUpSpawnCount", m_maxHeatUpSpawnCount);

    GUI::BulletText("MaxSpawnRate");
    GUI::DragFloat("##MaxSpawnRate", m_maxSpawnRate, 0.02f, 0, 100);

    GUI::BulletText("MinSpawnRate");
    GUI::DragFloat("##MinSpawnRate", m_minSpawnRate, 0.02f, 0, 100);

    GUI::BulletText("ChangeInterval");
    GUI::DragFloat("##ChangeInterval", m_changeInterval, 10, 0, 6000);

    if (GUI::Button("Output_HeatUpData"))
    {
        HeatUpData outputDatas;
        outputDatas.m_isHeatUp = m_isHeatUp;
        outputDatas.m_maxHeatUpSpawnCount = m_maxHeatUpSpawnCount;
        outputDatas.m_maxSpawnRate = m_maxSpawnRate;
        outputDatas.m_minSpawnRate = m_minSpawnRate;
        outputDatas.m_changeInterval = m_changeInterval;

        std::string outputFileName = "EnemyData/" + std::to_string(m_volAndStageNum[0]) + "_" + std::to_string(m_volAndStageNum[1]) + "_" + std::to_string(m_spawnerIndex) + ".heatData";
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
void ButiEngine::OutputCereal(const HeatUpData& arg_vec_data, const std::string& arg_fileName)
{
    std::stringstream stream;

    cereal::PortableBinaryOutputArchive binOutArchive(stream);
    binOutArchive(arg_vec_data);

    std::ofstream outputFile(GlobalSettings::GetResourceDirectory() + arg_fileName, std::ios::binary);

    outputFile << stream.str();

    outputFile.close();
    stream.clear();
}
void ButiEngine::InputCereal(HeatUpData& arg_ref_output_vec_data, const std::string& arg_fileName)
{
    std::stringstream stream;

    std::ifstream inputFile(GlobalSettings::GetResourceDirectory() + arg_fileName, std::ios::binary);

    stream << inputFile.rdbuf();

    cereal::PortableBinaryInputArchive binInputArchive(stream);

    binInputArchive(arg_ref_output_vec_data);
}
