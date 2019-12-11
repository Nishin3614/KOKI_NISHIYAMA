// ------------------------------------------------------------------------------------------
//
// サウンド処理 [sound.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// ------------------------------------------------------------------------------------------
//
// サウンドファイル
//
// ------------------------------------------------------------------------------------------
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// タイトル
	SOUND_LABEL_BGM_GAME,			// ゲーム
	SOUND_LABEL_BGM_GAMEOVER,		// ゲームオーバー
	SOUND_LABEL_BGM_CLEAR,			// クリア
	SOUND_LABEL_BGM_RANKINGMOVE,	// ランキング
	SOUND_LABEL_SE_START,			// 決定音
	SOUND_LABEL_SE_LOWKICK,			// ヒット音
	SOUND_LABEL_SE_HIGHKICK,		// エネミーが倒された音
	SOUND_LABEL_SE_CATFOOD,			// 猫缶音
	SOUND_LABEL_SE_DIAMOND,			// ダイヤモンド音
	SOUND_LABEL_SE_CATRECOVERY,		// 回復アイテム音
	SOUND_LABEL_SE_CATDAMEGE,		// プレイヤが攻撃された音
	SOUND_LABEL_SE_CATDIE,			// プレイヤが死んだ音
	SOUND_LABEL_SE_BOSSLAST,		// ボスが倒された音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

// ------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------------------------------------------------------
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
