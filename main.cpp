#include "Framework.hpp"
#include "Light.hpp"
#include "EventManager.hpp"
//OpenGL groundwork + camera and renderer init-----------------------------------------------------------------------------------------------------------

shared_ptr<Renderer> renderer;
shared_ptr<Camera> cam;
EventManager* events;
void eventloop() {
    events->eventloop();
}

int main() {
    srand(static_cast <unsigned> (time(0)));//seed random at the start of the program so we dont forget

    FrameWork frameWork;//inits opengl, camera and renderer
    renderer = shared_ptr<Renderer>(frameWork.renderer);//i really cant be bothered to type framework.renderer and framework.cam all the time
    cam = shared_ptr<Camera>(frameWork.cam);

    events = new EventManager(renderer, cam);
    //draws till the end. 
    frameWork.startDrawingLoop(eventloop);//drawingLoop calls eventloop every frame
    //good practise: free up all memory at the end of the program

    delete events;
    return 0;
}
