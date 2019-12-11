// ------------------------------------------------------------------------------------------
//
// main処理の説明[main.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

#ifndef _MAIN_H_
#define _MAIN_H_	 // ファイル名を基準を決める

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>	// ウィンドウアプリケーション
#include "d3dx9.h"
#include <stdlib.h>		// システムヘッダー
#include <stdio.h>		// 一般的なヘッダー
#include <time.h>		// タイムヘッダー
#include <math.h>		// 数学関数ヘッダー
#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"						// 入力機器用
#include "xaudio2.h"					// BGM

// ------------------------------------------------------------------------------------------
//
// ライブラリのリンク
//
// ------------------------------------------------------------------------------------------
#pragma comment(lib,"d3d9.lib")		// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")	// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	// DirectXコンポーネント(部品)
#pragma comment (lib,"winmm.lib")
#pragma comment (lib, "dinput8.lib")

// ------------------------------------------------------------------------------------------
//
// マクロ関数定義
//
// ------------------------------------------------------------------------------------------
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2)
#define INTERVAL_TIME (60 * 10)

// ------------------------------------------------------------------------------------------
//
// 構造体
//
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float       rhw;	// 1.0で固定
	D3DCOLOR    col;	// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャー
} VERTEX_2D;

// ------------------------------------------------------------------------------------------
//
// 前方宣言
//
// ------------------------------------------------------------------------------------------
class CRenderer;
class CScene;

// ------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------------------------------------------------------
#if _DEBUG
int GetFPS(void);
#endif // _DEBUG

#endif // !_MAIN_H_