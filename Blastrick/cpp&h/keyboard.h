// ------------------------------------------
//
// 入力ヘッダー処理 [keyboard.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------
#include "main.h"
#include "input.h"

// ------------------------------------------
//
// マクロ定義
//
// ------------------------------------------
#define	NUM_KEY_MAX			(256)	// キーの最大数

// ------------------------------------------
//
// 前方宣言
//
// ------------------------------------------

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CKeyboard : public CInput
{
public:
	/* 関数 */
	CKeyboard();
	~CKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetKeyboardPress(int nKey);	// 押されている情報取得
	bool GetKeyboardTrigger(int nKey);	// 押した情報取得

protected:
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;	// 入力デバイスへのポインタ
	BYTE m_aState[NUM_KEY_MAX];		// 入力している情報
	BYTE m_aTrigger[NUM_KEY_MAX];	// 入力時情報
};
#endif
