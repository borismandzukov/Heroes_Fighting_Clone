#include "Units/SpecialAbility.hpp"

class FrostZone : public SpecialAbility{
private:
    int m_mana_cost;
    int m_radios;
    int m_damage;
    int m_percent_slow;

public:
    FrostZone(int mana_cost, int radios, int damage, int percent_slow): SpecialAbility("FrostZone"),
    m_damage(damage),m_radios(radios), m_mana_cost(mana_cost), m_percent_slow(percent_slow)
    {}

    bool Use(Character* user, std::vector<Character*>& targeted_Charectures) override{
        if (!user || !user->UseMana(m_mana_cost)) {
            return false;  // Not enough mana or null user
        }

        for (Character* c : targeted_Charectures){
            if (c && c->IsAlive()){
                c->SlowDown(m_percent_slow);
            }
        }
        return true;
    }
};