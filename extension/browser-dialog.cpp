#if defined(Hiro_BrowserDialog)

struct BrowserDialogWindow {
  Application::Namespace tr{"BrowserDialog"};

  BrowserDialogWindow(BrowserDialog::State& state) : state(state) {}
  auto accept() -> void;
  auto activate() -> void;
  auto change() -> void;
  auto context() -> void;
  auto isFolder(const string& name) -> bool;
  auto isMatch(const string& name) -> bool;
  auto run() -> BrowserDialog::Response;
  auto setPath(string path) -> void;

private:
  Window window;
    VerticalLayout layout{&window};
      HorizontalLayout pathLayout{&layout, Size{~0, 0}, 5};
        LineEdit pathName{&pathLayout, Size{~0, 0}, 0};
        Button pathRefresh{&pathLayout, Size{0, 0}, 0};
        Button pathNew{&pathLayout, Size{0, 0}, 0};
        Button pathHome{&pathLayout, Size{0, 0}, 0};
        Button pathUp{&pathLayout, Size{0, 0}, 0};
      ListView view{&layout, Size{~0, ~0}, 5};
      HorizontalLayout controlLayout{&layout, Size{~0, 0}};
        ComboButton filterList{&controlLayout, Size{0, 0}, 5};
        LineEdit fileName{&controlLayout, Size{~0, 0}, 5};
        ComboButton optionList{&controlLayout, Size{0, 0}, 5};
        Button acceptButton{&controlLayout, Size{80, 0}, 5};
        Button cancelButton{&controlLayout, Size{80, 0}, 5};

  PopupMenu contextMenu;
    MenuItem createAction{&contextMenu};
    MenuItem renameAction{&contextMenu};
    MenuItem removeAction{&contextMenu};

  BrowserDialog::State& state;
  BrowserDialog::Response response;
  vector<vector<string>> filters;
};

//accept button clicked, or enter pressed on file name line edit
//also called by table view activate after special case handling
auto BrowserDialogWindow::accept() -> void {
  auto batched = view.batched();

  if(state.action == "openFile" && batched.size() == 1) {
    string name = batched[0].text();
    if(isFolder(name)) return setPath({state.path, name});
    response.selected.append(string{state.path, name});
  }

  if(state.action == "openFiles" && batched) {
    for(auto item : batched) {
      string name = item.text();
      if(isFolder(name)) {
        response.selected.reset();
        return;
      }
      response.selected.append(string{state.path, name});
    }
  }

  if(state.action == "openFolder" && batched.size() == 1) {
    string name = batched[0].text();
    if(!isMatch(name)) return setPath({state.path, name});
    response.selected.append(string{state.path, name, "/"});
  }

  if(state.action == "openObject" && batched.size() == 1) {
    string name = batched[0].text();
    if(!isMatch(name) && isFolder(name)) return setPath({state.path, name});
    response.selected.append(string{state.path, name, isFolder(name) ? "/" : ""});
  }

  if(state.action == "saveFile") {
    string name = fileName.text();
    if(!name || isFolder(name)) return;
    if(file::exists({state.path, name})) {
      if(MessageDialog("File already exists. Overwrite it?").question() != "Yes") return;
    }
    response.selected.append(string{state.path, name});
  }

  if(state.action == "selectFolder") {
    if(!batched) {
      response.selected.append(state.path);
    } else if(batched.size() == 1) {
      string name = batched[0].text();
      if(isFolder(name)) response.selected.append(string{state.path, name, "/"});
    }
  }

  if(response.selected) window.setModal(false);
}

//table view item double-clicked, or enter pressed on selected table view item
auto BrowserDialogWindow::activate() -> void {
  auto batched = view.batched();

  if(state.action == "saveFile" && batched.size() == 1) {
    string name = batched[0].text();
    if(isFolder(name)) return setPath({state.path, name});
    fileName.setText(name);
  }

  if(state.action == "selectFolder" && batched.size() == 1) {
    string name = batched[0].text();
    if(isFolder(name)) return setPath({state.path, name});
  }

  accept();
}

//table view item changed
auto BrowserDialogWindow::change() -> void {
  auto batched = view.batched();
  if(state.action == "openFile") {
    acceptButton.setEnabled(batched.size() == 1);
  }
  if(state.action == "openFiles") {
    bool enabled = true;
    for(auto item : batched) enabled &= !isFolder(item.text());
    if(batched.size() == 1 && isFolder(batched[0].text())) enabled = true;
    acceptButton.setEnabled(enabled);
  }
  if(state.action == "openFolder") {
    acceptButton.setEnabled(batched.size() == 1);
  }
  if(state.action == "openObject") {
    acceptButton.setEnabled(batched.size() == 1);
  }
  if(state.action == "saveFile") {
    if(batched.size() == 1) {
      string name = batched[0].text();
      if(!isFolder(name)) fileName.setText(name).doChange();
    }
  }
  if(state.action == "selectFolder") {
    acceptButton.setEnabled(!batched || (batched.size() == 1 && isFolder(batched[0].text())));
  }
}

