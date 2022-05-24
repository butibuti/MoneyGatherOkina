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

void ButiEngine::SoundPlayerComponent::OnShowUI()
{
    bool isEdited = false;
    for (auto& soundTagAndVolume : m_umap_soundVolume) {
        GUI::Text(soundTagAndVolume.first+":");
        GUI::SameLine();
        isEdited|=GUI::DragFloat("##"+ soundTagAndVolume.first, soundTagAndVolume.second,0.01f,0,10.0); 
        GUI::SameLine();
        if (GUI::ArrowButton("##play" + soundTagAndVolume.first, GUI::GuiDir_Left)) {
            gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(soundTagAndVolume.first, soundTagAndVolume.second);
        }
    }
    if (isEdited) {
        OutputCereal(m_umap_soundVolume, "SoundVolume.bin");
    }
}

void ButiEngine::SoundPlayerComponent::Start()
{
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
    gameObject.lock()->GetApplication().lock()->GetSoundManager()->PlaySE(arg_sound,m_umap_soundVolume.at(arg_sound.GetID())*arg_volume);
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

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SoundPlayerComponent::Clone()
{
    auto output = ObjectFactory::Create<SoundPlayerComponent>();
    output->m_umap_soundVolume = m_umap_soundVolume;
    return output;
}
