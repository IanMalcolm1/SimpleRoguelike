#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "GraphicsThings/TextRenderer.h"
#include "Logs/GameLog.h"


class MessagesUI {
private:
	GameLog* log;

	SDL_Renderer* renderer;
	SDL_Texture* spritesheet;

	SDL_Rect mainViewport;
	int totalHeight;
	std::vector<std::pair<std::string, int>> formattedMsgs;

	TextRenderer textRenderer;
	TextRenderingSpecs textSpecs;

	void makeFormattedMessages();

public:
	MessagesUI(int fontSize = 3) : renderer(NULL), spritesheet(NULL),
   mainViewport({0,0,0,0}), totalHeight(0), log(nullptr),
	textRenderer(), textSpecs(fontSize) {};

	void initialize(GameLog* log, SDL_Renderer* renderer, SDL_Texture* spritesheet);

	void render(const SDL_Rect& viewport);

	void processScroll(int x, int y, int offset, bool ctrlDown);
};
