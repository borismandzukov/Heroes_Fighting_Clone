#ifndef BERSERKER_HPP
#define BERSERKER_HPP

#include "Units/Character.hpp"
#include "Units/SpecialAbilities/BloodLust.hpp"
#include <nlohmann/json.hpp>

class Berserker : public Character {
private:
    BloodLust m_BloodLust;
    std::string m_texture_path;
protected:
    std::string GetCharacterClassName() const override {
        return "berserker";
    }

    bool LoadSpecificData(const nlohmann::json& character_data) override {
        try {
            auto abilities = character_data["abilities"]["bloodlust"];
            
            // radios,dmg,chance,mana
            m_BloodLust = BloodLust(
                abilities["mana_cost"].get<int>(),
                abilities["damage"].get<int>(),
                abilities["steal_health"].get<float>()
            );

            m_texture_path = character_data["visuals"]["path"].get<std::string>();
            
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading Berserker specific data: " << e.what() << std::endl;
            return false;
        }
    }

public:
    explicit Berserker(const std::string& json_path = "") 
        : Character("", 0, 0, 0, 0, 0),
          m_BloodLust(0, 0, 0.0f)
    {
        if (!json_path.empty()) {
            LoadFromJson(json_path);
        }
    }

    bool UseLifeSacrifice(std::vector<Character*>& targets) {
        return m_BloodLust.Use(this, targets);
    }

    BloodLust& GetLifeSacrifice() {
        return m_BloodLust;
    }

    const std::string& GetTexturePath() const {
        return m_texture_path;
    }
};
#endif