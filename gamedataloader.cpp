#include "gamedataloader.h"

// L'instance est NULL par defaut
GameDataLoader* GameDataLoader::instance = NULL;

// Nom de fichier temporaire
char* GameDataLoader::tempFile = "ram:/temp.dat";

// Constructeur
GameDataLoader::GameDataLoader() {
	bool loaded = loadData();
	if (!loaded) {
		oslWaitKey();
		oslQuit();
	}
}

// Destructeur
GameDataLoader::~GameDataLoader() {
}

// Recuperer l'instance
GameDataLoader* GameDataLoader::getData(void) {
	if (instance == NULL) instance = new GameDataLoader();
	return instance;
}

// Affichage de l'avancement
void GameDataLoader::showLoadingScreen(int index, int total, bool error) {
	static OSL_IMAGE* loadImage = NULL;
	if (loadImage == NULL) {
		loadImage = oslCreateImage(48, 8, OSL_IN_VRAM, OSL_PF_8888);
		oslClearImage(loadImage, 0xFF000000);
		oslSetDrawBuffer(loadImage);
		oslSetTextColor(0xFFCCCCCC);
		oslSetBkColor(0x00000000);
		oslDrawString(0, 0, "Loading");
		oslSetDrawBuffer(OSL_DEFAULT_BUFFER);
		loadImage->stretchX <<= 2;
		loadImage->stretchY <<= 2;
		loadImage->x = 144;
		loadImage->y = 96;
	}
	oslClearScreen(0xFF000000);
	oslDrawGradientRect(40, 224, 440, 232, 0xFF606060, 0xFF606060, 0xFFA0A0A0, 0xFFA0A0A0);
	oslDrawFillRect(40, 224, 40+(400*index/(total+1)), 232, 0xFFFFFFFF);
	oslDrawImage(loadImage);
	//oslPrintf("Loading %d of %d (%s)\n", index, total, (error) ? "Failed" : "OK");
	if (error) {
		oslSetTextColor(0xFF0000FF);
		oslDrawString(2 + ((480-7*(46+12))>>1), 180, "Unable to load gamedata.pak, please consider reinstalling.");
		oslDrawString(2 + ((480 - 7*30)>>1), 194, "The application will now exit.");
	}
	if (index == total || error) {
		oslDeleteImage(loadImage);
		loadImage = NULL;
	}
}

// Gestion de l'affichage de l'avancement
bool GameDataLoader::refreshDisplay(bool status) {
	static int index = 0;
	index++;
	oslStartDrawing();
	GameDataLoader::showLoadingScreen(index, 16, !status);
	oslEndDrawing();
	oslEndFrame();
	oslSyncFrame();
	return !status;
}

// Charger une image
bool GameDataLoader::loadImage(char *file, OSL_IMAGE **img, int location, int pixelFormat) {
	if (((*img) = oslLoadImageFilePNG(file, location, pixelFormat)) != NULL) return true;
	if (((*img) = oslLoadImageFileJPG(file, location, pixelFormat)) != NULL) return true;
	if (((*img) = oslLoadImageFileGIF(file, location, pixelFormat)) != NULL) return true;
	return false;
}

// Charger un son
bool GameDataLoader::loadSound(char *file, OSL_SOUND **snd, int stream) {
	if (((*snd) = oslLoadSoundFileWAV(file, stream)) != NULL) return true;
	if (((*snd) = oslLoadSoundFileBGM(file, stream)) != NULL) return true;
	// MP3, MOD et AT3 produisent des erreurs de link (probablement un probleme de config de mon cote)
//	if (((*snd) = oslLoadSoundFileMP3(file, stream)) != NULL) return true;
//	if (((*snd) = oslLoadSoundFileMOD(file, stream)) != NULL) return true;
//	if (((*snd) = oslLoadSoundFileAT3(file, stream)) != NULL) return true;
	return false;
}

// Chargement d'une image depuis le fichier
bool GameDataLoader::loadImageData(FILE *handle, int dataSize, OSL_IMAGE **img, int location, int pixelFormat) {
	// Variables
	bool imageLoaded;
	OSL_VIRTUALFILENAME virtualFile;
	char* buffer = new char[dataSize];
	// Test
	if (buffer == NULL) return false;
	// Chargement du fichier
	if (fread(buffer, sizeof(char), dataSize, handle) != (unsigned int)dataSize) {
		delete[] buffer;
		return false;
	}
	// Donnees du fichier virtuel
	virtualFile.name = tempFile;
	virtualFile.data = (void*)buffer;
	virtualFile.size = dataSize;
	virtualFile.type = &VF_MEMORY;
	oslAddVirtualFileList(&virtualFile, 1);
	// Chargement de l'image
	imageLoaded = GameDataLoader::loadImage(tempFile, img, location, pixelFormat);
	// Nettoyage
	oslRemoveVirtualFileList(&virtualFile, 1);
	delete[] buffer;
	return imageLoaded;
}

