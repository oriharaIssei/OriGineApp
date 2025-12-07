#include "SpeedFor3dUIComponent.h"

/// engine
#define RESOURCE_DIRECTORY
#include "EngineInclude.h"
#include "texture/TextureManager.h"

#ifdef DEBUG
/// editor
#include "myGui/MyGui.h"
/// uitil
#include "myFileSystem/MyFileSystem.h"
#endif // DEBUG

void to_json(nlohmann::json& j, const SpeedFor3dUIComponent& c) {
    j = nlohmann::json{
        {"numbersTexturePath", c.numbersTexturePath},
        {"worldPosition", c.worldPosition},
        {"offset", c.offset},
        {"planeScaleInteger", c.planeScaleInteger},
        {"planeScaleDecimal", c.planeScaleDecimal},
        {"planeMarginInteger", c.planeMarginInteger},
        {"planeMarginDecimal", c.planeMarginDecimal},
        {"marginBetweenIntegerAndDecimal", c.marginBetweenIntegerAndDecimal},
        {"digitInteger", c.digitInteger},
        {"digitDecimal", c.digitDecimal}};
}

void from_json(const nlohmann::json& j, SpeedFor3dUIComponent& c) {
    j.at("numbersTexturePath").get_to(c.numbersTexturePath);
    j.at("worldPosition").get_to(c.worldPosition);
    j.at("offset").get_to(c.offset);
    j.at("planeScaleInteger").get_to(c.planeScaleInteger);
    j.at("planeScaleDecimal").get_to(c.planeScaleDecimal);
    j.at("planeMarginInteger").get_to(c.planeMarginInteger);
    j.at("planeMarginDecimal").get_to(c.planeMarginDecimal);
    j.at("marginBetweenIntegerAndDecimal").get_to(c.marginBetweenIntegerAndDecimal);

    j.at("digitInteger").get_to(c.digitInteger);
    j.at("digitDecimal").get_to(c.digitDecimal);
}

void SpeedFor3dUIComponent::Initialize(Entity* /*entity*/) {
    if (!this->numbersTexturePath.empty()) {
        textureIndex = TextureManager::LoadTexture(this->numbersTexturePath, [this](uint32_t newIdx) {
            this->SettingOnLoadTexture(newIdx);
        });
    }
}

void SpeedFor3dUIComponent::Edit(Scene* /*scene*/, Entity* /*entity*/, [[maybe_unused]] const std::string& parentLabel) {

#ifdef DEBUG
    ImGui::Text("Sprite");
    DragGuiVectorCommand("WorldPosition##" + parentLabel, worldPosition, 0.01f);

    DragGuiVectorCommand("Offset##" + parentLabel, offset, 0.1f);
    DragGuiVectorCommand("Plane Scale Integer##" + parentLabel, planeScaleInteger, 0.1f);
    DragGuiVectorCommand("Plane Scale Decimal##" + parentLabel, planeScaleDecimal, 0.1f);

    ImGui::Spacing();

    DragGuiVectorCommand("Plane Margin Integer##" + parentLabel, planeMarginInteger, 0.1f);
    DragGuiVectorCommand("Plane Margin Decimal##" + parentLabel, planeMarginDecimal, 0.1f);
    DragGuiVectorCommand("Margin Between Integer And Decimal##" + parentLabel, marginBetweenIntegerAndDecimal, 0.1f);

    ImGui::Spacing();

    auto askLoadTexture = [&](const std::string& parentLabel) {
        bool askLoad = false;

        std::string label = "Load Texture##" + parentLabel;
        askLoad           = ImGui::Button(label.c_str());
        askLoad |= ImGui::ImageButton(
            reinterpret_cast<ImTextureID>(TextureManager::GetDescriptorGpuHandle(textureIndex).ptr),
            ImVec2(18, 18), ImVec2(0, 0), ImVec2(1, 1), 6);
        return askLoad;
    };
    if (askLoadTexture(parentLabel)) {
        std::string directory, fileName;
        std::string textureDefaultDirectory = std::string(kApplicationResourceDirectory) + "/Texture";
        if (myfs::selectFileDialog(textureDefaultDirectory, directory, fileName, {"png"})) {
            auto command = std::make_unique<SetterCommand<std::string>>(
                &this->numbersTexturePath,
                textureDefaultDirectory + "/" + directory + "/" + fileName,
                [this](std::string*) { textureIndex = TextureManager::LoadTexture(this->numbersTexturePath, [this](uint32_t newIdx) {
                                           this->SettingOnLoadTexture(newIdx);
                                       }); }, true);

            EditorController::GetInstance()->PushCommand(std::move(command));
        }
    }

    ImGui::Spacing();

    ImGui::SeparatorText("Digit");

    InputGuiCommand("Digit Integer##" + parentLabel, digitInteger, "%d");
    InputGuiCommand("Digit Decimal##" + parentLabel, digitDecimal, "%d");

#endif
}

void SpeedFor3dUIComponent::SettingOnLoadTexture(uint32_t texIdx) {
    const auto& metaData = TextureManager::GetTexMetadata(texIdx);

    if (metaData.width == 0 || metaData.height == 0) {
        LOG_ERROR("Texture metadata is invalid. Width or height is zero.");
        return;
    }

    numbersTextureSize = {static_cast<float>(metaData.width), static_cast<float>(metaData.height)};
    // *0.1 は 10桁であることを想定して
    numberTileSize = {numbersTextureSize[X] * 0.1f, numbersTextureSize[Y]};
}
