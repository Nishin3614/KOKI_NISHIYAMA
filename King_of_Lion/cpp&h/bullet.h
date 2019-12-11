// ------------------------------------------------------------------------------------------
//
// 弾処理 [bullet.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"
#include "wall.h"
#include "materials.h"

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 posOld;			// 前回の位置
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// 回転量
	D3DXVECTOR3 size;			// サイズ
	D3DXMATRIX  mtxWorldBullet;	// マトリックス
	WALL		*pWall;			// 対象の壁のポイント
	MATERIALS	*pMaterials;	// 対象の素材のポイント
	int			nLife;			// 出現時間
	int			nPattern;		// アニメーションパターン
	int			nCntAnim;		// アニメーションカウント
	int			nIdxShadow;		// 影のID
	int			nType;			// 種類
	int			nAttack;		// 攻撃力
	bool		bUse;			// 使用状態
} BULLET;						

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
// 弾のテクスチャー設定
void SetTexBullet(VERTEX_3D *pVtx,float fBeginTex,float fEndTex);
// 弾の設定
void SetBullet(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 move ,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nLife,
	int nType,
	int nAttack);
// 弾の取得
BULLET *GetBullet(void);
#endif
