#pragma once


class Map 
{
public:
	Map();

	bool IsBlockAtPixel(int px, int py) const;
	void DrawMap() const;

private:
	int tiles[MAP_H][MAP_W];
};