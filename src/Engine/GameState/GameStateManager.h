// ----------------------------------------------------------------------------
// Project Name		:	Game State Manager
// File Name		:	GameStateManager.h
// Author			:	Thomas Komair tkomair@digipen.edu
// Purpose			:	Declares the functions and data for the Game State Manager
// ----------------------------------------------------------------------------
#ifndef GAMESTATE_MANAGER_H_
#define GAMESTATE_MANAGER_H_

typedef  void (*fp)();

// game State functions - externs because declared in the cpp file
extern fp GameStateInit;
extern fp GameStateLoad;
extern fp GameStateUnload;
extern fp GameStateFree;
extern fp GameStateUpdate;
extern fp GameStateRender;

// Variables used to store the current previous and next state
// also store which one is the initial state (useful when restarting the game).
// extern because declared in the cpp file
extern unsigned gGameStateInit;
extern unsigned gGameStateCurrent;
extern unsigned gGameStatePrevious;
extern unsigned gGameStateNext;

// This function implements the game loop and is called in main
void GSM_GameLoop();

// Call this to switch states
void GSM_UpdatePtrs();

// Call this to initialize the game state manager
void GSM_Init(unsigned gameStateInit);

// Call this function to add a state to the GameStateManager
void GSM_AddGameState(unsigned gameStateIdx, fp Load, fp Init, fp Update, fp Render, fp Free, fp Unload);

#endif