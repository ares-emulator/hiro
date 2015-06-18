#if defined(Hiro_Canvas)

namespace hiro {

static auto Canvas_drop(GtkWidget* widget, GdkDragContext* context, signed x, signed y,
GtkSelectionData* data, unsigned type, unsigned timestamp, pCanvas* p) -> void {
  if(!p->state().droppable) return;
  lstring paths = DropPaths(data);
  if(paths.empty()) return;
  p->self().doDrop(paths);
}

static auto Canvas_expose(GtkWidget* widget, GdkEventExpose* event, pCanvas* p) -> signed {
  p->_onExpose(event);
  return true;
}

static auto Canvas_mouseLeave(GtkWidget* widget, GdkEventButton* event, pCanvas* p) -> signed {
  p->self().doMouseLeave();
  return true;
}

static auto Canvas_mouseMove(GtkWidget* widget, GdkEventButton* event, pCanvas* p) -> signed {
  p->self().doMouseMove({(signed)event->x, (signed)event->y});
  return true;
}

static auto Canvas_mousePress(GtkWidget* widget, GdkEventButton* event, pCanvas* p) -> signed {
  switch(event->button) {
  case 1: p->self().doMousePress(Mouse::Button::Left); break;
  case 2: p->self().doMousePress(Mouse::Button::Middle); break;
  case 3: p->self().doMousePress(Mouse::Button::Right); break;
  }
  return true;
}

static auto Canvas_mouseRelease(GtkWidget* widget, GdkEventButton* event, pCanvas* p) -> signed {
  switch(event->button) {
  case 1: p->self().doMouseRelease(Mouse::Button::Left); break;
  case 2: p->self().doMouseRelease(Mouse::Button::Middle); break;
  case 3: p->self().doMouseRelease(Mouse::Button::Right); break;
  }
  return true;
}

auto pCanvas::construct() -> void {
  gtkWidget = gtk_drawing_area_new();
  gtk_widget_add_events(gtkWidget,
    GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_POINTER_MOTION_MASK);

  setDroppable(state().droppable);
  _rasterize();
  _redraw();

  //todo: need to work around GTK+ library bug:
  //after calling destruct(), construct() with state.droppable == true;
  //GTK+ will throw SIGBUS inside g_signal_connect_data() on one of the below connections

  g_signal_connect(G_OBJECT(gtkWidget), "button-press-event", G_CALLBACK(Canvas_mousePress), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "button-release-event", G_CALLBACK(Canvas_mouseRelease), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "drag-data-received", G_CALLBACK(Canvas_drop), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "expose-event", G_CALLBACK(Canvas_expose), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "leave-notify-event", G_CALLBACK(Canvas_mouseLeave), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "motion-notify-event", G_CALLBACK(Canvas_mouseMove), (gpointer)this);

  pWidget::construct();
}

auto pCanvas::destruct() -> void {
  _release();
  if(gtkWidget) gtk_widget_destroy(gtkWidget), gtkWidget = nullptr;
  gtkParent = nullptr;
}

auto pCanvas::minimumSize() const -> Size {
  return {max(0, state().size.width()), max(0, state().size.height())};
}

auto pCanvas::setColor(Color color) -> void {
  mode = Mode::Color;
  update();
}

auto pCanvas::setData(Size size) -> void {
  mode = Mode::Data;
  update();
}

auto pCanvas::setDroppable(bool droppable) -> void {
  if(droppable) {
    gtk_drag_dest_set(gtkWidget, GTK_DEST_DEFAULT_ALL, nullptr, 0, GDK_ACTION_COPY);
    gtk_drag_dest_add_uri_targets(gtkWidget);
  }
}

auto pCanvas::setGeometry(Geometry geometry) -> void {
  update();
  pWidget::setGeometry(geometry);
}

auto pCanvas::setGradient(Color topLeft, Color topRight, Color bottomLeft, Color bottomRight) -> void {
  mode = Mode::Gradient;
  update();
}

auto pCanvas::setIcon(const image& icon) -> void {
  mode = Mode::Icon;
  update();
}

auto pCanvas::update() -> void {
  _rasterize();
  _redraw();
}

auto pCanvas::_onExpose(GdkEventExpose* expose) -> void {
  if(surface == nullptr) return;

  signed sx = 0, sy = 0, dx = 0, dy = 0;
  signed width = surfaceWidth;
  signed height = surfaceHeight;
  auto geometry = pSizable::state().geometry;

  if(width <= geometry.width()) {
    sx = 0;
    dx = (geometry.width() - width) / 2;
  } else {
    sx = (width - geometry.width()) / 2;
    dx = 0;
    width = geometry.width();
  }

  if(height <= geometry.height()) {
    sy = 0;
    dy = (geometry.height() - height) / 2;
  } else {
    sy = (height - geometry.height()) / 2;
    dy = 0;
    height = geometry.height();
  }

  gdk_draw_pixbuf(gtk_widget_get_window(gtkWidget), nullptr, surface, sx, sy, dx, dy, width, height, GDK_RGB_DITHER_NONE, 0, 0);
}

auto pCanvas::_rasterize() -> void {
  signed width = 0;
  signed height = 0;

  if(mode == Mode::Color || mode == Mode::Gradient) {
    width = pSizable::state().geometry.width();
    height = pSizable::state().geometry.height();
  } else {
    width = state().size.width();
    height = state().size.height();
  }

  if(width <= 0 || height <= 0) return;

  if(width != surfaceWidth || height != surfaceHeight) _release();
  surfaceWidth = width;
  surfaceHeight = height;

  if(!surface) surface = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, width, height);
  uint32_t* buffer = (uint32_t*)gdk_pixbuf_get_pixels(surface);

  if(mode == Mode::Color) {
    uint32_t color = state().color.value();
    for(auto n : range(width * height)) buffer[n] = color;
  }

  if(mode == Mode::Gradient) {
    image fill;
    fill.allocate(width, height);
    fill.gradient(
      state().gradient[0].value(), state().gradient[1].value(), state().gradient[2].value(), state().gradient[3].value()
    );
    memory::copy(buffer, fill.data(), fill.size());
  }

  if(mode == Mode::Icon) {
    auto icon = state().icon;
    icon.scale(width, height);
    icon.transform();
    memory::copy(buffer, icon.data(), icon.size());
  }

  if(mode == Mode::Data) {
    memory::copy(buffer, state().data.data(), state().data.size() * sizeof(uint32_t));
  }

  //ARGB -> ABGR conversion
  for(auto n : range(width * height)) {
    uint32_t color = *buffer;
    color = (color & 0xff00ff00) | ((color & 0xff0000) >> 16) | ((color & 0x0000ff) << 16);
    *buffer++ = color;
  }
}

auto pCanvas::_redraw() -> void {
  if(gtk_widget_get_realized(gtkWidget)) {
    gdk_window_invalidate_rect(gtk_widget_get_window(gtkWidget), nullptr, true);
  }
}

auto pCanvas::_release() -> void {
  if(surface) {
    g_object_unref(surface);
    surface = nullptr;
  }
  surfaceWidth = 0;
  surfaceHeight = 0;
}

}

#endif
