// ------------------------------------------------------------------------------------------
//
// ビルボード処理 [billboard.h]
// Author : Koki Nishiyama
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
#define MATERIALS_MAX (128)

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------状態---------- //
typedef enum
{
	BILLBOARDTYPE_WOOD = 0,
	BILLBOARDTYPE_ENTER,
	BILLBOARDTYPE_TREE,
	BILLBOARDTYPE_RION,
	BILLBOARDTYPE_MAX
} BILLBOARDTYPE;

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
	WALL		*pWall;				// 対象の壁のポイント
	MATERIALS	*pMaterials;		// 対象の素材のポイント
	int			nType;				// 種類
	float		fAlpha;				// アルファ値変化
	bool		bDisp;				// 表示状態なら
	bool		bUse;				// 使用状態
} BILLBOARD;						

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

BILLBOARD *GetBillboard(void);

void SetPositionBillboard(int nType, D3DXVECTOR3 pos);
void DeleteBillboard(int nType);

#endif
