int mapNum = -1, numOfGhosts, numOfStars, life = 3, score;
int initGhostRow[NumberOfMaps][2] = {{15, 17}, {13, 15}, {13, 15}};
float ghostSpeed, pacmanSpeed; // speed : blocks per second
coordinate pacmanPos, initPacmanPos[NumberOfMaps] = {{13, 25},{13, 23}, {13, 23}};
ghost ghosts[7];
float cherry, speedBoostTime;
bool death = false;
int bestScore;