#include "text/FontDescriptor.hpp"

namespace fui {

FontDescriptor::FontDescriptor() {
  weight = FontWeightUndefined;
  width = FontWidthUndefined;
  italic = false;
  monospace = false;
}

FontDescriptor::FontDescriptor(const char* path, const char* postscriptName, const char* family, const char* style,
                               const std::string& lang, FontWeight weight, FontWidth width, bool italic, bool monospace)
: path(path)
, postscriptName(postscriptName)
, family(family)
, style(style)
, lang(lang)
, weight(weight)
, width(width)
, italic(italic)
, monospace(monospace) {}

} // namespace fui
