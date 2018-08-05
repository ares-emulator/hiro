#if defined(Hiro_Application)
struct Application {
  Application() = delete;

  static auto doMain() -> void;
  static auto font() -> Font;
  static auto kill() -> void;
  static auto locale() -> Locale&;
  static auto modal() -> bool;
  static auto name() -> string;
  static auto onMain(const function<void ()>& callback = {}) -> void;
  static auto run() -> void;
  static auto scale() -> float;
  static auto scale(float value) -> float;
  static auto pendingEvents() -> bool;
  static auto processEvents() -> void;
  static auto quit() -> void;
  static auto screenSaver() -> bool;
  static auto setFont(const Font& font = {}) -> void;
  static auto setName(const string& name = "") -> void;
  static auto setScale(float scale = 1.0) -> void;
  static auto setScreenSaver(bool screenSaver = true) -> void;
  static auto unscale(float value) -> float;

  struct Windows {
    static auto doModalChange(bool modal) -> void;
    static auto onModalChange(const function<void (bool)>& callback = {}) -> void;
  };

  struct Cocoa {
    static auto doAbout() -> void;
    static auto doActivate() -> void;
    static auto doPreferences() -> void;
    static auto doQuit() -> void;
    static auto onAbout(const function<void ()>& callback = {}) -> void;
    static auto onActivate(const function<void ()>& callback = {}) -> void;
    static auto onPreferences(const function<void ()>& callback = {}) -> void;
    static auto onQuit(const function<void ()>& callback = {}) -> void;
  };

  struct Namespace : Locale::Namespace {
    Namespace(const string& value) : Locale::Namespace(Application::locale(), value) {}
  };

//private:
  struct State {
    Font font;
    Locale locale;
    int modal = 0;
    string name;
    function<void ()> onMain;
    bool quit = false;
    float scale = 1.0;
    bool screenSaver = true;

    struct Windows {
      function<void (bool)> onModalChange;
      function<bool ()> onScreenSaver;
    } windows;

    struct Cocoa {
      function<void ()> onAbout;
      function<void ()> onActivate;
      function<void ()> onPreferences;
      function<void ()> onQuit;
    } cocoa;
  };

  static auto initialize() -> void;
  static auto state() -> State&;
};
#endif
