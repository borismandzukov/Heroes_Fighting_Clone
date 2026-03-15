#ifndef CRYOMANCER_HPP
#define CRYOMANCER_HPP

#include "Units/Character.hpp"
#include "Units/SpecialAbilities/Frostzone.hpp"
#include <nlohmann/json.hpp>

class Cryomancer : public Character {
private:
    FrostZone m_frostzone;
    std::string m_texture_path;
protected:
    std::string GetCharacterClassName() const override {
        return "cryomancer";
    }

    bool LoadSpecificData(const nlohmann::json& character_data) override {
        try {
            auto abilities = character_data["abilities"]["frostzone"];
            
            // radios,dmg,chance,mana
            m_frostzone = FrostZone(
                abilities["mana_cost"].get<int>(),
                abilities["radios"].get<int>(),
                abilities["damage"].get<int>(),
                abilities["percent_slow"].get<int>()
            );

            m_texture_path = character_data["visuals"]["path"].get<std::string>();
            
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading Cryomancer specific data: " << e.what() << std::endl;
            return false;
        }
    }

public:
    explicit Cryomancer(const std::string& json_path = "") 
        : Character("", 0, 0, 0, 0, 0),
          m_frostzone(0, 0, 0, 0)
    {
        if (!json_path.empty()) {
            LoadFromJson(json_path);
        }
    }

    bool UseLifeSacrifice(std::vector<Character*>& targets) {
        return m_frostzone.Use(this, targets);
    }

    FrostZone& GetLifeSacrifice() {
        return m_frostzone;
    }

    const std::string& GetTexturePath() const {
        return m_texture_path;
    }
};
#endif