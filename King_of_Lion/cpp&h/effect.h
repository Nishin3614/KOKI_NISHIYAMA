// ------------------------------------------------------------------------------------------
//
// エフェクト処理 [effect.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------出現時の形---------- //
typedef enum
{
	EFFECTAPPEARANCE_NEUTRAL = 0,
	EFFECTAPPEARANCE_NORMAL,		// 位置はそのまま
	EFFECTAPPEARANCE_RAND,			// 位置バラバラ
	EFFECTAPPEARANCE_RING,			// 位置輪っか
	EFFECTAPPEARANCE_MAX
} EFFECTAPPEARANCE;

// ---------出現後の動き---------- //
typedef enum
{
	EFFECTMOVETYPE_NEUTRAL = 0,	
	EFFECTMOVETYPE_NORMAL,		// 普通
	EFFECTMOVETYPE_UP,			// 上昇
	EFFECTMOVETYPE_DOWN,		// 下降
	EFFECTMOVETYPE_G,			// 重力
	EFFECTMOVETYPE_RAND,		// ランダム
	EFFECTMOVETYPE_MAX
} EFFECTMOVETYPE;

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
typedef struct
{								
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 size;			// サイズ
	D3DXVECTOR3 rot;			// 回転
	D3DXMATRIX  mtxWorldEffect;	// 行列
	D3DXCOLOR	col;			// 色
	EFFECTMOVETYPE MoveType;	// 移動タイプ
	float fSize;				// サイズ
	float fAlpha;				// アルファ値変化
	float fDestSize;			// サイズ変化
	int nLife;					// ライフ
	int nType;					// タイプ
	float fG;					// 重力
	bool bUse;			// 使用状態
} EFFECT;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

// 設定
// エフェクト基本情報の設定
void SetInformation(
	D3DXVECTOR3 pos,			// 位置
	D3DXCOLOR col,				// 色
	EFFECTAPPEARANCE Appearnce,	// 出現時
	EFFECTMOVETYPE Movetype ,	// 移動タイプ
	int nMaxEffect,				// 最大数
	float fSize,				// サイズ
	float fSpeed, 				// 速さ
	int nLife					// ライフ
	);

void SetMove(
	D3DXVECTOR3 pos,			// 位置
	D3DXCOLOR col,				// 色
	EFFECTMOVETYPE Movetype,	// 移動タイプ
	float fSize, 				// サイズ
	float fSpeed,				// 速さ
	int nLife					// ライフ
);

// effectの設定
void SetEffect(
	D3DXVECTOR3 pos,	// 位置
	D3DXVECTOR3 move,	// 移動
	D3DXCOLOR col,		// 色
	float fSize, 		// サイズ
	float fDestSize,	// サイズ変更
	float fG,			// 重力
	int nLife,			// ライフ
	int nType,			// タイプ
	int nMaxEffect		// 最大数
);
// エフェクトの情報
EFFECT *GetEffect(void);
#endif
