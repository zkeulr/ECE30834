#include "gui.h"
#include "scene.h"
#include "framebuffer.h"
#include <iostream> // TODO: delete

void GUI::cb_DBG_i(Fl_Button *, void *)
{
  DBG_cb();
}

void GUI::cb_DBG(Fl_Button *o, void *v)
{
  ((GUI *)(o->parent()->user_data()))->cb_DBG_i(o, v);
}

GUI::GUI()
{
  // Main window
  uiw = new Fl_Double_Window(400, 300, "GUI");
  uiw->user_data((void *)(this));

  // Framebuffer (GL viewport) for drawing & key handling
  fb = new FrameBuffer(10, 10, 256, 256);
  fb->box(FL_DOWN_FRAME);

  // Debug button
  Fl_Button *o = new Fl_Button(280, 20, 100, 40, "DBG");
  o->selection_color(FL_DARK_RED);
  o->callback((Fl_Callback *)cb_DBG);

  uiw->end();
}

int main(int argc, char **argv)
{
  scene = new Scene;
  return Fl::run();
}

void GUI::show()
{
  uiw->show();
  fb->take_focus();
}

void GUI::DBG_cb()
{
  scene->DBG();
}
