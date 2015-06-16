#if defined(Hiro_HorizontalScroller)

namespace hiro {

struct pHorizontalScroller : pWidget {
  Declare(HorizontalScroller, Widget)

  auto minimumSize() const -> Size override;
  auto setLength(unsigned length) -> void;
  auto setPosition(unsigned position) -> void;

  auto onChange(WPARAM wparam) -> void;
};

}

#endif
