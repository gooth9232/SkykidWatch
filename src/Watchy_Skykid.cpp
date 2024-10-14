#include "Watchy_Skykid.h"

#include "FreeSansBold6pt7b.h"

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 544)
const unsigned char* sea_target[2] = {
    bb,
    cv
};
const unsigned char* sea_target_mask[2] = {
    bb_mask,
    cv_mask
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 544)
const unsigned char* land_target[2] = {
    enemy_base,
    enemy_factory
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 96)
const unsigned char* land_enemy[2] = {
    land_enemy1,
    land_enemy2
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 96)
const unsigned char* land_enemy_mask[2] = {
    land_enemy1_mask,
    land_enemy2_mask
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 96)
const unsigned char* sea_enemy[2] = {
    sea_enemy1,
    sea_enemy2
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 96)
const unsigned char* sea_enemy_mask[2] = {
    sea_enemy1_mask,
    sea_enemy2_mask
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 2464)
const unsigned char* num_allArray[10] = {
    num_0,
    num_1,
    num_2,
    num_3,
    num_4,
    num_5,
    num_6,
    num_7,
    num_8,
    num_9
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 2560)
const unsigned char* num_mask_allArray[10] = {
    num_0_mask,
    num_1_mask,
    num_2_mask,
    num_3_mask,
    num_4_mask,
    num_5_mask,
    num_6_mask,
    num_7_mask,
    num_8_mask,
    num_9_mask
};


bool isDaytime = true;
bool isLand = true;

void WatchySkykid::drawWatchFace(){

    if (currentTime.Hour < 6 || currentTime.Hour >= 18) {
        isDaytime = false;
    }
    else {
        isDaytime = true;
    }

#ifdef WATCHY_SIM
    srand(currentTime.Hour + currentTime.Minute);
    int isLandNum = rand() % 2;
#else
    randomSeed(currentTime.Hour + currentTime.Minute);
    int isLandNum = random(2);
#endif
    if (isLandNum == 0) {
        isLand = false;
    }
    else {
        isLand = true;
    }

    display.fillScreen(isDaytime ? GxEPD_WHITE : GxEPD_BLACK);

    //Backgroud
    drawBackground();

    //Hour
    int hour = 0;
    if (currentTime.Hour >= 12) {
        hour = currentTime.Hour - 12;
    }
    else {
        hour = currentTime.Hour;
    }
    if (hour >= 10) {
        drawSeg(hour / 10, 1, 6, true);
    }
    drawSeg(hour % 10, 6, 6);

    //Minute
    drawSeg(currentTime.Minute / 10, 18, 9);
    drawSeg(currentTime.Minute % 10, 28, 9);

    //Battery
    drawBattery();

    //Date
    drawDate();
}

void WatchySkykid::drawBackground() {
    //Title
    display.drawBitmap(0, 0, title, INDEX_SIZE * 19, INDEX_SIZE * 4, isDaytime ? GxEPD_BLACK : GxEPD_WHITE);

    //Land or Sea
    display.drawBitmap(0, INDEX_SIZE * 44, isLand ? land : sea, 200, INDEX_SIZE * 6, isDaytime ? GxEPD_BLACK : GxEPD_WHITE );

    //Sun or Moon
    display.drawBitmap(INDEX_SIZE * 40, INDEX_SIZE * 5, isDaytime ? sun : moon, INDEX_SIZE * 8, INDEX_SIZE * 8, isDaytime ? GxEPD_BLACK : GxEPD_WHITE);

    // AirEnemy
    drawAirEnemy();

    // LandEnemy
    drawLandEnemy();

    // target
    drawTarget();

    // Player
#ifdef WATCHY_SIM
    srand(currentTime.Hour + currentTime.Minute + 37);
    int playerLocate = rand() % 3;
#else
    randomSeed(currentTime.Hour + currentTime.Minute + 37);
    int playerLocate = random(3);
#endif

    if (isDaytime) {
        display.drawBitmap(INDEX_SIZE * (3 + 5 * playerLocate), INDEX_SIZE * 20, player_mask, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_BLACK);
    }
    display.drawBitmap(INDEX_SIZE * (3 + 5 * playerLocate), INDEX_SIZE * 20, player, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_WHITE);

    // Vinus
    display.drawBitmap(INDEX_SIZE * 40, INDEX_SIZE * 30, vinus1, INDEX_SIZE * 8, INDEX_SIZE * 16, isDaytime ? GxEPD_BLACK : GxEPD_WHITE);

    // bomb
    drawBomb(playerLocate);
}

