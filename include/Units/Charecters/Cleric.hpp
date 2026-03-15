#ifndef CLERIC_HPP
#define CLERIC_HPP

#include "Units/Character.hpp"
#include "Units/SpecialAbilities/Healing.hpp"
#include <nlohmann/json.hpp>

class Cleric : public Character {
private:
    Healing m_Heal;
    std::string m_texture_path;
protected:
    std::string GetCharacterClassName() const override {
        return "cleric";
    }

    bool LoadSpecificData(const nlohmann::json& character_data) override {
        try {
            auto abilities = character_data["abilities"]["healing"];
            
            // radios,dmg,chance,mana
            m_Heal = Healing(
                abilities["heal"].get<int>(),
                abilities["mana_cost"].get<int>()
            );

            m_texture_path = character_data["visuals"]["path"].get<std::string>();
            
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading Cleric specific data: " << e.what() << std::endl;
            return false;
        }
    }

public:
    explicit Cleric(const std::string& json_path = "") 
        : Character("", 0, 0, 0, 0, 0),
          m_Heal(0, 0)
    {
        if (!json_path.empty()) {
            LoadFromJson(json_path);
        }
    }

    bool UseLifeSacrifice(std::vector<Character*>& targets) {
        return m_Heal.Use(this, targets);
    }

    Healing& GetLifeSacrifice() {
        return m_Heal;
    }

    const std::string& GetTexturePath() const {
        return m_texture_path;
    }
};
#endif