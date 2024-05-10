// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <json_test_data.h>

namespace coal::test {

const char* kServersAndCharactersResponse = R"([
  {
    "type": "servers_and_characters",
    "servers": [
      {
        "name": "I",
        "region": "EU",
        "players": 35,
        "key": "EUI",
        "addr": "eud1.adventure.land",
        "port": 2053
      },
      {
        "name": "II",
        "region": "EU",
        "players": 20,
        "key": "EUII",
        "addr": "eud1.adventure.land",
        "port": 2083
      },
      {
        "name": "PVP",
        "region": "EU",
        "players": 2,
        "key": "EUPVP",
        "addr": "eud1.adventure.land",
        "port": 2087
      },
      {
        "name": "I",
        "region": "US",
        "players": 24,
        "key": "USI",
        "addr": "usd1.adventure.land",
        "port": 2053
      },
      {
        "name": "II",
        "region": "US",
        "players": 36,
        "key": "USII",
        "addr": "usd1.adventure.land",
        "port": 2083
      },
      {
        "name": "III",
        "region": "US",
        "players": 12,
        "key": "USIII",
        "addr": "usd1.adventure.land",
        "port": 2096
      },
      {
        "name": "PVP",
        "region": "US",
        "players": 2,
        "key": "USPVP",
        "addr": "usd1.adventure.land",
        "port": 2087
      },
      {
        "name": "I",
        "region": "ASIA",
        "players": 14,
        "key": "ASIAI",
        "addr": "eud1.adventure.land",
        "port": 8443
      }
    ],
    "characters": [
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
      },
      {
        "id": "6321827088957440",
        "name": "That",
        "level": 86,
        "type": "rogue",
        "online": 0,
        "skin": "sarmor1h",
        "cx": {
          "hair": "hairdo210",
          "head": "fmakeup01",
          "chin": "beard112"
        },
        "in": "bank",
        "map": "bank",
        "x": -4.217169409531738,
        "y": -142.92596804470654,
        "home": "EUII"
      },
      {
        "id": "5812528843587584",
        "name": "Abuse",
        "level": 65,
        "type": "merchant",
        "online": 0,
        "skin": "marmor12a",
        "cx": {
          "hair": "hairdo520",
          "head": "fmakeup01",
          "hat": "hat407"
        },
        "in": "bank",
        "map": "bank",
        "x": 0,
        "y": -37,
        "home": "EUI"
      },
      {
        "id": "5686048096518144",
        "name": "Dory",
        "level": 86,
        "type": "priest",
        "online": 0,
        "skin": "mbody5e",
        "cx": {
          "hair": "hairdo402",
          "upper": "marmor12e"
        },
        "in": "desertland",
        "map": "desertland",
        "x": -8.928375622597855,
        "y": -1907.9885273894286,
        "home": "EUII"
      },
      {
        "id": "6513169006067712",
        "name": "Melira",
        "level": 67,
        "type": "warrior",
        "online": 0,
        "skin": "sarmor2d",
        "cx": {
          "hair": "hairdo105",
          "head": "fmakeup01"
        },
        "in": "goobrawl",
        "map": "goobrawl",
        "x": 28.931406480513523,
        "y": 0.5371930448197872,
        "home": "EUII"
      },
      {
        "id": "5841468299673600",
        "name": "Mequ",
        "level": 68,
        "type": "ranger",
        "online": 0,
        "skin": "marmor5a",
        "cx": {
          "hair": "hairdo106",
          "head": "mmakeup00",
          "makeup": "facemakeup02",
          "upper": "marmor5a"
        },
        "in": "desertland",
        "map": "desertland",
        "x": 80.0000001,
        "y": -1205.5875204928566,
        "home": "EUPVP"
      },
      {
        "id": "5383673741836288",
        "name": "Combi",
        "level": 40,
        "type": "merchant",
        "online": 0,
        "skin": "marmor12b",
        "cx": {
          "hair": "hairdo520",
          "head": "fmakeup01"
        },
        "in": "bank",
        "map": "bank",
        "x": 0,
        "y": -37,
        "home": "EUII"
      },
      {
        "id": "6141058542469120",
        "name": "Collect",
        "level": 40,
        "type": "merchant",
        "online": 0,
        "skin": "marmor12b",
        "cx": {
          "hair": "hairdo520",
          "head": "fmakeup01"
        },
        "in": "main",
        "map": "main",
        "x": 168,
        "y": -134,
        "home": "EUII"
      }
    ],
    "tutorial": {
      "step": 8,
      "completed": [],
      "finished": true,
      "task": false,
      "progress": 100
    },
    "code_list": {
      "1": [
        "AbuseCode",
        5
      ],
      "2": [
        "ThisCode",
        6
      ],
      "3": [
        "ThatCode",
        3
      ],
      "4": [
        "DoryCode",
        5
      ],
      "5812528843587584": [
        "Abuse",
        3220
      ],
      "5841468299673600": [
        "Mequ",
        3258
      ],
      "6321827088957440": [
        "That",
        1931
      ],
      "6513169006067712": [
        "Melira",
        3123
      ],
      "5686048096518144": [
        "Dory",
        3060
      ],
      "6223051783405568": [
        "This",
        1870
      ]
    },
    "mail": 0,
    "rewards": []
  }
])";

}  // namespace coal::test