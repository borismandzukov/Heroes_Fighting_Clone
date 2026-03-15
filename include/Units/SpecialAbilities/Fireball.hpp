#include "Units/SpecialAbility.hpp"
#include <random>

class Fireball : public SpecialAbility{
private:
    int m_mana_cost;
    int m_radios;
    int m_damage;
    float m_chance_to_miss;

    float randomFloat(float min, float max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

public:
    Fireball(int radios, int damage, float chance_to_miss, int mana_cost): SpecialAbility("Fireball"), 
    m_radios(radios), m_damage(damage), m_chance_to_miss(chance_to_miss), m_mana_cost(mana_cost)
    {}

    bool Use(Character* user, std::vector<Character*>& targeted_Charectures) override{
        if (!user || !user->UseMana(m_mana_cost)) {
            return false;  // Not enough mana or null user
        }

        for (Character* c : targeted_Charectures){
            if (c && c->IsAlive()){
                float x = randomFloat(0.0f,1.0f);
                if (x < m_chance_to_miss){
                    // do nothing it missed
                }
                else{
                    c->TakeDamage(m_damage);
                }
            }
        }
        return true;
        
    }
};