void WatchySkykid::drawSeg(const int& num, int index_x, int index_y, bool one_left)
{
    int disp_size_x = 10;
    if (num == 1) {
        disp_size_x = 5;
        if (one_left) {
            index_x += 0; // left side
        }
        else {
            index_x += 5; // right side
        }
    }
    display.drawBitmap(INDEX_SIZE * index_x, INDEX_SIZE * index_y, num_allArray[num], INDEX_SIZE * disp_size_x, INDEX_SIZE * 12, isDaytime ? GxEPD_BLACK : GxEPD_WHITE);

    if (isDaytime) {
        display.drawBitmap(INDEX_SIZE * index_x, INDEX_SIZE * index_y, num_mask_allArray[num], INDEX_SIZE * disp_size_x, INDEX_SIZE * 12, GxEPD_WHITE);
    }
}

const int air_enemy_indexs[4][2] = {
    {18,24},
    {27,33},
    {36,22},
    {42,18},
};

void WatchySkykid::drawAirEnemy()
{
#ifdef WATCHY_SIM
  srand(currentTime.Hour + currentTime.Minute + 11);
#else
  randomSeed(currentTime.Hour + currentTime.Minute + 11);
#endif

    int choiced[2] = { -1, -1 };
    int randMax = 4;
    for (int cnt = 0; cnt < 2; ) {
#ifdef WATCHY_SIM
        int choice = rand()% randMax;
#else
        int choice = random(randMax);
#endif
        int cnt2 = 0;
        for (; cnt2 < 2; cnt2++) {
            if (choiced[cnt2] == choice) {
                break;
            }
        }
        if (cnt2 != 2) {
            continue;
        }
        choiced[cnt] = choice;

        cnt++;
    }

    if (!isDaytime) {
        display.drawBitmap(INDEX_SIZE * air_enemy_indexs[choiced[0]][0], INDEX_SIZE * air_enemy_indexs[choiced[0]][1], air_enemy_mask, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_WHITE);
        display.drawBitmap(INDEX_SIZE * air_enemy_indexs[choiced[1]][0], INDEX_SIZE * air_enemy_indexs[choiced[1]][1], air_enemy_mask, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_WHITE);
    }

    display.drawBitmap(INDEX_SIZE * air_enemy_indexs[choiced[0]][0], INDEX_SIZE * air_enemy_indexs[choiced[0]][1], air_enemy, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_BLACK);
    display.drawBitmap(INDEX_SIZE * air_enemy_indexs[choiced[1]][0], INDEX_SIZE * air_enemy_indexs[choiced[1]][1], air_enemy, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_BLACK);

}

