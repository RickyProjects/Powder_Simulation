#include "raylib.h"
#include <iostream>
#include <vector>
using namespace std;

//swaps locations
void swap(vector<vector<int>>& grid, int x1, int y1, int x2, int y2){
    int temp = grid[y1][x1];
    grid[y1][x1] = grid[y2][x2];
    grid[y2][x2] = temp;
}

void addSand(vector<vector<int>>& grid, int x1, int y1){
    grid[y1][x1] = 1;
}

void addStone(vector<vector<int>>& grid, int x1, int y1){
    grid[y1][x1] = 2;
}

void addWater(vector<vector<int>>& grid, int x1, int y1){
    grid[y1][x1] = 3;
}

void addLava(vector<vector<int>>& grid, int x1, int y1){
    grid[y1][x1] = 4;
}

void remove(vector<vector<int>>& grid, int x1, int y1){
    grid[y1][x1] = 0;
}

int getLength(vector<vector<int>>& grid){
    return grid[0].size();
}

int getHeight(vector<vector<int>>& grid){
    return grid.size();
}

bool airBelow(vector<vector<int>>& grid, int x, int y){
    int height = getHeight(grid);
    if (y == height - 1){
        return false;
    } else if (grid[y + 1][x] != 0){
        return false;
    } else {
        return true;
    }
}

bool airBottomLeft(vector<vector<int>>& grid, int x, int y){
    int height = getHeight(grid);
    if (y == height - 1){
        return false;
    } else if(x == 0) {
        return false;
    } else if (grid[y + 1][x - 1] != 0){
        return false;
    } else {
        return true;
    }
}

bool airBottomRight(vector<vector<int>>& grid, int x, int y){
    int height = getHeight(grid);
    int length = getLength(grid);

    if (y == height - 1){
        return false;
    } else if(x == length - 1) {
        return false;
    } else if (grid[y + 1][x + 1] != 0){
        return false;
    } else {
        return true;
    }
}

bool airLeft(vector<vector<int>>& grid, int x, int y) {
    if (x == 0) {
        return false;
    } else if (grid[y][x - 1] != 0) {
        return false;
    } else {
        return true;
    }
}

bool airRight(vector<vector<int>>& grid, int x, int y) {
    int length = getLength(grid);
    if (x == length - 1) {
        return false;
    } else if (grid[y][x + 1] != 0) {
        return false;
    } else {
        return true;
    }
}

bool airTopLeft(vector<vector<int>>& grid, int x, int y){
    if (y == 0) {
        return false;
    } else if (x == 0) {
        return false;
    } else if (grid[y - 1][x - 1] != 0 || grid[y - 1][x - 1] != 3) {
        return false;
    } else {
        return true;
    }
}

bool airTopRight(vector<vector<int>>& grid, int x, int y){
    int length = getLength(grid);
    if (y == 0) {
        return false;
    } else if (x == length - 1) {
        return false;
    } else if (grid[y - 1][x + 1] != 0 || grid[y - 1][x + 1] != 3) {
        return false;
    } else {
        return true;
    }
}

bool airAbove(vector<vector<int>>& grid, int x, int y){
    int height = getHeight(grid);
    if (y == 0){
        return false;
    } else if (grid[y - 1][x] == 0 || grid[y - 1][x] == 3){
        return true;
    } else {
        return false;
    }
}

void checkObsidian(vector<vector<int>>& grid) {
    int height = getHeight(grid);
    int length = getLength(grid);

    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < length; x++) {
            int material = grid[y][x];

            if (material == 6) {
                continue; 
            }

            // water touching lava
            if (material == 3) {
                if (y + 1 < height && grid[y + 1][x] == 4) {
                    grid[y + 1][x] = 5;
                    grid[y][x] = 6;
                } else if (x > 0 && grid[y][x - 1] == 4) {
                    grid[y][x - 1] = 5;
                    grid[y][x] = 6;
                } else if (x + 1 < length && grid[y][x + 1] == 4) {
                    grid[y][x + 1] = 5;
                    grid[y][x] = 6;
                }
            }

            // lava touching water
            else if (material == 4) {
                if (y + 1 < height && grid[y + 1][x] == 3) {
                    grid[y + 1][x] = 5;
                    grid[y][x] = 6;
                } else if (x > 0 && grid[y][x - 1] == 3) {
                    grid[y][x - 1] = 5;
                    grid[y][x] = 6;
                } else if (x + 1 < length && grid[y][x + 1] == 3) {
                    grid[y][x + 1] = 5;
                    grid[y][x] = 6;
                }
            }
        }
    }        
}

