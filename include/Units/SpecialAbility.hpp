#include <string>
#include <vector>
#include "Units\Character.hpp"

class SpecialAbility
{
public:
    std::string name;
    explicit SpecialAbility(std::string n) : name(std::move(n)){}
    virtual ~SpecialAbility() = default;

    virtual bool Use(Character* user, std::vector<Character*>& targeted_Charectures);
};