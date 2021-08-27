/*
 * Group Members: Christian Davis, Irvin Tancioco, Jessie Martinez 
 */

// DO NOT CHANGE OR REMOVE THE FOLLOWING LINES
#ifndef __DEFINE_PLAYER_FUNCTIONS_CPP__
#define __DEFINE_PLAYER_FUNCTIONS_CPP__

#include <cstring>
#include <iomanip>
#include <iostream>
using namespace std;

#include "mapFunctions.cpp"
#include "utilityFunctions.cpp"
// DO NOT CHANGE OR REMOVE THE PRECEDING LINES

/*
 * PLAYER STATE CONSTANTS
 */
const char LOOKING_UP = '^';
const char LOOKING_DOWN = 'v';
const char LOOKING_LEFT = '<';
const char LOOKING_RIGHT = '>';
const char LOOKING_DEAD = 'X';
const int INVENTORY_INDEX_HEALTH = 0;
const int INVENTORY_INDEX_KEYS = 1;
const int INVENTORY_INDEX_PEBBLES = 2;
const int INVENTORY_INDEX_PLANK = 3;
const int INVENTORY_INDEX_ROPE = 4;
const int INVENTORY_INDEX_SLINGSHOT = 5;
const int INVENTORY_LENGTH = 6;
const int SLINGSHOT_DISTANCE = 4;
const int SLINGSHOT_MAX_DAMAGE = 3;

/*
 * PLAYER STATE
 */
int playerX, playerY;
char playerSymbol;
int INVENTORY_ARRAY[INVENTORY_LENGTH] = { 0, 0, 0, 0, 0, 0 };

/*
 * FUNCTION PROTOTYPES
 */
/* Instructions STEP 1
void inventoryAdd(const char);
bool inventoryHas(const char);
bool inventoryUse(const char);
void inventorySet(const char[], const int[], const int);
bool printInventoryRow(const int, const int); */
int convertMapSquareToInventoryIndex(const char);
char convertInventoryIndexToItemChar(const int);
bool playerIsLookingAt(const int, const int);     // May need to be prototyped
/*
int getLookingAtX(const int currentX, const char currentSymbol)
{
	switch(currentSymbol)
	{
		case LOOKING_UP:
		case LOOKING_DOWN:
			return currentX;
		case LOOKING_LEFT:
			return (currentX - 1);
		case LOOKING_RIGHT:
			return (currentX + 1);
	}
	return currentX;
}

int getLookingAtY(const int currentY, const char currentSymbol)
{
	switch(currentSymbol)
	{
		case LOOKING_LEFT:
		case LOOKING_RIGHT:
			return currentY;
		case LOOKING_UP:
			return (currentY - 1);
		case LOOKING_DOWN:
			return (currentY + 1);
	}
	return currentY;
}*/

//void getLookingAtLocation(const int currentX, const int currentY, const char currentSymbol, int lookingAtX, int lookingAtY)
void getLookingAtLocation(const int currentX, const int currentY, const char currentSymbol, int& lookingAtX, int& lookingAtY) // Instructions STEP 17a
{
	// DO NOT CHANGE THE FUNCTION NAME OR RETURN TYPE.
	// DO NOT CHANGE THE FIRST THREE PARAMETERS.

	// ADD MISSING CODE BELOW
	switch(currentSymbol)
	{
		case LOOKING_LEFT:
            lookingAtX = currentX - 1; // Instructions STEP 17b
            lookingAtY = currentY; // Instructions STEP 17b
			break;
		case LOOKING_RIGHT:
            lookingAtX = currentX + 1; // Instructions STEP 17b
            lookingAtY = currentY; // Instructions STEP 17b
			break;
		case LOOKING_UP:
            lookingAtX = currentX; // Instructions STEP 17b
            lookingAtY = currentY - 1; // Instructions STEP 17b
            break;
		case LOOKING_DOWN:
            lookingAtX = currentX; // Instructions STEP 17b
            lookingAtY = currentY + 1; // Instructions STEP 17b
            break;
		default:
			lookingAtX = currentX;
			lookingAtY = currentY;
	}
}

int doPlayerHit(const int hitpoints)
{
	INVENTORY_ARRAY[INVENTORY_INDEX_HEALTH] = max(0, INVENTORY_ARRAY[INVENTORY_INDEX_HEALTH] - hitpoints);
	return INVENTORY_ARRAY[INVENTORY_INDEX_HEALTH];
}

bool playerIsAlive()
{
	return (INVENTORY_ARRAY[INVENTORY_INDEX_HEALTH] > 0);
}

char getPlayerSymbol()
{
	return (playerIsAlive() ? playerSymbol : LOOKING_DEAD);
}

