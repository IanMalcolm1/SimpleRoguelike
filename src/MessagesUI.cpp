#include "Interface/UIScreens/MessagesUI.h"


void MessagesUI::initialize(GameLog* log, SDL_Renderer* renderer, SDL_Texture* spritesheet) {
   this->log = log;

	this->renderer = renderer;
	this->spritesheet = spritesheet;

	textRenderer.initialize(renderer, spritesheet);
}


void MessagesUI::render(const SDL_Rect& viewport) {
	SDL_RenderSetViewport(renderer, &viewport);

	std::vector<GameText>* recentMessages = log->getRecentMessages();

	if (recentMessages->size() == 0) {
		return;
	}

	mainViewport.x = viewport.x;
	mainViewport.y = viewport.y;
	mainViewport.h = viewport.h;

	if (viewport.w != mainViewport.w) {
		mainViewport.w = viewport.w;
		makeFormattedMessages();
	}
	else if (recentMessages->size() != formattedMsgs.size()) {
		makeFormattedMessages();
	}
	
	int startY = viewport.h + startOffset;

	for (int i = formattedMsgs.size()-1; i >= 0; i--) {
		if (startY < 0) {
			break;
		}

		if (startY - formattedMsgs[i].getHeight() > mainViewport.h) {
			startY -= (formattedMsgs[i].getHeight() + textSpecs.messageSpacing);
			continue;
		}

		startY = textRenderer.renderFormattedText(textSpecs, formattedMsgs.at(i), startY, (TEXT_RENDER_UP|TEXT_ALIGN_LEFT));
		startY -= textSpecs.messageSpacing;
	}
}

void MessagesUI::processScroll(int x, int y, int offset, bool ctrlDown) {
	if (ctrlDown) {
		textSpecs.modifyFontSize(offset);
		makeFormattedMessages();
	}
	else {
		startOffset += offset * textSpecs.fontSizePixels;
	}

	if (startOffset < -textSpecs.margin || totalHeight < mainViewport.h - textSpecs.margin) {
		startOffset = -textSpecs.margin;
	}
	else if (totalHeight < startOffset + mainViewport.h - textSpecs.margin) {
		startOffset = totalHeight - mainViewport.h + textSpecs.margin;
	}
}

void MessagesUI::makeFormattedMessages() {
	std::vector<GameText>* recentMessages = log->getRecentMessages();
	int entriesAdded;

	textSpecs.setViewportWidth(mainViewport.w);

	entriesAdded = recentMessages->size() - formattedMsgs.size();

	if (entriesAdded<=0) {
		formattedMsgs.clear();
		totalHeight = 0;
		entriesAdded = recentMessages->size();
	}

	for (int i = formattedMsgs.size(); i < recentMessages->size(); i++) {
		FormattedText fMessage = textRenderer.formatGameText(textSpecs, recentMessages->at(i));
		totalHeight += fMessage.getHeight();
		formattedMsgs.push_back(fMessage);
	}

   if (entriesAdded == recentMessages->size()) {
	   totalHeight += (entriesAdded-1) * textSpecs.messageSpacing;
   }
   else {
	   totalHeight += entriesAdded * textSpecs.messageSpacing;
   }
}
