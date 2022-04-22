#pragma once
#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "sombrero/shared/FastColor.hpp"
#include <string>
#include <vector>

namespace Qosmetics::Walls::QwallConversion
{
    struct LegacyDescriptor
    {
        std::string authorName;
        std::string objectName;
        std::string description;

        LegacyDescriptor(const std::string& authorName = "", const std::string& objectName = "", const std::string& description = "") : authorName(authorName), objectName(objectName), description(description) {}
        LegacyDescriptor(const rapidjson::Document& doc)
        {
            authorName = doc["authorName"].GetString();
            objectName = doc["objectName"].GetString();
            description = doc["description"].GetString();
        }
    };

    struct LegacyConfig
    {
        bool replaceCoreMaterial = false;
        bool replaceCoreMesh = false;
        bool disableFrame = false;
        bool disableFakeGlow = false;
        bool replaceFrameMaterial = false;
        bool replaceFrameMesh = false;
        bool scoreSubmissionDisabled = false;
        bool moreThan1Core = false;
        bool moreThan1Frame = false;

        LegacyConfig(){};
        LegacyConfig(const rapidjson::Document& doc)
        {
            replaceCoreMaterial = doc["replaceCoreMaterial"].GetBool();
            replaceCoreMesh = doc["replaceCoreMesh"].GetBool();
            disableFrame = doc["disableFrame"].GetBool();
            disableFakeGlow = doc["disableFakeGlow"].GetBool();
            replaceFrameMaterial = doc["replaceFrameMaterial"].GetBool();
            replaceFrameMesh = doc["replaceFrameMesh"].GetBool();
            scoreSubmissionDisabled = doc["scoreSubmissionDisabled"].GetBool();
            moreThan1Core = doc["moreThan1Core"].GetBool();
            moreThan1Frame = doc["moreThan1Frame"].GetBool();
        }
    };

    void ConvertOldQwalls(std::function<void()> onFinished = nullptr);

    std::vector<std::string> GetQwallFiles();
    std::vector<std::pair<std::string, std::string>> GetNonConverted(std::vector<std::string> filePaths);
    custom_types::Helpers::Coroutine ConvertAllFoundQwalls(std::vector<std::pair<std::string, std::string>> oldNewPathPairs, std::function<void()> onFinished = nullptr);
}