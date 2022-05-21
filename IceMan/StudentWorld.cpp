#include "StudentWorld.h"

GameWorld* createStudentWorld(std::string assetDir)
{
    return new StudentWorld(assetDir);
}

std::string StudentWorld::setPrecision(const unsigned int& val, const unsigned int& precision) {
    int digit = (val < 10) ? 1 : int(log10(val) + 1); // prevent log10(0) error
    if (digit > precision) return std::to_string(val); // if val is longer than the given precision, just print val
    else return std::string(precision - digit, '0').append(std::to_string(val));
}

void StudentWorld::setDisplayText() {
    std::string s = "Lvl: " + setPrecision(getLevel(), 2) + "  Lives: " + std::to_string(getLives()) +
        "  Hlth: " + setPrecision(ice_man->getHP(), 3) + "%  Wtr: " + setPrecision(ice_man->getWater(), 2) +
        "  Gld: " + setPrecision(ice_man->getGold(), 2) + "  Oil Left: " + setPrecision(num_oil, 2) +
        "  Sonar: " + setPrecision(ice_man->getSonar(), 2) + "  Scr: " + setPrecision(getScore(), 6);

    setGameStatText(s);
}


void StudentWorld::initIce() {
    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 64; ++y) {
            ice[x][y] = std::make_shared<Ice>(x, y, this);
            if (((x >= 30 && x <= 33) && (y >= 4 && y <= 59)) || y >= 60) {
                ice[x][y]->setVisible(false);
                ice[x][y]->setDead();
            }
        }
    }
}


void StudentWorld::digIce(const unsigned int& x, const unsigned int& y, const int& dir) {
    // ABOUT THIS FUNC: dig ice(set invisible and dead state) when iceman moves
    bool isThereIce = false; // if there's even one ice to get deleted, set true

    switch (dir) {
    case KEY_PRESS_UP:
        for (int i = 0; i < 4; ++i) {
            if (ice[x + i][y + 3]->isVisible()) {
                isThereIce = true;
                ice[x + i][y + 3]->setVisible(false);
                ice[x + i][y + 3]->setDead();
            }
        }
        break;

    case KEY_PRESS_DOWN:
        for (int i = 0; i < 4; ++i) {
            if (ice[x + i][y]->isVisible()) {
                isThereIce = true;
                ice[x + i][y]->setVisible(false);
                ice[x + i][y]->setDead();
            }
        }
        break;

    case KEY_PRESS_RIGHT:
        for (int i = 0; i < 4; ++i) {
            if (ice[x + 3][y + i]->isVisible()) {
                isThereIce = true;
                ice[x + 3][y + i]->setVisible(false);
                ice[x + 3][y + i]->setDead();
            }
        }
        break;

    case KEY_PRESS_LEFT:
        for (int i = 0; i < 4; ++i) {
            if (ice[x][y + i]->isVisible()) {
                isThereIce = true;
                ice[x][y + i]->setVisible(false);
                ice[x][y + i]->setDead();
            }
        }
        break;
    }
    if (isThereIce) playSound(SOUND_DIG); // prevent redundant sound play. even though 4 ice got deleted, play sound once
}


bool StudentWorld::isIcy(const int& x, const int& y, const int& dir) const {
    // ABOUT THIS FUNC: check is there ice next to the given item's x, y
    //                if the given dir is right, it will check right side ice
    //                this func could also be used when a protester hit ice, turn around and go apposite side..
    bool isThereIce = false;

    switch (dir) {
    case Actor::up:
        for (int i = 0; i < 4; ++i) {
            if (ice[x + i][y + 3]->isVisible()) {
                isThereIce = true;
            }
        }
        break;

    case Actor::down:
        for (int i = 0; i < 4; ++i) {
            if (ice[x + i][y - 1]->isVisible()) {
                isThereIce = true;
            }
        }
        break;

    case Actor::right:
        for (int i = 0; i < 4; ++i) {
            if (ice[x + 3][y + i]->isVisible()) {
                isThereIce = true;
            }
        }
        break;

    case Actor::left:
        for (int i = 0; i < 4; ++i) {
            if (ice[x - 1][y + i]->isVisible()) {
                isThereIce = true;
            }
        }
        break;
    }
    return isThereIce;
}

bool StudentWorld::isBouldery(const unsigned int ch) {
    bool flag = false;
    switch (ch) {
    case KEY_PRESS_UP:
        // if the top bar of the ice_man box touches the bottom bar of the boulder box, return true, else return false
        for (auto actor : actors.at(boulder)) {
            for (int i = 0; i < 4; i++) {
                int x1 = ice_man->getX() + i; 
                int y1 = ice_man->getY() + 4;

                for (int j = 0; j < 4; j++) {
                    int x2 = actor->getX() + j;
                    int y2 = actor->getY();

                    if (x1 == x2 && y1 == y2) {
                        return true;
                    }

                }
            }
        }

        break;
    case KEY_PRESS_DOWN:
        for (auto actor : actors.at(boulder)) {
            for (int i = 0; i < 4; i++) {
                int x1 = ice_man->getX() + i;
                int y1 = ice_man->getY();

                for (int j = 0; j < 4; j++) {
                    int x2 = actor->getX() + j;
                    int y2 = actor->getY() + 4;

                    if (x1 == x2 && y1 == y2) {
                        flag = true;
                    }

                }
            }
        }
        break;
    case KEY_PRESS_LEFT:
        for (auto actor : actors.at(boulder)) {
            for (int i = 0; i < 4; i++) {
                int x1 = ice_man->getX();
                int y1 = ice_man->getY() + i;

                for (int j = 0; j < 4; j++) {
                    int x2 = actor->getX() + 4;
                    int y2 = actor->getY() + j;

                    if (x1 == x2 && y1 == y2) {
                        flag = true;
                    }

                }
            }
        }
        break;
    case KEY_PRESS_RIGHT:
        for (auto actor : actors.at(boulder)) {
            for (int i = 0; i < 4; i++) {
                int x1 = ice_man->getX() + 4;
                int y1 = ice_man->getY() + i;

                for (int j = 0; j < 4; j++) {
                    int x2 = actor->getX();
                    int y2 = actor->getY() + j;

                    if (x1 == x2 && y1 == y2) {
                        flag = true;
                    }

                }
            }
        }
        break;
    }

    return flag;
}


void StudentWorld::initGold() {}
void StudentWorld::initSonar() {}
void StudentWorld::initBoulder() {}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp