// ------------------------------------------------------------------------------------------
//
// モデル処理 [giraffe.h]
// Author : 
//
// ------------------------------------------------------------------------------------------
#ifndef _GIRAFFE_H_
#define _GIRAFFE_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

#include "wall.h"

#include "floor.h"

#include "materials.h"

#include "model.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define GIRAFFETYPE_MAX (11)
#define	MAX_GIRAFFE (128)
#define GIRAFFE_TEX1 "data/MODEL"

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------

// ---------モーションセット---------- //
typedef enum
{
	GIRAFFEMOSIONSET_NEUTRAL = 0,
	GIRAFFEMOSIONSET_MOVE,
	GIRAFFEMOSIONSET_JUMP,
	GIRAFFEMOSIONSET_LANDING,
	GIRAFFEMOSIONSET_NORMALATACK,
	GIRAFFEMOSIONSET_MAX
} GIRAFFEMOSIONSET;

// ---------素材情報---------- //
typedef enum
{
	GIRAFFEDRAWTYPE_00_WOOD_GREEN = 0,
	GIRAFFEDRAWTYPE_MAX
} GIRAFFEDRAWTYPE;
// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------

// ---------- キリン ---------- //
typedef struct
{
	MODEL		model[GIRAFFETYPE_MAX];				// モデル情報
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 posold;				// 前の位置
	D3DXVECTOR3 posLast;			// 行きたい位置
	D3DXVECTOR3 posKeyBetween;		// キー間の距離
	D3DXVECTOR3	posOrigin;			// 初期位置
	D3DXVECTOR3 move;				// 移動量
	D3DXVECTOR3 rot;				// 現在回転量
	D3DXVECTOR3 rotLast;			// 向きたい方向
	D3DXVECTOR3 rotbetween;			// 回転の差分
	D3DXVECTOR3 rotKeyBetween;		// キー間の回転の差分
	D3DXVECTOR3 vtxMinMaterials;	// モデルの位置の最小値
	D3DXVECTOR3 vtxMaxMaterials;	// モデルの位置の最大値
	D3DXVECTOR3 size;				// モデルのサイズ
	D3DXMATRIX  mtxWorldGiraffe;		// ワールドマトリックス
	WALL		*pWall;				// 対象の壁のポイント
	FLOOR		*pMeshField;		// 対象の床のポイント
	MATERIALS	*pMaterials;		// 対象の素材のポイント
	int			nMotionType;		// モーションタイプ
	int			nMotionTypeOld;		// 前回のモーションタイプ
	int			nParent;			// 親
	int			nIndex;				// パーツ
	int			nFram;				// フレーム
	int			nCntKeySet;			// キーセットカウント
	int			nIdxShadow;			// 影の番号
	float		fLength;			// 長さ
	bool		bUse;				// 使用状況
	bool		bDisp;				// 表示状態
} GIRAFFE;

// ---------- モデルキー情報 ---------- //
typedef struct
{
	int nLoop;						// ループ
	int nNumKey;					// キー情報の数
	KEY_INFO KeyInfo[5];			// キー情報(モーション数)
} MOSION_GIRAFFE;


// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitGiraffe(void);
void UninitGiraffe(void);
void UpdateGiraffe(void);
void DrawGiraffe(void);

// 選択
bool SeleGiraffe(int nCntGiraffe);

void SetGiraffe(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void NotDispGiraffe(void);		// 素材の情報を非表示

// セーブ
void SaveGiraffe(void);

// ロード
void LoadGiraffe(void);
GIRAFFE *GetGiraffe(void);
#endif
