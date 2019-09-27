#include "text/FontDescriptor.hpp"
using namespace fui;

std::vector<FontDescriptor> webFontDescriptors;

ResultSet getAvailableFonts() {
  ResultSet res;
  return res;
}
ResultSet findFonts(FontDescriptor*) {
  ResultSet res;
  return res;
}
FontDescriptor* findFont(FontDescriptor* request) {
  for (auto&& fd : webFontDescriptors) {

    if (fd.lang == request->lang && fd.style == request->style) { return new FontDescriptor(fd); }
  }
  return nullptr;
}
FontDescriptor* substituteFont(char*, char*) { return nullptr; }

void setupWebFonts(const std::vector<FontDescriptor>& fontDescriptors) { webFontDescriptors = fontDescriptors; }
