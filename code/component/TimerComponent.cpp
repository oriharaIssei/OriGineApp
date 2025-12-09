#include "TimerComponent.h"

/// engine
#include "texture/TextureManager.h"
#define RESOURCE_DIRECTORY
#include "EngineInclude.h"

#ifdef DEBUG
#include "myFileSystem/MyFileSystem.h"
#include "myGui/MyGui.h"
#endif // DEBUG

TimerComponent::TimerComponent() {}

TimerComponent::~TimerComponent() {}

void TimerComponent::Initialize(Entity* /*_entity*/) {
    currentTime_ = maxTime_;
}

void TimerComponent::Edit(Scene* /*_scene*/, Entity* /*_entity*/, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef DEBUG
    CheckBoxCommand("IsStarted For Using System##" + _parentLabel, isStarted_);
    DragGuiCommand("Max Time (s)##" + _parentLabel, maxTime_, 0.1f, 100.f, 1.f);
#endif // DEBUG
}

void TimerComponent::Finalize() {
}

void to_json(nlohmann::json& j, const TimerComponent& c) {
    j = nlohmann::json{
        {"isStarted", c.isStarted_},
        {"maxTime", c.maxTime_}};
}

void from_json(const nlohmann::json& j, TimerComponent& c) {
    j.at("maxTime").get_to(c.maxTime_);
    j.at("isStarted").get_to(c.isStarted_);
}

void to_json(nlohmann::json& j, const TimerForSpriteComponent& c) {
    j = nlohmann::json{
        {"numbersTexturePath", c.numbersTexturePath},
        {"offset", c.offset},
        {"spriteSizeInteger", c.spriteSizeInteger},
        {"spriteSizeDecimal", c.spriteSizeDecimal},
        {"anchorOnWindow", c.anchorOnWindow},
        {"spriteMarginInteger", c.spriteMarginInteger},
        {"spriteMarginDecimal", c.spriteMarginDecimal},
        {"marginBetweenIntegerAndDecimal", c.marginBetweenIntegerAndDecimal},
        {"digitInteger", c.digitInteger},
        {"digitDecimal", c.digitDecimal}};
}

void from_json(const nlohmann::json& j, TimerForSpriteComponent& c) {
    j.at("numbersTexturePath").get_to(c.numbersTexturePath);
    j.at("offset").get_to(c.offset);
    j.at("spriteSizeInteger").get_to(c.spriteSizeInteger);
    j.at("spriteSizeDecimal").get_to(c.spriteSizeDecimal);
    j.at("spriteMarginInteger").get_to(c.spriteMarginInteger);
    j.at("spriteMarginDecimal").get_to(c.spriteMarginDecimal);
    j.at("marginBetweenIntegerAndDecimal").get_to(c.marginBetweenIntegerAndDecimal);

    j.at("anchorOnWindow").get_to(c.anchorOnWindow);
    j.at("digitIntegerForSprite").get_to(c.digitInteger);
    j.at("digitDecimalForSprite").get_to(c.digitDecimal);
}

void TimerForSpriteComponent::Initialize(Entity* /*entity*/) {
    if (!this->numbersTexturePath.empty()) {
        textureIndex = TextureManager::LoadTexture(this->numbersTexturePath, [this](uint32_t newIdx) {
            this->SettingOnLoadTexture(newIdx);
        });
    }
}

void TimerForSpriteComponent::Edit(Scene* /*scene*/, Entity* /*entity*/, [[maybe_unused]] const std::string& parentLabel) {

#ifdef DEBUG
    ImGui::Text("Sprite");

    DragGuiVectorCommand("Offset##" + parentLabel, offset, 0.1f);
    DragGuiVectorCommand("SpriteSizeInteger##" + parentLabel, spriteSizeInteger, 0.1f);
    DragGuiVectorCommand("SpriteSizeDecimal##" + parentLabel, spriteSizeDecimal, 0.1f);

    ImGui::Spacing();

    DragGuiVectorCommand("AnchorOnWindow##" + parentLabel, anchorOnWindow, 0.1f);

    ImGui::Spacing();

    DragGuiVectorCommand("SpriteMarginInteger##" + parentLabel, spriteMarginInteger, 0.1f);
    DragGuiVectorCommand("SpriteMarginDecimal##" + parentLabel, spriteMarginDecimal, 0.1f);
    DragGuiVectorCommand("MarginBetweenIntegerAndDecimal##" + parentLabel, marginBetweenIntegerAndDecimal, 0.1f);

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
        if (myfs::SelectFileDialog(textureDefaultDirectory, directory, fileName, {"png"})) {
            auto command = std::make_unique<SetterCommand<std::string>>(
                &this->numbersTexturePath,
                textureDefaultDirectory + "/" + directory + "/" + fileName,
                [this](std::string*) { textureIndex = TextureManager::LoadTexture(this->numbersTexturePath, [this](uint32_t newIdx) {
                                           this->SettingOnLoadTexture(newIdx);
                                       }); }, true);

            EditorController::GetInstance()->PushCommand(std::move(command));
        }
    }

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Digit");

    InputGuiCommand("Digit Integer" + parentLabel, digitInteger, "%d");
    InputGuiCommand("Digit Decimal" + parentLabel, digitDecimal, "%d");

#endif
}

void TimerForSpriteComponent::SettingOnLoadTexture(uint32_t texIdx) {
    const auto& metaData = TextureManager::GetTexMetadata(texIdx);

    if (metaData.width == 0 || metaData.height == 0) {
        LOG_ERROR("Texture metadata is invalid. Width or height is zero.");
        return;
    }

    numbersTextureSize = {static_cast<float>(metaData.width), static_cast<float>(metaData.height)};
    // *0.1 は 10桁であることを想定して
    numberTileSize = {numbersTextureSize[X] * 0.1f, numbersTextureSize[Y]};
}
