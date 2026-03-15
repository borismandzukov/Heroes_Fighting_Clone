#include "Units/SpecialAbility.hpp"

class LifeSacrufice : public SpecialAbility {
private:
    float m_health_percent_share;
    int m_damage;
    int m_mana_cost;

public:
    LifeSacrufice(float health_percent_share, int mana_cost, int damage): SpecialAbility("LifeShare"),
    m_health_percent_share(health_percent_share),
    m_mana_cost(mana_cost),
    m_damage(damage){}

    bool Use(Character* user, std::vector<Character*>& targeted_Characters) override {
        if (!user || !user->UseMana(m_mana_cost)) {
            return false;  // Not enough mana or null user
        }

        for (Character* c : targeted_Characters){
            if(c->IsAlive()){
                user->TakeDamage(m_damage*m_health_percent_share);
                c->TakeDamage(m_damage+(m_damage*m_health_percent_share));
            }
        }
        return true;
    }
};