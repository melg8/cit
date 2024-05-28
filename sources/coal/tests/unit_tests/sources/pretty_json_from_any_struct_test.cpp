// SPDX-FileCopyrightText: © 2024 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <testing_framework.h>

#include <pretty_json_from_any_struct.h>

#include <string>

namespace coal::test {
struct TestStruct {
  int x = 42;
  std::string field = "test";
};

SCENARIO("pretty json creation from struct") {
  SECTION("custom struct with fields") {
    TestStruct test_struct{};
    const auto json = JsonFrom(test_struct);
    const auto expected =
        "{\n"
        "   \"x\": 42,\n"
        "   \"field\": \"test\"\n"
        "}";
    CHECK(json == expected);
  }
}

}  // namespace coal::test
