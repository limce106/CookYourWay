// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CookYourWaySaveGame.h"

UCookYourWaySaveGame::UCookYourWaySaveGame()
{
	Day = 0;
	PlayerBistroAreaID = 16;
	CompetitorAreaID = { 5, 7, 11, 14, 18, 21 };
	StoreAreaID = { 2, 10, 23 };
	CustomerNames = { "Amy", "Michelle" };
}
