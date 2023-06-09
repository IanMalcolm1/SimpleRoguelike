#pragma once

#include <unordered_set>

struct TileCoords {
	int x, y;

	TileCoords(int x = -1, int y = -1) : x(x), y(y) {};

	bool operator == (const TileCoords coords) const {
		return (x == coords.x && y == coords.y);
	}

	bool operator != (const TileCoords coords) const {
		return (!(x == coords.x && y == coords.y));
	}

	TileCoords operator + (const TileCoords coords) const {
		return TileCoords(x + coords.x, y + coords.y);
	}

   bool isAdjacentTo(TileCoords coords) {
      if (x-coords.x <= 1 && x-coords.x >= -1 &&
            y-coords.y <= 1 && y-coords.y >= -1)
         return true;

      return false;
   }

   struct HashFunction {
      std::size_t operator()(const TileCoords& coord) const {
         std::size_t xHash = std::hash<int>()(coord.x);
         std::size_t yHash = std::hash<int>()(coord.y) << 1;
         return xHash ^ yHash;
      }
   };
};
