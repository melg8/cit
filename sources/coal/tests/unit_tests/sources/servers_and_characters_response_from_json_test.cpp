// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <testing_framework.h>

#include <json_test_data.h>
#include <servers_and_characters_response_from_json.h>

#include <spdlog/spdlog.h>
#include <glaze/glaze.hpp>

#include <algorithm>

namespace coal::test {

inline auto Prettify(const auto& in, auto& out) noexcept -> void {
  glz::context ctx{};
  glz::detail::prettify_json<glz::opts{}>(ctx, in, out);
  spdlog::info("After prettify ctx error?: {}, code: {}", ctx.includer_error,
               static_cast<int>(ctx.error));
}

inline auto RemoveSpaces(std::string str) {
  str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
  return str;
}

struct CustomLayout {
  std::string region = {};
  int players = 0;
};

[[nodiscard]] static auto AreEqual(double x_1, double x_2) noexcept {
  static const auto kEps = 1e-5;
  return std::abs(x_1 - x_2) < kEps;
}

SCENARIO("json parsing") {
  SECTION("server from json") {
    static const auto kJsonServerText = R"(
      {
        "name": "I",
        "region": "EU",
        "players": 35,
        "key": "EUI",
        "addr": "eud1.adventure.land",
        "port": 2053
      })";

    const auto s = glz::read_json<Server>(kJsonServerText);
    CHECK(s);
    const Server server = s.value();
    CHECK(server.name == "I");
    CHECK(server.region == "EU");
    CHECK(server.players == 35);
    CHECK(server.key == "EUI");
    CHECK(server.addr == "eud1.adventure.land");
    CHECK(server.port == 2053);
  }

  SECTION("character from json") {
    static const auto kJsonCharacterText = R"(
      {
        "id": "6223051783405568",
        "name": "This",
        "level": 86,
        "type": "rogue",
        "online": 0,
        "skin": "sarmor1h",
        "cx": {
          "hair": "hairdo522",
          "head": "fmakeup02",
          "chin": "beard112"
        },
        "in": "main",
        "map": "main",
        "x": 69.95957619408078,
        "y": -96.0977505564107,
        "home": "EUII"
      })";

    const auto s = glz::read_json<Character>(kJsonCharacterText);
    CHECK(s);
    Character character = s.value();
    CHECK(character.id == "6223051783405568");
    CHECK(character.name == "This");
    CHECK(character.level == 86);
    CHECK(character.type == "rogue");
    CHECK(AreEqual(character.online, 0));
    CHECK(character.skin == "sarmor1h");
    CHECK(character.cx["hair"] == "hairdo522");
    CHECK(character.cx["head"] == "fmakeup02");
    CHECK(character.cx["chin"] == "beard112");
    CHECK(character.in == "main");
    CHECK(character.map == "main");
    CHECK(AreEqual(character.x, 69.95957619408078));
    CHECK(AreEqual(character.y, -96.0977505564107));
    CHECK(character.home == "EUII");
  }

  SECTION("local server character from json") {
    static const auto kJsonLocalServereCharacterText = R"(
         {
            "home": "EUI",
            "in": "bank",
            "name": "Hold",
            "cx": {
               "hair": "hairdo520",
               "hat": "hat407",
               "head": "fmakeup01"
            },
            "id": "6195934853595136",
            "skin": "marmor12b",
            "type": "merchant",
            "online": 0,
            "level": 60,
            "map": "bank",
            "y": -37,
            "x": 0
         })";
    const auto s = glz::read_json<Character>(kJsonLocalServereCharacterText);
    CHECK(s);
    Character character = s.value();
    CHECK(character.id == "6195934853595136");
  }

  SECTION("online server character from json") {
    static const auto kJsonOnlineCharacterText = R"(
          {
            "id": "6223051783405568",
            "name": "This",
            "level": 86,
            "type": "rogue",
            "online": 20676.759000000002,
            "server": "EUII",
            "secret": "Or8EfmvFlKW0aQPGEHNptocM",
            "skin": "sarmor1h",
            "cx": {
               "hair": "hairdo522",
               "head": "fmakeup02",
               "chin": "beard112"
            },
            "in": "main",
            "map": "main",
            "x": 69.95957619408078,
            "y": -96.0977505564107,
            "home": "EUII"
         }
      )";
    const auto s = glz::read_json<Character>(kJsonOnlineCharacterText);
    CHECK(s);
    Character character = s.value();
    CHECK(character.id == "6223051783405568");
    CHECK(character.server.value() == "EUII");
    CHECK(character.secret.value() == "Or8EfmvFlKW0aQPGEHNptocM");
  }

  SECTION("tutorial status from json") {
    [[maybe_unused]] static const auto kJsonTutorialText = R"(
    {
      "step": 8,
      "completed": [],
      "finished": true,
      "task": false,
      "progress": 100
    })";
    const auto s = glz::read_json<TutorialStatus>(kJsonTutorialText);
    CHECK(s);
    TutorialStatus tutorial_status = s.value();
    CHECK(tutorial_status.step == 8);
    CHECK(tutorial_status.completed.empty());
    CHECK(tutorial_status.finished);
    CHECK_FALSE(tutorial_status.task);
    CHECK(tutorial_status.progress == 100);
  }

  SECTION("code element from json") {
    static const auto kJsonCodeText = R"(
    [
        "Abuse",
        3220
    ]
  )";
    const auto s = glz::read_json<CodeElement>(kJsonCodeText);
    CHECK(s);

    const auto value = s.value();
    CHECK(std::get<std::string>(value[0]) == "Abuse");
    CHECK(std::get<size_t>(value[1]) == 3220);
  }
  SECTION("servers and characters response from json") {
    const auto s = glz::read_json<ServersAndCharactersResponse>(
        ReducedFrom(kServersAndCharactersResponse));
    CHECK(s);
    const auto servers_and_characters = s.value();
    CHECK(servers_and_characters.type == "servers_and_characters");
    CHECK(servers_and_characters.servers.size() == 8);
    CHECK(servers_and_characters.characters.size() == 4);
    CHECK(servers_and_characters.code_list.size() == 10);
    CHECK(servers_and_characters.mail == 0);
    CHECK(servers_and_characters.rewards.size() == 0);
  }
  SECTION("local servers and characters response from json") {
    const auto s = glz::read_json<ServersAndCharactersResponse>(
        ReducedFrom(kLocalServersAndCharactersResponse));
    CHECK(s);
    const auto servers_and_characters = s.value();
    CHECK(servers_and_characters.type == "servers_and_characters");
    CHECK(servers_and_characters.servers.size() == 1);
    CHECK(servers_and_characters.characters.size() == 4);
    CHECK(servers_and_characters.code_list.size() == 8);
    CHECK(servers_and_characters.mail == 0);
    CHECK(servers_and_characters.rewards.size() == 0);
  }

  SECTION("different layout from json") {
    static const auto kJsonLayoutText1 = R"(
      {
        "region": "EU",
        "players": 4
      }
    )";
    const auto s1 = glz::read_json<CustomLayout>(kJsonLayoutText1);
    CHECK(s1);

    static const auto kJsonLayoutText2 = R"(
      {
        "players": 4,
        "region": "EU"
      }
    )";
    const auto s2 = glz::read_json<CustomLayout>(kJsonLayoutText2);
    CHECK(s2);
  }
}

}  // namespace coal::test
