#pragma once
#include "gui.h"
#include "framebuffer.h"

class Scene
{
public:
	GUI *gui;
	FrameBuffer *fb;
	Scene();
	void DBG();
	void Scene::NewButton();
};

extern Scene *scene;