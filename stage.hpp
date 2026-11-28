#pragma once
#include "enemy.hpp"

struct Enemy_spawn{
    Enemy enemy;
    float time;
    bool spawned=false;
};
struct Zoomout{
    float time;
    float size;
    bool outed=false;
};
class Stage{
    private:
        std::string name;
    public:
        std::vector<Enemy_spawn> enemy_spawns;
        std::vector<Zoomout> zoomouts;

        Stage(std::string name);
        std::string GetName();
        void AddEnemy(Enemy enemy,float time);
        void AddZoomout(float time,float size);
};