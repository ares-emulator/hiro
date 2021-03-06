#if defined(Hiro_Canvas)

auto mCanvas::allocate() -> pObject* {
  return new pCanvas(*this);
}

//

auto mCanvas::alignment() const -> Alignment {
  return state.alignment;
}

auto mCanvas::color() const -> Color {
  return state.color;
}

auto mCanvas::data() -> u32* {
  return (u32*)state.icon.data();
}

auto mCanvas::gradient() const -> Gradient {
  return state.gradient;
}

auto mCanvas::icon() const -> multiFactorImage {
  return state.icon;
}

auto mCanvas::setAlignment(Alignment alignment) -> type& {
  state.alignment = alignment;
  signal(setAlignment, alignment);
  return *this;
}

auto mCanvas::setColor(Color color) -> type& {
  state.color = color;
  state.gradient = {};
  state.icon = {};
  signal(setColor, color);
  return *this;
}

auto mCanvas::setGradient(Gradient gradient) -> type& {
  state.color = {};
  state.gradient = gradient;
  state.icon = {};
  signal(setGradient, gradient);
  return *this;
}

auto mCanvas::setIcon(const multiFactorImage& icon) -> type& {
  state.color = {};
  state.gradient = {};
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mCanvas::setSize(Size size) -> type& {
  multiFactorImage icon;
  icon.allocate(size.width(), size.height());
  return setIcon(icon);
}

auto mCanvas::size() const -> Size {
  return {(s32)state.icon.width(), (s32)state.icon.height()};
}

auto mCanvas::update() -> type& {
  signal(update);
  return *this;
}

#endif
