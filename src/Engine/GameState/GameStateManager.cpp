// ----------------------------------------------------------------------------
// Project Name		:	Game State Manager
// File Name		:	GameStateManager.cpp
// Author			:	Thomas Komair tkomair@digipen.edu
// Creation Date	:	February 5, 2012
// Purpose			:	Implements the function of the GameStateManager
// History			:
// - 5/02/2012		:	Initial Implementation
// - 1/24/2019		:	Edited by Alejandro Balea (alejandro.balea@digipen.edu)
// ----------------------------------------------------------------------------
#include <iostream>
#include "GameStateList.h"		// GS_RESTART, GS_QUIT
#include "GameStateManager.h"	// File's function prototypes and data

// Max states in the game
#define MAX_GAME_STATES 64

/*!------------------------------------------------------------------------------------------------
	\brief This structure holds the id of the gamestate and the 6 function pointers for the state.
-------------------------------------------------------------------------------------------------*/
struct GameStateInfo
{
	unsigned GameStateId;
	
	fp Load;
	fp Unload;
	fp Init;
	fp Free;
	fp Update;
	fp Render; // Draw
};

// Stores the current, previsou and next states along with the initialize state
unsigned gGameStateInit;
unsigned gGameStateCurrent;
unsigned gGameStatePrevious;
unsigned gGameStateNext;

// Game State functions
fp GameStateInit = 0;
fp GameStateLoad = 0;
fp GameStateUnload = 0;
fp GameStateFree = 0;
fp GameStateUpdate = 0;
fp GameStateRender = 0;


// An array of GameStateInfo to hold all the game state information.
GameStateInfo gGameStates[MAX_GAME_STATES];

// The number of actual game states.
unsigned gGameStatesCount = 0;

/*!------------------------------------------------------------------------------------------------
 \brief This is a dummy function that doesn't do anything.
 \details
 	Use this function as a substitute if one of the function pointers passed to the 
	GSM_AddGameState function is NULL.
-------------------------------------------------------------------------------------------------*/
void dummy()
{}

/*------------------------------------------------------------------------------------------------
 \brief This function adds a state to the game state manager.
 \param gameStateIdx
 	Represents the ID of the state. This id is declared in GameStateList.h
 \param Load,Init,Update,Render,Free,Unload 
	Function pointers to the game states functions

 \details:
	The function adds a state to the GameStateManager by storing the passed arguments in the
	gGameStates array (declared above). It must first make sure that the current count of game
	states is not over the MAX_GAME_STATES macro(defined above). Then it must go through all 
	the previouly added game states and make sure that we are not adding a duplicate; if we are 
	we simply return. For example:
	
	GSM_AddGameState(GS_LEVEL1,Level1Load, Level1Init, Level1Update, Level1Render, Level1Free, Level1Unload); 
		-> GS_LEVEL1 has not been added, we add the state to the array.
	GSM_AddGameState(GS_LEVEL1,....) 
		-> This time GS_LEVEL1 exists, we don't add a duplicate, return.

	Adding a state is simply storing the gameStateIdx and all the function pointers in the gGameStates array
	of GameStateInfo structure. If one of the passed function pointer is NULL, then you MUST store
	the address of dummy. this will insure that you don't try to call a function using a NULL pointer.
	For Example:

		GSM_AddGameState(GS_LEVEL1, Level1Load, NULL, Level1Update, Level1Render, NULL, Level1Unload);
			-> Here the Init and Free function pointers are pointing to dummy.
-------------------------------------------------------------------------------------------------*/
void GSM_AddGameState(unsigned gameStateIdx, fp Load, fp Init, fp Update, fp Render, fp Free, fp Unload)
{
	// Check if there is space for game states
	if (gGameStatesCount < MAX_GAME_STATES)
	{
		// Create a for loop in order to check if there is already another game state with the same Id
		for (int i = 0; i < gGameStatesCount; i++)
		{
			if (gGameStates[i].GameStateId == gameStateIdx) // If that's so, return
				return; 
		}

		// Set the game state's Id in the array
		gGameStates[gGameStatesCount].GameStateId = gameStateIdx;

		// Check if each one of the inputted functions is NULL or not
		if (Load != NULL)
			gGameStates[gGameStatesCount].Load = Load; // If not, set it to the Load fp
		else
			gGameStates[gGameStatesCount].Load = &dummy; // If that's so, set the dummy function to it

		// The same thing, but with different inputs and outputs
		if (Init != NULL)
			gGameStates[gGameStatesCount].Init = Init;
		else
			gGameStates[gGameStatesCount].Init = &dummy;

		if (Update != NULL)
			gGameStates[gGameStatesCount].Update = Update;
		else
			gGameStates[gGameStatesCount].Update = &dummy;

		if (Render != NULL)
			gGameStates[gGameStatesCount].Render = Render;
		else
			gGameStates[gGameStatesCount].Render = &dummy;

		if (Free != NULL)
			gGameStates[gGameStatesCount].Free = Free;
		else
			gGameStates[gGameStatesCount].Free = &dummy;

		if (Unload != NULL)
			gGameStates[gGameStatesCount].Unload = Unload;
		else
			gGameStates[gGameStatesCount].Unload = &dummy;

		// Increment gGameStatesCount
		gGameStatesCount++;
	}
	else // If there is not enough space, send an error message
		std::cout << "ERROR: GSM_AddGameState()" << std::endl;

	// Close all the files
	_fcloseall();
}