void displayGrid(vector<vector<int>>& grid){
    int length = getLength(grid);
    int height = getHeight(grid);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < length; x++) {
            cout << grid[y][x] << " ";
        }
        cout << endl;
    }        
    cout << endl;
}

void gravityTick(vector<vector<int>>& grid) {
    int length = getLength(grid);
    int height = getHeight(grid);

    static int lavaFrameCounter = 0;
    lavaFrameCounter++;
    bool lavaShouldMove = (lavaFrameCounter % 2 == 0);

    static int steamFrameCounter = 0;
    steamFrameCounter++;
    bool steamShouldMove = (steamFrameCounter % 2 == 0); // Adjust this as needed

    // static bool steamGoesLeft = false;
    // static int steamFrameCounter = 0;
    // steamFrameCounter++;
    // if (steamFrameCounter % 5 == 0) {
    //     steamGoesLeft = !steamGoesLeft;
    // }

    static vector<vector<int>> waterDir(height, vector<int>(length, 1));
    static vector<vector<int>> lavaDir(height, vector<int>(length, 1));

    for (int y = height - 2; y >= 0; y--) {
        for (int x = 0; x < length; x++) {
            int block = grid[y][x];

            if (block == 1) {
                if (airBelow(grid, x, y)) {
                    swap(grid, x, y, x, y + 1);
                } else if (airBottomLeft(grid, x, y)) {
                    swap(grid, x, y, x - 1, y + 1);
                } else if (airBottomRight(grid, x, y)) {
                    swap(grid, x, y, x + 1, y + 1);
                }
            } else if (block == 2){
                // stone doesnt move
            } else if (block == 3) {
                if (airBelow(grid, x, y)) {
                    swap(grid, x, y, x, y + 1);
                } else {
                    int d = waterDir[y][x];
                    bool moved = false;

                    if (d == 1 && airRight(grid, x, y)) {
                        swap(grid, x, y, x + 1, y);
                        waterDir[y][x + 1] = 1;
                        moved = true;
                    } else if (d == -1 && airLeft(grid, x, y)) {
                        swap(grid, x, y, x - 1, y);
                        waterDir[y][x - 1] = -1;
                        moved = true;
                    }

                    if (!moved) waterDir[y][x] *= -1;
                }
            } else if (block == 4) {
                if (!lavaShouldMove) continue;

                if (airBelow(grid, x, y)) {
                    swap(grid, x, y, x, y + 1);
                } else {
                    int d = lavaDir[y][x];
                    bool moved = false;

                    if (d == 1 && airRight(grid, x, y)) {
                        swap(grid, x, y, x + 1, y);
                        lavaDir[y][x + 1] = 1;
                        moved = true;
                    } else if (d == -1 && airLeft(grid, x, y)) {
                        swap(grid, x, y, x - 1, y);
                        lavaDir[y][x - 1] = -1;
                        moved = true;
                    }

                    if (!moved) lavaDir[y][x] *= -1;
                }
            } else if (block == 5){
                // obsidian (5) stays still
            } else if (block == 6) {
                if (!steamShouldMove) continue;
                
                if (airAbove(grid, x, y)) {
                    swap(grid, x, y, x, y - 1);
                } else if (airTopLeft(grid, x, y)){
                    swap(grid, x, y, x - 1, y - 1);
                } else if (airTopRight(grid, x, y)) {
                    swap(grid, x, y, x + 1, y - 1);
                }
            }
        }
    }
}