auto BrowserDialogWindow::context() -> void {
  auto batched = view.batched();
  if(!batched) {
    createAction.setVisible(true);
    renameAction.setVisible(false);
    removeAction.setVisible(false);
  } else if(batched.size() == 1) {
    createAction.setVisible(false);
    renameAction.setVisible(true);
    removeAction.setVisible(true);
  } else {
    createAction.setVisible(false);
    renameAction.setVisible(false);
    removeAction.setVisible(true);
  }
  contextMenu.setVisible();
}

auto BrowserDialogWindow::isFolder(const string& name) -> bool {
  return directory::exists({state.path, name});
}

auto BrowserDialogWindow::isMatch(const string& name) -> bool {
  if(auto selectedItem = filterList.selected()) {
    for(auto& filter : filters[selectedItem->offset()]) {
      if(name.match(filter)) return true;
    }
  }
  return false;
}

auto BrowserDialogWindow::run() -> BrowserDialog::Response {
  response = {};

  layout.setPadding(5);
  pathName.onActivate([&] { setPath(pathName.text()); });
  pathRefresh.setBordered(false).setIcon(Icon::Action::Refresh).onActivate([&] { setPath(state.path); });
  pathNew.setBordered(false).setIcon(Icon::Action::NewFolder).onActivate([&] { createAction.doActivate(); });
  pathHome.setBordered(false).setIcon(Icon::Go::Home).onActivate([&] { setPath(Path::user()); });
  pathUp.setBordered(false).setIcon(Icon::Go::Up).onActivate([&] { setPath(Location::dir(state.path)); });
  view.setBatchable(state.action == "openFiles").onActivate([&] { activate(); }).onChange([&] { change(); });
  view.onContext([&] { context(); });
  filterList.setVisible(state.action != "selectFolder").onChange([&] { setPath(state.path); });
  for(auto& filter : state.filters) {
    auto part = filter.split("|", 1L);
    filterList.append(ComboButtonItem().setText(part.left()));
  }
  optionList.setCollapsible().setVisible((bool)state.options).onChange([&] { response.option = optionList.selected().text(); });
  for(auto& option : state.options) {
    optionList.append(ComboButtonItem().setText(option));
  }
  optionList.doChange();  //updates response.option to point to the default (first) option
  fileName.setVisible(state.action == "saveFile").onActivate([&] { accept(); }).onChange([&] {
    auto name = fileName.text();
    acceptButton.setEnabled(name && !isFolder(name));
    fileName.setBackgroundColor(acceptButton.enabled() ? Color{} : Color{255, 224, 224});
  });
  acceptButton.onActivate([&] { accept(); });
  if(state.action.beginsWith("open")) acceptButton.setText(tr("Open"));
  if(state.action.beginsWith("save")) acceptButton.setText(tr("Save"));
  if(state.action.beginsWith("select")) acceptButton.setText(tr("Select"));
  cancelButton.setText(tr("Cancel")).onActivate([&] { window.setModal(false); });

  if(!state.filters) state.filters.append("All|*");
  for(auto& filter : state.filters) {
    auto part = filter.split("|", 1L);
    filters.append(part.right().split(":"));
  }

  createAction.setIcon(Icon::Action::NewFolder).setText("Create Folder ...").onActivate([&] {
    if(auto name = NameDialog()
    .setTitle("Create Folder")
    .setText("Enter a new folder name:")
    .setIcon(Icon::Emblem::Folder)
    .setPlacement(Placement::Center, window)
    .create()
    ) {
      directory::create({state.path, name});
      pathRefresh.doActivate();
    }
  });

  renameAction.setIcon(Icon::Application::TextEditor).setText("Rename ...").onActivate([&] {
    auto batched = view.batched();
    if(batched.size() != 1) return;
    auto name = batched[0].text();
    if(directory::exists({state.path, name})) {
      if(auto rename = NameDialog()
      .setTitle({"Rename ", name})
      .setText("Enter the new folder name:")
      .setIcon(Icon::Emblem::Folder)
      .setPlacement(Placement::Center, window)
      .rename(name)
      ) {
        if(name == rename) return;
        if(!directory::rename({state.path, name}, {state.path, rename})) return (void)MessageDialog()
        .setTitle("Error")
        .setText("Failed to rename folder.")
        .setPlacement(Placement::Center, window)
        .error();
        pathRefresh.doActivate();
      }
    } else if(file::exists({state.path, name})) {
      if(auto rename = NameDialog()
      .setTitle({"Rename ", name})
      .setText("Enter the new file name:")
      .setIcon(Icon::Emblem::File)
      .setPlacement(Placement::Center, window)
      .rename(name)
      ) {
        if(name == rename) return;
        if(!file::rename({state.path, name}, {state.path, rename})) return (void)MessageDialog()
        .setTitle("Error")
        .setText("Failed to rename file.")
        .setPlacement(Placement::Center, window)
        .error();
        pathRefresh.doActivate();
      }
    }
  });

  removeAction.setIcon(Icon::Action::Remove).setText("Delete ...").onActivate([&] {
    auto batched = view.batched();
    if(!batched) return;
    if(MessageDialog()
    .setTitle("Remove Selected")
    .setText({"Are you sure you want to permanently delete the selected item", batched.size() == 1 ? "" : "s", "?"})
    .setPlacement(Placement::Center, window)
    .question() == "No") return;
    for(auto& item : batched) {
      auto name = item.text();
      if(directory::exists({state.path, name})) {
        if(!directory::remove({state.path, name})) {
          if(MessageDialog()
          .setTitle("Warning")
          .setText({"Failed to remove ", name, "\n\nContinue trying to remove remaining items?"})
          .question() == "No") break;
        }
      } else if(file::exists({state.path, name})) {
        if(!file::remove({state.path, name})) {
          if(MessageDialog()
          .setTitle("Warning")
          .setText({"Failed to remove ", name, "\n\nContinue trying to remove remaining items?"})
          .question() == "No") break;
        }
      }
    }
    pathRefresh.doActivate();
  });

  setPath(state.path);

  window.onClose([&] { window.setModal(false); });
  window.setTitle(state.title);
  window.setSize({640, 480});
  window.setPlacement(state.placement, state.relativeTo);
  window.setDismissable();
  window.setVisible();
  view.setFocused();
  Application::processEvents();
  view->resizeColumns();
  window.setModal();
  window.setVisible(false);

  return response;
}

