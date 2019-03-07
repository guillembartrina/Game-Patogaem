
#include "SceneHandler.hpp"

SceneHandler::SceneHandler()
{
    addRequest = false;
    replace = false;
    popRequest = false;
}

SceneHandler::~SceneHandler() {}

void SceneHandler::init()
{
    applyRequests();
}

void SceneHandler::addScene(std::unique_ptr<Scene> newScene_, bool replace_)
{
    addRequest = true;
    replace = replace_;
    newScene = std::move(newScene_);
}

void SceneHandler::popScene()
{
    popRequest = true;
}

void SceneHandler::applyRequests()
{
    if(addRequest)
    {
        if(!scenes.empty())
        {
            if(replace)
            {
                scenes.pop();
            }
            else
            {
                scenes.top()->pause();
            }
        }

        scenes.push(std::move(newScene));

        scenes.top()->init();

        addRequest = false;
    }

    if(popRequest)
    {
        if(!scenes.empty())
        {
            scenes.pop();

            if(!scenes.empty())
            {
                scenes.top()->resume();
            }
        }

        popRequest = false;
    }
}

Scene& SceneHandler::activeScene()
{
    return *scenes.top();
}

bool SceneHandler::exitRequest() const
{
    return scenes.empty();
}