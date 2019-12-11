// ------------------------------------------------------------------------------------------
//
// 読み込み・書き出し処理 [Load_Save.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _LOAD_SAVE_H_
#define _LOAD_SAVE_H_

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
} LOAD_SAVE;

// ------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------------------------------------------------------
void InitLoad_Save(void);

void SaveData(int nPoint);
void LoadDate(void);
LOAD_SAVE *GetLoad_Save();
#endif
