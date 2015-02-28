namespace hiro {

struct pKeyboard {
  static auto poll() -> vector<bool>;
  static auto pressed(unsigned code) -> bool;

  static auto _pressed(char* state, uint16_t code) -> bool;
  static auto _translate(unsigned code) -> signed;

  static auto initialize() -> void;
};

}
