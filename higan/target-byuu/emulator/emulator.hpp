struct Emulator {
  static auto construct() -> void;
  auto load(const string& name, const vector<uint8_t>& data) -> void;
  virtual auto load() -> void = 0;
  virtual auto load(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> = 0;
  virtual auto input(higan::Node::Input) -> void = 0;

  struct Port {
    string name;
  };

  struct Game {
    string name;
    string manifest;
    vector<uint8_t> data;
  };

  shared_pointer<higan::Interface> interface;
  string name;
  string abbreviation;
  vector<string> extensions;
  vector<Port> ports;

  higan::Node::Object root;
  Game game;

  struct Settings {
    string gamePath;
  } settings;
};

struct FamicomEmulator : Emulator {
  FamicomEmulator();
  auto load() -> void;
  auto load(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file>;
  auto input(higan::Node::Input) -> void;
};

extern vector<shared_pointer<Emulator>> emulators;
extern shared_pointer<Emulator> emulator;
