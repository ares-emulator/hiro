#if defined(Hiro_AboutDialog)

struct AboutDialog {
  using type = AboutDialog;

  auto setAlignment(Alignment = Alignment::Center) -> type&;
  auto setAlignment(sWindow relativeTo, Alignment = Alignment::Center) -> type&;
  auto setCopyright(const string& copyright = "", const string& uri = "") -> type&;
  auto setDescription(const string& description = "") -> type&;
  auto setLicense(const string& license = "", const string& uri = "") -> type&;
  auto setLogo(const multiFactorImage& logo = {}) -> type&;
  auto setName(const string& name = "") -> type&;
  auto setVersion(const string& version = "") -> type&;
  auto setWebsite(const string& website = "", const string& uri = "") -> type&;
  auto show() -> void;

private:
  struct State {
    Alignment alignment = Alignment::Center;
    string copyright;
    string copyrightURI;
    string description;
    string license;
    string licenseURI;
    multiFactorImage logo;
    string name;
    sWindow relativeTo;
    string version;
    string website;
    string websiteURI;
  } state;
};

#endif
