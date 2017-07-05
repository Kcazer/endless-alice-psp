#include "alice.h"

// Pour les sauts
int Alice::verticalSpeed[13] = {6, 5, 5, 5, 4, 3, 3, 3, 2, 1, 1, 1, 0};

// Partie non existante pour le moment
Alice *Alice::pAlice = NULL;

// Constructeur protected
Alice::Alice(void) {
    sprite = GameDataLoader::getData()->Graphics.alice;
    reset();
}

// Destructeur
Alice::~Alice(void) {}

// Constructeur de singleton
Alice *Alice::getAlice(void) {
    if (pAlice == NULL) pAlice = new Alice();
    return pAlice;
}

// Reset
void Alice::reset(void) {
    status = STAND;
    posX = 50;
    posY = 50;
}

// Desinner Alice
void Alice::draw(void) {
    //oslDrawFillRect(0.75*(posX-8), 0.75*(posY-30), 0.75*(posX+8), 0.75*posY, 0xFF0000FF);
    oslDrawImageXY(sprite, 0.75 * (posX - 16), 0.75 * (posY - 32));
}

// Mise à jour
void Alice::update(void) {
    // Horizontal
    if (osl_pad.held.left) {
        posX -= 3;
        if (oslImageIsMirroredH(sprite)) oslMirrorImageH(sprite);
    } else if (osl_pad.held.right) {
        posX += 3;
        if (!oslImageIsMirroredH(sprite)) oslMirrorImageH(sprite);
    }
    // Vertical
    if (osl_pad.held.up) posY -= 7;
    else if (osl_pad.held.down) posY += 7;
}

// Pour l'affichage en mode scroll
int Alice::deltaY(void) {
    if (posY < 224) return 0;
    if (posY > 312) return -88;
    return (224 - posY);

}
