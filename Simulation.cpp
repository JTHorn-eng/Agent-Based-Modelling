#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Agent.h"
#include "Simulation.h"

using namespace std;

Simulation::Simulation(int num_es, int d_w, int d_h) 
{

    number_of_entities = num_es;
    display_w = d_w;
    display_h = d_h;
    display_bounds[0] = 0;
    display_bounds[1] = 0;
    display_bounds[2] = d_w;
    display_bounds[3] = d_h;
    boids_centroid = sf::Vector2f(0.0f, 0.0f);
    for (int x = 0; x < number_of_entities; x++) {
        entities.push_back(Agent(x));
        boids_centroid += entities[x].position;
    }
}

float Simulation::distance(Agent* a, Agent* b) 
{
    return sqrt(pow((*a).position.x - (*b).position.x, 2) + pow((*a).position.y - (*b).position.y, 2));
}


void Simulation::calc_centroid(Agent* a, vector<Agent>* as) 
{
    sf::Vector2f asd = sf::Vector2f(0.0f, 0.0f);   
    for (std::vector<Agent>::iterator it = (*as).begin(); it != (*as).end(); ++it) {

        asd += (*it).position;
    }
    
    asd.x /= ((*as).size() - 1);
    asd.y /= ((*as).size() - 1);

    (*a).centroid = asd;

}
void Simulation::avoid_others(Agent* a, vector<Agent>* as)
{
    const float min_distance = 20.0f;
    for (std::vector<Agent>::iterator it = (*as).begin(); it != (*as).end(); ++it) {
        if ((*a).id != (*it).id) {
            if (Simulation::distance(a, &(*it)) <= min_distance) {
                (*a).velocity.x += 0.05f * ((*a).position.x - (*it).position.x);
                (*a).velocity.y += 0.05f * ((*a).position.y - (*it).position.y);
            }
        }
    }
}

void Simulation::flyToCenter(Agent* a, vector<Agent>* as) 
{

    float cx = 0.0f;
    float cy = 0.0f;
    int neighbours = 0;
    for (std::vector<Agent>::iterator it = (*as).begin(); it != (*as).end(); ++it) {

        if ((*a).id != (*it).id) {
            if (Simulation::distance(a, &(*it)) <= visual_range) {
                cx += (*it).position.x;
                cy += (*it).position.y;
                neighbours += 1;
            }
        }
    }

    if (neighbours) {

        cx /= neighbours;
        cy /= neighbours;

        (*a).velocity.x += 0.005f * (cx - (*a).position.x);
        (*a).velocity.y += 0.005f * (cy - (*a).position.y);
    }


}

void Simulation::match_velocity(Agent* a, vector<Agent>* as)
{   
    float avgdx = 0.0f;
    float avgdy = 0.0f;
    int neighbours = 0;
    for (std::vector<Agent>::iterator it = (*as).begin(); it != (*as).end(); ++it) {

        if ((*a).id != (*it).id) {
            if (Simulation::distance(a, &(*it)) <= visual_range) {
                avgdx += (*it).velocity.x;
                avgdy += (*it).velocity.y;
                neighbours += 1;
            }
        }

    }

    if (neighbours) {

        avgdx /= neighbours;
        avgdy /= neighbours;

        (*a).velocity.x += 0.05f * (avgdx - (*a).velocity.x);
        (*a).velocity.y += 0.05f * (avgdy - (*a).velocity.y);
    }

}


void Simulation::run() {

    sf::RenderWindow window(sf::VideoMode(display_w, display_h), "");
    window.setTitle("WORKING");    
    window.setVerticalSyncEnabled(true); // call it once, after creating the window
    while (window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            // "close requested" event: we close the window
            if (e.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        for (std::vector<Agent>::iterator it = entities.begin(); it != entities.end(); ++it) {
            
            Simulation::calc_centroid(&(*it), &entities);
            Simulation::avoid_others(&(*it), &entities);
            Simulation::match_velocity(&(*it), &entities);
            Simulation::flyToCenter(&(*it), &entities);
    
            (*it).update(display_bounds);
            window.draw(*it);
        }
        window.display();
    }

}