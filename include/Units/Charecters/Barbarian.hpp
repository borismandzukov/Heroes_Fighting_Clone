#ifndef BARBARIAN_HPP
#define BARBARIAN_HPP

#include "Units/Character.hpp"
#include "Units/SpecialAbilities/LifeSacrufice.hpp"
#include <nlohmann/json.hpp>

class Barbarian : public Character {
private:
    LifeSacrufice m_life_sacrifice;
    std::string m_texture_path;

protected:
    std::string GetCharacterClassName() const override {
        return "barbarian";
    }

    bool LoadSpecificData(const nlohmann::json& character_data) override {
        try {
            auto abilities = character_data["abilities"]["life_sacrifice"];
            
            // persent,mana,damage
            m_life_sacrifice = LifeSacrufice(
                abilities["percentage"].get<float>(),
                abilities["mana_cost"].get<int>(),
                abilities["damage"].get<int>()
            );

            m_texture_path = character_data["visuals"]["path"].get<std::string>();
            
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading Barbarian specific data: " << e.what() << std::endl;
            return false;
        }
    }

public:
    explicit Barbarian(const std::string& json_path = "") 
        : Character("", 0, 0, 0, 0, 0),
          m_life_sacrifice(0.0f, 0, 0)
    {
        if (!json_path.empty()) {
            LoadFromJson(json_path);
        }
    }

    bool UseLifeSacrifice(std::vector<Character*>& targets) {
        return m_life_sacrifice.Use(this, targets);
    }

    LifeSacrufice& GetLifeSacrifice() {
        return m_life_sacrifice;
    }

    const std::string& GetTexturePath() const {
        return m_texture_path;
    }
};

#endif