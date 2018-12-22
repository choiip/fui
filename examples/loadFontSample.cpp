#include <algorithm>
#include <iostream>
#include <cstring>
#include <memory>
#include "text/FontDescriptor.hpp"
using namespace fui;

std::ostream& operator<<(std::ostream& os, const FontDescriptor& desc) {
    os << "" << desc.family << "\t" << desc.italic << " " << desc.monospace << " " << desc.path << "\t"
              << desc.postscriptName << "\t" << desc.style << "\t" << desc.weight << "\t" << desc.width << '\n';
    os << desc.lang << '\n';
    return os;            
}
int main() {
  auto fontDescriptors = getAvailableFonts();
  // sort by family
  std::sort(fontDescriptors.begin(), fontDescriptors.end(),
            [](FontDescriptor* a, FontDescriptor* b) { return a->family < b->family; });

  // print result
  std::cout << "Available fonts:\n";
  for (auto&& desc : fontDescriptors) {
    std::cout << *desc << '\n';
  }

  FontDescriptor requestFont;
  requestFont.lang = "zh-hk";
  auto resultFont = std::unique_ptr<FontDescriptor>(findFont(&requestFont));

  std::cout << *resultFont << '\n'; 

  return 0;
}
