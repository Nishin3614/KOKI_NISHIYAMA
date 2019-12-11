// ------------------------------------------
//
// インプット処理の説明[input.h]
// Author : Koki Nishiyama
//
// ------------------------------------------

#ifndef _INPUT_H_
#define _INPUT_H_	 // ファイル名を基準を決める

// ------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------
#include "main.h"

// ------------------------------------------
//
// マクロ関数
//
// ------------------------------------------

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CInput
{
public:
	/* 列挙型 */
	typedef enum
	{
		DEVICETYPE_KEYBOARD = 0,
		DEVICETYPE_JOYPAD
	} DEVICETYPE;

	/* 関数 */
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	static void ReleaseAll(void);							// 親子リリース

protected:
	static LPDIRECTINPUT8 m_pInput;							// 入力デバイス

private:
};

#endif // !_INPUT_H_