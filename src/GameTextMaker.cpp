#include "GraphicsThings/GameTextMaker.h"
#include "Logs/DebugLogger.h"
#include <stack>


GameText GameTextMaker::makeGameText(std::string rawText) {
	TextColorMap textColorMap;

	std::stack<Color> colorStack;
	colorStack.push(Color(255, 255, 255));

	std::string newText;
	int newTextIndex = -1;

	for (int i = 0; i < rawText.size(); i++) {
		if (rawText[i] == '<' && rawText[i + 1] == '/') {
			if (newTextIndex != -1) {
				textColorMap.addColorNode(colorStack.top(), newTextIndex);
			}

			colorStack.push(readColor(i, rawText));
		}

		else if (rawText[i] == '\\' && rawText[i + 1] == '>') {
			textColorMap.addColorNode(colorStack.top(), newTextIndex);
			colorStack.pop();

			i++;
		}

		else {
			newText.push_back(rawText[i]);
			newTextIndex++;
		}
	}

	//the final color, if it's needed
	if (textColorMap.empty()) {
		textColorMap.addColorNode(colorStack.top(), newTextIndex);
	}
	else {
		if (textColorMap.lastIndex() != newTextIndex) {
			textColorMap.addColorNode(colorStack.top(), newTextIndex);
		}
	}

	return GameText(newText, textColorMap);
}

int GameTextMaker::readColorRGBValue(int& index, std::string& text) {
	std::string value;

	for (int counter = 0; counter < 3; counter++) {
		if (index >= text.size()) {
			DebugLogger::log("Game message format error: Reading RGB values exceeded length of string\n" + text);
			return 0;
		}
		else {
			value.push_back(text[index]);
			index++;
		}
	}
	return std::stoi(value);
}


Color GameTextMaker::readColorByRGB(int& index, std::string& text) {
	Color color;

	color.r = readColorRGBValue(index, text);
	color.g = readColorRGBValue(index, text);
	color.b = readColorRGBValue(index, text);

	return color;
}

Color GameTextMaker::readColorByColorName(int& index, std::string& text) {
	std::string colorName;

	while (index < text.size()) {
		if (text[index] == ':') {
			return colorMap.getColor(colorName);
		}
		else {
			colorName.push_back(text[index]);
		}

		index++;
	}

	DebugLogger::log("Game message format error: End of string reached while trying to parse color name\n" + text);
	return Color(255, 255, 255);
}


Color GameTextMaker::readColor(int& index, std::string& text) {
	index += 2;

	if (text[index] >= '0' && text[index] <= '9') {
		return readColorByRGB(index, text);
	}

	else {
		return readColorByColorName(index, text);
	}
}
