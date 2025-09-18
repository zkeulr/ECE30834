#ifndef gui_h
#define gui_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include "framebuffer.h"

class GUI
{
private:
  inline void cb_Play_i(Fl_Button *, void *);
  static void cb_Play(Fl_Button *, void *);
  inline void cb_Record_i(Fl_Button *, void *);
  static void cb_Record(Fl_Button *o, void *v);

public:
  GUI();
  void show();
  void Record_cb();
  void Play_cb();
  Fl_Double_Window *uiw;
  FrameBuffer *fb;
};
#endif
