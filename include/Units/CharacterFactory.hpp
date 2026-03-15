#pragma once
#include <functional>
#include <memory>
#include <unordered_map>

class Character;

class CharacterFactory
{
public:
    using Creator = std::function<std::unique_ptr<Character>(const std::string&)>;

    static void Register(const std::string& class_name, Creator creator);
    static std::unique_ptr<Character> Create(
        const std::string& class_name,
        const std::string& json_path
    );

private:
    static std::unordered_map<std::string, Creator>& GetRegistry();
};