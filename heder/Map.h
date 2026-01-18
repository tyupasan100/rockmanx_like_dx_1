#pragma once


class Map 
{
public:
	Map();

	bool IsBlockAtPixel(int px, int py) const;
	int	 GetGroundY(int px, int py) const;
	int  GetGroundX(int px, int py) const;
	void DrawMap() const;

private:
	TileType tiles[MAP_H][MAP_W];
};