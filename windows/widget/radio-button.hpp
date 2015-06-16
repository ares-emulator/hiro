#if defined(Hiro_RadioButton)

namespace hiro {

struct pRadioButton : pWidget {
  Declare(RadioButton, Widget)

  auto minimumSize() -> Size;
  auto setBordered(bool bordered) -> void;
  auto setChecked() -> void;
  auto setGroup(sGroup group) -> void override;
  auto setIcon(const image& icon) -> void;
  auto setOrientation(Orientation orientation) -> void;
  auto setText(const string& text) -> void;

  auto onActivate() -> void;

  auto _setState() -> void;

  HBITMAP hbitmap = 0;
  HIMAGELIST himagelist = 0;
};

}

#endif
