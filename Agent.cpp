#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include "Agent.h"
#include <math.h>
#include <cmath>

#define PI 3.141592653

using namespace std;

Agent::Agent(int id) 
{
    this->id = id;
    position = sf::Vector2f(rand() % 800, rand() % 600);
    velocity = sf::Vector2f((rand() % 10) * 0.01f, (rand() % 10) * 0.01f);
    centroid = sf::Vector2f(0.0f, 0.0f);
    
    float width = 10.0f;
    float height = 10.0f;

    // load the tileset texture
    if (!m_texture.loadFromFile("test_texture.png")) {
        std::cout << "False";	
    }

    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(3);

    m_vertices[0].position = sf::Vector2f(position.x + 0, position.y + height);
    m_vertices[1].position = sf::Vector2f(position.x + (width / 2.0f), position.y + 0);
    m_vertices[2].position = sf::Vector2f(position.x + width, position.y +  height); 
    
    m_vertices[0].texCoords = sf::Vector2f(0.5f, 0.5f);
    m_vertices[1].texCoords = sf::Vector2f(1.0f, 1.0f);
    m_vertices[2].texCoords = sf::Vector2f(0.0f, 1.0f);

    setPosition(position);
    setOrigin(position.x + (width / 2.0f), position.y + (height / 2.0f));
     
}

void Agent::collision(int display_bounds[]) 
{

    // Left
    if (position.x <= display_bounds[0]) {
        position.x = display_bounds[0];
        velocity.x = -velocity.x;
    }

    // Right
    if (position.x >= display_bounds[2]) {
        position.x = display_bounds[2];
        velocity.x = -velocity.x;
    }
    
    // Top
    if (position.y <= display_bounds[0]) {
        position.y = display_bounds[0];
        velocity.y = -velocity.y;
    }
    
    // Bottom
    if (position.y >= display_bounds[3]) {
        position.y = display_bounds[3];
        velocity.y = -velocity.y;
    }
}

void Agent::move(int display_bounds[]) 
{   
    sf::Vector2f wind = sf::Vector2f(0.9f, 0.9f);
    Agent::collision(display_bounds);
    position += 0.005f * (velocity + wind);

    setPosition(position);
    setRotation(atan(velocity.y / velocity.x) * (180 / PI));

}

void Agent::update(int display_bounds[]) 
{
    Agent::move(display_bounds);
}