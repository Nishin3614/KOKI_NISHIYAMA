// ------------------------------------------------------------------------------------------
//
// アイテム処理 [item.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _ITEM_H_
#define _ITEM_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

#include "wall.h"

#include "floor.h"

#include "materials.h"

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 posold;			// 前回の位置情報
	D3DXVECTOR3 rot;			// 回転量
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 size;			// サイズ
	D3DXMATRIX  mtxWorldItem;	// マトリックス
	float		fRadius;		// 半径
	int			nLife;			// 出現時間
	int			nIdxShadow;		// 影のID
	int			nType;			// 種類
	int			nAttack;		// 攻撃力
	bool		bUse;			// 使用状態

	// 外部干渉
	WALL		*pWall;					// 対象の壁のポイント
	FLOOR		*pMeshField;			// 対象の床のポイント
	MATERIALS	*pMaterials;			// 対象の素材のポイント

} ITEM;						

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

// アイテムの設定
void SetItem(
	D3DXVECTOR3 pos,	// 位置
	D3DXVECTOR3 rot,	// 回転
	D3DXVECTOR3 move,	// 移動量
	D3DXVECTOR3 size,	// サイズ
	float	fRadius,	// 半径
	int nType,			// タイプ
	int nLife			// ライフ
);

void InforItem(void);

ITEM *GetItem(void);
#endif
