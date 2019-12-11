// ------------------------------------------------------------------------------------------
//
// サウンド処理 [sound.h]
// Author : Koki Nishiyama
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
	SOUND_LABEL_SE_ENTER,			// 決定音
	SOUND_LABEL_SE_ATTACK,			// 攻撃
	SOUND_LABEL_SE_COLISION,		// 当たった時
	SOUND_LABEL_SE_EAT,				// 食べたとき
	SOUND_LABEL_SE_BEAM,			// 弾発射時
	SOUND_LABEL_SE_BOSSVOICE,		// ボスの鳴き声
	SOUND_LABEL_SE_RANKING,			// ランキング発表
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
