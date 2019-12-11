// ------------------------------------------------------------------------------------------
//
// コントローラーヘッダー処理 [joypad.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _JOYPAD_H_
#define _JOYPAD_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------
#include "main.h"
#include "input.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------
#define MAX_GAMEPAD (1)

// ------------------------------------------------------------------------------------------
//
// 前方宣言
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------
class CJoypad : public CInput
{
public:
	/* 構造体番号(enum) */
	typedef enum
	{
		KEY_X = 0,
		KEY_Y,
		KEY_A,
		KEY_B,
		KEY_LEFT_1,
		KEY_RIGHT_1,
		KEY_LEFT_2,
		KEY_RIGHT_2,
		KEY_LEFT_STICK,
		KEY_RIGHT_STICK,
		KEY_BACK,
		KEY_START,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_UP,
		KEY_DOWN,
		KEY_MAX,
	}KEY;

	/* 関数 */
	CJoypad();
	~CJoypad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nJoy, KEY key);
	bool GetTrigger(int nJoy, KEY key);
	bool GetRelease(int nJoy, KEY key);
	bool GetRepeat(int nJoy, KEY key);
	int GetTriggerLeft(int nJoy, KEY key);
	int GetTriggerRight(int nJoy, KEY key);
	void GetStickLeft(int nJoy, int *pValueH, int *pValueV);
	void GetStickRight(int nJoy, int *pValueH, int *pValueV);

protected:
private:
	/* 変数宣言 */
	static LPDIRECTINPUTDEVICE8 m_pJoyDevice[MAX_GAMEPAD];	// 入力デバイスへのポインタ
	DIJOYSTATE2 m_aState[MAX_GAMEPAD];				// ゲームパッド状態を受け取るワーク
	D3DXVECTOR3 m_aStateAxis[MAX_GAMEPAD];			// スティックの位置情報
	int m_nCntWaitRepeat;						// カウントマックス
	int m_nIntervalRepeat;						// 往復の間
	static int m_nCntpad;								// カウント
	int m_aStateRepeatCnt[MAX_GAMEPAD][KEY_MAX];	// リピート回数
	bool m_abState[MAX_GAMEPAD][KEY_MAX];			// ゲームパッド状態の保存場所
	bool m_abStateTrigger[MAX_GAMEPAD][KEY_MAX];	// ゲームパッド状態のオン・オフ
	bool m_abStateRelease[MAX_GAMEPAD][KEY_MAX];	// ゲームパッド状態の返し
	bool m_abStateRepeat[MAX_GAMEPAD][KEY_MAX];		// ゲームパッド状態を繰り返し

	/* 関数 */
	static BOOL CALLBACK EnumCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);	// 能力取得
	void SetState(int nJoy);															// ボタン設定
};
#endif
