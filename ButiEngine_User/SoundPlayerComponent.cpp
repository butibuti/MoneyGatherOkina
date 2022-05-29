#include "stdafx_u.h"
#include "SoundPlayerComponent.h"

namespace ButiEngine {
void OutputCereal(const std::unordered_map<std::string, float>& arg_v, const std::string& arg_fileName)
{
    std::stringstream stream;


    cereal::PortableBinaryOutputArchive binOutArchive(stream);
    binOutArchive(arg_v);

    std::ofstream outputFile(GlobalSettings::GetResourceDirectory() + "Sound/" + arg_fileName, std::ios::binary);

    outputFile << stream.str();

    outputFile.close();
    stream.clear();
}

void InputCereal(std::unordered_map<std::string, float>& arg_v, const std::string& arg_fileName)
{
    if (!Util::ExistFile(GlobalSettings::GetResourceDirectory() + "Sound/" + arg_fileName)) {
        return;
    }
    std::stringstream stream;

    std::ifstream inputFile(GlobalSettings::GetResourceDirectory()+"Sound/" + arg_fileName, std::ios::binary);

    stream << inputFile.rdbuf();

    cereal::PortableBinaryInputArchive binInputArchive(stream);


    binInputArchive(arg_v);
}

}


void ButiEngine::SoundPlayerComponent::OnSet()
{
    InputCereal(m_umap_soundVolume, "SoundVolume.bin");
    auto soundTags= gameObject.lock()->GetResourceContainer()->GetSoundTags();
    for (auto soundTag : soundTags) {
        if (!m_umap_soundVolume.count(soundTag.GetID())) {
            m_umap_soundVolume.emplace( soundTag.GetID(),1.0f );
        }
    }
}

void ButiEngine::SoundPlayerComponent::OnRemove()
{
    StopLoopSound();
}

void ButiEngine::SoundPlayerComponent::OnShowUI()
{
    bool isEdited = false;
    static float masterVolume=1.0f;
    if (GUI::DragFloat(("MasterVolume"), masterVolume, 0.01f, 0.0f, 1.0f)) {
        gameObject.lock()->GetApplication().lock()->GetSoundManager()->SetMasterVolume(masterVolume);
    }

    for (auto& soundTagAndVolume : m_umap_soundVolume) {
        GUI::Text(soundTagAndVolume.first+":");
        GUI::SameLine();
        isEdited|=GUI::DragFloat("##"+ soundTagAndVolume.first, soundTagAndVolume.second,0.01f,0,10.0); 
        GUI::SameLine();
        if (GUI::ArrowButton("##play" + soundTagAndVolume.first, GUI::GuiDir_Right)) {
            gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(soundTagAndVolume.first, soundTagAndVolume.second);
        }GUI::SameLine();
        if (GUI::ArrowButton("##isolatePlay" + soundTagAndVolume.first, GUI::GuiDir_Right)) {
            gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(soundTagAndVolume.first, soundTagAndVolume.second,true);
        }
    }
    if (isEdited) {
        OutputCereal(m_umap_soundVolume, "SoundVolume.bin");
    }
}

void ButiEngine::SoundPlayerComponent::Start()
{
    m_vec_loopIndices.push_back("_");
}

void ButiEngine::SoundPlayerComponent::PlayBGM(SoundTag arg_sound, const float arg_volume)
{
    if (!m_umap_soundVolume.count(arg_sound.GetID())) {
        return;
    }
    gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlayBGM(arg_sound, m_umap_soundVolume.at(arg_sound.GetID()) * arg_volume);
}

void ButiEngine::SoundPlayerComponent::PlaySE(SoundTag arg_sound, const float arg_volume)
{
    if (!m_umap_soundVolume.count(arg_sound.GetID())) {
        return;
    }
    gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(arg_sound, m_umap_soundVolume.at(arg_sound.GetID()) * arg_volume,false);
}
void ButiEngine::SoundPlayerComponent::PlayIsolateSE(SoundTag arg_sound, const float arg_volume)
{
    if (!m_umap_soundVolume.count(arg_sound.GetID())) {
        return;
    }
    gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(arg_sound, m_umap_soundVolume.at(arg_sound.GetID()) * arg_volume,true);
}

void ButiEngine::SoundPlayerComponent::PlayControllableSE(SoundTag arg_sound, const std::int32_t arg_index,  const float arg_volume, const bool arg_isLoop)
{
    if (!m_umap_soundVolume.count(arg_sound.GetID())) {
        return;
    }
    gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlayControllableSE(arg_sound, arg_index,m_umap_soundVolume.at(arg_sound.GetID()) * arg_volume,arg_isLoop);
}

void ButiEngine::SoundPlayerComponent::SetControllableSEVolume(const std::uint32_t arg_index, const float arg_volume)
{
    gameObject.lock()->GetApplication().lock()->GetSoundManager()->SetControllableSEVolume(arg_index, arg_volume);
}


float ButiEngine::SoundPlayerComponent::GetVolume(SoundTag arg_sound) const
{
    if (!m_umap_soundVolume.count(arg_sound.GetID())) {
        return 0.0f;
    }
    return m_umap_soundVolume.count(arg_sound.GetID());
}

void ButiEngine::SoundPlayerComponent::SetVolume(SoundTag arg_sound, const float arg_volume)
{
    if (!m_umap_soundVolume.count(arg_sound.GetID())) {
        return;
    }
    m_umap_soundVolume.at(arg_sound.GetID()) = arg_volume;
}

void ButiEngine::SoundPlayerComponent::SetLoopIndex(const std::string& arg_indexName)
{
    auto end = m_vec_loopIndices.end();
    for (auto itr = m_vec_loopIndices.begin(); itr != end; ++itr)
    {
        if ((*itr) == "_" || (*itr) == arg_indexName)
        {
            (*itr) = arg_indexName;
            return;
        }
    }

    m_vec_loopIndices.push_back(arg_indexName);
}

std::int32_t ButiEngine::SoundPlayerComponent::GetLoopIndex(const std::string& arg_indexName)
{
    std::int32_t index = 0;
    auto end = m_vec_loopIndices.end();
    for (auto itr = m_vec_loopIndices.begin(); itr != end; ++itr)
    {
        if ((*itr) == arg_indexName)
        {
            return index;
        }
        index++;
    }

    return 0;
}

void ButiEngine::SoundPlayerComponent::DestroyLoopIndex(const std::string& arg_indexName)
{
    auto end = m_vec_loopIndices.end();
    for (auto itr = m_vec_loopIndices.begin(); itr != end; ++itr)
    {
        if ((*itr) == arg_indexName)
        {
            (*itr) = "_";
            return;
        }
    }
}

//再生中のループサウンドがある場合、全て破棄する
void ButiEngine::SoundPlayerComponent::StopLoopSound()
{
    std::int32_t index = 0;
    auto end = m_vec_loopIndices.end();
    for (auto itr = m_vec_loopIndices.begin(); itr != end; ++itr)
    {
        if ((*itr) != "_")
        {
            GetManager().lock()->GetApplication().lock()->GetSoundManager()->DestroyControllableSE(index);
        }
        index++;
    }

}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SoundPlayerComponent::Clone()
{
    auto output = ObjectFactory::Create<SoundPlayerComponent>();
    output->m_umap_soundVolume = m_umap_soundVolume;
    return output;
}
