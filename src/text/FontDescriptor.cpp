#include "text/FontDescriptor.hpp"

namespace fui {

FontDescriptor::FontDescriptor() {
  weight = FontWeightUndefined;
  width = FontWidthUndefined;
  italic = false;
  monospace = false;
}

FontDescriptor::FontDescriptor(const char *path, const char *postscriptName, const char *family, const char *style, 
                FontWeight weight, FontWidth width, bool italic, bool monospace)
: path(path)
, postscriptName(postscriptName)
, family(family)
, style(style)
, weight(weight)
, width(width)
, italic(italic)
, monospace(monospace)
{}

FontDescriptor::FontDescriptor(FontDescriptor *desc)
: path(desc->path)
, postscriptName(desc->postscriptName)
, family(desc->family)
, style(desc->style)
, weight(desc->weight)
, width(desc->width)
, italic(desc->italic)
, monospace(desc->monospace)
{}

}
