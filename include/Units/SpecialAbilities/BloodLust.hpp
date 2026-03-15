#include "Units/SpecialAbility.hpp"


class BloodLust : public SpecialAbility{
private:
    int m_mana_cost;
    int m_damage;
    float m_steal_health;
public:
    BloodLust(int mana_cost, int damage, float steal_health): SpecialAbility("BloodLust"),
    m_damage(damage), m_mana_cost(mana_cost), m_steal_health(steal_health){}

   bool Use(Character* user, std::vector<Character*>& targeted_Charectures) override{
        if (!user || !user->UseMana(m_mana_cost)) {
            return false;  // Not enough mana or null user
        }
         for (Character* c : targeted_Charectures){
            if (c && c->IsAlive()){
                c->TakeDamage(m_damage);
                user->Heal(std::ceil((m_damage - c->GetDefence())* m_steal_health));
            }
        }
        return true;
    }
};