#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <cmath>


class Entity : public sf::Drawable, public sf::Transformable
{
public:

    float speed = 0.05f;
	float x = rand() % 800;
	float y = rand() % 600;
    float ax = 0.0f;
    float ay = 0.0f;
	float dx = (float) (std::rand() % 10) * speed;
	float dy = (float) (std::rand() % 10) * speed;
    // float dy = (float) -0.1f;
    // float dx = (float) -0.1f;
    float width = 10.0f;
    float height = 10.0f;
    int id = 0;
    float centroid_x = 0.0f;
    float centroid_y = 0.0f;
    float flock_x = 0.0f;
    float flock_y = 0.0f;

    void move() {
        y += dy;
        x += dx;
        setPosition(x, y);
        //setRotation(atan(dy / (dx + 0.001f)) * (180.0f/3.1415f));
        sf::Vector2f origin = getOrigin(); // = (10, 20)
        if (x <= 0) {
            x = 1;
            dx = -dx;
        }

        if (x >= 800) {
            x = 799;
            dx = -dx;

        }

        if (y <= 0) {
            y = 1;      
            dy = -dy;
        }

        if (y >= 600) {
            y = 599;      
            dy = -dy;
        }

        // std::cout << "Entity Position X: " << x << "  Y: " << y << std::endl;
        // std::cout << "Origin Position X: " << origin.x << "  Y: " << origin.y << std::endl;
    }

	Entity(int id) {
	
		// load the tileset texture
        if (!m_texture.loadFromFile("test_texture.png")) {
            std::cout << "False";	
		}

		m_vertices.setPrimitiveType(sf::Triangles);
		m_vertices.resize(3);
	
		m_vertices[0].position = sf::Vector2f(x, y+height);
		m_vertices[1].position = sf::Vector2f(x + (width / 2.0f), y);
		m_vertices[2].position = sf::Vector2f(x + width, y+height); 
		
		m_vertices[0].texCoords = sf::Vector2f(0.5f, 0.5f);
		m_vertices[1].texCoords = sf::Vector2f(1.0f, 1.0f);
		m_vertices[2].texCoords = sf::Vector2f(0.0f, 1.0f);

      
        setPosition(x, y);
        setOrigin(x + (width / 2.0f), y + (height / 2.0f));
        //std::cout << "Init new entity" << std::endl;
        this->id = id;
    }

    ~Entity() {
        //std::cout << "Deleting entity" << std::endl;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the entity's transform -- combine it with the one that was passed by the caller
        states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

        // apply the texture
        states.texture = &m_texture;

        // you may also override states.shader or states.blendMode if you want

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_texture;
};



float distance(Entity* e1, Entity* e2) {
    //return std::sqrt(std::pow((e1.x) - (e2.x), 2) + std::pow((e1.y) - (e2.y), 2)); 
    return std::abs(e1->x - e2->x) + std::abs(e1->y - e2->y); 

}


using namespace std::chrono;


int main()
{

    std::srand(time(0));
    int numEntities = 20;

    std::vector<Entity> entities;
    float centroid_x = 0.0f;
    float centroid_y = 0.0f;

    steady_clock::time_point t1 = steady_clock::now(); 

    for (int x = 0; x < numEntities; x++) {
        entities.push_back(Entity(x));
    }

    for (std::vector<Entity>::iterator it = entities.begin(); it != entities.end(); ++it) {
        centroid_x += (*it).x;
        centroid_y += (*it).y;
    }

    sf::Vector2f flock_pos = sf::Vector2f(400.0f, 300.0f);

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    window.setTitle("IT's WoRKInG");    
	
    window.setVerticalSyncEnabled(true); // call it once, after creating the window

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        duration<double> time_span = duration_cast<duration<double>>(steady_clock::now() - t1);

        std::cout << time_span.count() << std::endl;



        for (std::vector<Entity>::iterator it = entities.begin(); it != entities.end(); ++it) {

            centroid_x -= (*it).x;
            centroid_y -= (*it).y;


            // Rule 1
            float rv1_x = 0.02f * (it->centroid_x - it->x);
            float rv1_y = 0.02f * (it->centroid_y - it->y);

            // Rule 2
            float rv2_x = 0.0f;
            float rv2_y = 0.0f;

            float r3_x = 0.0f;
            float r3_y = 0.0f;

            if ((int) time_span.count() % 1 == 0) {
                (*it).flock_x = rand() % 800;
                (*it).flock_y = rand() % 600;
            }

            for (std::vector<Entity>::iterator e = entities.begin(); e != entities.end(); ++e) {

                if (!((*it).id == (*e).id)) {

                    if (distance(&(*it), &(*e)) < 20.0f) {
                        rv2_x -= ((*e).x - (*it).x);
                        rv2_y -= ((*e).y - (*it).y);
                         
                    }

                    r3_x += (*e).dx;
                    r3_y += (*e).dy;
                } 
            }

            rv2_x *= 1.0f;
            rv2_y *= 1.0f;

            r3_x /= (entities.size() - 1);
            r3_y /= (entities.size() - 1);

            float rv3_x = 0.125f * (r3_x - (*it).dx);
            float rv3_y = 0.125f * (r3_y - (*it).dy);

            float rv4_x = (((*it).flock_x - (*it).x));
            float rv4_y = ( ((*it).flock_y - (*it).y));

            //Update velocities
            it->dx = 0.01f * (rv3_x);
            it->dy = 0.01f* (rv3_y);
            
            it->move();

            centroid_x += (*it).x;
            centroid_y += (*it).y;

            (*it).centroid_x = centroid_x / (entities.size() - 1);
            (*it).centroid_y = centroid_y / (entities.size() - 1);

            window.draw(*it);
        }

        window.display();   
    }
    
    return 0;
}
