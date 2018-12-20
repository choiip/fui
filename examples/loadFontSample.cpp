#include <algorithm>
#include <iostream>
#include <cstring>
#include "text/FontDescriptor.hpp"
using namespace fui;

int main() {
  auto fontDescriptors = getAvailableFonts();
  // sort by family
  std::sort(fontDescriptors.begin(), fontDescriptors.end(),
            [](FontDescriptor* a, FontDescriptor* b) { return a->family < b->family; });

  // print result
  std::cout << "Available fonts:\n";
  for (auto&& desc : fontDescriptors) {
    std::cout << "" << desc->family << "\t" << desc->italic << " " << desc->monospace << " " << desc->path << "\t"
              << desc->postscriptName << "\t" << desc->style << "\t" << desc->weight << "\t" << desc->width << '\n';
  }
  return 0;
}
