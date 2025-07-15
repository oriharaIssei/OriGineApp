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

void TimerComponent::Initialize(GameEntity* /*_entity*/) {
    currentTime_ = maxTime_;
}

bool TimerComponent::Edit() {
    bool isChanged = false;

#ifdef DEBUG
    isChanged = CheckBoxCommand("IsStarted For Using System", isStarted_);
    isChanged |= DragGuiCommand("Max Time (s)", maxTime_, 0.1f, 100.f, 1.f);

#endif // DEBUG

    return isChanged;
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

void TimerForSpriteComponent::Initialize(GameEntity* /*_entity*/) {
    if (!this->numbersTexturePath_.empty()) {
        textureIndex_ = TextureManager::LoadTexture(this->numbersTexturePath_, [this](uint32_t _newIdx) {
            this->settingOnLoadTexture(_newIdx);
        });
    }
}

bool TimerForSpriteComponent::Edit() {
    bool isChanged = false;
#ifdef DEBUG
    ImGui::Text("Sprite");

    isChanged |= DragGuiVectorCommand("Offset", offset_, 0.1f);
    isChanged |= DragGuiVectorCommand("SpriteSizeInteger", spriteSizeInteger_, 0.1f);
    isChanged |= DragGuiVectorCommand("SpriteSizeDecimal", spriteSizeDecimal_, 0.1f);

    ImGui::Spacing();

    isChanged |= DragGuiVectorCommand("AnchorOnWindow", anchorOnWindow_, 0.1f);

    ImGui::Spacing();

    isChanged |= DragGuiVectorCommand("SpriteMarginInteger", spriteMarginInteger_, 0.1f);
    isChanged |= DragGuiVectorCommand("SpriteMarginDecimal", spriteMarginDecimal_, 0.1f);
    isChanged |= DragGuiVectorCommand("MarginBetweenIntegerAndDecimal", marginBetweenIntegerAndDecimal_, 0.1f);

    ImGui::Spacing();

    auto askLoadTexture = [&]() {
        bool askLoad = false;

        askLoad = ImGui::Button("Load Texture");
        askLoad |= ImGui::ImageButton(
            reinterpret_cast<ImTextureID>(TextureManager::getDescriptorGpuHandle(textureIndex_).ptr),
            ImVec2(18, 18), ImVec2(0, 0), ImVec2(1, 1), 6);
        return askLoad;
    };
    if (askLoadTexture()) {
        std::string directory, fileName;
        std::string textureDefaultDirectory = std::string(kApplicationResourceDirectory) + "/Texture";
        if (myfs::selectFileDialog(textureDefaultDirectory, directory, fileName, {"png"})) {
            auto command = std::make_unique<SetterCommand<std::string>>(
                &this->numbersTexturePath_,
                textureDefaultDirectory + "/" + directory + "/" + fileName,
                [this](std::string*) { textureIndex_ = TextureManager::LoadTexture(this->numbersTexturePath_, [this](uint32_t _newIdx) {
                                           this->settingOnLoadTexture(_newIdx);
                                       }); }, true);

            EditorController::getInstance()->pushCommand(std::move(command));
        }
    }

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Digit");

    isChanged |= InputGuiCommand("Digit For Sprite", digitForSprite_, "%d");
    isChanged |= SlideGuiCommand("Digit Integer For Sprite", digitIntegerForSprite_, 0, digitForSprite_, "%d");
    isChanged |= SlideGuiCommand("Digit Decimal For Sprite", digitDecimalForSprite_, 0, digitForSprite_ - digitIntegerForSprite_, "%d");

#endif
    return isChanged;
}

void TimerForSpriteComponent::settingOnLoadTexture(uint32_t _texIdx) {
    const auto& metaData = TextureManager::getTexMetadata(_texIdx);

    if (metaData.width == 0 || metaData.height == 0) {
        LOG_ERROR("Texture metadata is invalid. Width or height is zero.");
        return;
    }

    numbersTextureSize_ = {static_cast<float>(metaData.width), static_cast<float>(metaData.height)};
    // *0.1 は 10桁であることを想定して
    numberTileSize_ = {numbersTextureSize_[X] * 0.1f, numbersTextureSize_[Y]};
}
