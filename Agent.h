#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <random>

#ifndef Agent_H
#define Agent_H

using namespace std;

class Agent : public sf::Drawable, public sf::Transformable {

    public:
        int id;
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f centroid;

        Agent(int id);
           
        void collision(int display_bounds[]); 
        void calc_centroid();
        void move(int display_bounds[]); 
        void update(int display_bounds[]);

    private:
        sf::VertexArray m_vertices;
        sf::Texture m_texture;

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

};

#endif