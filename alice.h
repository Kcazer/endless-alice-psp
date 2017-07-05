#pragma once

#include <oslib/oslib.h>
#include "gamedataloader.h"
#include "endlessalice.h"

class Alice {
public:
    ~Alice(void);

    static Alice *getAlice(void);

    int deltaY(void);

    void reset(void);

    void draw(void);

    void update(void);

    void setMap(char *map) { this->map = map; }

protected:
    // methodes
    Alice(void);

    int valueAt(int x, int y);

    // Membres
    int posX, posY;
    char map[20][15];
    OSL_IMAGE *sprite;
    EndlessAlice *scene;
    static int verticalSpeed[13];
    enum Status {
        STAND, WALK, JUMP1, JUMP2, FALL1, FALL2, SLIDE, SHOOT, HIT
    };
private:
    static Alice *pAlice;
    Status status;
};
