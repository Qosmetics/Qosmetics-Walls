#include "QwallConversion.hpp"
#include "WallObjectConfig.hpp"

#include "logging.hpp"
#include "qosmetics-core/shared/Data/Descriptor.hpp"
#include "qosmetics-core/shared/Utils/BundleUtils.hpp"
#include "qosmetics-core/shared/Utils/FileUtils.hpp"
#include "static-defines.hpp"

#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "UnityEngine/ImageConversion.hpp"
#include "UnityEngine/TextAsset.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "zip/shared/zip.h"

#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"

#include <algorithm>

#include "assets.hpp"

#define coro(...) custom_types::Helpers::CoroutineHelper::New(__VA_ARGS__)

namespace Qosmetics::Walls::QwallConversion
{
    void ConvertOldQwalls(std::function<void()> onFinished)
    {
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(coro(ConvertAllFoundQwalls(GetNonConverted(GetQwallFiles()), std::move(onFinished))));
    }

    std::vector<std::string> GetQwallFiles()
    {
        std::vector<std::string> result;

        Qosmetics::Core::FileUtils::GetFilesInFolderPath("qwall", wall_path, result);
        Qosmetics::Core::FileUtils::GetFilesInFolderPath("qwall", box_path, result);
        return result;
    }

    std::vector<std::pair<std::string, std::string>> GetNonConverted(std::vector<std::string> filePaths)
    {
        std::vector<std::pair<std::string, std::string>> toConvert;
        toConvert.reserve(filePaths.size());
        for (auto file : filePaths)
        {
            std::string fileName(Qosmetics::Core::FileUtils::GetFileName(file, true));
            std::replace(fileName.begin(), fileName.end(), ' ', '_');

            std::string convertedFilePath = fmt::format("{}/{}.box", box_path, fileName);

            if (!fileexists(convertedFilePath))
            {
                if (fileexists(fmt::format("{}/{}", box_path, file)))
                    toConvert.emplace_back(std::make_pair(fmt::format("{}/{}", box_path, file), convertedFilePath));
                else
                    toConvert.emplace_back(std::make_pair(fmt::format("{}/{}", wall_path, file), convertedFilePath));
            }
        }

        return toConvert;
    }

    custom_types::Helpers::Coroutine ConvertAllFoundQwalls(std::vector<std::pair<std::string, std::string>> oldNewPathPairs, std::function<void()> onFinished)
    {
        // all pairs should be things to convert
        for (auto& pair : oldNewPathPairs)
        {
            std::string oldPath = pair.first;
            INFO("converting {}", oldPath);
            std::string newPath = pair.second;

            // if it exists anyways just skip it
            if (fileexists(newPath))
            {
                ERROR("{} Existed, continuing...", newPath);
                continue;
            }

            if (!fileexists(oldPath))
            {
                ERROR("{} did not exist, continuing...", oldPath);
                continue;
            }
            UnityEngine::AssetBundle* bundle = nullptr;
            INFO("Loading bundle...");
            co_yield coro(Qosmetics::Core::BundleUtils::LoadBundleFromFileAsync(oldPath, bundle));

            if (!bundle)
            {
                ERROR("Couldn't load bundle from file, continuing...");
                continue;
            }

            std::string androidFileName(Qosmetics::Core::FileUtils::GetFileName(oldPath));
            std::replace(androidFileName.begin(), androidFileName.end(), ' ', '_');

            /// load descriptor
            UnityEngine::TextAsset* descriptorAsset = nullptr;
            INFO("Getting descriptor...");
            co_yield coro(Qosmetics::Core::BundleUtils::LoadAssetFromBundleAsync(bundle, "descriptor", descriptorAsset));
            std::string descriptorText = descriptorAsset->get_text();
            rapidjson::Document descriptorDoc;
            descriptorDoc.Parse(descriptorText);
            LegacyDescriptor legacyDescriptor = LegacyDescriptor(descriptorDoc);

            Qosmetics::Core::Descriptor actualDescriptor(legacyDescriptor.authorName, legacyDescriptor.objectName, legacyDescriptor.description, newPath, "thumbnail.png");

            /// load config
            UnityEngine::TextAsset* configAsset = nullptr;
            INFO("Getting config...");
            co_yield coro(Qosmetics::Core::BundleUtils::LoadAssetFromBundleAsync(bundle, "config", configAsset));

            std::string configText = configAsset->get_text();
            rapidjson::Document configDoc;
            configDoc.Parse(configText);
            LegacyConfig legacyConfig = LegacyConfig(configDoc);

            Qosmetics::Walls::WallObjectConfig actualConfig(legacyConfig.replaceCoreMaterial, legacyConfig.replaceFrameMaterial, legacyConfig.replaceCoreMesh, legacyConfig.replaceFrameMesh, legacyConfig.disableFrame, legacyConfig.disableFakeGlow);

            INFO("Making package.json...");
            Qosmetics::Core::Manifest manifest(newPath, androidFileName, actualDescriptor, actualConfig);

            auto doc = manifest.ToJson();
            rapidjson::StringBuffer buffer;
            buffer.Clear();
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
            doc.Accept(writer);

            /// load thumbnail
            UnityEngine::Texture2D* thumbnail = nullptr;
            INFO("Getting thumbnail...");
            co_yield coro(Qosmetics::Core::BundleUtils::LoadAssetFromBundleAsync(bundle, "thumbnail", thumbnail));
            auto imageData = UnityEngine::ImageConversion::EncodeToPNG(thumbnail);

            int error = 0;
            auto zip = zip_open(newPath.data(), ZIP_CREATE, &error);
            zip_flags_t write;

            zip_error_t err;
            auto package_buffer = zip_source_buffer_create(buffer.GetString(), buffer.GetSize(), 0, &err);
            zip_file_add(zip, "package.json", package_buffer, ZIP_FL_ENC_GUESS);

            co_yield nullptr;
            std::vector<char> qwallFile = readbytes(oldPath);
            auto qwall_buffer = zip_source_buffer_create(qwallFile.data(), qwallFile.size(), 0, &err);
            zip_file_add(zip, androidFileName.c_str(), qwall_buffer, ZIP_FL_ENC_GUESS);

            co_yield nullptr;
            zip_source_t* thumbnail_buffer = zip_source_buffer_create(imageData.begin(), imageData.size(), 0, &err);
            zip_file_add(zip, "thumbnail.png", thumbnail_buffer, ZIP_FL_ENC_GUESS);

            zip_close(zip);
            bundle->Unload(true);

            // TODO: make this actually delete again
            // deletefile(oldPath);
            co_yield nullptr;
        }

        if (onFinished)
            onFinished();
        co_return;
    }
}