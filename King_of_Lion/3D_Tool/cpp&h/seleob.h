// ------------------------------------------------------------------------------------------
//
// オブの選択処理 [seleob.h]
// Author : koki_nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _SELEOB_H_
#define _SELEOB_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
//
// 列挙型
//
// ------------------------------------------------------------------------------------------
// 選択状態
typedef enum
{
	SELEOBTYPE_NONE = 0,	// なし
	SELEOBTYPE_FLOOR,		// 床
	SELEOBTYPE_MATERIALS,	// 素材
	SELEOBTYPE_GIRAFFE,		// キリン
	SELEOBTYPE_BUFFALO,		// バッファロー
	SELEOBTYPE_MEERKAT,		// ミーアキャット
	SELEOBTYPE_CROCODILE,	// クロコダイル
	SELEOBTYPE_WALL,		// 壁
	SELEOBTYPE_BILLBOARD,	// ビルボード
	SELEOBTYPE_MAX,
} SELEOBTYPE;

// 方向
typedef enum
{
	SELEOBDICTION_X = 0,	// X
	SELEOBDICTION_Y,		// Y
	SELEOBDICTION_Z,		// Z
	SELEOBDICTION_MAX
} SELEOBDIRECTION;
// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 Worldpos;
	D3DXVECTOR3 Screenpos;
	D3DXVECTOR3 rot;
	D3DXMATRIX  mtxWorldSeleOb;
	SELEOBTYPE	type;
	int			nNumber;
	bool		bUse;
} SELEOB;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitSeleob(void);
void UninitSeleob(void);
void UpdateSeleob(void);
void DrawSeleob(void);

#endif
