#include <SDL.h>
#include <cstdlib>
#include <renderer.h>
#include <config.h>
#include <init.h>
#include <stdexcept>
#include <algorithm>

const double gunOffset = 16.5;

int main(int argc, char* argv[]) {
    Global::init();

    double playerSpeed = 500;

    uint32_t curTick, prevTick, diffTick;
    curTick = prevTick = 0;
    prevTick = SDL_GetTicks();

    std::weak_ptr<Objects::Object> controlledObject = Global::playerObject;
    std::weak_ptr<Objects::Object> tempObjectPtr;

    SDL_Event event;
    bool quit = false;
    bool moveUp = false;
    bool moveLeft = false;
    bool moveDown = false;
    bool moveRight = false;
    while (not quit) {
        //SDL_setFramerate(Global::FPSmanager.get(), 144);

        prevTick = curTick;
        curTick = SDL_GetTicks();
        diffTick = curTick - prevTick;

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_q:
                    quit = true;
                    break;
                case SDLK_w:
                    moveUp = true;
                    break;
                case SDLK_a:
                    moveLeft = true;
                    break;
                case SDLK_s:
                    moveDown = true;
                    break;
                case SDLK_d:
                    moveRight = true;
                    break;
                case SDLK_TAB:
                    if (controlledObject.lock() == Global::playerObject)
                        tempObjectPtr = Global::object;
                    else
                        tempObjectPtr = Global::playerObject;
                    controlledObject = tempObjectPtr;
                    Global::playerCamera->setPivotObject(controlledObject.lock());
                    break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                case SDLK_w:
                    moveUp = false;
                    break;
                case SDLK_a:
                    moveLeft = false;
                    break;
                case SDLK_s:
                    moveDown = false;
                    break;
                case SDLK_d:
                    moveRight = false;
                    break;
                }
                break;
            }
        }

        Vector2D moveVector = { 0, 0 };
        if (moveUp)     moveVector += Vector2D(0, -playerSpeed * diffTick / 1000);
        if (moveDown)   moveVector += Vector2D(0, playerSpeed * diffTick / 1000);
        if (moveLeft)   moveVector += Vector2D(-playerSpeed * diffTick / 1000, 0);
        if (moveRight)  moveVector += Vector2D(playerSpeed * diffTick / 1000, 0);
        controlledObject.lock()->move(moveVector);

//        Global::playerObject->rotate(0.01);
//        Global::object->rotate(-0.02);
        Global::playerObject->lookAt(Global::object->getPosition());
        double offsetAngle = asin(
            gunOffset / (Global::object->getPosition() - Global::playerObject->getPosition()).len()
        );
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "offsetAngle = %lf", offsetAngle);
        Global::playerObject->rotate(
            offsetAngle
        );
        Global::object->lookAt(Global::playerObject->getPosition());

        try {
            Renderer::getInstance().render();
        } catch (std::logic_error& e) {
            SDL_LogCritical(
                SDL_LOG_CATEGORY_APPLICATION, 
                "Failed to render. what(): %s", 
                e.what()
            );
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}