#include "GameWinMain.h"
#include "GameEngine.h"

#include "Game.h"	
#include <cstdio>
//-----------------------------------------------------------------
// Create GAME_ENGINE global (singleton) object and pointer
//-----------------------------------------------------------------
GameEngine myGameEngine;
GameEngine* GAME_ENGINE{ &myGameEngine };


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    AllocConsole();
    FILE* pFile;
    freopen_s(&pFile, "CONOUT$", "w", stdout); 
    freopen_s(&pFile, "CONOUT$", "w", stderr);
    freopen_s(&pFile, "CONIN$", "r", stdin);

    GAME_ENGINE->SetGame(new Game());                                       // any class that implements AbstractGame

    return GAME_ENGINE->Run(hInstance, nCmdShow);           // here we 
}

