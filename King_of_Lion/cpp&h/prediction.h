// ------------------------------------------------------------------------------------------
//
// 予測線処理 [prediction.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _PRIDICTION_H_
#define _PRIDICTION_H_

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
	D3DXVECTOR3 rot;
	D3DXMATRIX  mtxWorldPrediction;
	D3DXCOLOR	col;
	D3DXVECTOR3	size;
	bool		bUse;
} PRIDICTION;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitPrediction(void);
void UninitPrediction(void);
void UpdatePrediction(void);
void DrawPrediction(void);

// 予測線の設定
int SetPrediction(D3DXVECTOR3 pos, D3DXVECTOR3 size,D3DXCOLOR col);

// 予測線の位置設定
void SetPositionPrediction(int nIdxPrediction, D3DXVECTOR3 pos);

// 予測線の削除
void DeletePrediction(int nIdxPrediction);

// 予測線の情報
PRIDICTION *GetPrediction(void);
#endif
