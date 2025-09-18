#include "gui.h"
#include "scene.h"
#include "framebuffer.h"

void GUI::cb_Play_i(Fl_Button *, void *)
{
  Play_cb();
}

void GUI::cb_Play(Fl_Button *o, void *v)
{
  ((GUI *)(o->parent()->user_data()))->cb_Play_i(o, v);
}

void GUI::Play_cb()
{
  scene->Play();
}

void GUI::cb_Record_i(Fl_Button *, void *)
{
  Record_cb();
}

void GUI::cb_Record(Fl_Button *o, void *v)
{
  ((GUI *)(o->parent()->user_data()))->cb_Record_i(o, v);
}

void GUI::Record_cb()
{
  scene->Record();
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
  Fl_Button *o = new Fl_Button(280, 20, 100, 40, "Play");
  o->selection_color(FL_DARK_RED);
  o->callback((Fl_Callback *)cb_Play);

  // Playback button
  // load path from file
  // play it out

  uiw->end();
}

void GUI::show()
{
  uiw->show();
  fb->take_focus();
}

int main(int argc, char **argv)
{
  scene = new Scene;
  return Fl::run();
}