/*------------------------------------------------------------------------------------------------
 \brief	This function initializes the game state manager.
 \param	gameStateInit 
 	Represents the ID of the initial state. This id is declared in GameStateList.h
	Note that the state must be added previous to the initialization.

 \details
	- Simply store this state as the initial state by storing it into gameStateInit (next, previous and current).
	- Call GameStateManagerUpdate to change the pointers. 
	- Remember that you must write to "Output.txt" in this function. (Use the FileIOManager).
-------------------------------------------------------------------------------------------------*/
// Call this to initialize the game state manager
void GSM_Init(unsigned gameStateInit)
{
	_fcloseall();

	// Set the input to all the game state parameters
	gGameStateInit = gameStateInit;
	gGameStateCurrent = gameStateInit;
	gGameStatePrevious = gameStateInit;
	gGameStateNext = gameStateInit;
}


/*!------------------------------------------------------------------------------------------------
 \brief	This function adds a state to the game state manager.
 \details	
	This function updates the function pointers of the GameStateManager. It will look up the 
	value of gGameStateCurrent in the gGameStates array. If the value is equal to the id of one 
	of the added states (see GSM_AddGameState()function above), then it sets the GameStateManager
	function pointers to the corresponding functions (again added in the GSM_AddGameState function).

	if the value of gGameStateCurrent is equal to GS_RESTART or GS_QUIT then the GSM function pointers
	fill point to the dummy function.

	if the value of gGameStateCurrent is not either of the two above, then you must return and 
	print an error.

	- Remember that you must write to "Output.txt" in this function. (Use the FileIOManager)
-------------------------------------------------------------------------------------------------*/
void GSM_UpdatePtrs()
{
	// If the game state is "restart" or "quit", set all the game states to the dummy function and return
	if (gGameStateCurrent == GS_RESTART || gGameStateCurrent == GS_QUIT)
	{
		GameStateInit = &dummy;
		GameStateLoad = &dummy;
		GameStateUnload = &dummy;
		GameStateFree = &dummy;
		GameStateUpdate = &dummy;
		GameStateRender = &dummy;

		return;
	}

	// Create a for loop
	for (int i = 0; i < MAX_GAME_STATES; i++)
	{
		// If the current game state is one of the inputted Ids, put them in the array and break the loop
		if (gGameStateCurrent == gGameStates[i].GameStateId)
		{
			GameStateInit = gGameStates[i].Init;
			GameStateLoad = gGameStates[i].Load;
			GameStateUnload = gGameStates[i].Unload;
			GameStateFree = gGameStates[i].Free;
			GameStateUpdate = gGameStates[i].Update;
			GameStateRender = gGameStates[i].Render;

			break;
		}
	}

	_fcloseall();
}

/*!------------------------------------------------------------------------------------------------
	\brief Implements the gameloop seen in class. 
	\details
		Remember that the game loop is implemented using two loops: The inner loop calls Update and 
		Render as long as we're not changing game state (that is, the current game state and the next are
		still the same value). The outer loop handles game state changing. This implies game state
		loading/unloading, initialization and freeing, as well as handling the GS_QUIT and GS_RESTART 
		special game states. 
------------------------------------------------------------------------------------------------*/
void GameLoop()
{
	// A "while" loop while the current game state is not "quit"
	while (gGameStateCurrent != GS_QUIT)
	{
		// If the current game state is "restart", set everything to the previous game state
		if (gGameStateCurrent == GS_RESTART)
		{
			gGameStateCurrent = gGameStatePrevious;
			gGameStateNext = gGameStatePrevious;
		}
		else // If not, update the pointers and load the new game state
		{
			GSM_UpdatePtrs();
			GameStateLoad();
		}

		// Initialize the game state
		GameStateInit();

		// While the current game state is equal to the next one (no game state changes)
		while (gGameStateCurrent == gGameStateNext)
		{
			// @TODO : Check for inputs, update and render the game state
			//Input_Handle();
			GameStateUpdate();
			GameStateRender();
		}

		// Free the game state
		GameStateFree();

		// If the next game state is not "restart", unload the game state
		if (gGameStateNext != GS_RESTART)
			GameStateUnload();

        // Move all the gGameStates to the next state
		gGameStatePrevious = gGameStateCurrent;
		gGameStateCurrent = gGameStateNext;
	}
}
