// ------------------------------------------------------------------------------------------
//
// 爆発処理 [explosion.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// タイプ
typedef enum
{
	EXPLOSIONTYPE_THUNDER = 0,
	EXPLOSIONTYPE_HIBANA,
	EXPLOSIONTYPE_MAX
} EXPLOSIONTYPE;

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 size;
	D3DXVECTOR3 rot;
	D3DXMATRIX  mtxWorldExplosion;
	D3DXCOLOR	col;
	float fSize;		// サイズ
	float fAlpha;		// アルファ値変化
	float fDestSize;	// サイズ変化
	float fG;			// 重力
	int nLife;			// ライフ
	EXPLOSIONTYPE type;	// タイプ
	int nAttack;		// 攻撃力
	int nCounterAnim;	// カウンタ
	int nPatternAnim;	// パターンNo.
	bool bUse;
} EXPLOSION;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);

// 爆発設定
void SetExplosion(
	D3DXVECTOR3 pos,	// 位置
	EXPLOSIONTYPE type,	// タイプ
	float fSize, 		// サイズ
	int nAttack			// 攻撃力
);
// テクスチャー設定
void SetTexExplosion(
	VERTEX_3D *pVtx,	// 位置頂点情報
	float fBeginTex,	// 初めのテクスチャー
	float fEndTex		// 最後のテクスチャー
);
// 位置の設定
void SetPosExplosion(
	VERTEX_3D *pVtx,	// 位置頂点情報
	int nCntExplosion	// カウント
);

// 爆発の情報
EXPLOSION *GetExplosion(void);
#endif
