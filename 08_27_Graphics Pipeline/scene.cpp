#include "GL/glew.h"

#include "scene.h"

Scene *scene;


#include <iostream>
#include <fstream>
#include <strstream>

using namespace std;


Scene::Scene() {


	int u0 = 20;
	int v0 = 40;
	int h = 400;
	int w = 600;
	fb = new FrameBuffer(u0, v0, w, h);
	fb->position(u0, v0);
	fb->label("SW Framebuffer");
	fb->show();
	fb->redraw();

	gui = new GUI();
	gui->show();
	gui->uiw->position(u0, v0 + fb->h + v0);

}


void Scene::DBG() {

	int stepsN = 100;
	for (int si = 0; si < stepsN; si++) {
		fb->Set(0xFFFFFFFF);
		int v = fb->h / 2 + si;
		for (int u = 100; u < 200; u++) {
			fb->Set(u, v, 0xFF000000);
		}
		fb->redraw();
		Fl::check();
	}
	return;

	fb->Set(0xFF00FF00);
	fb->redraw();
	return;

	for (int i = 0; i < fb->w*fb->h; i++) {
		fb->pix[i] = 0xFF0000FF;
	}
	
	fb->redraw();

	cerr << endl;
	cerr << "INFO: pressed DBG button on GUI" << endl;

}

void Scene::NewButton() {
	cerr << "INFO: pressed New button on GUI" << endl;
}

