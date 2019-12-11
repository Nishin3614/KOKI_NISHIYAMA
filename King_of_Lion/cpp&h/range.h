// ------------------------------------------------------------------------------------------
//
// 爆発処理 [range.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _RANGE_H_
#define _RANGE_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 size;
	D3DXVECTOR3 rot;
	D3DXMATRIX  mtxWorldRange;
	D3DXCOLOR	col;
	int			nType;				// タイプ
	bool		bUse;
	bool		bRange;	// 範囲に入ったら遷移
} RANGE;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitRange(void);
void UninitRange(void);
void UpdateRange(void);
void DrawRange(void);

void SetRange(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	int			nType);

void PlayerPos_Range(void);

// 爆発の情報
RANGE *GetRange(void);
#endif
