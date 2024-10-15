#ifndef WATCHY_SKYKID_H
#define WATCHY_SKYKID_H

#ifdef WATCHY_SIM
#include "..\..\Watchy.h"
#else // WATCHY_SIM
#include <Watchy.h>
#endif // WATCHY_SIM
#include "Skykid.h"

#define INDEX_SIZE 4

class WatchySkykid : public Watchy{
    public:
        using Watchy::Watchy;
        void drawWatchFace();
        void drawBackground();
        void drawBattery();
        void drawSeg(const int& num, int index_x, int index_y, bool one_left = false);
        void drawAirEnemy();
        void drawLandEnemy();
        void drawTarget();
        void drawDate();
        void drawBomb(const int& playerLocate);
};

#endif