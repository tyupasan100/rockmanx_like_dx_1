#include "Common.h"

Map::Map()
{
    for (int y = 0; y < MAP_H; ++y)
        for (int x = 0; x < MAP_W; ++x)
            tiles[y][x] = TileType::Air;

    for (int x = 0; x < MAP_W; ++x)
        tiles[MAP_H - 1][x] = TileType::Block;

    for (int x = 0; x < MAP_W; ++x)
        tiles[MAP_H - 3][x] = TileType::Block;

    tiles[MAP_H - 4][10] = TileType::Block;
    tiles[MAP_H - 4][11] = TileType::Block;
    tiles[MAP_H - 4][12] = TileType::Block;
    tiles[MAP_H - 5][11] = TileType::Block;

    tiles[MAP_H - 4][9] = TileType::SlopeUpRight;
    tiles[MAP_H - 5][10] = TileType::SlopeUpRight;
    tiles[MAP_H - 5][12] = TileType::SlopeUpLeft;
    tiles[MAP_H - 4][13] = TileType::SlopeUpLeft;

    tiles[MAP_H - 4][17] = TileType::Block;
    tiles[MAP_H - 5][17] = TileType::Block;
    tiles[MAP_H - 6][17] = TileType::Block;

    for (int y = 0; y < MAP_H; ++y)
        tiles[y][0] = TileType::Block;
}


bool Map::IsBlockAtPixel(int px, int py) const
{
	int tx = px / TILE_SIZE;
	int ty = py / TILE_SIZE;

	if (tx < 0 || tx >= MAP_W || ty < 0 || ty >= MAP_H)
		return false;

	return tiles[ty][tx] == TileType::Block;
}

int Map::GetGroundY(int px, int py) const
{
    int tx = px / TILE_SIZE;
    int ty = py / TILE_SIZE;

    TileType t = tiles[ty][tx];

    int localX = px % TILE_SIZE;

    switch (t)
    {
    case TileType::Block:
        return ty * TILE_SIZE;

    case TileType::SlopeUpRight:
        // 左=低い、右=高い
        return ty * TILE_SIZE + (TILE_SIZE - localX);

    case TileType::SlopeUpLeft:
        return ty * TILE_SIZE + localX +1;

    default:
        return -1;
    }
}

int Map::GetGroundX(int px, int py) const
{
    int tx = px / TILE_SIZE;
    int ty = py / TILE_SIZE;

    TileType t = tiles[ty][tx];

    int localY = py % TILE_SIZE;

    switch (t)
    {
    case TileType::Block:
        return tx * TILE_SIZE;

    case TileType::SlopeUpRight:
        // 左=低い、右=高い
        return tx * TILE_SIZE + (TILE_SIZE - localY - 1);

    case TileType::SlopeUpLeft:
        return tx * TILE_SIZE + localY;

    default:
        return -1;
    }
}

void Map::DrawMap() const
{
    for (int y = 0; y < MAP_H; ++y)
    {
        for (int x = 0; x < MAP_W; ++x)
        {
            switch (tiles[y][x])
            {
            case TileType::Block:
                DrawBox(x * TILE_SIZE, y * TILE_SIZE, (x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE, GetColor(100, 100, 100), TRUE);
                break;

            case TileType::SlopeUpRight:
                // 左=低い、右=高い
                DrawTriangle(x * TILE_SIZE, (y + 1) * TILE_SIZE, (x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE, (x + 1) * TILE_SIZE, y * TILE_SIZE, GetColor(200, 100, 100), TRUE);
                break;

            case TileType::SlopeUpLeft:
                DrawTriangle(x * TILE_SIZE, y * TILE_SIZE, (x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE, x * TILE_SIZE, (y + 1) * TILE_SIZE, GetColor(100, 100, 200), TRUE);
                break;

            default:
                break;
            }
        }
    }
}