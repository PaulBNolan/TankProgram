#pragma once

/// <summary>
/// @author Paul Nolan
/// @version 1.0
/// </summary>

enum class GameState
{
	GAME_RUNNING,
	GAME_WIN,
	GAME_LOSE
};

enum class AiBehaviour
{
	SEEK_PLAYER,
	STOP,
	RETREAT,
	FIND_HEALTH,
	FIND_COVER
};