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
        {"numbersTexturePath", c.numbersTexturePath_},
        {"offset", c.offset_},
        {"spriteSizeInteger", c.spriteSizeInteger_},
        {"spriteSizeDecimal", c.spriteSizeDecimal_},
        {"anchorOnWindow", c.anchorOnWindow_},
        {"spriteMarginInteger", c.spriteMarginInteger_},
        {"spriteMarginDecimal", c.spriteMarginDecimal_},
        {"marginBetweenIntegerAndDecimal", c.marginBetweenIntegerAndDecimal_},
        {"digitForSprite", c.digitForSprite_},
        {"digitIntegerForSprite", c.digitIntegerForSprite_},
        {"digitDecimalForSprite", c.digitDecimalForSprite_}};
}

void from_json(const nlohmann::json& j, TimerForSpriteComponent& c) {
    j.at("numbersTexturePath").get_to(c.numbersTexturePath_);
    j.at("offset").get_to(c.offset_);
    j.at("spriteSizeInteger").get_to(c.spriteSizeInteger_);
    j.at("spriteSizeDecimal").get_to(c.spriteSizeDecimal_);
    j.at("spriteMarginInteger").get_to(c.spriteMarginInteger_);
    j.at("spriteMarginDecimal").get_to(c.spriteMarginDecimal_);
    j.at("marginBetweenIntegerAndDecimal").get_to(c.marginBetweenIntegerAndDecimal_);

    j.at("anchorOnWindow").get_to(c.anchorOnWindow_);
    j.at("digitForSprite").get_to(c.digitForSprite_);
    j.at("digitIntegerForSprite").get_to(c.digitIntegerForSprite_);
    j.at("digitDecimalForSprite").get_to(c.digitDecimalForSprite_);
}

void TimerForSpriteComponent::Initialize(Entity* /*_entity*/) {
    if (!this->numbersTexturePath_.empty()) {
        textureIndex_ = TextureManager::LoadTexture(this->numbersTexturePath_, [this](uint32_t _newIdx) {
            this->SettingOnLoadTexture(_newIdx);
        });
    }
}

void TimerForSpriteComponent::Edit(Scene* /*_scene*/, Entity* /*_entity*/, [[maybe_unused]] const std::string& _parentLabel) {

#ifdef DEBUG
    ImGui::Text("Sprite");

    DragGuiVectorCommand("Offset##" + _parentLabel, offset_, 0.1f);
    DragGuiVectorCommand("SpriteSizeInteger##" + _parentLabel, spriteSizeInteger_, 0.1f);
    DragGuiVectorCommand("SpriteSizeDecimal##" + _parentLabel, spriteSizeDecimal_, 0.1f);

    ImGui::Spacing();

    DragGuiVectorCommand("AnchorOnWindow##" + _parentLabel, anchorOnWindow_, 0.1f);

    ImGui::Spacing();

    DragGuiVectorCommand("SpriteMarginInteger##" + _parentLabel, spriteMarginInteger_, 0.1f);
    DragGuiVectorCommand("SpriteMarginDecimal##" + _parentLabel, spriteMarginDecimal_, 0.1f);
    DragGuiVectorCommand("MarginBetweenIntegerAndDecimal##" + _parentLabel, marginBetweenIntegerAndDecimal_, 0.1f);

    ImGui::Spacing();

    auto askLoadTexture = [&](const std::string& _parentLabel) {
        bool askLoad = false;

        std::string label = "Load Texture##" + _parentLabel;
        askLoad           = ImGui::Button(label.c_str());
        askLoad |= ImGui::ImageButton(
            reinterpret_cast<ImTextureID>(TextureManager::GetDescriptorGpuHandle(textureIndex_).ptr),
            ImVec2(18, 18), ImVec2(0, 0), ImVec2(1, 1), 6);
        return askLoad;
    };
    if (askLoadTexture(_parentLabel)) {
        std::string directory, fileName;
        std::string textureDefaultDirectory = std::string(kApplicationResourceDirectory) + "/Texture";
        if (myfs::selectFileDialog(textureDefaultDirectory, directory, fileName, {"png"})) {
            auto command = std::make_unique<SetterCommand<std::string>>(
                &this->numbersTexturePath_,
                textureDefaultDirectory + "/" + directory + "/" + fileName,
                [this](std::string*) { textureIndex_ = TextureManager::LoadTexture(this->numbersTexturePath_, [this](uint32_t _newIdx) {
                                           this->SettingOnLoadTexture(_newIdx);
                                       }); }, true);

            EditorController::GetInstance()->PushCommand(std::move(command));
        }
    }

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Digit");

    InputGuiCommand("Digit For Sprite##" + _parentLabel, digitForSprite_, "%d");
    SlideGuiCommand("Digit Integer For Sprite##" + _parentLabel, digitIntegerForSprite_, 0, digitForSprite_, "%d");
    SlideGuiCommand("Digit Decimal For Sprite##" + _parentLabel, digitDecimalForSprite_, 0, digitForSprite_ - digitIntegerForSprite_, "%d");

#endif
}

void TimerForSpriteComponent::SettingOnLoadTexture(uint32_t _texIdx) {
    const auto& metaData = TextureManager::GetTexMetadata(_texIdx);

    if (metaData.width == 0 || metaData.height == 0) {
        LOG_ERROR("Texture metadata is invalid. Width or height is zero.");
        return;
    }

    numbersTextureSize_ = {static_cast<float>(metaData.width), static_cast<float>(metaData.height)};
    // *0.1 は 10桁であることを想定して
    numberTileSize_ = {numbersTextureSize_[X] * 0.1f, numbersTextureSize_[Y]};
}
