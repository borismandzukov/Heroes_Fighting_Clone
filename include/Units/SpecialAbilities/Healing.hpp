#include "Units/SpecialAbility.hpp"

class Healing : public SpecialAbility {
private:
    int m_heal;
    int m_mana_cost;

public:
    Healing(int heal, int mana_cost) : SpecialAbility("Heal"),
        m_heal(heal), m_mana_cost(mana_cost) {}

    bool Use(Character* user, std::vector<Character*>& targeted_Characters) override {
        if (!user || !user->UseMana(m_mana_cost)) {
            return false;  // Not enough mana or null user
        }
        
        for (Character* c : targeted_Characters) {
            if (c && c->IsAlive()) {
                c->Heal(m_heal);
            }
        }
        return true;
    }
};