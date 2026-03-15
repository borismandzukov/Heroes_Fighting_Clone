#include <vector>
#include "Units/All_Characters.hpp"

enum class CharacterType {
    Barbarian,
    Berserker,
    Cleric,
    Cryomancer,
    Wizard
};

class Player
{
private:
    std::vector<int> number_of_charectures;
    std::vector<std::unique_ptr<Character>> m_characters;
protected:
    
public:
    Player();
    bool Add_Characters(const std::vector<std::pair<CharacterType, int>>& entries);

};

Player::Player()
{
}

bool Player::Add_Characters(
    const std::vector<std::pair<CharacterType, int>>& entries)
{
    for (const auto& [type, count] : entries)
    {
        for (int i = 0; i < count; ++i)
        {
            auto character = Character(type);

            if (!character)
                return false;

            m_characters.push_back(std::move(character));
        }
    }

    return true;
}