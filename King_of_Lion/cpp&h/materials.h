// ------------------------------------------------------------------------------------------
//
// 素材処理 [materials.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _MATERIALS_H_
#define _MATERIALS_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

#include "wall.h"

#include "floor.h"

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------オブジェクト情報---------- //
typedef enum
{
	MATERIALS_OBJECT_00_WOOD_GREEN = 0,
	MATERIALS_OBJECT_01_WOOD_RED,
	MATERIALS_OBJECT_02_WOOD_ORANGE,
	MATERIALS_OBJECT_MAX
} MATERIALS_OBJECT;

// ---------アイテム情報---------- //
typedef enum
{
	MATERIALS_ITEM_00_APPLE = MATERIALS_OBJECT_MAX,
	MATERIALS_ITEM_01_ACTINIDIA,
	MATERIALS_ITEM_MAX
} MATERIALS_ITEM;

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------

// ---------- 能力 ---------- //
typedef struct
{
	int nHPRecovery;
	int nMPRecovery;
} ABILITY;

// ---------- 素材 ---------- //
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 posold;				// 前の位置
	D3DXVECTOR3 posKeyBetween;		// キー間の距離
	D3DXVECTOR3 move;				// 移動量
	D3DXVECTOR3 rot;				// 現在回転量
	D3DXVECTOR3 rotLast;			// 向きたい方向
	D3DXVECTOR3 rotbetween;			// 回転の差分
	D3DXVECTOR3 rotKeyBetween;		// キー間の回転の差分
	D3DXVECTOR3	size;			// サイズ
	D3DXVECTOR3 vtxMinMaterials;	// モデルの位置の最小値
	D3DXVECTOR3 vtxMaxMaterials;	// モデルの位置の最大値
	D3DXMATRIX  mtxWorldMaterials;	// ワールドマトリックス
	FLOOR		*pFloor;			// 床
	int			nDrawType;			// 描画タイプ
	int			nTexType;			// テクスチャータイプ
	int			nSelect;			// オブジェクトかアイテムか
	int			IdxShadow;			// 影のNO.
	int			nIdxDome;			// ドームのNO.
	float		fRadius;			// 半径
	bool		bHit;				// 当たった判定
	bool		bUse;				// 使用状況
	bool		bDisp;				// 表示状態
	// 能力変化
	ABILITY		ability;			// 能力
} MATERIALS;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitMaterials(void);
void UninitMaterials(void);
void UpdateMaterials(void);
void DrawMaterials(void);

// 当たり判定
int ColisionMaterials(
	D3DXVECTOR3 *pos,				// 現在のポジション
	D3DXVECTOR3 *posOld,			// 前回のポジション
	D3DXVECTOR3 *move,				// 移動量
	D3DXVECTOR3 *size,				// サイズ
	MATERIALS	**ppMaterialsRet	// 素材のポインターのポインター
);

// 設定
void SetMaterials(
	D3DXVECTOR3 pos,			// 位置
	D3DXVECTOR3 rot,			// 回転
	D3DXVECTOR3 move,			// 移動量
	int			nDrawType,		// 描画タイプ
	int			nHPRecovery,	// HP用
	int			nMPRecovery		// MP用
);

MATERIALS *GetMaterials(void);
#endif
