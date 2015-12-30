#if defined(Hiro_CheckButton)

@implementation CocoaCheckButton : NSButton

-(id) initWith:(hiro::mCheckButton&)checkButtonReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    checkButton = &checkButtonReference;

    [self setTarget:self];
    [self setAction:@selector(activate:)];
    [self setBezelStyle:NSRegularSquareBezelStyle];
    [self setButtonType:NSOnOffButton];
  }
  return self;
}

-(IBAction) activate:(id)sender {
  checkButton->state.checked = [self state] != NSOffState;
  checkButton->doToggle();
}

@end

namespace hiro {

auto pCheckButton::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaCheckButton = [[CocoaCheckButton alloc] initWith:self()];
    pWidget::construct();

    setBordered(state().bordered);
    setChecked(state().checked);
    setImage(state().image);
    setOrientation(state().orientation);
    setText(state().text);
  }
}

auto pCheckButton::destruct() -> void {
  @autoreleasepool {
    [cocoaView release];
  }
}

auto pCheckButton::minimumSize() const -> Size {
  Size size = pFont::size(self().font(true), state().text);

  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(size.width() + state().image.width());
    size.setHeight(max(size.height(), state().image.height()));
  }

  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(size.width(), state().image.width()));
    size.setHeight(size.height() + state().image.height());
  }

  return {size.width() + 20, size.height() + 4};
}

auto pCheckButton::setBordered(bool bordered) -> void {
}

auto pCheckButton::setChecked(bool checked) -> void {
  @autoreleasepool {
    [cocoaView setState:checked ? NSOnState : NSOffState];
  }
}

auto pCheckButton::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry({
    geometry.x() - 2, geometry.y() - 2,
    geometry.width() + 4, geometry.height() + 4
  });
}

auto pCheckButton::setImage(const Image& image) -> void {
  @autoreleasepool {
    if(!image) {
      [cocoaView setImage:nil];
      return;
    }

    [cocoaView setImage:NSMakeImage(image)];
  }
}

auto pCheckButton::setOrientation(Orientation orientation) -> void {
  @autoreleasepool {
    if(orientation == Orientation::Horizontal) [cocoaView setImagePosition:NSImageLeft];
    if(orientation == Orientation::Vertical  ) [cocoaView setImagePosition:NSImageAbove];
  }
}

auto pCheckButton::setText(const string& text) -> void {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

}

#endif
