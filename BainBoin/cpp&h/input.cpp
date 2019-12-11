// ------------------------------------------
//
// インプット処理の説明[input.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------
#include "input.h"

// ------------------------------------------
//
// マクロ関数
//
// ------------------------------------------

// ------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------
LPDIRECTINPUT8 CInput::m_pInput = NULL;

// ------------------------------------------
// コンストラクタ
// ------------------------------------------
CInput::CInput()
{
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CInput::~CInput()
{
}

// ------------------------------------------
// 初期化処理
// ------------------------------------------
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの作成
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8, 
		(void**)&m_pInput, 
		NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

// ------------------------------------------
// 終了処理
// ------------------------------------------
void CInput::Uninit(void)
{
}

// ------------------------------------------
// 更新処理
// ------------------------------------------
void CInput::Update(void)
{
}

// ------------------------------------------
// デバイス全体の終了処理
// ------------------------------------------
void CInput::ReleaseAll(void)
{
	// DirectKeyboardオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}
