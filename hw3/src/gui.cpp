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

  Fl_Button *btn = (Fl_Button *)Fl::focus();
  if (scene->isRecording)
  {
    btn->label("Stop recording");
  }
  else
  {
    btn->label("Record");
  }
}

GUI::GUI()
{
  uiw = new Fl_Double_Window(400, 300, "GUI");
  uiw->user_data((void *)(this));

  // fb = new FrameBuffer(10, 10, 256, 256);
  // fb->box(FL_DOWN_FRAME);

  Fl_Button *playBtn = new Fl_Button(280, 20, 100, 40, "Play");
  playBtn->selection_color(FL_DARK_RED);
  playBtn->callback((Fl_Callback *)cb_Play);

  Fl_Button *recordBtn = new Fl_Button(280, 70, 100, 40, "Record");
  recordBtn->selection_color(FL_DARK_GREEN);
  recordBtn->callback((Fl_Callback *)cb_Record);

  uiw->end();
}

void GUI::show()
{
  uiw->show();
  // fb->take_focus();
}

int main(int argc, char **argv)
{
  scene = new Scene;
  return Fl::run();
}