int main() {
    const int length = 100;
    const int height = 100;

    // main grid
    vector<vector<int>> grid(height, vector<int>(length, 0));

    // colors
    int material = 1;
    Color bg = {30, 30, 30, 255};
    Color sand = {240, 197, 79, 255};
    Color water = {77, 133, 255, 255};
    Color stone = {102, 105, 115, 255};
    Color lava = {255, 107, 54, 255};
    Color steam = {212, 228, 255, 255};
    Color obsidian = {79, 44, 125, 255};
    //Color selected = {138, 242, 131, 255};
    
    // window and scale
    const int winW = 1200, winH = 900;
    InitWindow(winW, winH, "Sand (raylib)");
    SetTargetFPS(120);

    // size of each cell in pixels; center the grid in the window
    const float cell = (float)min(winW / length, winH / height);
    const float drawW = cell * length;
    const float drawH = cell * height;
    const float offX = (winW - drawW) * 0.5f;
    const float offY = (winH - drawH) * 0.5f;

    while (!WindowShouldClose()) {

        // placement and removal of material
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 m = GetMousePosition();
            int gx = (int)((m.x - offX) / cell);
            int gy = (int)((m.y - offY) / cell);
            if (gx >= 0 && gx < length && gy >= 0 && gy < height) {
                if(material == 1){
                    addSand(grid, gx, gy);
                } else if (material == 2){
                    addStone(grid, gx, gy);
                } else if (material == 3){
                    addWater(grid, gx, gy);
                } else if (material == 4){
                    addLava(grid, gx, gy);
                }
                
            }
        } else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            Vector2 m = GetMousePosition();
            int gx = (int)((m.x - offX) / cell);
            int gy = (int)((m.y - offY) / cell);
            if (gx >= 0 && gx < length && gy >= 0 && gy < height) {
                remove(grid, gx, gy);
            }
        }

        // gravity step
        static int frameCounter = 0;
        frameCounter++;
        if (frameCounter >= 2) {
            checkObsidian(grid);
            gravityTick(grid);
            frameCounter = 0;
        }

        // drawing
        BeginDrawing();
        ClearBackground(bg);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < length; ++x) {
                if (grid[y][x] == 1) {
                    DrawRectangle((int)(offX + x * cell),
                                  (int)(offY + y * cell),
                                  (int)cell, (int)cell,
                                  sand);
                } else if (grid[y][x] == 2) {
                    DrawRectangle((int)(offX + x * cell),
                                  (int)(offY + y * cell),
                                  (int)cell, (int)cell,
                                  stone);
                } else if (grid[y][x] == 3) {
                    DrawRectangle((int)(offX + x * cell),
                                  (int)(offY + y * cell),
                                  (int)cell, (int)cell,
                                  water);
                } else if (grid[y][x] == 4) {
                    DrawRectangle((int)(offX + x * cell),
                                  (int)(offY + y * cell),
                                  (int)cell, (int)cell,
                                  lava);
                } else if (grid[y][x] == 5) {
                    DrawRectangle((int)(offX + x * cell),
                                  (int)(offY + y * cell),
                                  (int)cell, (int)cell,
                                  obsidian);
                } else if (grid[y][x] == 6) {
                    DrawRectangle((int)(offX + x * cell),
                                  (int)(offY + y * cell),
                                  (int)cell, (int)cell,
                                  steam);
                }
            }
        }
        // material selection
        Vector2 m = GetMousePosition();
        int x = (int)m.x;
        int y = (int)m.y;
        
        // sand button
        if (x >= 15 && x <= 139 && y >= 15 && y <= 139 && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            DrawRectangle(10, 10, 129, 129, sand);
        } else if (x >= 15 && x <= 139 && y >= 15 && y <= 139 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            DrawRectangle(17, 17, 115, 115, sand);
            material = 1;
        } else {
            DrawRectangle(15, 15, 119, 119, sand);
        }

        // stone button
        if (x >= 15 && x <= 139 && y >= 165 && y <= 289 && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            DrawRectangle(10, 160, 129, 129, stone);
        } else if (x >= 15 && x <= 139 && y >= 165 && y <= 289 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            DrawRectangle(17, 167, 115, 115, stone);
            material = 2;
        } else {
            DrawRectangle(15, 165, 119, 119, stone);
        }

        // water button
        if (x >= 15 && x <= 139 && y >= 315 && y <= 439 && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            DrawRectangle(10, 310, 129, 129, water);
        } else if (x >= 15 && x <= 139 && y >= 315 && y <= 439 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            DrawRectangle(17, 317, 115, 115, water);
            material = 3;
        } else {
            DrawRectangle(15, 315, 119, 119, water);
        }

        // lava button
        if (x >= 15 && x <= 139 && y >= 465 && y <= 589 && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            DrawRectangle(10, 460, 129, 129, lava);
        } else if (x >= 15 && x <= 139 && y >= 465 && y <= 589 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            DrawRectangle(17, 467, 115, 115, lava);
            material = 4;
        } else {
            DrawRectangle(15, 465, 119, 119, lava);
        }

        // grid outline
        
        DrawRectangleLines((int)offX, (int)offY, (int)drawW, (int)drawH, RAYWHITE);
        DrawFPS(10, winH - 25);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}