void inventoryAdd(const char item)
{
	int inventoryIndex = convertMapSquareToInventoryIndex(item);
	if (inventoryIndex >= 0)
	{
		switch(item)
		{
			case MAP_SQUARE_HEALTH:
			case MAP_SQUARE_KEY:
			case MAP_SQUARE_PEBBLE:
			case MAP_SQUARE_PLANK:
			case MAP_SQUARE_ROPE:
				INVENTORY_ARRAY[inventoryIndex] += 1;
				break;
			case MAP_SQUARE_PEBBLES:
				INVENTORY_ARRAY[inventoryIndex] += 2;
				break;
			case MAP_SQUARE_SLINGSHOT:
				INVENTORY_ARRAY[inventoryIndex] = 1;
				break;
		}
	}
}

bool inventoryHas(const char item, const int count)
{
	int inventoryIndex = convertMapSquareToInventoryIndex(item);
	return (inventoryIndex >= 0 && INVENTORY_ARRAY[inventoryIndex] >= count);
}

bool inventoryHas(const char item)
{
	return inventoryHas(item, 1);
}

bool inventoryUse(const char item, const int count)
{
	bool success = false;

	int inventoryIndex = convertMapSquareToInventoryIndex(item);
	if (inventoryIndex >= 0)
	{
		switch(inventoryIndex)
		{
			case INVENTORY_INDEX_HEALTH:
			case INVENTORY_INDEX_KEYS:
			case INVENTORY_INDEX_PEBBLES:
			case INVENTORY_INDEX_PLANK:
			case INVENTORY_INDEX_ROPE:
				if (INVENTORY_ARRAY[inventoryIndex] > 0)
				{
					INVENTORY_ARRAY[inventoryIndex] -= count;
					success = true;
				}
				break;
			case INVENTORY_INDEX_SLINGSHOT:
				if (INVENTORY_ARRAY[inventoryIndex] > 0)
				{
					success = true;
				}
				break;
		}
	}

	return success;
}

bool inventoryUse(const char item)
{
	return inventoryUse(item, 1);
}

void inventorySet(const char newInventoryItems[], const int newInventoryValues[], const int count)
{
	for (int i = 0; i < INVENTORY_LENGTH; i++)
	{
		bool itemIsInNewList = false;

		for (int j = 0; j < count; j++)
		{
			int index = convertMapSquareToInventoryIndex(newInventoryItems[j]);
			if (index == i)
			{
				INVENTORY_ARRAY[index] = newInventoryValues[j];
				itemIsInNewList = true;
				break;
			}
		}

		if (!itemIsInNewList)
		{
			INVENTORY_ARRAY[i] = 0;
		}
	}
}

bool printInventoryRow(const int row, const int displayWidth)
{
	bool success = false;

	const int extraSpaceWidth = 1;
	const int symbolWidth = max(0, min(2, displayWidth));
	const int numberWidth = max(0, min(4, displayWidth - symbolWidth));
	const int nameWidth = max(0, displayWidth - (numberWidth + symbolWidth + extraSpaceWidth));

	switch(row)
	{
		case 1:
		{
			int itemIndex = INVENTORY_INDEX_KEYS;
			int count = INVENTORY_ARRAY[itemIndex];
			if (count > 0)
			{
				cout << right << setw(numberWidth) << count << left << ' ' << setw(symbolWidth) << convertInventoryIndexToItemChar(itemIndex) << setw(nameWidth) << "Key";
				success = true;
			}
			break;
		}
		case 2:
		{
			int itemIndex = INVENTORY_INDEX_ROPE;
			int count = INVENTORY_ARRAY[itemIndex];
			if (count > 0)
			{
				cout << right << setw(numberWidth) << count << left << ' ' << setw(symbolWidth) << convertInventoryIndexToItemChar(itemIndex) << setw(nameWidth) << "Rope";
				success = true;
			}
			break;
		}
		case 3:
		{
			int itemIndex = INVENTORY_INDEX_PLANK;
			int count = INVENTORY_ARRAY[itemIndex];
			if (count > 0)
			{
				cout << right << setw(numberWidth) << count << left << ' ' << setw(symbolWidth) << convertInventoryIndexToItemChar(itemIndex) << setw(nameWidth) << "Wood Plank";
				success = true;
			}
			break;
		}
		case 5:
		{
			bool hasSlingshot = INVENTORY_ARRAY[INVENTORY_INDEX_SLINGSHOT] > 0;
			int itemIndex = INVENTORY_INDEX_PEBBLES;
			int count = INVENTORY_ARRAY[itemIndex];
			if (hasSlingshot)
			{
				cout << right << setw(numberWidth) << count << left << ' ' << setw(symbolWidth) << MAP_SQUARE_SLINGSHOT << setw(nameWidth) << "Slingshot";
				success = true;
			}
			else if (count > 0)
			{
				cout << right << setw(numberWidth) << count << left << ' ' << setw(symbolWidth) << convertInventoryIndexToItemChar(itemIndex) << setw(nameWidth) << "Pebble";
				success = true;
			}
			break;
		}
		case 9:
		{
			int itemIndex = INVENTORY_INDEX_HEALTH;
			int count = INVENTORY_ARRAY[itemIndex];
			cout << right << setw(numberWidth) << count << left << ' ' << setw(symbolWidth) << convertInventoryIndexToItemChar(itemIndex) << setw(nameWidth) << "Health";
			success = true;
			break;
		}
	}

	return success;
}

