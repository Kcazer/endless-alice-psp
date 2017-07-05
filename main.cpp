//The main library OSLib
#include <oslib/oslib.h>
#include <stdlib.h>
#include <time.h>
#include "endlessalice.h"

//The callbacks
PSP_MODULE_INFO("Endless Alice", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int main() {
    //Initialisation
    srand(time(0));
    oslInit(0);
    oslInitGfx(OSL_PF_5650, 1);
    oslInitAudio();

    EndlessAlice *sc = EndlessAlice::getScene();

    //Boucle principale
    while (!osl_quit) {
        // Debut drawing
        oslStartDrawing();
        // Lecture des touches
        oslReadKeys();
        // Update
        sc->update();
        sc->draw();

        // Fin drawing
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();
    }

    // Nettoyage de la memoire

    //Leave the program
    oslDeinitAudio();
    oslEndGfx();
    oslQuit();
    return 0;
}