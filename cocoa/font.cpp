#if defined(Hiro_Font)

namespace hiro {

auto pFont::size(const Font& font, const string& text) -> Size {
  if(NSFont* nsFont = create(font)) {
    return size(nsFont, text);
  }
  return {0, 0};
}

auto pFont::size(NSFont* font, const string& text) -> Size {
  NSString* cocoaText = [NSString stringWithUTF8String:text];
  NSDictionary* fontAttributes = [NSDictionary dictionaryWithObjectsAndKeys:font, NSFontAttributeName, nil];
  NSSize size = [cocoaText sizeWithAttributes:fontAttributes];
  return {(s32)ceil(size.width), (s32)ceil(size.height)};
}

auto pFont::family(const string& family) -> string {
  if(family == Font::Serif) return "Baskerville";
  if(family == Font::Mono ) return "Menlo";
  return [[[NSFont systemFontOfSize:0] familyName] UTF8String];
}

auto pFont::create(const Font& font) -> NSFont* {
  auto fontName = family(font.family());
  NSString* family = [NSString stringWithUTF8String:fontName];
  CGFloat size = Application::scale(font.size() ? font.size() : 8);
  NSFontTraitMask traits = 0;

  if(font.bold()) traits |= NSBoldFontMask;
  if(font.italic()) traits |= NSItalicFontMask;

//note: below properties are not exposed by hiro::Font; traits are saved here for possible future use
//if(font.narrow()) traits |= NSNarrowFontMask;
//if(font.expanded()) traits |= NSExpandedFontMask;
//if(font.condensed()) traits |= NSCondensedFontMask;
//if(font.smallCaps()) traits |= NSSmallCapsFontMask;

  size *= 1.5;  //scale to point sizes (for consistency with other operating systems)
  return [[NSFontManager sharedFontManager] fontWithFamily:family traits:traits weight:5 size:size];
}

}

#endif
