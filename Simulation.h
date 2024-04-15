
#include <vector>
#include "Agent.h"
#include <SFML/Graphics.hpp>


#ifndef Simulation_H
#define Simulation_H
class Simulation {

    public:
        int number_of_entities;
        int display_w;
        int display_h;
        int display_bounds[4];
        const float visual_range = 75.0f;

        vector<Agent> entities;

        sf::Vector2f boids_centroid;

        Simulation(int num_es, int d_w, int d_h);
        
        void run();
        void calc_centroid(Agent* a, vector<Agent>* as);
        float distance(Agent* a, Agent* b);
        void avoid_others(Agent* a, vector<Agent>* as);
        void match_velocity(Agent* a, vector<Agent>* as);
        void flyToCenter(Agent* a, vector<Agent>* as);
};

#endif


