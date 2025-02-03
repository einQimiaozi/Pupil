#pragma once

#include <filesystem>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>

#include "runtime/core/macro.h"
#include "runtime/core/serializer/serializer.h"
#include "runtime/function/global/global_context.h"

#include "_generated/serializer/all_serializer.h"

namespace Pupil {
    class AssetManager {
    public:
        template<typename AssetType>
        bool loadAsset(const std::string& asset_url, AssetType& out_asset) const {
            std::filesystem::path asset_path = std::filesystem::absolute(runtime_global_context.config_manager->root_path / asset_url);
            std::ifstream asset_json_file(asset_path);
            if (!asset_json_file) {
                LOG_ERROR("open file: {} failed!", asset_path.generic_string());
                return false;
            }

            std::stringstream buffer;
            buffer << asset_json_file.rdbuf();
            std::string asset_json_text(buffer.str());

            std::string error;
            auto&& asset_json = Json::parse(asset_json_text, error);
            if (!error.empty()) {
                LOG_ERROR("parse json file {} failed!", asset_url);
                return false;
            }

            Serializer::read(asset_json, out_asset);
            return true;
        }

        template<typename AssetType>
        bool saveAsset(const AssetType& out_asset, const std::string& asset_url) const {
            std::ofstream asset_json_file(std::filesystem::absolute(runtime_global_context.config_manager->root_path / asset_url));
            if (!asset_json_file) {
                LOG_ERROR("open file {} failed!", asset_url);
                return false;
            }

            auto&& asset_json      = Serializer::write(out_asset);
            std::string&& asset_json_text = asset_json.dump();

            asset_json_file << asset_json_text;
            asset_json_file.flush();

            return true;
        }
    };
}