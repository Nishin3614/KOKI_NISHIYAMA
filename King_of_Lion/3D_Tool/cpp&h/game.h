// ------------------------------------------------------------------------------------------
//
// game処理の説明[game.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

#ifndef _GAME_H_
#define _GAME_H_	 // ファイル名を基準を決める

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include "main.h"

// ------------------------------------------------------------------------------------------
//
// 列挙型
//
// ------------------------------------------------------------------------------------------
// ========================
// 状態
// ========================
typedef enum
{
	GAMESTATE_3D = 0,	// 2D
	GAMESTATE_MAX 
} GAMESTATE;

// ========================
// ステージ構成
// ========================
typedef enum
{
	GAMESTAGE_1 = 0,	// ステージ1
	GAMESTAGE_2,	// ステージ2
	GAMESTAGE_3,	// ステージ3
	GAMESTAGE_4,	// ステージ4
	GAMESTAGE_5,	// ステージ5
	GAMESTAGE_MAX
} GAMESTAGE;

// ========================
// 選択状態
// ========================
typedef enum
{
	GAMESELECT_NONE = 0,	// なんもない
	GAMESELECT_FLOOR,		// 床
	GAMESELECT_MATERIALS,	// 素材
	GAMESELECT_GIRAFFE,		// キリン
	GAMESELECT_WALL,		// 壁
	GAMESELECT_BILLBOARD,	// ビルボード
	GAMESELECT_SELECT,		// 選択状態
	GAMESELECT_MAX
} GAMESELECT;

// ------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------------------------------------------------------

// 基本プロトタイプ	//
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetGameState(GAMESTATE state);
GAMESTATE GetGameStete(void);

void NotDisp_Game(GAMESELECT select);
void SetGameSelect(GAMESELECT select);
GAMESELECT GetGameSelect(void);

// ゲームステージ取得
int GetGameStage(void);

#endif