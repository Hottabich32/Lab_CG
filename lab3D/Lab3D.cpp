#include <SFML/Graphics.hpp>
#include <optional>
#include "Scene.hpp"

int main() {
    const unsigned WINDOW_WIDTH = 1920;
    const unsigned WINDOW_HEIGHT = 1080;

    Camera camera(Vector3D(0, -2.0, -5), Vector3D(0, 0, 0), Vector3D(0, 1, 0),
        WINDOW_WIDTH, WINDOW_HEIGHT);

    Scene scene(camera);
    if (!scene.get_model().load_from_file("cube.obj")) return 0;

    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "3D Engine");
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);
        scene.render(window);
        window.display();

        scene.rotate(0.01f);
    }
    return 0;
}