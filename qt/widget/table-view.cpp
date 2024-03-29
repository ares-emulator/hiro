#if defined(Hiro_TableView)

namespace hiro {

auto pTableView::construct() -> void {
  qtWidget = qtTableView = new QtTableView(*this);
  qtTableView->setAllColumnsShowFocus(true);
  qtTableView->setContextMenuPolicy(Qt::CustomContextMenu);
  qtTableView->setRootIsDecorated(false);
  qtTableView->setHeaderHidden(true);
  #if HIRO_QT==4
  qtTableView->header()->setMovable(false);
  #elif HIRO_QT==5
  qtTableView->header()->setSectionsMovable(false);
  #endif

  qtTableViewDelegate = new QtTableViewDelegate(*this);
  qtTableView->setItemDelegate(qtTableViewDelegate);

  qtTableView->connect(qtTableView, SIGNAL(itemActivated(QTreeWidgetItem*, int)), SLOT(onActivate(QTreeWidgetItem*, int)));
  qtTableView->connect(qtTableView, SIGNAL(itemSelectionChanged()), SLOT(onChange()));
  qtTableView->connect(qtTableView, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(onContext(const QPoint&)));
  qtTableView->connect(qtTableView->header(), SIGNAL(sectionClicked(int)), SLOT(onSort(int)));
  qtTableView->connect(qtTableView, SIGNAL(itemChanged(QTreeWidgetItem*, int)), SLOT(onToggle(QTreeWidgetItem*, int)));

  setBackgroundColor(state().backgroundColor);
  setBatchable(state().batchable);
  setBordered(state().bordered);
  setForegroundColor(state().foregroundColor);
  setHeadered(state().headered);
  setSortable(state().sortable);

  pWidget::construct();
}

auto pTableView::destruct() -> void {
if(Application::state().quit) return;  //TODO: hack
  delete qtTableViewDelegate;
  delete qtTableView;
  qtWidget = qtTableView = nullptr;
  qtTableViewDelegate = nullptr;
}

auto pTableView::append(sTableViewColumn column) -> void {
}

auto pTableView::append(sTableViewItem item) -> void {
  lock();
  if(auto self = item->self()) {
    self->qtItem = new QTreeWidgetItem(qtTableView);
    self->_setState();
  }
  unlock();
}

auto pTableView::remove(sTableViewColumn column) -> void {
}

auto pTableView::remove(sTableViewItem item) -> void {
}

auto pTableView::resizeColumns() -> void {
  auto lock = acquire();

  vector<s32> widths;
  s32 minimumWidth = 0;
  s32 expandable = 0;
  for(auto column : range(self().columnCount())) {
    s32 width = _width(column);
    widths.append(width);
    minimumWidth += width;
    if(self().column(column).expandable()) expandable++;
  }

  s32 maximumWidth = self().geometry().width() - 6;
  if(auto scrollBar = qtTableView->verticalScrollBar()) {
    if(scrollBar->isVisible()) maximumWidth -= scrollBar->geometry().width();
  }

  s32 expandWidth = 0;
  if(expandable && maximumWidth > minimumWidth) {
    expandWidth = (maximumWidth - minimumWidth) / expandable;
  }

  for(auto column : range(self().columnCount())) {
    s32 width = widths[column];
    if(self().column(column).expandable()) width += expandWidth;
    qtTableView->setColumnWidth(column, width);
  }
}

auto pTableView::setAlignment(Alignment alignment) -> void {
}

auto pTableView::setBackgroundColor(Color color) -> void {
  //note: QPalette::AlternateBase can be used for alternating row colors
  static auto defaultColor = qtTableView->palette().color(QPalette::Base);

  auto palette = qtTableView->palette();
  palette.setColor(QPalette::Base, CreateColor(color, defaultColor));
  qtTableView->setPalette(palette);
  qtTableView->setAutoFillBackground((bool)color);
}

auto pTableView::setBatchable(bool batchable) -> void {
  lock();
  qtTableView->setSelectionMode(batchable ? QAbstractItemView::ExtendedSelection : QAbstractItemView::SingleSelection);
  unlock();
}

auto pTableView::setBordered(bool bordered) -> void {
  qtTableView->repaint();
}

auto pTableView::setForegroundColor(Color color) -> void {
  static auto defaultColor = qtTableView->palette().color(QPalette::Text);

  auto palette = qtTableView->palette();
  palette.setColor(QPalette::Text, CreateColor(color, defaultColor));
  qtTableView->setPalette(palette);
  qtTableView->setAutoFillBackground((bool)color);
}

auto pTableView::setHeadered(bool headered) -> void {
  qtTableView->setHeaderHidden(!headered);
}

auto pTableView::setSortable(bool sortable) -> void {
  #if HIRO_QT==4
  qtTableView->header()->setClickable(sortable);
  #elif HIRO_QT==5
  qtTableView->header()->setSectionsClickable(sortable);
  #endif
}

//called on resize/show events
auto pTableView::_onSize() -> void {
  //resize columns only if at least one column is expandable
  for(auto& column : state().columns) {
    if(column->expandable()) return resizeColumns();
  }
}

auto pTableView::_width(u32 column) -> u32 {
  if(auto width = self().column(column).width()) return width;
  u32 width = 1;
  if(!self().column(column).visible()) return width;
  if(state().headered) width = max(width, _widthOfColumn(column));
  for(auto row : range(state().items.size())) {
    width = max(width, _widthOfCell(row, column));
  }
  return width;
}

auto pTableView::_widthOfColumn(u32 _column) -> u32 {
  u32 width = 8;
  if(auto column = self().column(_column)) {
    if(auto& icon = column->state.icon) {
      width += icon.width() + 4;
    }
    if(auto& text = column->state.text) {
      width += pFont::size(column->font(true), text).width();
    }
    if(column->state.sorting != Sort::None) {
      width += 12;
    }
  }
  return width;
}

auto pTableView::_widthOfCell(u32 _row, u32 _column) -> u32 {
  u32 width = 8;
  if(auto item = self().item(_row)) {
    if(auto cell = item->cell(_column)) {
      if(cell->state.checkable) {
        width += 16 + 4;
      }
      if(auto& icon = cell->state.icon) {
        width += icon.width() + 4;
      }
      if(auto& text = cell->state.text) {
        width += pFont::size(cell->font(true), text).width();
      }
    }
  }
  return width;
}

auto QtTableView::onActivate(QTreeWidgetItem* qtItem, int column) -> void {
  if(p.locked()) return;

  for(auto& item : p.state().items) {
    if(auto self = item->self()) {
      if(qtItem == self->qtItem) {
        if(auto cell = item->cell(column)) {
          return p.self().doActivate(cell);
        }
      }
    }
  }

  p.self().doActivate({});
}

auto QtTableView::onChange() -> void {
  for(auto& item : p.state().items) {
    item->state.selected = false;
    if(auto self = item->self()) {
      if(self->qtItem->isSelected()) item->state.selected = true;
    }
  }
  if(!p.locked()) p.self().doChange();
}

auto QtTableView::onContext(const QPoint&) -> void {
  if(p.locked()) return;

  //todo: determine actual cell clicked instead of returning the first cell
  auto item = p.self().selected();
  auto cell = item.cell(0);
  p.self().doContext(cell);
}

auto QtTableView::onSort(int columnNumber) -> void {
  if(auto column = p.self().column(columnNumber)) {
    if(!p.locked() && p.state().sortable) p.self().doSort(column);
  }
}

auto QtTableView::onToggle(QTreeWidgetItem* qtItem, int column) -> void {
  for(auto& item : p.state().items) {
    if(auto self = item->self()) {
      if(qtItem == self->qtItem) {
        if(auto cell = item->cell(column)) {
          cell->state.checked = (qtItem->checkState(column) == Qt::Checked);
          if(!p.locked()) p.self().doToggle(cell);
        }
      }
    }
  }
}

auto QtTableView::mousePressEvent(QMouseEvent* event) -> void {
  QTreeWidget::mousePressEvent(event);
  if(event->button() == Qt::RightButton) {
    //todo: determine actual cell clicked instead of returning the first cell
    auto item = p.self().selected();
    auto cell = item.cell(0);
    p.self().doContext(cell);
  }
}

auto QtTableView::resizeEvent(QResizeEvent* event) -> void {
  QTreeWidget::resizeEvent(event);
  p._onSize();
}

auto QtTableView::showEvent(QShowEvent* event) -> void {
  QTreeWidget::showEvent(event);
  p._onSize();
}

QtTableViewDelegate::QtTableViewDelegate(pTableView& p) : QStyledItemDelegate(p.qtTableView), p(p) {
}

auto QtTableViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const -> void {
  QStyledItemDelegate::paint(painter, option, index);
  if(p.state().bordered) {
    QPen pen;
    pen.setColor(QColor(160, 160, 160));
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawRect(option.rect);
  }
}

}

#endif
