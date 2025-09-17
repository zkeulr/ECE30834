#ifndef gui_h
#define gui_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include "framebuffer.h"

class GUI
{
private:
  inline void cb_DBG_i(Fl_Button *, void *);
  static void cb_DBG(Fl_Button *, void *);

public:
  GUI();
  void show();
  void DBG_cb();
  Fl_Double_Window *uiw;
  FrameBuffer *fb;
};
#endif
