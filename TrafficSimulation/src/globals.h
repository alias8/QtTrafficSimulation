#ifndef GLOBALS_H
#define GLOBALS_H

struct Globals{
    int normalDemographic;
    int businessmanDemographic;
    int touristDemographic;
    int maxSpeedWeight;
    int collisionRadiusWeight;
    int collisionSpeedWeight;
    int narrowSpeedWeight;
    int initial_scene_items;
    int max_people;
    int density_check;
    bool DEBUG_DISPLAY_ALTERNATIVE_PATHS;
    bool DEBUG_DISPLAY_GRIDSQUARES;
    bool DEBUG_DISPLAY_NODES;
    bool MOBILE;
    bool FIXED;
    double distanceWeight = 0.5;
    int trafficLightWeight = 600;
    int densityWeight = 15;
    int score_check_frequency = 40;
    bool MOBILE_CHECK_ONCE = true;
};

#endif // GLOBALS_H




