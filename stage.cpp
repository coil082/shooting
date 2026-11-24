#include "stage.hpp"

Stage::Stage(std::string name):name(name){};
std::string Stage::GetName(){
    return name;
};
void Stage::AddEnemy(Enemy enemy,float time){
    enemy_spawns.push_back(Enemy_spawn{enemy,time});
};
void Stage::AddZoomout(float time,float size){
    zoomouts.push_back(Zoomout{time,size});
};
