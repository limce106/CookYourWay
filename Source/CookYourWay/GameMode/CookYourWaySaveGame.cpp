// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CookYourWaySaveGame.h"

UCookYourWaySaveGame::UCookYourWaySaveGame()
{
	Day = 0;
	PlayerBistroAreaID = 16;
	CompetitorAreaID = { 5, 7, 11, 14, 18, 21 };
	StoreAreaID = { 2, 10, 23 };
	TotalAsset = 0;
	CustomerNames = { "Amy", "Michelle" };
	int32 PlayerBistroTotalCust = 0;
	TMap<int32, int32> CompetitorTotalCust = {
		{5, 0},
		{7, 0},
		{11, 0},
		{14, 0},
		{18, 0},
		{21, 0}
	}; 
	int32 PlayerBistroTotalRateSum = 0;
	TMap<int32, int32> CompetitorTotalRateSum = {
		{5, 0},
		{7, 0},
		{11, 0},
		{14, 0},
		{18, 0},
		{21, 0}
	};
}
