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

const char* kLocalServersAndCharactersResponse = R"(
   {
      "tutorial": {
         "completed": [],
         "progress": 100,
         "step": 0,
         "task": false
      },
      "code_list": {
         "6195934853595136": [
            "Hold",
            83
         ],
         "1": [
            "Hold",
            94
         ],
         "2": [
            "Lucky",
            221
         ],
         "3": [
            "Melok",
            129
         ],
         "4": [
            "Orca",
            155
         ],
         "4929297458397184": [
            "Orca",
            187
         ],
         "5351509923463168": [
            "Lucky",
            301
         ],
         "5773722388529152": [
            "Melok",
            225
         ]
      },
      "type": "servers_and_characters",
      "mail": 0,
      "characters": [
         {
            "home": "EUI",
            "secret": "G6pHWhxVwkkmNxelpxnVtg6T",
            "server": "EUI",
            "in": "desertland",
            "name": "Orca",
            "cx": {
               "hair": "hairdo106",
               "head": "makeup117"
            },
            "id": "4929297458397184",
            "skin": "marmor5a",
            "type": "ranger",
            "online": 202707.848,
            "level": 77,
            "map": "desertland",
            "y": 119.10026085158528,
            "x": -818.5618815881277
         },
         {
            "home": "EUI",
            "secret": "T0gLopJzAfvAWlTBuxWdso2q",
            "server": "EUI",
            "in": "desertland",
            "name": "Lucky",
            "cx": {
               "hair": "hairdo106",
               "head": "makeup117"
            },
            "id": "5351509923463168",
            "skin": "marmor5a",
            "type": "ranger",
            "online": 202685.56100000002,
            "level": 77,
            "map": "desertland",
            "y": 156.51512233625692,
            "x": -808.3772770917886
         },
         {
            "home": "EUI",
            "secret": "pHFX34liBFzRvTyTbZozTnDN",
            "server": "EUI",
            "in": "desertland",
            "name": "Melok",
            "cx": {
               "hair": "hairdo106",
               "head": "makeup117"
            },
            "id": "5773722388529152",
            "skin": "marmor5a",
            "type": "ranger",
            "online": 154724.986,
            "level": 77,
            "map": "desertland",
            "y": 173.69869051045737,
            "x": -836.7974770928782
         },
         {
            "home": "EUI",
            "secret": "RQ1ody4o8ptnppT3KnJ57XnV",
            "server": "EUI",
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
            "online": 2768.357,
            "level": 60,
            "map": "bank",
            "y": -26.0000001,
            "x": -2.4444444444444446
         }
      ],
      "servers": [
         {
            "region": "EU",
            "players": 4,
            "addr": "127.0.0.1",
            "name": "I",
            "port": 8022,
            "key": "EUI"
         }
      ],
      "rewards": []
   }
]
)";

}  // namespace coal::test
