// CodinGame - Coders of the Caribbean
// Adrian Dale 14/04/2017
#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

// Normally one would not pollute the global namespace with these in a header file,
// but since all these files will end up in a single file in the CodinGame IDE, it doesn't matter
using namespace std;
using namespace std::chrono;

// Includes below here should be added to the BuildCodinGameFile.bat script
// They will be replaced by the actual files in the final file for the IDE
#include "Vector2.h" // TAG_REMOVE
#include "HexGrid.h" // TAG_REMOVE
#include "Entity.h" // TAG_REMOVE
#include "Ship.h" // TAG_REMOVE
#include "PlayerShip.h" // TAG_REMOVE
#include "EnemyShip.h" // TAG_REMOVE
#include "Cannonball.h" // TAG_REMOVE
#include "Mine.h" // TAG_REMOVE
#include "Barrel.h" // TAG_REMOVE
#include "ShipAction.h" // TAG_REMOVE
#include "ShipPosition.h" // TAG_REMOVE
#include "PathFinder.h" // TAG_REMOVE
#include "GameController.h" // TAG_REMOVE