const int land_enemy_indexs[4][2] = {
    {20,42},
    {25,42},
    {30,42},
    {35,42}
};
void WatchySkykid::drawLandEnemy()
{
#ifdef WATCHY_SIM
    srand(currentTime.Hour + currentTime.Minute + 23);
#else
    randomSeed(currentTime.Hour + currentTime.Minute + 23);
#endif

    int choiced[2] = { -1, -1 };
    int randMax = 4;
    for (int cnt = 0; cnt < 2; ) {
#ifdef WATCHY_SIM
        int choice = rand() % randMax;
#else
        int choice = random(randMax);
#endif
        int cnt2 = 0;
        for (; cnt2 < 2; cnt2++) {
            if (choiced[cnt2] == choice) {
                break;
            }
        }
        if (cnt2 != 2) {
            continue;
        }
        choiced[cnt] = choice;

        cnt++;
    }

    randMax = 2;
    int choicedEnemy[2] = { -1, -1 };
    for (int cnt = 0; cnt < 2; cnt++) {
#ifdef WATCHY_SIM
        choicedEnemy[cnt] = rand() % randMax;
#else
        choicedEnemy[cnt] = random(randMax);
#endif
    }

    if (isLand) {
        display.drawBitmap(INDEX_SIZE * land_enemy_indexs[choiced[0]][0], INDEX_SIZE * land_enemy_indexs[choiced[0]][1], land_enemy_mask[choicedEnemy[0]], INDEX_SIZE * 4, INDEX_SIZE * 4, isDaytime ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(INDEX_SIZE * land_enemy_indexs[choiced[1]][0], INDEX_SIZE * land_enemy_indexs[choiced[1]][1], land_enemy_mask[choicedEnemy[1]], INDEX_SIZE * 4, INDEX_SIZE * 4, isDaytime ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(INDEX_SIZE* land_enemy_indexs[choiced[0]][0], INDEX_SIZE* land_enemy_indexs[choiced[0]][1], land_enemy[choicedEnemy[0]], INDEX_SIZE * 4, INDEX_SIZE * 4, isDaytime ? GxEPD_BLACK : GxEPD_WHITE);
        display.drawBitmap(INDEX_SIZE* land_enemy_indexs[choiced[1]][0], INDEX_SIZE* land_enemy_indexs[choiced[1]][1], land_enemy[choicedEnemy[1]], INDEX_SIZE * 4, INDEX_SIZE * 4, isDaytime ? GxEPD_BLACK : GxEPD_WHITE);
    }
    else {
        display.drawBitmap(INDEX_SIZE * land_enemy_indexs[choiced[0]][0], INDEX_SIZE * land_enemy_indexs[choiced[0]][1], sea_enemy_mask[choicedEnemy[0]], INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_BLACK);
        display.drawBitmap(INDEX_SIZE * land_enemy_indexs[choiced[1]][0], INDEX_SIZE * land_enemy_indexs[choiced[1]][1], sea_enemy_mask[choicedEnemy[1]], INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_BLACK);
        display.drawBitmap(INDEX_SIZE * land_enemy_indexs[choiced[0]][0], INDEX_SIZE * land_enemy_indexs[choiced[0]][1], sea_enemy[choicedEnemy[0]], INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_WHITE);
        display.drawBitmap(INDEX_SIZE * land_enemy_indexs[choiced[1]][0], INDEX_SIZE * land_enemy_indexs[choiced[1]][1], sea_enemy[choicedEnemy[1]], INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_WHITE);
    }

}




void WatchySkykid::drawTarget()
{
#ifdef WATCHY_SIM
    srand(currentTime.Hour + currentTime.Minute + 31);
    int choice = rand() % 2;
#else
    randomSeed(currentTime.Hour + currentTime.Minute + 31);
    int choice = random(2);
#endif
    if (isLand) {
        display.drawBitmap(INDEX_SIZE * 2, INDEX_SIZE * 38, land_mask, INDEX_SIZE * 16, INDEX_SIZE * 8, isDaytime ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(INDEX_SIZE * 2, INDEX_SIZE * 38, land_target[choice], INDEX_SIZE * 16, INDEX_SIZE * 8, isDaytime ? GxEPD_BLACK : GxEPD_WHITE);
    }
    else {
        display.drawBitmap(INDEX_SIZE * 2, INDEX_SIZE * 38, sea_target_mask[choice], INDEX_SIZE * 16, INDEX_SIZE * 8, GxEPD_BLACK);
        display.drawBitmap(INDEX_SIZE * 2, INDEX_SIZE * 38, sea_target[choice], INDEX_SIZE * 16, INDEX_SIZE * 8, GxEPD_WHITE);
    }



}

void WatchySkykid::drawBattery(){
    int8_t batteryLevel = 0;
    float VBAT = getBatteryVoltage();
    if(VBAT > 3.8){
        batteryLevel = 3;
    }
    else if(VBAT > 3.4 && VBAT <= 3.8){
        batteryLevel = 2;
    }
    else if(VBAT > 3.0 && VBAT <= 3.4){
        batteryLevel = 1;
    }
    else if(VBAT <= 3.0){
        batteryLevel = 0;
    }

    for(int8_t batterySegments = 0; batterySegments < batteryLevel; batterySegments++){
        if (isDaytime) {
            display.drawBitmap(INDEX_SIZE * 46 - INDEX_SIZE * 4 * batterySegments, INDEX_SIZE * 0, player_mask, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_BLACK); //mask
        }
            display.drawBitmap(INDEX_SIZE * 46 - INDEX_SIZE * 4 *batterySegments, INDEX_SIZE * 0, player, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_WHITE); //player

    }
}

void WatchySkykid::drawDate() {
    display.setTextColor(isDaytime ? GxEPD_BLACK : GxEPD_WHITE);
    display.setFont(&FreeSansBold6pt7b);

    String score = "BARON ";
    if (currentTime.Month < 10) {
        score += " ";
    }
    score += currentTime.Month;
    if (currentTime.Day < 10) {
        score += "0";
    }
    score += currentTime.Day;
    display.setCursor(INDEX_SIZE * 19, INDEX_SIZE * 3);
    display.print(score);
}

void WatchySkykid::drawBomb(const int &playerLocate) {
    const int bomb_indexs[3][2] = {
        {4,25},
        {4,29},
        {4,33},
    };

    for (int cnt = 0; cnt < 3; cnt++) {
        display.drawBitmap(INDEX_SIZE * (bomb_indexs[cnt][0] + playerLocate * 5), INDEX_SIZE * bomb_indexs[cnt][1], bomb, INDEX_SIZE * 2, INDEX_SIZE * 4, isDaytime ? GxEPD_BLACK : GxEPD_WHITE); // draw
    }

    switch (playerLocate) {
    case 0:
        display.drawBitmap(INDEX_SIZE * 3, INDEX_SIZE * 37, five_hundred_mask, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_WHITE); // mask
        display.drawBitmap(INDEX_SIZE * 3, INDEX_SIZE * 37, five_hundred, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_BLACK); // 500
        display.drawBitmap(INDEX_SIZE * 3, INDEX_SIZE * 41, exporde_mask, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_WHITE); // mask
        display.drawBitmap(INDEX_SIZE * 3, INDEX_SIZE * 41, exporde, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_BLACK); // exporde
        break;
    case 1:
        display.drawBitmap(INDEX_SIZE * 12, INDEX_SIZE * 18, medal_mask, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_WHITE); // mask
        display.drawBitmap(INDEX_SIZE * 12, INDEX_SIZE * 18, medal, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_BLACK); // medal
        display.drawBitmap(INDEX_SIZE * 8, INDEX_SIZE * 37, thousand_mask, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_WHITE); // mask
        display.drawBitmap(INDEX_SIZE * 8, INDEX_SIZE * 37, thousand, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_BLACK); // 1000
        display.drawBitmap(INDEX_SIZE * 8, INDEX_SIZE * 41, exporde_mask, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_WHITE); // mask
        display.drawBitmap(INDEX_SIZE * 8, INDEX_SIZE * 41, exporde, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_BLACK); // exporde

        // Vinus
        display.drawBitmap(INDEX_SIZE * 40, INDEX_SIZE * 30, vinus1, INDEX_SIZE * 8, INDEX_SIZE * 16, isDaytime ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(INDEX_SIZE * 40, INDEX_SIZE * 30, vinus2, INDEX_SIZE * 8, INDEX_SIZE * 16, isDaytime ? GxEPD_BLACK : GxEPD_WHITE);

        break;
    case 2:
        display.drawBitmap(INDEX_SIZE * 13, INDEX_SIZE * 37, five_hundred_mask, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_WHITE); // mask
        display.drawBitmap(INDEX_SIZE * 13, INDEX_SIZE * 37, five_hundred, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_BLACK); // 500
        display.drawBitmap(INDEX_SIZE * 13, INDEX_SIZE * 41, exporde_mask, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_WHITE); // mask
        display.drawBitmap(INDEX_SIZE * 13, INDEX_SIZE * 41, exporde, INDEX_SIZE * 4, INDEX_SIZE * 4, GxEPD_BLACK); // exporde
        break;
    default:
        break;
    }

}