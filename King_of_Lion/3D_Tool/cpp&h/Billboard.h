// ------------------------------------------------------------------------------------------
//
// ポリゴン処理 [billboard.h]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

#include "wall.h"

#include "materials.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define BILLBOARD_MAX (128)

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 posOld;			// 前回の位置
	D3DXVECTOR3 rot;			// 回転量
	D3DXVECTOR3 size;			// サイズ
	D3DXMATRIX  mtxWorldBillboard;	// マトリックス
	WALL		*pWall;			// 対象の壁のポイント
	MATERIALS	*pMaterials;	// 対象の素材のポイント
	int			nType;			// 種類
	bool		bUse;			// 使用状態
	bool		bDisp;			// 表示状態
} BILLBOARD;						

typedef struct
{
	D3DXVECTOR3 size;									// サイズ
	int nType;											// タイプ
} BILLBOARDSET;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);

void SetBillboard(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nType);

void NotDispBillboard(void);	// 表示切替時に表示に

// セーブ
void SaveBillboard(void);

// ロード
void LoadBillboard(void);

BILLBOARD *GetBillboard(void);
#endif
