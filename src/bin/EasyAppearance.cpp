#include "EasyAppearance.h"
#include "include/Utils.h"
namespace EasyAppearance
{
	static const std::string configDir = "Data/SKSE/EasyAppearance/Config";
	static const char* configSection = "EasyAppearance";
	static void copyAppearance(RE::TESNPC* a_src, RE::TESNPC* a_des)
	{
		INFO("Copying appearance from {} to {}", a_src->GetName(), a_des->GetName());
		INFO("headparts");
		RE::free(a_des->headParts);
		a_des->headParts = (RE::BGSHeadPart**)RE::malloc(sizeof(RE::BGSHeadPart*) * a_src->numHeadParts);
		a_des->numHeadParts = a_src->numHeadParts;
		for (int8_t i = 0; i < a_src->numHeadParts; i++) {
			a_des->headParts[i] = a_src->headParts[i];
		}
		INFO("height and weight");
		a_des->height = a_src->height;
		a_des->weight = a_src->weight;
		INFO("head related data");
		a_des->headRelatedData = a_src->headRelatedData;
		INFO("texture lighting");
		a_des->bodyTintColor = a_src->bodyTintColor;
		INFO("face morph and parts");
		a_des->faceData = a_src->faceData;
		INFO("tint layer");
		a_des->tintLayers = a_src->tintLayers;
		
	}
	static void patchSingleConfig(const char* a_path)
	{
		CSimpleIniA ini;
		ini.LoadFile(a_path);
		CSimpleIniA::TNamesDepend keys;
		ini.GetAllKeys(configSection, keys);
		for (auto& key_it : keys) {
			const char* key = key_it.pItem;
			const char* value = ini.GetValue(configSection, key);
			INFO("{}", key);
			INFO("{}", value);
			if (!value) {
				continue;
			}
			std::vector<std::string_view> srcMetaData;
			std::vector<std::string_view> desMetaData;
			Utils::splitSV(srcMetaData, std::string_view(value), '|');
			Utils::splitSV(desMetaData, std::string_view(key), '|');
			
			RE::TESNPC* srcActor;
			RE::TESNPC* desActor;
			if (Utils::lookup(srcMetaData[0], srcMetaData[1], srcActor) &&
				Utils::lookup(desMetaData[0], desMetaData[1], desActor)) {
				copyAppearance(srcActor, desActor);
			}
		}
	}
	
	void patch()
	{
		INFO("Loading NPC mappings...");
		if (!std::filesystem::is_directory(configDir)) {
			ERROR("Error: config directory not found");
			return;
		}
		for (const auto& one_entry : std::filesystem::directory_iterator(configDir)) {
			std::string one_path = one_entry.path().string();
			INFO("Loading from {}", one_path);
			patchSingleConfig(one_path.c_str());
		}
		
	}

}
