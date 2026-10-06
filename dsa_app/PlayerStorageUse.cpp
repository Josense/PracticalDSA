#include <dsa/PlayerStorage.h>
#include <iostream>
#include <algorithm>
#include <ranges>

void usePlayerStorage() {
    PlayerStorage party;

    party.AddPlayer(1, 500, 100.0f, "Hero");
    party.AddPlayer(2, 50, 25.0f, "Sidekick");
    party.AddPlayer(3, 9000, 150.0f, "Boss");

    for (PlayerRef p : party.GetView()) {
        if (p.score > 100) {
            p.Heal(10.0f);
        }
    }

    auto view = party.GetView();
    auto toughest = std::ranges::max_element(view, {},
      [](const PlayerRef& p) {
        return p.health;
      }
    );

    if (toughest != view.end()) {
        std::cout << "Toughest: " << (*toughest).name << "\n";
    }

    auto mvps = party.GetView()
      | std::views::filter([](const PlayerRef& p) {
          return p.score > 100;
        })
      | std::views::transform([](const PlayerRef& p) {
          return p.name;
        });

    std::cout << "MVPs: ";
    std::ranges::for_each(mvps, [](const std::string& name) {
      std::cout << name << " ";
    });
    std::cout << "\n";
}