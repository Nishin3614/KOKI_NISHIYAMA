// ------------------------------------------------------------------------------------------
//
// コメントヘッダー処理の説明[tutorialcomment.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

#ifndef _TUTORIALCOMMENT_H_
#define _TUTORIALCOMMENT_H_	 // ファイル名を基準を決める

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include "main.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------

#define TUTORIALCOMMENT_ON_Y (320)		// 上

// ------------------------------------------------------------------------------------------
//
// コメント種類
//
// ------------------------------------------------------------------------------------------

typedef enum
{
	TUTORIALCOMMENTTYPE_BG = 0,
	TUTORIALCOMMENTTYPE_KEYBOARD,
	TUTORIALCOMMENTTYPE_JOYPAD,
	TUTORIALCOMMENTTYPE_MAX
}TUTORIALCOMMENTTYPE;
// ------------------------------------------------------------------------------------------
//
// 構造体定義
//
// ------------------------------------------------------------------------------------------

typedef struct
{
	TUTORIALCOMMENTTYPE Type;	// タイプ
	D3DXVECTOR3 pos;	// 位置
	D3DXCOLOR	col;	// カラー
	float fXSize;		// Xサイズ
	float fYSize;		// Yサイズ
	bool bUse;			// 使用しているかどうか
} TUTORIALCOMMENT;

// ------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------------------------------------------------------

// 基本プロトタイプ	//
void InitTutorialComment(void);
void UninitTutorialComment(void);
void UpdateTutorialComment(void);
void DrawTutorialComment(void);
void SetTutorialComment(
	TUTORIALCOMMENTTYPE Type,
	D3DXVECTOR3 pos,
	D3DXCOLOR col,
	float fXSize,
	float fYSize);

TUTORIALCOMMENT *GetTutorialComment(void);

#endif