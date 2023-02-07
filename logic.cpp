#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "logic.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
ifstream myFile;
myFile.open(fileName);
if(!myFile.is_open()){
    return nullptr;
}
else if(myFile.is_open()){
        string line;
        std::stringstream inSS;
		getline(myFile,line);
		if(line.size() == 0  || line[0] =='0'){
			return nullptr;
		}
		if(!(isdigit(line[0]))){
			return nullptr;
		}
        inSS << line;
        inSS >> maxRow;
        inSS >> maxCol;
        string line1;
        getline(myFile,line1);
//        cout << "player coords" << line1 << endl;
        std::stringstream inSSS;
        inSSS << line1;
        int rOw;
        int cOl;
        inSSS >> rOw;
        inSSS >> cOl;
        player.row = rOw;
        player.col = cOl;
//       cout << "rOw cOl " << rOw << " " << cOl << endl;
//        cout << "player coords " << player.row << " " << player.col << endl;
        player.col;
        bool point;
        char c;
        char** map = createMap(maxRow,maxCol);
        for(int i = 0; i < maxRow; i++){
            for(int j = 0; j < maxCol; j++){
            point = true;
            while (point){
                myFile.get(c);
                if(c == ' ' || c == '\n' || (c!='0'&&c!='$'&&c!='@'&&c!='M'&&c!='+'&&c!='-'&&c!='?'&&c!='!')){
                }
                else{
                map[i][j] = c;
                point = false;
                }
            }
            }
        }
        map[player.row][player.col] = 'o';
return map;
    }
    return nullptr;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    if(input == MOVE_DOWN){
        ++nextRow;
    }
    else if(input == MOVE_UP){
        --nextRow;
    }
    else if(input == MOVE_RIGHT){
        ++nextCol;
    }
    else if(input == MOVE_LEFT){
        --nextCol;
    }
    else{
    }
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    char** map = new char*[maxRow];
    for(int i = 0; i < maxRow; i++) {
        map[i] = new char[maxCol];
        for(int j = 0; j < maxCol; j++){
            map[i][j] = TILE_OPEN;
        }
    }
    return map;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    for(int i = 0; i < maxRow; i++){
        delete[] map[i];
    }
    delete[] map;
    maxRow = 0;
    map = nullptr;
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    int newMaxRow = maxRow*2;
    int newMaxCol = maxCol*2;
    char** map1 = createMap(2*maxRow, 2*maxCol);
    int pRow;
    int pCol;
    for(int i = 0; i < maxRow; i++){
        for (int j = 0; j < maxCol; j++){
            if (map[i][j] == 'o'){
                pRow = i;
                pCol = j;
                map[i][j] = TILE_OPEN;
            }
            else{}
        }
    }
    for(int i = 0; i < 2*maxRow; i++){
        for (int j = 0; j < 2*maxCol; j++){
            if(i<maxRow){
                if(j<maxCol){
                    map1[i][j] = map[i][j];
                }
                else{
                    map1[i][j] = map[i][j-maxCol];
                }
            }
            else{
                if(j<maxCol){
                    map1[i][j] = map[i-maxRow][j];
                }
                else{
                    map1[i][j] = map[i-maxRow][j-maxCol];
                }
            }
        }
    }
deleteMap(map, maxRow);
map1[pRow][pCol]='o';
maxRow = newMaxRow;
maxCol = newMaxCol;
    return map1;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    int status;
    if(nextRow >= maxRow || nextRow < 0){
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY;
    }
    else if(nextCol >= maxCol || nextCol < 0){
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY;
    }
    else if(map[nextRow][nextCol] == '+' || map[nextRow][nextCol] == 'M' || (map[nextRow][nextCol] == '!' && player.treasure == 0)){
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY;
    }
    else if(map[nextRow][nextCol] == '$'){
        player.treasure += 1;
        status = STATUS_TREASURE;
    }
    else if(map[nextRow][nextCol] == '@'){
        status = STATUS_AMULET;
    }
    else if(map[nextRow][nextCol] == '?'){
        status = STATUS_LEAVE;
    }
    else if(map[nextRow][nextCol] == '!' && player.treasure >= 1){
        status = STATUS_ESCAPE;
    }
    else if(map[nextRow][nextCol] == '-'){
        status = STATUS_MOVE;
    }
    map[player.row][player.col] = '-';
    player.row = nextRow;
    player.col = nextCol;
    map[player.row][player.col] = 'o';
    return status;
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {
for(int i = player.row - 1; i >= 0; i--){
    if(map[i][player.col] == 'M'){
        map[i+1][player.col] = 'M';
        map[i][player.col] = '-';
    }
    else if(map[i][player.col]== '+'){
        break;
    }
}
for(int i = player.row + 1; i < maxRow; i++){
    if(map[i][player.col] == 'M'){
        map[i-1][player.col] = 'M';
        map[i][player.col] = '-';
    }
    else if(map[i][player.col]== '+'){
        break;
    }
}
for(int i = player.col - 1; i >= 0; i--){
    if(map[player.row][i] == 'M'){
        map[player.row][i+1] = 'M';
        map[player.row][i] = '-';
    }
    else if(map[player.row][i]== '+'){
        break;
    }
}
for(int i = player.col + 1; i < maxCol; i++){
    if(map[player.row][i] == 'M'){
        map[player.row][i-1] = 'M';
        map[player.row][i] = '-';
    }
    else if(map[player.row][i]== '+'){
        break;
    }
}
if(map[player.row][player.col] == 'M'){
    return true;
}
else{
    return false;
}
}
