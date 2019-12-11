// ----------------------------------------
//
// デバッグ表示処理の説明[debugproc.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "debugproc.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

// ----------------------------------------
//
// グローバル変数
//
// ----------------------------------------

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------
LPD3DXFONT g_pDebugFont;
char g_aDebugStr[2048];

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void InitDebug(LPDIRECT3DDEVICE9 m_pDevice)
{
	// デバッグ表示用フォントを設定
	D3DXCreateFont(m_pDevice, 24, 10, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "ＭＳ ゴシック", &g_pDebugFont);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void UninitDebug(void)
{
	g_pDebugFont->Release();
}

// ----------------------------------------
// デバッグ表示処理
// ----------------------------------------
void PrintDebug(char * fmt, ...)
{
	// 変数宣言
	va_list va;	// 可変変数
	int nCntString;

	nCntString = strlen(g_aDebugStr);
	// 可変引数を1個にまとめる
	va_start(va, fmt);
	// まとめて表示
	vsprintf(&g_aDebugStr[nCntString], fmt, va);
	va_end(va);
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void DrawDebug(void)
{
	// 変数宣言
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// 表示場所・範囲
	// テキスト描画
	g_pDebugFont->DrawText(NULL, g_aDebugStr, -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	// テキストの初期化
	g_aDebugStr[0] = '\0';
}

