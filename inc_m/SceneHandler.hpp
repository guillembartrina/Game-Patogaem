#ifndef SCENE_HANDLER_HPP
#define SCENE_HANDLER_HPP

#include <memory>
#include <stack>

#include "Scene.hpp"

class Scene;

class SceneHandler : public sf::NonCopyable
{
public:

    SceneHandler();
    ~SceneHandler();

    void init();

    void addScene(std::unique_ptr<Scene> newScene_, bool replace_ = true);
    void popScene();

    void applyRequests();

    Scene& activeScene();

    bool exitRequest() const;

private:

    std::stack<std::unique_ptr<Scene>> scenes;

    bool addRequest;
    std::unique_ptr<Scene> newScene;
    bool replace;

    bool popRequest;
};

#endif