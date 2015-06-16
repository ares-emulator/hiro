#if defined(Hiro_ListView)

auto mListViewColumn::allocate() -> pObject* {
  return new pListViewColumn(*this);
}

//

auto mListViewColumn::active() const -> bool {
  if(auto listView = parentListView()) return listView->state.activeColumn == offset();
  return false;
}

auto mListViewColumn::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mListViewColumn::editable() const -> bool {
  return state.editable;
}

auto mListViewColumn::expandable() const -> bool {
  return state.expandable;
}

auto mListViewColumn::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mListViewColumn::horizontalAlignment() const -> double {
  return state.horizontalAlignment;
}

auto mListViewColumn::icon() const -> image {
  return state.icon;
}

auto mListViewColumn::remove() -> type& {
  if(auto listView = parentListView()) listView->remove(*this);
  return *this;
}

auto mListViewColumn::resizable() const -> bool {
  return state.resizable;
}

auto mListViewColumn::setActive() -> type& {
  if(auto listView = parentListView()) listView->state.activeColumn = offset();
  signal(setActive);
  return *this;
}

auto mListViewColumn::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mListViewColumn::setEditable(bool editable) -> type& {
  state.editable = editable;
  signal(setEditable, editable);
  return *this;
}

auto mListViewColumn::setExpandable(bool expandable) -> type& {
  state.expandable = expandable;
  signal(setExpandable, expandable);
  return *this;
}

auto mListViewColumn::setFont(const string& font) -> type& {
  state.font = font;
  signal(setFont, this->font(true));
  return *this;
}

auto mListViewColumn::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mListViewColumn::setHorizontalAlignment(double alignment) -> type& {
  alignment = max(0.0, min(1.0, alignment));
  state.horizontalAlignment = alignment;
  signal(setHorizontalAlignment, alignment);
  return *this;
}

auto mListViewColumn::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mListViewColumn::setResizable(bool resizable) -> type& {
  state.resizable = resizable;
  signal(setResizable, resizable);
  return *this;
}

auto mListViewColumn::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mListViewColumn::setVerticalAlignment(double alignment) -> type& {
  alignment = max(0.0, min(1.0, alignment));
  state.verticalAlignment = alignment;
  signal(setVerticalAlignment, alignment);
  return *this;
}

auto mListViewColumn::setVisible(bool visible) -> type& {
  state.visible = visible;
  signal(setVisible, visible);
  return *this;
}

auto mListViewColumn::setWidth(signed width) -> type& {
  state.width = max(0, width);
  signal(setWidth, width);
  return *this;
}

auto mListViewColumn::text() const -> string {
  return state.text;
}

auto mListViewColumn::verticalAlignment() const -> double {
  return state.verticalAlignment;
}

auto mListViewColumn::width() const -> signed {
  return state.width;
}

#endif
