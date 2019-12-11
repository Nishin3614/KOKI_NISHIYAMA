// ------------------------------------------
//
// gametly処理の説明[main.h]
// Author : Koki Nishiyama
//
// ------------------------------------------

#ifndef _MAIN_H_
#define _MAIN_H_	 // ファイル名を基準を決める

// ------------------------------------------
// インクルードファイル
// ------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>	// ウィンドウアプリケーション
#include "d3dx9.h"
#include <stdlib.h>		// システムヘッダー
#include <stdio.h>		// 一般的なヘッダー
#include <time.h>		// タイムヘッダー
#include <math.h>		// 数学関数ヘッダー
#include <vector>
#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"						// 入力機器用
#include "xaudio2.h"					// BGM

// ------------------------------------------
//
// ライブラリのリンク
//
// ------------------------------------------
#pragma comment(lib,"d3d9.lib")		// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")	// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	// DirectXコンポーネント(部品)
#pragma comment (lib,"winmm.lib")
#pragma comment (lib, "dinput8.lib")

// ------------------------------------------
//
// マクロ関数定義
//
// ------------------------------------------
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 頂点カラー / テクスチャ座標 )
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2)
// 3Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線ベクトル /頂点カラー / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)
#define INTERVAL_TIME (60 * 10)

// ------------------------------------------
//
// 構造体
//
// ------------------------------------------
// ２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float       rhw;	// 1.0で固定
	D3DCOLOR    col;	// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャー
} VERTEX_2D;

// 3Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;		// テクスチャー
} VERTEX_3D;

// ------------------------------------------
//
// 前方宣言
//
// ------------------------------------------
class CRenderer;

// ------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------
#if _DEBUG
int GetFPS(void);
#endif // _DEBUG

#endif // !_MAIN_H_