int convertMapSquareToInventoryIndex(const char mapSquare)
{
	switch(mapSquare)
	{
		case MAP_SQUARE_HEALTH:
			return INVENTORY_INDEX_HEALTH;
		case MAP_SQUARE_KEY:
			return INVENTORY_INDEX_KEYS;
		case MAP_SQUARE_PEBBLE:
		case MAP_SQUARE_PEBBLES:
			return INVENTORY_INDEX_PEBBLES;
		case MAP_SQUARE_PLANK:
			return INVENTORY_INDEX_PLANK;
		case MAP_SQUARE_ROPE:
			return INVENTORY_INDEX_ROPE;
		case MAP_SQUARE_SLINGSHOT:
			return INVENTORY_INDEX_SLINGSHOT;
	}
	return -1;
}

char convertInventoryIndexToItemChar(const int index)
{
	switch(index)
	{
		case INVENTORY_INDEX_HEALTH:
			return MAP_SQUARE_HEALTH;
		case INVENTORY_INDEX_KEYS:
			return MAP_SQUARE_KEY;
		case INVENTORY_INDEX_PEBBLES:
			return MAP_SQUARE_PEBBLE;
		case INVENTORY_INDEX_PLANK:
			return MAP_SQUARE_PLANK;
		case INVENTORY_INDEX_ROPE:
			return MAP_SQUARE_ROPE;
		case INVENTORY_INDEX_SLINGSHOT:
			return MAP_SQUARE_SLINGSHOT;
	}
	return ' ';
}

void getFarthestActionableLocation(const int maxActionDistance, const bool mustBeEmpty, int& locationX, int& locationY)
{
    locationX = playerX;
    locationY = playerY;
    int nextX, nextY, farthestEmptyX = playerX, farthestEmptyY = playerY;
    for (int i = 0; i < maxActionDistance; i++)
    {
        /* The farthest actionable location is the square (defined by locationX and locationY)
         * that is the farthest location the player can affect. This location may not necessarily
         * be empty. Use farthestEmptyX and farthestEmptyY to keep track of the farthest empty
         * actionable location you have found so far. Either way, the farthest actionable location
         * must be directly in front of the player, whichever direction that may be. The player
         * must be able to see this location, meaning that the player must be able to see past
         * every square between the player's location and the farthest actionable location. Take
         * into account the maxActionDistance provided.
         */
        
        /* -- MISSING CODE -- LOTS OF IT -- */
        if      (playerIsLookingAt(locationX + i, locationY))
        {
            nextX = locationX + i;
            nextY = locationY;
        }
        else if (playerIsLookingAt(locationX - i, locationY))
        {
            nextX = locationX - i;
            nextY = locationY;
        }
        else if (playerIsLookingAt(locationX, locationY + i))
        {
            nextX = locationX;
            nextY = locationY + i;
        }
        else if (playerIsLookingAt(locationX, locationY - i))
        {
            nextX = locationX;
            nextY = locationY - i;
        }
        else
        {
            nextX = locationX;
            nextY = locationY;
        }
        if (getMapSquare(nextX, nextY) == MAP_SQUARE_EMPTY)
        {
            farthestEmptyX = nextX;
            farthestEmptyY = nextY;
        }
    }
    
    if (mustBeEmpty)
    {
        locationX = farthestEmptyX;
        locationY = farthestEmptyY;
    }
}

bool playerIsLookingAt(const int x, const int y)
{
    // returns true only if the player is looking at the given location (x,y) and can actually see it
    //
    int distanceX = playerX - x;
    int distanceY = playerY - y;
    bool isLooking = false;
    if(distanceX == 0)
    {
        if (playerSymbol == LOOKING_UP && distanceY > 0)
        {
            isLooking = true;
        }
        else if (playerSymbol == LOOKING_DOWN && distanceY < 0)
        {
            isLooking = true;
        }
    }
    else if (distanceY == 0)
    {
        if (playerSymbol == LOOKING_RIGHT && distanceX < 0)
        {
            isLooking = true;
        }
        else if (playerSymbol == LOOKING_LEFT && distanceX > 0)
        {
            isLooking = true;
        }
        
    }
  
    return (isLooking &&
            twoLocationsAreVisibleToEachOther(playerX, playerY, x, y)); // Instructions STEP 19
    //
    
    /***********
     VERSION 2
     ***********/
    /*
    // returns true only if the player is looking at the given location (x,y) and can actually see it
    int doLookAtX, doLookAtY;
    getLookingAtLocation(playerX, playerY, playerSymbol, doLookAtX, doLookAtY);
    //return false;
    return (twoLocationsAreVisibleToEachOther(x, y, playerX, playerY) &&
            twoLocationsAreVisibleToEachOther(x, y, doLookAtX, doLookAtY)); // Instructions STEP 19
    */
}

// DO NOT CHANGE OR REMOVE THE FOLLOWING LINE
#endif
// DO NOT CHANGE OR REMOVE THE PRECEDING LINE
