#include "Units/CharacterFactory.hpp"

std::unordered_map<std::string, CharacterFactory::Creator>&
CharacterFactory::GetRegistry()
{
    static std::unordered_map<std::string, Creator> registry;
    return registry;
}

void CharacterFactory::Register(
    const std::string& class_name,
    Creator creator)
{
    GetRegistry()[class_name] = creator;
}

std::unique_ptr<Character> CharacterFactory::Create(
    const std::string& class_name,
    const std::string& json_path)
{
    auto& registry = GetRegistry();

    if (registry.find(class_name) == registry.end())
        return nullptr;

    return registry[class_name](json_path);
}