#include "Interface/UIScreens/AdventureUI.h"


void AdventureUI::initialize(SDL_Renderer* renderer, SDL_Texture* spritesheet) {
   this->renderer = renderer;
   this->spritesheet = spritesheet;

   viewports.fullscreen = {0,0,0,0};

   sceneUI.initialize(adventure->getPlayerManager(), renderer, spritesheet);
   messagesUI.initialize(adventure->getGameLog(), renderer, spritesheet);
   playerUI.initialize(adventure, renderer, spritesheet);
   lookUI.initialize(adventure, renderer, spritesheet);
}

void AdventureUI::cleanUp() {
   sceneUI.cleanUp();
}

void AdventureUI::render(const SDL_Rect& screenDimensions) {
   if (screenDimensions.h != viewports.fullscreen.h ||
      screenDimensions.w != viewports.fullscreen.w) {
      updateViewports(screenDimensions);
   }

   sceneUI.render(adventure->getScene(), viewports.map);
   resetRendererAndDrawBorder(viewports.map);

   messagesUI.render(viewports.messages);
   resetRendererAndDrawBorder(viewports.messages);

   playerUI.render(viewports.player);
   resetRendererAndDrawBorder(viewports.player);

   lookUI.render(adventure->getScene(), viewports.look);
   resetRendererAndDrawBorder(viewports.look);
}

void AdventureUI::processCursorLocation(int x, int y) {
   sceneUI.processCursorLocation(x, y);
}
void AdventureUI::processCursorMovement() {
   adventure->getScene()->getMap()->flagMouseMoved();
}
void AdventureUI::processScroll(int x, int y, int offset, bool ctrlDown) {
	SDL_Point point = { x,y };

	if (SDL_PointInRect(&point, &viewports.map)) {
      sceneUI.processScroll(x, y, offset);
	}
	if (SDL_PointInRect(&point, &viewports.messages)) {
      messagesUI.processScroll(x, y, offset, ctrlDown);
	}
	if (SDL_PointInRect(&point, &viewports.player)) {
      playerUI.processScroll(x, y, offset, ctrlDown);
	}
	if (SDL_PointInRect(&point, &viewports.look)) {
      lookUI.processScroll(x, y, offset, ctrlDown);
	}
}
void AdventureUI::processClick(int x, int y, bool isRightClick) {
   sceneUI.processClick(x, y, isRightClick);
}
void AdventureUI::processKeyPress(SDL_Keycode keycode) {
   return;
}




void AdventureUI::updateViewports(const SDL_Rect& screenDimensions) {
	int screenBorderSpacing = 5;
	int viewportSpacing = 8;

   viewports.fullscreen = screenDimensions;

	/* x-axis */
	viewports.player.x = viewports.look.x = screenBorderSpacing;
	viewports.player.w = viewports.look.w = (1 * screenDimensions.w / 5) - (screenBorderSpacing + viewportSpacing/2);

	viewports.map.x = (1 * screenDimensions.w / 5) + (viewportSpacing/2);
	viewports.map.w = (4 * screenDimensions.w / 5) - (screenBorderSpacing + viewportSpacing/2);

	viewports.messages.x = (1 * screenDimensions.w / 5) + (viewportSpacing/2);
	viewports.messages.w = (4 * screenDimensions.w / 5) - (screenBorderSpacing + viewportSpacing/2);

	/* y-axis */
	viewports.player.y = screenBorderSpacing;
	viewports.player.h = screenDimensions.h/2 - viewportSpacing/2;

   viewports.look.y = viewports.player.y + viewports.player.h + viewportSpacing;
   viewports.look.h = (screenDimensions.h - viewports.look.y) - screenBorderSpacing;

	viewports.map.y = screenBorderSpacing;
	viewports.map.h = (4 * screenDimensions.h / 5) - (screenBorderSpacing + viewportSpacing/2);

	viewports.messages.y = (4 * screenDimensions.h / 5) + (viewportSpacing/2);
	viewports.messages.h = (1 * screenDimensions.h / 5) - (screenBorderSpacing + viewportSpacing/2);
}

void AdventureUI::resetRendererAndDrawBorder(SDL_Rect& currentViewport) {
	SDL_RenderSetViewport(renderer, &viewports.fullscreen);
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &currentViewport);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetTextureAlphaMod(spritesheet, 255);
	SDL_SetTextureColorMod(spritesheet, 255, 255, 255);
}



void AdventureUI::processWindowSizeChange() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	sceneUI.processWindowSizeChange();
#endif
}