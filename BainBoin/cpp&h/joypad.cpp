// ------------------------------------------
//
// コントローラー処理 [joypad.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "joypad.h"

// ------------------------------------------
//
// マクロ定義
//
// ------------------------------------------
#define GAMEPAD_MAX_RANGE (1024)
#define COUNT_WAIT_REPEAT (20)
#define INTERVAL_REPEAT (1)

// ------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------
LPDIRECTINPUTDEVICE8 CJoypad::m_pJoyDevice[MAX_GAMEPAD] = {};	// 入力デバイスへのポインタ
int CJoypad::m_nCntpad = 0;								// カウント

// ------------------------------------------
//
// グローバル変数
//
// ------------------------------------------

// ------------------------------------------
// コンストラクタ
// ------------------------------------------
CJoypad::CJoypad()
{
	for (int nCntpad = 0; nCntpad < MAX_GAMEPAD; nCntpad++)
	{
		// クリア
		memset(&m_aState[nCntpad], 0, sizeof(m_aState[nCntpad]));
		memset(&m_abState[nCntpad], 0, sizeof(m_abState[nCntpad]));
		memset(&m_abStateTrigger[nCntpad], 0, sizeof(m_abStateTrigger[nCntpad]));
		memset(&m_abStateRelease[nCntpad], 0, sizeof(m_abStateRelease[nCntpad]));
		memset(&m_abStateRepeat[nCntpad], 0, sizeof(m_abStateRepeat[nCntpad]));
	}
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CJoypad::~CJoypad()
{
}

// ------------------------------------------
// 初期化
// ------------------------------------------
HRESULT CJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// デバイスの初期化
	CInput::Init(hInstance, hWnd);
	// 時間
	m_nCntWaitRepeat = COUNT_WAIT_REPEAT;
	m_nIntervalRepeat = INTERVAL_REPEAT;

	for (int nCntpad = 0; nCntpad < MAX_GAMEPAD; nCntpad++)
	{
		// 入力デバイス（ゲームパッド）の作成
		if (FAILED(m_pInput->EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			EnumCallback,
			NULL,
			DIEDFL_ATTACHEDONLY)) ||
			m_pJoyDevice[nCntpad] == NULL)
		{
			return E_FAIL;
		}
		// データフォーマットを設定
		if (FAILED(m_pJoyDevice[nCntpad]->SetDataFormat(&c_dfDIJoystick2)))
		{
			return E_FAIL;
		}
		// 協調モードを設定（フォアグラウンド＆非排他モード）
		if (FAILED(m_pJoyDevice[nCntpad]->SetCooperativeLevel(
			hWnd,
			(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return E_FAIL;
		}

		/* -------------------パッドの設定------------------- */
		// 変数宣言
		DIPROPRANGE diprg;			// デバイスの設定
		// 構造体の中身をNULLにする
		ZeroMemory(&diprg, sizeof(diprg));

		/* ----------diprgの設定---------- */
		/* diphの中身 */
		diprg.diph.dwSize = sizeof(diprg);				// diprg構造体のメモリを格納
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);	// diph構造体のメモリを格納
		diprg.diph.dwHow = DIPH_BYOFFSET;				// 何の情報なのじゃ

														/* パッドの最小値・最大値*/
		diprg.lMin = -GAMEPAD_MAX_RANGE;
		diprg.lMax = GAMEPAD_MAX_RANGE;

		/* それぞれのパッド情報を設定 */
		// X軸の設定
		diprg.diph.dwObj = DIJOFS_X;	// パッドのxの情報
		m_pJoyDevice[nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の設定
		diprg.diph.dwObj = DIJOFS_Y;	// パッドのyの情報
		m_pJoyDevice[nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z軸の設定
		diprg.diph.dwObj = DIJOFS_Z;	// パッドのzの情報
		m_pJoyDevice[nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// RZ軸の設定
		diprg.diph.dwObj = DIJOFS_RZ;	// パッドのrzの情報
		m_pJoyDevice[nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 入力制御開始
		m_pJoyDevice[nCntpad]->Acquire();

		// クリア
		memset(&m_aState[nCntpad], 0, sizeof(m_aState[nCntpad]));
		memset(&m_abState[nCntpad], 0, sizeof(m_abState[nCntpad]));
		memset(&m_abStateTrigger[nCntpad], 0, sizeof(m_abStateTrigger[nCntpad]));
		memset(&m_abStateRelease[nCntpad], 0, sizeof(m_abStateRelease[nCntpad]));
		memset(&m_abStateRepeat[nCntpad], 0, sizeof(m_abStateRepeat[nCntpad]));
		//// 入力デバイス(キーボード)の開放
		if (m_pJoyDevice[nCntpad] != NULL)
		{
			m_aState[nCntpad].lX = 0;
			m_aState[nCntpad].lY = 0;
		}
	}


	// 初期化完了
	return S_OK;
}

// ------------------------------------------
// 終了
// ------------------------------------------
void CJoypad::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
	{
		// 入力デバイス(キーボード)の開放
		if (m_pJoyDevice[nCnt]!= NULL)
		{
			m_pJoyDevice[nCnt]->Unacquire();	// キーボードへのアクセス権を開放
			m_pJoyDevice[nCnt]->Release();
			m_pJoyDevice[nCnt] = NULL;
		}
	}

	// コントローラーデバイスの初期化
	CInput::Uninit();
}

// ------------------------------------------
// 更新
// ------------------------------------------
void CJoypad::Update(void)
{
	bool aJoyState[KEY_MAX];	// キーボードの入力情報
	int nCntPad;
	int nCntJoy;

	for (nCntPad = 0; nCntPad < MAX_GAMEPAD; nCntPad++)
	{
		if (m_pJoyDevice[nCntPad] == NULL)
		{
			return;
		}

		for (nCntJoy = 0; nCntJoy < KEY_MAX; nCntJoy++)
		{
			// キープレス情報保存
			aJoyState[nCntJoy] = m_abState[nCntPad][nCntJoy];
		}
		// デバイスからデータを取得
		if (SUCCEEDED(m_pJoyDevice[nCntPad]->GetDeviceState(sizeof(m_aState[nCntPad]), &m_aState[nCntPad])))
		{
			// キー情報設定
			SetState(nCntPad);

			// 軸位置を記録
			m_aStateAxis[nCntPad].y = (float)m_aState[nCntPad].lX / GAMEPAD_MAX_RANGE;
			m_aStateAxis[nCntPad].x = (float)m_aState[nCntPad].lY / GAMEPAD_MAX_RANGE;
			m_aStateAxis[nCntPad].z = (float)m_aState[nCntPad].lZ / GAMEPAD_MAX_RANGE;

			for (nCntJoy = 0; nCntJoy < KEY_MAX; nCntJoy++)
			{
				// トリガー・リリース・リピート情報の作成
				m_abStateTrigger[nCntPad][nCntJoy] = (aJoyState[nCntJoy] ^ m_abState[nCntPad][nCntJoy]) & m_abState[nCntPad][nCntJoy];
				m_abStateRelease[nCntPad][nCntJoy] = (aJoyState[nCntJoy] ^ m_abState[nCntPad][nCntJoy]) & !m_abState[nCntPad][nCntJoy];
				m_abStateRepeat[nCntPad][nCntJoy] = m_abStateTrigger[nCntPad][nCntJoy];

				// ONの状態
				if (m_abStateRepeat[nCntPad][nCntJoy])
				{
					m_aStateRepeatCnt[nCntPad][nCntJoy]++;
					if (m_aStateRepeatCnt[nCntPad][nCntJoy] < m_nCntWaitRepeat)
					{
						if (m_abStateRepeat[nCntPad][nCntJoy] == 1)
						{
							m_abStateRepeat[nCntPad][nCntJoy] = m_abState[nCntPad][nCntJoy];
						}

						else
						{
							m_abStateRepeat[nCntPad][nCntJoy] = 0;
						}
					}

					else
					{
						if ((m_abStateRepeat[nCntPad][nCntJoy] - m_nCntWaitRepeat) % m_nIntervalRepeat == 0)
						{
							m_abStateRepeat[nCntPad][nCntJoy] = m_abState[nCntPad][nCntJoy];
						}

						else
						{
							m_abStateRepeat[nCntPad][nCntJoy] = 0;
						}
					}
				}

				else
				{
					m_aStateRepeatCnt[nCntPad][nCntJoy] = 0;
					m_abStateRepeat[nCntPad][nCntJoy] = 0;
				}
			}
		}
		else
		{
			m_pJoyDevice[nCntPad]->Acquire();
		}
	}
}

// ------------------------------------------
// ジョイパッド能力取得
// ------------------------------------------
BOOL CALLBACK CJoypad::EnumCallback(const DIDEVICEINSTANCE * pdidInstance, VOID * pContext)
{
	// 変数宣言
	static GUID pad_discrimination;
	DIDEVCAPS diDevCaps;

	if (FAILED(m_pInput->CreateDevice(pdidInstance->guidInstance, &m_pJoyDevice[m_nCntpad], NULL)))
	{
		return DIENUM_CONTINUE;
	}

	// デバイスの能力取得
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(m_pJoyDevice[m_nCntpad]->GetCapabilities(&diDevCaps)))
	{
		m_pJoyDevice[m_nCntpad]->Release();
		m_pJoyDevice[m_nCntpad] = NULL;
		return DIENUM_CONTINUE;
	}

	// デバイスの識別子を保存
	pad_discrimination = pdidInstance->guidInstance;
	return DIENUM_STOP;
}

// ------------------------------------------
// ボタンの設定
// ------------------------------------------
void CJoypad::SetState(int nJoy)
{
	// 変数宣言
	int nKey;

	if (m_aState[nJoy].rgdwPOV[0] >= 225 * 100 && m_aState[nJoy].rgdwPOV[0] <= 315 * 100)
	{
		// 十字キー[左]が押されている
		m_abState[nJoy][KEY_LEFT] = true;
	}

	else
	{
		// 十字キー[左]が押されている
		m_abState[nJoy][KEY_LEFT] = false;
	}

	if (m_aState[nJoy].rgdwPOV[0] >= 45 * 100 && m_aState[nJoy].rgdwPOV[0] <= 135 * 100)
	{
		// 十字キー[右]が押されている
		m_abState[nJoy][KEY_RIGHT] = true;
	}

	else
	{
		// 十字キー[右]が押されている
		m_abState[nJoy][KEY_RIGHT] = false;
	}

	if ((m_aState[nJoy].rgdwPOV[0] >= 315 * 100 && m_aState[nJoy].rgdwPOV[0] <= 360 * 100) ||
		(m_aState[nJoy].rgdwPOV[0] >= 0 * 100 && m_aState[nJoy].rgdwPOV[0] <= 45 * 100))
	{
		// 十字キー[上]が押されている
		m_abState[nJoy][KEY_UP] = true;
	}

	else
	{
		// 十字キー[上]が押されている
		m_abState[nJoy][KEY_UP] = false;
	}

	if (m_aState[nJoy].rgdwPOV[0] >= 135 * 100 && m_aState[nJoy].rgdwPOV[0] <= 225 * 100)
	{
		// 十字キー[下]が押されている
		m_abState[nJoy][KEY_DOWN] = true;
	}

	else
	{
		// 十字キー[下]が押されている
		m_abState[nJoy][KEY_DOWN] = false;
	}

	for (nKey = KEY_X; nKey <= KEY_START; nKey++)
	{
		if (m_aState[nJoy].rgbButtons[nKey])
		{
			// ボタンが押されている
			m_abState[nJoy][nKey] = true;
		}

		else
		{
			m_abState[nJoy][nKey] = false;
		}
	}
}

// ------------------------------------------
// 押した処理
// ------------------------------------------
bool CJoypad::GetPress(int nJoy, KEY key)
{
	return m_abState[nJoy][key];
}

// ------------------------------------------
// 押されたときの処理
// ------------------------------------------
bool CJoypad::GetTrigger(int nJoy, KEY key)
{
	return m_abStateTrigger[nJoy][key];
}

// ------------------------------------------
// リリース処理
// ------------------------------------------
bool CJoypad::GetRelease(int nJoy, KEY key)
{
	return m_abStateRelease[nJoy][key];
}

// ------------------------------------------
// 往復処理
// ------------------------------------------
bool CJoypad::GetRepeat(int nJoy, KEY key)
{
	return m_abStateRepeat[nJoy][key];
}

// ------------------------------------------
// Lボタン押された処理
// ------------------------------------------
int CJoypad::GetTriggerLeft(int nJoy, KEY key)
{
	return m_aState[nJoy].rgbButtons[KEY_LEFT_2];
}

// ------------------------------------------
// Rボタン押された処理
// ------------------------------------------
int CJoypad::GetTriggerRight(int nJoy, KEY key)
{
	return m_aState[nJoy].rgbButtons[KEY_RIGHT_2];
}

// ------------------------------------------
// 左スティック押された処理
// ------------------------------------------
void CJoypad::GetStickLeft(int nJoy, int * pValueH, int * pValueV)
{
	*pValueH = m_aState[nJoy].lX;
	*pValueV = -m_aState[nJoy].lY;
}

// ------------------------------------------
// 右スティック押された処理
// ------------------------------------------
void CJoypad::GetStickRight(int nJoy, int * pValueH, int * pValueV)
{
	*pValueH = m_aState[nJoy].lZ;
	*pValueV = -m_aState[nJoy].lRz;
}
