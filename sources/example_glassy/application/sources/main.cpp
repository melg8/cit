// SPDX-FileCopyrightText: © 2022 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <iostream>

#include <big_num.h>

#include <QApplication>
#include <QMainWindow>

int main(int argc, char **argv) {
  QApplication app (argc, argv);


  QMainWindow main_window{};
  auto x = new int{2};

  std::cout << glassy::Sum(*x, 2) << '\n';
  std::cout.flush();

  main_window.show();
  return app.exec();
}
