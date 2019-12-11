// ------------------------------------------------------------------------------------------
//
// 影処理 [shadow.h]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _SHADOW_H_
#define _SHADOW_H_

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
	D3DXMATRIX  mtxWorldShadow;
	D3DXCOLOR	col;
	D3DXVECTOR3	size;
	bool		bUse;
} SHADOW;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

// 影の設定
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 size);

// 影の位置設定
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);

// 影の削除
void DeleteShadow(int nIdxShadow);

// 影の情報
SHADOW *GetShadow(void);
#endif
