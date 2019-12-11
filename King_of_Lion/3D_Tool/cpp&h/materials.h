// ------------------------------------------------------------------------------------------
//
// 素材処理 [materials.h]
// Author : 
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

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define MATERIALS_MAX (128)

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------

// ---------素材情報---------- //
typedef enum
{
	MATERIALSTYPE_00_WOOD_GREEN = 0,
	MATERIALSTYPE_00_WOOD_GREE1,
	MATERIALSTYPE_00_WOOD_GREE12,
	MATERIALSTYPE_MAX
} MATERIALSTYPE;

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------

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
	D3DXVECTOR3	size;				// サイズ
	D3DXVECTOR3 vtxMinMaterials;	// モデルの位置の最小値
	D3DXVECTOR3 vtxMaxMaterials;	// モデルの位置の最大値
	D3DXMATRIX  mtxWorldMaterials;	// ワールドマトリックス
	int			nDrawType;			// 描画タイプ
	int			nTexType;			// テクスチャータイプ
	float		fLength;			// 長さ
	bool		bUse;				// 使用状況
	bool		bDisp;				// 表示状態
} MATERIALS;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitMaterials(void);
void UninitMaterials(void);
void UpdateMaterials(void);
void DrawMaterials(void);

// 当たり判定
bool ColisionMaterials(
	D3DXVECTOR3 *pos,		// 現在のポジション
	D3DXVECTOR3 *posOld,	// 前回のポジション
	D3DXVECTOR3 *move,		// 移動量
	D3DXVECTOR3 *size,		// サイズ
	MATERIALS		**ppMaterialsRet);

void NotDisp_Materials(void);		// 素材の情報を非表示
MATERIALS *GetMaterials(void);

// 選択
bool SeleMaterials(int nCntMaterials);

// セーブ
void SaveMaterials(void);

// ロード
void LoadMaterials(void);

#endif
