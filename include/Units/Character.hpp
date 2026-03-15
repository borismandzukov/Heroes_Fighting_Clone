#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include "SFML/Graphics.hpp"
#include <nlohmann/json.hpp>

class Character {
private:
    std::string m_name_character;
    sf::Sprite m_sprite;
    sf::Texture m_texture;

    int m_health_current; 
    int m_health_max; 
    int m_speed;
    int m_attack_power; 
    int m_defence; 
    int m_mana_current; 
    int m_mana_max; 

    int ApplyRandomness(int base_value, double min_mult, double max_mult) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(min_mult, max_mult);
        return static_cast<int>(base_value * dis(gen));
    }

protected:
    // Virtual method that derived classes override to specify their class name
    virtual std::string GetCharacterClassName() const = 0;

    // Derived classes can override to load their specific abilities/data
    virtual bool LoadSpecificData(const nlohmann::json& character_data) {
        // Default: do nothing, return success
        return true;
    }

public:
    explicit Character(
        std::string name_character, 
        int health, 
        int speed, 
        int attack_power, 
        int defence, 
        int mana
    ): 
        m_name_character(std::move(name_character)),
        m_health_max(health),
        m_health_current(health),
        m_speed(speed),
        m_attack_power(attack_power),
        m_defence(defence),
        m_mana_max(mana),
        m_mana_current(mana)
    {}
    
    virtual ~Character() = default;

    bool LoadTexture(const std::string& texture_path) {
        if (!m_texture.loadFromFile(texture_path)) {
            return false;
        }
        m_sprite.setTexture(m_texture);
        return true;
    }

    // All JSON loading logic is here
    virtual bool LoadFromJson(const std::string& json_path) {
        try {
            std::ifstream file(json_path);
            if (!file.is_open()) {
                std::cerr << "Failed to open JSON file: " << json_path << std::endl;
                return false;
            }

            nlohmann::json j;
            file >> j;
            file.close();

            // Get the character class name from derived class
            std::string class_name = GetCharacterClassName();
            
            // Navigate to the character-specific object (e.g., "barbarian", "wizard", etc.)
            auto character_data = j[class_name];
            auto base = character_data["base"];
            auto randomness = character_data["randomness"];
            auto visuals = character_data["visuals"];

            // Load base stats
            int health = base["health"];
            int speed = base["speed"];
            int attack = base["attack"];
            int defense = base["defense"];
            int mana = base["mana"];

            // Apply randomness if enabled
            if (randomness["enabled"].get<bool>()) {
                double min_mult = randomness["min_multiplier"];
                double max_mult = randomness["max_multiplier"];
                
                auto attrs = randomness["attributes_to_randomize"];
                for (const auto& attr : attrs) {
                    std::string attr_name = attr.get<std::string>();
                    if (attr_name == "health") {
                        health = ApplyRandomness(health, min_mult, max_mult);
                    } else if (attr_name == "attack") {
                        attack = ApplyRandomness(attack, min_mult, max_mult);
                    } else if (attr_name == "defense") {
                        defense = ApplyRandomness(defense, min_mult, max_mult);
                    }
                }
            }

            // Set the stats
            m_name_character = base["name"].get<std::string>();
            m_health_max = health;
            m_health_current = health;
            m_speed = speed;
            m_attack_power = attack;
            m_defence = defense;
            m_mana_max = mana;
            m_mana_current = mana;

            // Load texture
            std::string texture_path = visuals["path"].get<std::string>();
            LoadTexture(texture_path);

            // Let derived class load its specific data (abilities, etc.)
            if (!LoadSpecificData(character_data)) {
                return false;
            }

            return true;

        } catch (const std::exception& e) {
            std::cerr << "Error loading Character JSON: " << e.what() << std::endl;
            return false;
        }
    }

    virtual void TakeDamage(int damage) {
        int actual_damage = damage - m_defence;
        if (actual_damage > 0) {
            m_health_current -= actual_damage;
            if (m_health_current < 0) {
                m_health_current = 0;
            }
        }
    }
    
    virtual void Heal(int heal_amount) {
        m_health_current += heal_amount;
        if (m_health_current > m_health_max) {
            m_health_current = m_health_max;
        }
    }
    
    virtual bool UseMana(int mana_cost) {
        if (m_mana_current >= mana_cost) {
            m_mana_current -= mana_cost;
            return true;
        }
        return false;
    }
    
    virtual void RestoreMana(int mana_amount) {
        m_mana_current += mana_amount;
        if (m_mana_current > m_mana_max) {
            m_mana_current = m_mana_max;
        }
    }

    virtual void SlowDown(int percentage_slow){
        if (percentage_slow > 90){
            percentage_slow = 90;
        }
        m_speed = std::ceil(m_speed * (100 - percentage_slow) / 100);

        if (m_speed < 1) {
            m_speed = 1;
        }
    }

    // Getters
    const std::string& GetName() const { return m_name_character; }
    sf::Sprite& GetSprite() { return m_sprite; }
    const sf::Sprite& GetSprite() const { return m_sprite; }
    
    int GetHealthCurrent() const { return m_health_current; }
    int GetHealthMax() const { return m_health_max; }
    int GetSpeed() const { return m_speed; }
    int GetAttackPower() const { return m_attack_power; }
    int GetDefence() const { return m_defence; }
    int GetManaCurrent() const { return m_mana_current; }
    int GetManaMax() const { return m_mana_max; }
    
    bool IsAlive() const { return m_health_current > 0; }
    
    // Setters
    void SetSpeed(int speed) { m_speed = speed; }
    void SetAttackPower(int attack_power) { m_attack_power = attack_power; }
    void SetDefence(int defence) { m_defence = defence; }
};

#endif