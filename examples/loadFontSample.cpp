#include <algorithm>
#include <cstring>
#include <memory>
#include <fui.hpp>
using namespace fui;

namespace std {
std::ostream& operator<<(std::ostream& os, const fui::FontDescriptor& desc) {
    os << "" << desc.family << "\t" << desc.italic << " " << desc.monospace << " " << desc.path << "\t"
              << desc.postscriptName << "\t" << desc.style << "\t" << desc.weight << "\t" << desc.width << '\n';
    os << desc.lang;
    return os;            
}
}

int main() {
  Logger logger;
  auto fontDescriptors = getAvailableFonts();
  // sort by family
  std::sort(fontDescriptors.begin(), fontDescriptors.end(),
            [](FontDescriptor* a, FontDescriptor* b) { return a->family < b->family; });

  // print result
  LOGD << "Available fonts:";
  for (auto&& desc : fontDescriptors) {
    LOGD << *desc;
  }

  FontDescriptor requestFont;
  requestFont.lang = "zh-hk";
  auto resultFont = std::unique_ptr<FontDescriptor>(findFont(&requestFont));

  LOGD << *resultFont; 

  return 0;
}
