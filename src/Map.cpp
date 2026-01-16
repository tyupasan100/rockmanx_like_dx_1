#include "Common.h"

Map::Map()
{
    for (int y = 0; y < MAP_H; ++y)
        for (int x = 0; x < MAP_W; ++x)
            tiles[y][x] = 0;

    for (int x = 0; x < MAP_W; ++x)
        tiles[MAP_H - 1][x] = 1;

    tiles[MAP_H - 2][10] = 1;
    tiles[MAP_H - 2][11] = 1;
    tiles[MAP_H - 2][12] = 1;
    tiles[MAP_H - 2][13] = 1;
    tiles[MAP_H - 2][14] = 1;
    tiles[MAP_H - 3][12] = 1;
}


bool Map::IsBlockAtPixel(int px, int py) const
{
	int tx = px / TILE_SIZE;
	int ty = py / TILE_SIZE;

	if (tx < 0 || tx >= MAP_W || ty < 0 || ty >= MAP_H)
		return false;

	return tiles[ty][tx] == 1;
}

void Map::DrawMap() const
{
    for (int y = 0; y < MAP_H; ++y)
    {
        for (int x = 0; x < MAP_W; ++x)
        {
            if (tiles[y][x] == 1)
            {
                DrawBox(
                    x * TILE_SIZE,
                    y * TILE_SIZE,
                    (x + 1) * TILE_SIZE,
                    (y + 1) * TILE_SIZE,
                    GetColor(100, 100, 100),
                    TRUE
                );
            }
        }
    }
}