#include "text/FontDescriptor.hpp"
using namespace fui;

ResultSet getAvailableFonts() { ResultSet res; return res; }
ResultSet findFonts(FontDescriptor *) { ResultSet res; return res; }
FontDescriptor *findFont(FontDescriptor *) { return nullptr; }
FontDescriptor *substituteFont(char *, char *) { return nullptr; }
