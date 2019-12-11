// ------------------------------------------------------------------------------------------
//
// 選択コメントヘッダー処理の説明[selectcoment.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _SELECTCOMENT_H_
#define _SELECTCOMENT_H_	 // ファイル名を基準を決める

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define SELECTCOMENT_ON_Y (320)		// 上
#define SELECTCOMENT_MAX (2)			// 画像数

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------タイプ---------- //
typedef enum
{
	SELECTCOMMENTTYPE_YES = 0,
	SELECTCOMMENTTYPE_NO,
	SELECTCOMMENTTYPE_COME,
	SELECTCOMMENTTYPE_MAX
} SELECTCOMMENTTYPE;

// ---------シーン---------- //
typedef enum
{
	SELECTCOMMENTSCENE_TUTORIALS = 0,
	SELECTCOMMENTSCENE_CONTINUE,
	SELECTCOMMENTSCENE_AREA,
	SELECTCOMMENTSCENE_MAX
} SELECTCOMMENTSCENE;

// ------------------------------------------------------------------------------------------
// 構造体定義
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR	col;		// カラー
	int nType;				// 種類
	int nCntCol;			// カラーカウント
	bool bUse;				// 使用しているかどうか
} SELECTCOMENT;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
// 基本プロトタイプ	//
void InitSelectComment(void);
void UninitSelectComment(void);
void UpdateSelectComment(void);
void DrawSelectComment(void);

void SceneSelectComment(int nScene);
int GetSelectCommentSelect(void);
#endif