auto BrowserDialogWindow::setPath(string path) -> void {
  path.transform("\\", "/");
  if((path || Path::root() == "/") && !path.endsWith("/")) path.append("/");
  pathName.setText(state.path = path);
  view.reset();
  auto contents = directory::icontents(path);

  for(auto content : contents) {
    bool isFolder = content.endsWith("/");
    if(isFolder) {
      content.trimRight("/", 1L);
      if(state.action == "openFolder" || state.action == "openObject") {
        if(isMatch(content)) continue;
      }
    } else {
      continue;
    }
    view.append(ListViewItem().setText(content).setIcon(Icon::Emblem::Folder));
  }

  for(auto content : contents) {
    bool isFolder = content.endsWith("/");
    if(isFolder) {
      content.trimRight("/", 1L);
      if(state.action != "openFolder" && state.action != "openObject") continue;
    } else {
      if(state.action == "openFolder") continue;
    }
    if(!isMatch(content)) continue;
    view.append(ListViewItem().setText(content).setIcon(isFolder ? (image)Icon::Action::Open : (image)Icon::Emblem::File));
  }

  Application::processEvents();
  view->resizeColumns();  //todo: on Windows, adding items may add vertical scrollbar; this hack corrects column width
  view.setFocused().doChange();
}

//

BrowserDialog::BrowserDialog() {
}

auto BrowserDialog::openFile() -> string {
  state.action = "openFile";
  if(!state.title) state.title = "Open File";
  if(auto result = _run()) return result.left();
  return {};
}

auto BrowserDialog::openFiles() -> vector<string> {
  state.action = "openFiles";
  if(!state.title) state.title = "Open Files";
  if(auto result = _run()) return result;
  return {};
}

auto BrowserDialog::openFolder() -> string {
  state.action = "openFolder";
  if(!state.title) state.title = "Open Folder";
  if(auto result = _run()) return result.left();
  return {};
}

auto BrowserDialog::openObject() -> string {
  state.action = "openObject";
  if(!state.title) state.title = "Open Object";
  if(auto result = _run()) return result.left();
  return {};
}

auto BrowserDialog::option() -> string {
  return response.option;
}

auto BrowserDialog::saveFile() -> string {
  state.action = "saveFile";
  if(!state.title) state.title = "Save File";
  if(auto result = _run()) return result.left();
  return {};
}

auto BrowserDialog::selected() -> vector<string> {
  return response.selected;
}

auto BrowserDialog::selectFolder() -> string {
  state.action = "selectFolder";
  if(!state.title) state.title = "Select Folder";
  if(auto result = _run()) return result.left();
  return {};
}

auto BrowserDialog::setFilters(const vector<string>& filters) -> type& {
  state.filters = filters;
  return *this;
}

auto BrowserDialog::setOptions(const vector<string>& options) -> type& {
  state.options = options;
  return *this;
}

auto BrowserDialog::setPath(const string& path) -> type& {
  state.path = path;
  return *this;
}

auto BrowserDialog::setPlacement(Placement placement, sWindow relativeTo) -> type& {
  state.placement = placement;
  state.relativeTo = relativeTo;
  return *this;
}

auto BrowserDialog::setTitle(const string& title) -> type& {
  state.title = title;
  return *this;
}

auto BrowserDialog::_run() -> vector<string> {
  if(!state.path) state.path = Path::user();
  response = BrowserDialogWindow(state).run();
  return response.selected;
}

#endif
