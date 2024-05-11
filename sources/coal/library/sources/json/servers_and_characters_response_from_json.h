// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef SERVERS_AND_CHARACTERS_RESPONSE_FROM_JSON_H
#define SERVERS_AND_CHARACTERS_RESPONSE_FROM_JSON_H

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace coal {

struct Character {
  std::string id = {};
  std::string name = {};
  size_t level = {};
  std::string type = {};
  double online = {};
  std::string skin = {};
  std::unordered_map<std::string, std::string> cx = {};
  std::string in = {};
  std::string map = {};
  double x = 0.0;
  double y = 0.0;
  std::string home = {};
  std::optional<std::string> server = {};
  std::optional<std::string> secret = {};
};

using Characters = std::vector<Character>;

struct Server {
  std::string name = {};
  std::string region = {};
  size_t players = 0u;
  std::string key = {};
  std::string addr = {};
  int port = {};
};
using Servers = std::vector<Server>;

struct TutorialStatus {
  size_t step = 0u;
  std::vector<std::string> completed = {};
  bool finished = false;
  bool task = false;
  size_t progress = 0;
};

using CodeElement = std::vector<std::variant<std::string, size_t>>;

using CodeMap = std::unordered_map<std::string, CodeElement>;

struct Reward {
  std::string something;
};

using Rewards = std::vector<Reward>;

struct ServersAndCharactersResponse {
  std::string type = {};
  Servers servers = {};
  Characters characters = {};
  TutorialStatus tutorial = {};
  CodeMap code_list = {};
  int mail = 0;
  Rewards rewards = {};
};

[[nodiscard]] inline auto ReducedFrom(std::string_view json_data) {
  return json_data.substr(1, json_data.size() - 2);
}

}  // namespace coal

#endif  // SERVERS_AND_CHARACTERS_RESPONSE_FROM_JSON_H