// Chargement d'un son depuis le fichier de donnees
bool GameDataLoader::loadSoundData(FILE *handle, int dataSize, OSL_SOUND **snd, int stream) {
	// Variables
	bool soundLoaded;
	OSL_VIRTUALFILENAME virtualFile;
	char* buffer = new char[dataSize];
	// Test
	if (buffer == NULL) return false;
	// Chargement du fichier
	if (fread(buffer, sizeof(char), dataSize, handle) != (unsigned int)dataSize) {
		delete[] buffer;
		return false;
	}
	// Donnees du fichier virtuel
	virtualFile.name = tempFile;
	virtualFile.data = (void*)buffer;
	virtualFile.size = dataSize;
	virtualFile.type = &VF_MEMORY;
	oslAddVirtualFileList(&virtualFile, 1);
	// Chargement de l'image
	soundLoaded = GameDataLoader::loadSound(tempFile, snd, stream);
	// Nettoyage
	oslRemoveVirtualFileList(&virtualFile, 1);
	delete[] buffer;
	return soundLoaded;
}

// Chargement de donnees custom
bool GameDataLoader::loadCustomData(FILE* handle, int dataSize, char **data) {
	// Taille des donnees
	(*data) = new char[dataSize];
	// Test
	if ((*data) == NULL) return false;
	// Chargement des donnees
	if (fread((*data), sizeof(char), dataSize, handle) != (unsigned int)dataSize) {
		delete[] (*data);
		return false;
	}
	return true;
}

// Chargement du fichier de donnees
bool GameDataLoader::loadData(void) {
	FILE* handle = fopen("res/gamedata.pak","rb");
	if (handle == NULL) {
		refreshDisplay(false);
		return false;
	}
	if (refreshDisplay(loadImageData(handle, 5827, &Graphics.mushroomBlack, OSL_IN_RAM, OSL_PF_8888))) return false;
		oslSetImageFrameSize(Graphics.mushroomBlack, 21, 21);
		oslSetImageFrame(Graphics.mushroomBlack, 0);
	if (refreshDisplay(loadImageData(handle, 5917, &Graphics.mushroomBlue, OSL_IN_RAM, OSL_PF_8888))) return false;
		oslSetImageFrameSize(Graphics.mushroomBlue, 21, 21);
		oslSetImageFrame(Graphics.mushroomBlue, 0);
	if (refreshDisplay(loadImageData(handle, 5549, &Graphics.mushroomGreen, OSL_IN_RAM, OSL_PF_8888))) return false;
		oslSetImageFrameSize(Graphics.mushroomGreen, 21, 21);
		oslSetImageFrame(Graphics.mushroomGreen, 0);
	if (refreshDisplay(loadImageData(handle, 5934, &Graphics.mushroomRed, OSL_IN_RAM, OSL_PF_8888))) return false;
		oslSetImageFrameSize(Graphics.mushroomRed, 21, 21);
		oslSetImageFrame(Graphics.mushroomRed, 0);
	if (refreshDisplay(loadImageData(handle, 5947, &Graphics.mushroomYellow, OSL_IN_RAM, OSL_PF_8888))) return false;
		oslSetImageFrameSize(Graphics.mushroomYellow, 21, 21);
		oslSetImageFrame(Graphics.mushroomYellow, 0);
	if (refreshDisplay(loadImageData(handle, 11418, &Graphics.alice, OSL_IN_RAM, OSL_PF_8888))) return false;
		oslSetImageFrameSize(Graphics.alice, 24, 24);
		oslSetImageFrame(Graphics.alice, 0);
	if (refreshDisplay(loadImageData(handle, 2465, &Graphics.life, OSL_IN_RAM, OSL_PF_8888))) return false;
		oslSetImageFrameSize(Graphics.life, 24, 24);
		oslSetImageFrame(Graphics.life, 0);
	if (refreshDisplay(loadImageData(handle, 3097, &Graphics.marisa, OSL_IN_RAM, OSL_PF_8888))) return false;
		oslSetImageFrameSize(Graphics.marisa, 24, 24);
		oslSetImageFrame(Graphics.marisa, 0);
	if (refreshDisplay(loadImageData(handle, 1613, &Graphics.tiles, OSL_IN_RAM, OSL_PF_8888))) return false;
		oslSetImageFrameSize(Graphics.tiles, 24, 24);
		oslSetImageFrame(Graphics.tiles, 0);
	if (refreshDisplay(loadImageData(handle, 12089, &Graphics.face, OSL_IN_RAM, OSL_PF_8888))) return false;
		oslSetImageFrameSize(Graphics.face, 48, 36);
		oslSetImageFrame(Graphics.face, 0);
	if (refreshDisplay(loadImageData(handle, 10947, &Graphics.background, OSL_IN_RAM, OSL_PF_8888))) return false;
	if (refreshDisplay(loadImageData(handle, 24450, &Graphics.ending, OSL_IN_RAM, OSL_PF_8888))) return false;
	if (refreshDisplay(loadImageData(handle, 767, &Graphics.gold, OSL_IN_RAM, OSL_PF_8888))) return false;
	if (refreshDisplay(loadImageData(handle, 1129, &Graphics.shangai, OSL_IN_RAM, OSL_PF_8888))) return false;
	if (refreshDisplay(loadImageData(handle, 11126, &Graphics.start, OSL_IN_RAM, OSL_PF_8888))) return false;
	if (refreshDisplay(loadImageData(handle, 24200, &Graphics.title, OSL_IN_RAM, OSL_PF_8888))) return false;
	fclose(handle);
	return true;
}
