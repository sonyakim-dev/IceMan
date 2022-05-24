#include "StudentWorld.h"

GameWorld* createStudentWorld(std::string assetDir)
{
    return new StudentWorld(assetDir);
}

std::string StudentWorld::setPrecision(const unsigned int& val, const unsigned int& precision) {
    int digit = (val < 10) ? 1 : int(log10(val) + 1); /// prevent log10(0) error
    if (digit > precision) return std::to_string(val); /// if val is longer than the given precision, just print val
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
    /// ABOUT THIS FUNC: dig ice(set invisible and dead state) when iceman moves
    bool isThereIce = false; /// if there's even one ice to get deleted, set true

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
    if (isThereIce) playSound(SOUND_DIG); /// prevent redundant sound play. even though 4 ice got deleted, play sound once
}


bool StudentWorld::isIcy(const int& x, const int& y, const int& dir) const {
    /// check is there ice next to the given item's x, y
    /// if the given dir is right, it will check right side ice
    /// this func could also be used when a protester hit ice, turn around and go apposite side..
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

void StudentWorld::initGold() {}
void StudentWorld::initSonar() {}
void StudentWorld::initBoulder() {}
void StudentWorld::initSquirt(const int& dir) {
    //  switch (dir) {
    //    case Actor::up :
    //      actors.emplace_back(std::make_shared<Squirt>(ice_man->getX(), ice_man->getY()+3, this));
    //      actors.back()->setDirection(Actor::up);
    //      actors.back()->setVisible(true);
    //      break;
    //
    //    case Actor::down :
    //      actors.emplace_back(std::make_shared<Squirt>(ice_man->getX(), ice_man->getY()-3, this));
    //      actors.back()->setDirection(Actor::down);
    //      actors.back()->setVisible(true);
    //      break;
    //
    //    case Actor::right :
    //      actors.emplace_back(std::make_shared<Squirt>(ice_man->getX()+3, ice_man->getY(), this));
    //      actors.back()->setDirection(Actor::right);
    //      actors.back()->setVisible(true);
    //      break;
    //
    //    case Actor::left :
    //      actors.emplace_back(std::make_shared<Squirt>(ice_man->getX()-3, ice_man->getY(), this));
    //      actors.back()->setDirection(Actor::left);
    //      actors.back()->setVisible(true);
    //      break;
    //  }
}