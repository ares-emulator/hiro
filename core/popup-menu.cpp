#if defined(Hiro_PopupMenu)

auto mPopupMenu::allocate() -> pObject* {
  return new pPopupMenu(*this);
}

auto mPopupMenu::destruct() -> void {
  for(auto& action : state.actions) action->destruct();
  mObject::destruct();
}

//

auto mPopupMenu::action(unsigned position) const -> sAction {
  if(position >= actions()) throw;
  return state.actions[position];
}

auto mPopupMenu::actions() const -> unsigned {
  return state.actions.size();
}

auto mPopupMenu::append(sAction action) -> type& {
  state.actions.append(action);
  action->setParent(this, actions() - 1);
  signal(append, action);
  return *this;
}

auto mPopupMenu::remove(sAction action) -> type& {
  signed offset = action->offset();
  signal(remove, action);
  state.actions.remove(offset);
  for(auto n : range(offset, actions())) {
    state.actions[n]->offset(-1);
  }
  action->setParent();
  return *this;
}

auto mPopupMenu::reset() -> type& {
  while(state.actions) remove(state.actions.last());
  return *this;
}

auto mPopupMenu::setVisible(bool visible) -> type& {
  signal(setVisible, visible);
  return *this;
}

#endif
