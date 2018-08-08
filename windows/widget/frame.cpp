#if defined(Hiro_Frame)

namespace hiro {

auto pFrame::construct() -> void {
  hwnd = CreateWindow(L"BUTTON", L"",
    WS_CHILD | BS_GROUPBOX,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  pWidget::construct();
  setText(state().text);
}

auto pFrame::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pFrame::append(sSizable sizable) -> void {
}

auto pFrame::remove(sSizable sizable) -> void {
}

auto pFrame::setEnabled(bool enabled) -> void {
  if(auto& sizable = state().sizable) sizable->setEnabled(sizable->enabled());
  pWidget::setEnabled(enabled);
}

auto pFrame::setGeometry(Geometry geometry) -> void {
  bool empty = !state().text;
  auto size = pFont::size(hfont, state().text);
  pWidget::setGeometry({
    geometry.x(),
    geometry.y() - (empty ? size.height() / 2 : 0),
    geometry.width(),
    geometry.height() + (empty ? size.height() / 2 : 0)
  });
  if(auto& sizable = state().sizable) {
    if(empty) size.setHeight(1);
    sizable->setGeometry({
      geometry.x() + 1,
      geometry.y() + size.height(),
      geometry.width() - 2,
      geometry.height() - (size.height() + 2)
    });
  }
}

auto pFrame::setText(const string& text) -> void {
  SetWindowText(hwnd, utf16_t(text));
}

auto pFrame::setVisible(bool visible) -> void {
  if(auto& sizable = state().sizable) sizable->setVisible(sizable->visible());
  pWidget::setVisible(visible);
}

}

#endif
