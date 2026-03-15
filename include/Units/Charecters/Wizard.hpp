#ifndef WIZARD_HPP
#define WIZARD_HPP

#include "Units/Character.hpp"
#include "Units/SpecialAbilities/Fireball.hpp"
#include <nlohmann/json.hpp>

class Wizard : public Character {
private:
    Fireball m_Fireball;
    std::string m_texture_path;
protected:
    std::string GetCharacterClassName() const override {
        return "wizard";
    }

    bool LoadSpecificData(const nlohmann::json& character_data) override {
        try {
            auto abilities = character_data["abilities"]["fireball"];
            
            // radios,dmg,chance,mana
            m_Fireball = Fireball(
                abilities["radios"].get<int>(),
                abilities["damage"].get<int>(),
                abilities["chanse_to_miss"].get<float>(),
                abilities["mana_cost"].get<int>()
            );

            m_texture_path = character_data["visuals"]["path"].get<std::string>();
            
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading Wizard specific data: " << e.what() << std::endl;
            return false;
        }
    }

public:
    explicit Wizard(const std::string& json_path = "") 
        : Character("", 0, 0, 0, 0, 0),
          m_Fireball(0, 0, 0.0f,0)
    {
        if (!json_path.empty()) {
            LoadFromJson(json_path);
        }
    }

    bool UseLifeSacrifice(std::vector<Character*>& targets) {
        return m_Fireball.Use(this, targets);
    }

    Fireball& GetLifeSacrifice() {
        return m_Fireball;
    }

    const std::string& GetTexturePath() const {
        return m_texture_path;
    }
};
#endif