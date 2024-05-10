// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#ifndef SERVERS_AND_CHARACTERS_RESPONSE_PARSER_H
#define SERVERS_AND_CHARACTERS_RESPONSE_PARSER_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace coal {

struct Character {};

using Characters = std::vector<Character>;

struct Server {};
using Servers = std::vector<Server>;

struct TutorialStatus {};
struct Code {};

struct Reward {};

using Rewards = std::vector<Reward>;

struct ServersAndCharactersResponse {
  std::string type = {};
  Servers servers = {};
  Characters characters = {};
  TutorialStatus tutorial = {};

  std::unordered_map<uint64_t, Code> code_list = {};
  int mail = 0;
  Rewards rewards = {};
};

}  // namespace coal

#endif  // SERVERS_AND_CHARACTERS_RESPONSE_PARSER_H