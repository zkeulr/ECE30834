#include "scene.h"

int currentOffset;
int completedPasses = 0;
Scene *animationScene = nullptr;

// The animation needs 30Hz frame rate
// and it should last 10s, during which
// the name string should cover the
// screen three times
void animateCallback(void *)
{
    if (!animationScene || !animationScene->fb)
    {
        return;
    }

    animationScene->fb->Set(0xFFFFFFFF);
    animationScene->writeName(currentOffset, 0xFF000000);
    currentOffset += animationScene->fb->w / 33;

    if (currentOffset >= animationScene->fb->w + animationScene->fb->w)
    {
        completedPasses++;
        if (completedPasses < 3)
        {
            currentOffset = -animationScene->fb->w;
        }
        else
        {
            return;
        }
    }

    if (completedPasses < 3)
    {
        Fl::repeat_timeout(1.0 / 30.0, animateCallback);
    }
}

int main(int argc, char **argv)
{
    animationScene = new Scene();
    animationScene->fb->show();
    currentOffset = -animationScene->fb->w;
    completedPasses = 0;

    Fl::add_timeout(0.5, animateCallback);
    return Fl::run();
}