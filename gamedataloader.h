#pragma once

#include <oslib/oslib.h>

class GameDataLoader {
public:
    ~GameDataLoader();

    static GameDataLoader *getData(void);

    static void showLoadingScreen(int index, int total, bool error);

    static bool loadImage(char *file, OSL_IMAGE **img, int location, int pixelFormat);

    static bool loadSound(char *file, OSL_SOUND **snd, int stream);

    struct {
        OSL_IMAGE *mushroomBlack;
        OSL_IMAGE *mushroomBlue;
        OSL_IMAGE *mushroomGreen;
        OSL_IMAGE *mushroomRed;
        OSL_IMAGE *mushroomYellow;
        OSL_IMAGE *alice;
        OSL_IMAGE *life;
        OSL_IMAGE *marisa;
        OSL_IMAGE *tiles;
        OSL_IMAGE *face;
        OSL_IMAGE *background;
        OSL_IMAGE *ending;
        OSL_IMAGE *gold;
        OSL_IMAGE *shangai;
        OSL_IMAGE *start;
        OSL_IMAGE *title;
    } Graphics;
protected:
    GameDataLoader(void);

    bool loadData(void);

    bool refreshDisplay(bool);

    bool loadSoundData(FILE *handle, int dataSize, OSL_SOUND **snd, int stream);

    bool loadImageData(FILE *handle, int dataSize, OSL_IMAGE **img, int location, int pixelFormat);

    bool loadCustomData(FILE *handle, int dataSize, char **data);

private:
    static GameDataLoader *instance;
    static char *tempFile;
};
