// ------------------------------------------------------------------------------------------
//
// ランキング処理 [ranking.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _RANKING_H_
#define _RANKING_H_

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS

#include "main.h"

// ------------------------------------------------------------------------------------------
//
// 構造体定義
//
// ------------------------------------------------------------------------------------------
typedef struct
{
	int nPoint;
	int nId;
} RANKING;

// ------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------------------------------------------------------
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);

// ランキング計算関数
void AddRanking(void);
#endif
