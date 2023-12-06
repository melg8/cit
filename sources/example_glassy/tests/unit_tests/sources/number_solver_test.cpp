// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <iostream>
#include <string>
#include <vector>

#include <common_macro.h>
#include <testing_framework.h>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

namespace glassy {

using SingleValue = uint8_t;
using DisplayLine = std::vector<SingleValue>;
using DisplayBuffer = std::vector<std::vector<SingleValue>>;
using Symbol = char;
using Symbols = std::string;
using LineIndex = uint8_t;

struct Pos {
  uint8_t x = 0;
  uint8_t y = 0;
};

enum struct Direction {
  kHorizontal = 0,
  kVertical = 1,
  kDiagonalDown = 2,
  kDiagonalUp = 3
};

struct Line {
  Pos head_pos = {};
  Direction direction = Direction::kHorizontal;
};

[[nodiscard]] FORCEINLINE Line LineFrom(LineIndex line_index) noexcept {
  switch (line_index) {
    default:
    case 0:
      return Line{{1, 0}, Direction::kHorizontal};
    case 1:
      return Line{{1 + 5, 1}, Direction::kVertical};
    case 2:
      return Line{{1 + 5, 1 + 5 + 1}, Direction::kVertical};
    case 3:
      return Line{{1, 1 + 5 + 1 + 5}, Direction::kVertical};
    case 4:
      return Line{{0, 1 + 5 + 1}, Direction::kVertical};
    case 5:
      return Line{{0, 1}, Direction::kVertical};
  }
}

[[nodiscard]] FORCEINLINE DisplayBuffer SingleSymbolBuffer(uint8_t width,
                                                           uint8_t height) {
  return DisplayBuffer{height, DisplayLine(width, SingleValue{0})};
}

/// Precondition DisplayBuffer must be already proper size.
FORCEINLINE void DrawLineToBuffer(Line line, DisplayBuffer& buffer) noexcept {
  const auto head_pos = line.head_pos;
  for (uint8_t i = 0; i < 5; ++i) {
    if (line.direction == Direction::kHorizontal) {
      buffer[head_pos.y][head_pos.x + i] = 1;
    } else if (line.direction == Direction::kVertical) {
      buffer[head_pos.y + i][head_pos.x] = 1;
    }
  }
}

[[nodiscard]] FORCEINLINE Symbol SymbolFrom(SingleValue value) noexcept {
  switch (value) {
    case 0:
      return '-';
    case 1:
      return '#';
    default:
      return ' ';
  }
}

[[nodiscard]] FORCEINLINE Symbols
SymbolsFrom(const DisplayLine& display_line) noexcept {
  return display_line | ranges::views::transform(SymbolFrom) |
         ranges::to<std::string>();
}

[[nodiscard]] FORCEINLINE Symbols
SymbolsFromBuffer(const DisplayBuffer& display_buffer) noexcept {
  return display_buffer | ranges::views::transform(SymbolsFrom) |
         ranges::views::join('\n') | ranges::to<std::string>();
}
}  // namespace glassy

namespace glassy::test {

SCENARIO("Conversion of values") {
  SECTION("get proper symbol from single value") {
    using Tests = std::vector<std::pair<SingleValue, Symbol>>;
    auto tests = Tests{{0, '-'}, {1, '#'}, {127, ' '}};
    std::ranges::for_each(
        tests,
        [&](auto test) { CHECK(SymbolFrom(test.first) == test.second); });
  }
}

SCENARIO("Conversion of display lines") {
  SECTION("get proper symbols from display line") {
    using Tests = std::vector<std::pair<DisplayLine, Symbols>>;
    auto tests = Tests{{{}, ""}, {{0}, "-"}, {{0, 0, 1, 0, 0}, "--#--"}};

    std::ranges::for_each(tests, [&](auto test) {
      CHECK(SymbolsFrom(test.first) == test.second);
    });
  }
}

SCENARIO("Conversion of display buffer") {
  SECTION("get proper symbols from display buffer") {
    using Tests = std::vector<std::pair<DisplayBuffer, Symbols>>;
    auto tests = Tests{
        {{{1, 0, 1, 0, 1}, {0, 1, 0, 1, 0}}, "#-#-#\n-#-#-"},
        {{{1}, {0}, {1}, {0}, {1}, {0}}, "#\n-\n#\n-\n#\n-"},
    };
    std::ranges::for_each(tests, [&](auto test) {
      CHECK(SymbolsFromBuffer(test.first) == test.second);
    });
  }
}

SCENARIO("Display buffer initialization") {
  SECTION("initialized buffer have right dimensions") {
    const auto width = 8;
    const auto height = 14;
    const auto buffer = SingleSymbolBuffer(width, height);
    CHECK(buffer.size() == height);
    CHECK(buffer[0].size() == width);
  }
}

SCENARIO("Drawing lines onto buffer display buffer") {
  static constexpr const char* const kSingleLineBuffer =
      "-#####--\n"
      "--------\n"
      "--------\n"
      "--------\n"
      "--------\n"
      "--------\n"
      "--------\n"
      "--------\n"
      "--------\n"
      "--------\n"
      "--------\n"
      "--------\n"
      "--------\n"
      "--------";

  SECTION("get properly filled buffer") {
    auto display_buffer = SingleSymbolBuffer(8, 14);
    const auto line = LineFrom(0);
    DrawLineToBuffer(line, display_buffer);
    CHECK(SymbolsFromBuffer(display_buffer) == kSingleLineBuffer);
  }
}

}  // namespace glassy::test
