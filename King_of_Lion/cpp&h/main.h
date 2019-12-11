// ------------------------------------------------------------------------------------------
//
// メイン処理 [main.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include <windows.h>
#include "d3dx9.h"

#include <stdlib.h>						// システムヘッダー
#include <stdio.h>						// 一般的なヘッダー
#include <time.h>						// タイムヘッダー
#include <math.h>						// 数学関数ヘッダー
#include "Calculation.h"				// 計算用ヘッダー

#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"

#include "xaudio2.h"

// ------------------------------------------------------------------------------------------
// ライブラリのリンク
// ------------------------------------------------------------------------------------------
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define SCREEN_WIDTH	(1280)	// ウインドウの幅
#define SCREEN_HEIGHT	(720)	// ウインドウの高さ

#define	NUM_VERTEX		(4)		// 頂点数
#define	NUM_POLYGON		(2)		// ポリゴン数
#define ANIMATION_FOUR (4)
#define ANIMATION_FOUR_FTEX (1.0f / ANIMATION_FOUR)
#define ANIMATION_SIX (6)
#define ANIMATION_SIX_FTEX (1.0f / ANIMATION_SIX)


// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 頂点カラー / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 3Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線ベクトル /頂点カラー / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// ------------------------------------------------------------------------------------------
// 画面モードの種類
// ------------------------------------------------------------------------------------------
typedef enum
{
	MODE_NONE = 0,
	MODE_TITLE,		// タイトル
	MODE_TUTORIAL,	// チュートリアル
	MODE_GAME,		// ゲーム
	MODE_CLEAR,		// リザルト
	MODE_GAMEOVER,	// ゲームオーバー
	MODE_STATI,		// 統計
	MODE_RANK,		// ランキング
	MODE_POUSE,		// ポーズ
	MODE_MAX
} MODE;

// ------------------------------------------------------------------------------------------
// 構造体定義
// ------------------------------------------------------------------------------------------
// ２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

// 3Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;		// テクスチャー
} VERTEX_3D;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void SetMode(MODE mode);
MODE GetMode(void);					// モードの取得
LPDIRECT3DDEVICE9 GetDevice(void);
LPD3DXFONT	GetFont(void);			// フォントへのポインタ

#endif