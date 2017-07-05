#include "endlessalice.h"

// Partie non existante pour le moment
EndlessAlice *EndlessAlice::pEndlessAlice = NULL;

// Constructeur
EndlessAlice::EndlessAlice(void) {
    // Recuperation des données
    gameData = GameDataLoader::getData();
    alice = Alice::getAlice();
    // Le buffer pour le dessin et le zoom simplifié
    drawBuffer = oslCreateImage(480, 360, OSL_IN_VRAM, OSL_PF_5650);
    oslClearImage(drawBuffer, 0xFF000000);
    // Affichage
    display = FULLSCREEN;
    drawBuffer->stretchX = 480;
    drawBuffer->stretchY = 270;
    // Démarrage du jeu
    loadMap("res/09.mpd");
}

// Destructeur
EndlessAlice::~EndlessAlice(void) {}

// Creation de l'instance de jeu
EndlessAlice *EndlessAlice::getScene(void) {
    if (pEndlessAlice == NULL) pEndlessAlice = new EndlessAlice();
    return pEndlessAlice;
}

// Fonctions de mise a jour
void EndlessAlice::update(void) {
    switch (status) {
        //	case MENU   : updateMenu(); break;
        //	case START  : updateStart(); break;
        case PLAY:
            updatePlay();
            break;
            //	case PAUSE : updatePause(); break;
            //	case GAMEOVER   : updateGameOver(); break;
        default :
            break;
    }
}

void EndlessAlice::updatePlay(void) {
    alice->update();
    if (osl_pad.pressed.triangle)
        switch (display) {
            case SCROLL:
                display = FULLSCREEN;
                drawBuffer->stretchX = 480;
                drawBuffer->stretchY = 270;
                drawBuffer->x = 0;
                drawBuffer->y = 0;
                break;
            case FULLSCREEN:
                display = FIT;
                drawBuffer->stretchX = 360;
                drawBuffer->stretchY = 270;
                drawBuffer->x = 60;
                drawBuffer->y = 1;
                break;
            case FIT:
            default:
                display = SCROLL;
                drawBuffer->stretchX = 480;
                drawBuffer->stretchY = 360;
                drawBuffer->x = 0;
                drawBuffer->y = 0;
                break;
        }
}


// Fonctions d'affichage
void EndlessAlice::draw(void) {
    switch (status) {
        //	case MENU   : drawMenu(); break;
        //	case START  : drawStart(); break;
        case PLAY:
            drawPlay();
            break;
            //	case PAUSE : drawPause(); break;
            //	case GAMEOVER   : drawGameOver(); break;
        default :
            break;
    }
}

void EndlessAlice::drawPlay(void) {
    // Variables
    int i, j;
    // On dessine sur le buffer
    oslSetDrawBuffer(drawBuffer);
    drawRegion(0, 0, 480, 360);
    // Background et Tiles
    oslDrawImage(gameData->Graphics.background);
    for (i = 0; i < 20; i++)
        for (j = 0; j < 15; j++) {
            oslSetImageFrame(gameData->Graphics.tiles, map[i][j]);
            oslDrawImageXY(gameData->Graphics.tiles, i * 24, j * 24);
        }
    // Alice
    alice->draw();
    // Retour au buffer normal (l'ecran)
    oslSetDrawBuffer(OSL_DEFAULT_BUFFER);
    drawRegion(0, 0, 480, 272);
    // Affichage
    oslClearScreen(0xFF000000);
    oslSetBilinearFilter(true);
    if (display == SCROLL) drawBuffer->y = alice->deltaY();
    oslDrawImage(drawBuffer);
    oslSetBilinearFilter(false);

}

// Chargement d'une map (testonly)
bool EndlessAlice::loadMap(char *file) {
    FILE *handle;
    char buffer[4];
    if ((handle = fopen(file, "rb")) == NULL) return false;
    if (fread(buffer, sizeof(char), 4, handle) != 4) {
        fclose(handle);
        return false;
    }
    if (fread(map, sizeof(char), 15 * 20, handle) != 300) {
        fclose(handle);
        return false;
    }
    fclose(handle);
    status = PLAY;
    return true;
}
