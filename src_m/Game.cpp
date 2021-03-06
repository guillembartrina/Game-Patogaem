#include "Game.hpp"

#include "Scene_Menu.hpp"

const bool Game::fullscreen = true;

const unsigned int Game::screenW = 800;
const unsigned int Game::screenH = 800;
const unsigned int Game::style = sf::Style::Default;
const std::string Game::title = "Title";

Game::Game()
{
  core.window = &window;
  core.sceneHandler = &sceneHandler;
  core.resources = &resources;	
}

Game::~Game() {}

void Game::run()
{
  init();
  loop();
}

void Game::init()
{  
  if(fullscreen)
  {
    window.create(sf::VideoMode::getFullscreenModes()[0], title, sf::Style::Fullscreen);
  }
  else
  {
    window.create(sf::VideoMode(screenW, screenH), title);
  }

  window.setFramerateLimit(60);
  window.setKeyRepeatEnabled(false);

  ImGui::SFML::Init(window);

  sceneHandler.addScene(std::unique_ptr<Scene>(new Scene_Menu(core)));

  sceneHandler.init();

  clock.restart();
}

void Game::loop()
{
  while (window.isOpen())
  {
    handleEvents();
    update();
    draw();

    sceneHandler.applyRequests();
    if(sceneHandler.exitRequest())
    {
      ImGui::SFML::Shutdown();
      window.close();
    }
  }
}

void Game::handleEvents()
{
  sf::Event event;
  while (window.pollEvent(event))
  {
    switch(event.type)
    {       
      default:
        break;
    }

    ImGui::SFML::ProcessEvent(event);

    sceneHandler.activeScene().handleEvents(event);
  }
}

void Game::update()
{
  sf::Time deltatime;
  deltatime = clock.restart();

  ImGui::SFML::Update(window, deltatime);

  sceneHandler.activeScene().update(deltatime);
}

void Game::draw()
{
  window.clear(sf::Color::Black);

  sceneHandler.activeScene().draw(window);
  
  ImGui::SFML::Render(window);

  window.display();
}