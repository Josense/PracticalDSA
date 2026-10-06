/**
 * 自定义 SoA 容器
 * lesson 6-1
 */
#pragma once
#include <ranges>
#include <string>
#include <vector>


struct PlayerRef {
    int& id;
    int& score;
    float& health;
    std::string& name;

    void Heal(float amount) {
        health += amount;
    }
};

class PlayerStorage {
    std::vector<int> m_ids;
    std::vector<int> m_scores;
    std::vector<float> m_healths;
    std::vector<std::string> m_names;

public:
    void AddPlayer(int id, int score, float health, std::string name) {
        m_ids.push_back(id);
        m_scores.push_back(score);
        m_healths.push_back(health);
        m_names.push_back(name);
    }

    auto GetView() {
        return std::views::zip(m_ids, m_scores, m_healths, m_names)
            | std::views::transform([](auto&& tuple) {
                auto& [id, score, health, name] = tuple;
                return PlayerRef {id, score, health, name};
            });
    }
};
