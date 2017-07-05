#pragma once

#include <oslib/oslib.h>
#include "gamedataloader.h"
#include "alice.h"

extern "C" void drawRegion(int x, int y, int width, int height);

class EndlessAlice {
public:
    // Pour creer l'instance de jeu (ou la modifier)
    static EndlessAlice *getScene(void);

    // Gestion de l'affichage et des touches
    void update(void);

    void draw(void);

    // Destructeur
    ~EndlessAlice(void);

// blah
    bool loadMap(char *file);

protected:
    // Constructeur protected
    EndlessAlice(void);

    // Pointeur vers le singleton EndlessAlice
    static EndlessAlice *pEndlessAlice;

    // Initialisation
    void gameInit(void);

    // Toutes les fonctions de mises a jour et d'afichage
    void updateMenu();

    void drawMenu();

    void updateStart();

    void drawStart();

    void updatePlay();

    void drawPlay();

    void updatePause();

    void drawPause();

    void updateGameOver();

    void drawGameOver();

    // Les enums
    enum Status {
        MENU, START, PLAY, PAUSE, OVER
    };
    enum Display {
        SCROLL, FULLSCREEN, FIT
    };
    // Les membres privates
    Alice *alice;               // Le joueur
    char map[20][15];           // Les données de la map
    OSL_IMAGE *drawBuffer;      // Le buffer pour le dessin
    GameDataLoader *gameData;   // Donnees de jeu
    Status status;              // L'etat du jeu
    Display display;            // Mode d'affichage
};