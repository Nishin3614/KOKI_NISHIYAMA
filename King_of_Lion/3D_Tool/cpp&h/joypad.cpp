// ------------------------------------------------------------------------------------------
//
// joypad入力処理 [joypad.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "joypad.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------
#define COUNT_WAIT_REPEAT (20)
#define INTERVAL_REPEAT (1)
#define MAX_GAMEPAD (4)
#define GAMEPAD_MAX_RANGE (1024)

// ------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ------------------------------------------------------------------------------------------
LPDIRECTINPUT8 g_pJoyput = NULL;							// DirectInputオブへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevJoyboard[MAX_GAMEPAD] = {};		// 入力デバイスへのポインタ
DIJOYSTATE2 g_aGamepadState[MAX_GAMEPAD];					// ゲームパッド状態を受け取るワーク
D3DXVECTOR3 g_aJoyStateAxis[MAX_GAMEPAD];					// スティックの位置情報
int g_nCntWaitRepeat=0;										// カウントマックス
int g_nIntervalRepeat=0;									// 往復の間
int g_nCntpad = 0;											// カウント
int g_aJoyStateRepeatCntGamepad[MAX_GAMEPAD][JOYPADKEY_MAX];	// リピート回数
bool g_aJoyStateGamepad[MAX_GAMEPAD][JOYPADKEY_MAX];			// ゲームパッド状態の保存場所
bool g_aJoyStateTriggerGamepad[MAX_GAMEPAD][JOYPADKEY_MAX];	// ゲームパッド状態のオン・オフ
bool g_aJoyStateReleaseGamepad[MAX_GAMEPAD][JOYPADKEY_MAX];	// ゲームパッド状態の返し
bool g_aJoyStateRepeatGamepad[MAX_GAMEPAD][JOYPADKEY_MAX];	// ゲームパッド状態を繰り返し

// ------------------------------------------------------------------------------------------
//
// キーボードの初期化
//
// ------------------------------------------------------------------------------------------
HRESULT InitJoyboard(HINSTANCE hInstance, HWND hWnd)
{
	// 変数宣言
	HRESULT hr;	// 結果

	// DirectInputオブジェクトの作成
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pJoyput, NULL);
	if (FAILED(hr))
	{
		return hr;
	}

	g_nCntWaitRepeat = COUNT_WAIT_REPEAT;
	g_nIntervalRepeat = INTERVAL_REPEAT;

	for (g_nCntpad = 0; g_nCntpad < MAX_GAMEPAD; g_nCntpad++)
	{
		// 入力デバイス（ゲームパッド）の作成
		hr = g_pJoyput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
		if (FAILED(hr) || g_pDevJoyboard[g_nCntpad] == NULL)
		{ 
			return hr;
		}

		// データフォーマットを設定
		hr = g_pDevJoyboard[g_nCntpad]->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			return hr;
		}

		// 協調モードを設定（フォアグラウンド＆非排他モード）
		hr = g_pDevJoyboard[g_nCntpad]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
		if (FAILED(hr))
		{
			return hr;
		}

		/* -------------------パッドの設定------------------- */
		// 変数宣言
		DIPROPRANGE diprg;			// デバイスの設定
		// 構造体の中身をNULLにする
		ZeroMemory(&diprg, sizeof(diprg));

		/* ----------diprgの中身---------- */
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
		g_pDevJoyboard[g_nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の設定
		diprg.diph.dwObj = DIJOFS_Y;	// パッドのyの情報
		g_pDevJoyboard[g_nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z軸の設定
		diprg.diph.dwObj = DIJOFS_Z;	// パッドのzの情報
		g_pDevJoyboard[g_nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// RZ軸の設定
		diprg.diph.dwObj = DIJOFS_RZ;	// パッドのrzの情報
		g_pDevJoyboard[g_nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 入力制御開始
		g_pDevJoyboard[g_nCntpad]->Acquire();

		// クリア
		memset(&g_aGamepadState[g_nCntpad], 0, sizeof(g_aGamepadState[g_nCntpad]));
		memset(&g_aJoyStateGamepad[g_nCntpad], 0, sizeof(g_aJoyStateGamepad[g_nCntpad]));
		memset(&g_aJoyStateTriggerGamepad[g_nCntpad], 0, sizeof(g_aJoyStateTriggerGamepad[g_nCntpad]));
		memset(&g_aJoyStateReleaseGamepad[g_nCntpad], 0, sizeof(g_aJoyStateReleaseGamepad[g_nCntpad]));
		memset(&g_aJoyStateRepeatGamepad[g_nCntpad], 0, sizeof(g_aJoyStateRepeatGamepad[g_nCntpad]));
	}
	// 初期化完了
	return S_OK;
}

// ------------------------------------------------------------------------------------------
//
// ジョイパッド能力取得
//
// ------------------------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	// 変数宣言
	static GUID pad_discrimination[2];
	DIDEVCAPS diDevCaps;

	if (FAILED(g_pJoyput->CreateDevice(pdidInstance->guidInstance, &g_pDevJoyboard[g_nCntpad], NULL)))
	{
		return DIENUM_CONTINUE;
	}

	// デバイスの能力取得
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(g_pDevJoyboard[g_nCntpad]->GetCapabilities(&diDevCaps)))
	{
		g_pDevJoyboard[g_nCntpad]->Release();
		g_pDevJoyboard[g_nCntpad] = NULL;
		return DIENUM_CONTINUE;
	}

	// デバイスの識別子を保存
	pad_discrimination[g_nCntpad] = pdidInstance->guidInstance;

	return DIENUM_STOP;
}
/*
// ------------------------------------------------------------------------------------------
//
// 入力の有無
//
// ------------------------------------------------------------------------------------------
BOOL CALLBACK EnumAxesCallback(const LPCDIDEVICEOBJECTINSTANCE pdidoi, LPVOID pContext)
{
	for (int nCntPad=0; nCntPad < 2; nCntPad++)
	{
		DIPROPRANGE diprg;

		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = pdidoi->dwType;
		diprg.diph.dwHow = DIPH_BYID;
		diprg.lMin = -32768;
		diprg.lMax = 32768;

		if (FAILED(g_pDevJoyboard[g_nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}
*/
// ------------------------------------------------------------------------------------------
//
// キーボードの終了処理
//
// ------------------------------------------------------------------------------------------
void UninitJoyboard(void)
{
	// 入力デバイス(キーボード)の開放
	for (g_nCntpad = 0; g_nCntpad < MAX_GAMEPAD; g_nCntpad++)
	{
		if (g_pDevJoyboard[g_nCntpad] != NULL)
		{
			//g_pDevJoyboard[g_nCntpad]->Unacquire();	// キーボードへのアクセス権を開放
			g_pDevJoyboard[g_nCntpad]->Release();
			g_pDevJoyboard[g_nCntpad] = NULL;

		}
	}
	// DirectInputオブジェクトの開放
	if (g_pJoyput != NULL)
	{
		g_pJoyput->Release();
		g_pJoyput = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// キーボードの更新処理
//
// ------------------------------------------------------------------------------------------
void UpdateJoyboard(void)
{
	bool aJoyState[JOYPADKEY_MAX];	// キーボードの入力情報
	int nCntPad;
	int nCntJoy;

	for (nCntPad = 0; nCntPad < MAX_GAMEPAD; nCntPad++)
	{
		if (g_pDevJoyboard[nCntPad] == NULL)
		{
			return;
		}

		// デバイスからデータを取得
		if (SUCCEEDED(g_pDevJoyboard[nCntPad]->GetDeviceState(sizeof(g_aGamepadState[nCntPad]), &g_aGamepadState[nCntPad])))
		{
			// キー情報設定
			SetJoyStateGamepad(nCntPad);

			// 軸位置を記録
			g_aJoyStateAxis[nCntPad].y = (float)g_aGamepadState[nCntPad].lX / GAMEPAD_MAX_RANGE;
			g_aJoyStateAxis[nCntPad].x = (float)g_aGamepadState[nCntPad].lY / GAMEPAD_MAX_RANGE;
			g_aJoyStateAxis[nCntPad].z = (float)g_aGamepadState[nCntPad].lZ / GAMEPAD_MAX_RANGE;

			for (nCntJoy = 0; nCntJoy < JOYPADKEY_MAX; nCntJoy++)
			{
				// キープレス情報保存
				aJoyState[nCntJoy] = g_aJoyStateGamepad[nCntPad][nCntJoy];	

				// トリガー・リリース・リピート情報の作成
				g_aJoyStateTriggerGamepad[nCntPad][nCntJoy] = (aJoyState[nCntJoy] ^ g_aJoyStateGamepad[nCntPad][nCntJoy]) & g_aJoyStateGamepad[nCntPad][nCntJoy];
				g_aJoyStateReleaseGamepad[nCntPad][nCntJoy] = (aJoyState[nCntJoy] ^ g_aJoyStateGamepad[nCntPad][nCntJoy]) & !g_aJoyStateGamepad[nCntPad][nCntJoy];
				g_aJoyStateRepeatGamepad[nCntPad][nCntJoy] = g_aJoyStateTriggerGamepad[nCntPad][nCntJoy];

				// ONの状態
				if (g_aJoyStateRepeatGamepad[nCntPad][nCntJoy])
				{
					g_aJoyStateRepeatCntGamepad[nCntPad][nCntJoy]++;
					if (g_aJoyStateRepeatCntGamepad[nCntPad][nCntJoy] < g_nCntWaitRepeat)
					{
						if (g_aJoyStateRepeatGamepad[nCntPad][nCntJoy] == 1)
						{
							g_aJoyStateRepeatGamepad[nCntPad][nCntJoy] = g_aJoyStateGamepad[nCntPad][nCntJoy];
						}

						else
						{
							g_aJoyStateRepeatGamepad[nCntPad][nCntJoy] = 0;
						}
					}

					else
					{
						if ((g_aJoyStateRepeatGamepad[nCntPad][nCntJoy] - g_nCntWaitRepeat) % g_nIntervalRepeat == 0)
						{
							g_aJoyStateRepeatGamepad[nCntPad][nCntJoy] = g_aJoyStateGamepad[nCntPad][nCntJoy];
						}

						else
						{
							g_aJoyStateRepeatGamepad[nCntPad][nCntJoy] = 0;
						}
					}
				}

				else
				{
					g_aJoyStateRepeatCntGamepad[nCntPad][nCntJoy] = 0;
					g_aJoyStateRepeatGamepad[nCntPad][nCntJoy] = 0;
				}
			}
		}
		else
		{
			g_pDevJoyboard[nCntPad]->Acquire();
		}
	}
}

// ------------------------------------------------------------------------------------------
//
// 押した処理
//
// ------------------------------------------------------------------------------------------

bool GetJoyboardPress(int nJoy, JOYPADKEY key)
{
	return g_aJoyStateGamepad[nJoy][key];
}

// ------------------------------------------------------------------------------------------
//
// 押されたときの処理
//
// ------------------------------------------------------------------------------------------

bool GetJoyboardTrigger(int nJoy, JOYPADKEY key)
{
	return g_aJoyStateTriggerGamepad[nJoy][key];
}

// ------------------------------------------------------------------------------------------
//
// 戻ってきたウルトラマン処理
//
// ------------------------------------------------------------------------------------------

bool GetJoyboardRelease(int nJoy, JOYPADKEY key)
{
	return g_aJoyStateReleaseGamepad[nJoy][key];
}

// ------------------------------------------------------------------------------------------
//
// 往復処理
//
// ------------------------------------------------------------------------------------------

bool GetJoyboardRepeat(int nJoy, JOYPADKEY key)
{
	return g_aJoyStateRepeatGamepad[nJoy][key];
}

// ------------------------------------------------------------------------------------------
//
// Lボタン押された処理
//
// ------------------------------------------------------------------------------------------

int GetGamepadTriggerLeft(int nJoy, JOYPADKEY key)
{
	return g_aGamepadState[nJoy].rgbButtons[JOYPADKEY_LEFT_2];
}

// ------------------------------------------------------------------------------------------
//
// Rボタン押された処理
//
// ------------------------------------------------------------------------------------------

int GetGamepadTriggerRight(int nJoy, JOYPADKEY key)
{
	return g_aGamepadState[nJoy].rgbButtons[JOYPADKEY_RIGHT_2];
}

// ------------------------------------------------------------------------------------------
//
// 左スティック押された処理
//
// ------------------------------------------------------------------------------------------

void GetGamepadStickLeft(int nJoy, int *pValueH, int *pValueV)
{
	*pValueH = g_aGamepadState[nJoy].lX;
	*pValueV = -g_aGamepadState[nJoy].lY;
}

// ------------------------------------------------------------------------------------------
//
// 右スティック押された処理
//
// ------------------------------------------------------------------------------------------

void GetGamepadStickRight(int nJoy, int * pValueH, int * pValueV)
{
	*pValueH = g_aGamepadState[nJoy].lZ;
	*pValueV = -g_aGamepadState[nJoy].lRz;
}

// ------------------------------------------------------------------------------------------
//
// キー情報設定
//
// ------------------------------------------------------------------------------------------
void SetJoyStateGamepad(int nJoy)
{
	// 変数宣言
	int nKey;

	if (g_aGamepadState[nJoy].rgdwPOV[0] >= 225 * 100 && g_aGamepadState[nJoy].rgdwPOV[0] <= 315 * 100)
	{
		// 十字キー[左]が押されている
		g_aJoyStateGamepad[nJoy][JOYPADKEY_LEFT] = true;
	}

	else
	{
		// 十字キー[左]が押されている
		g_aJoyStateGamepad[nJoy][JOYPADKEY_LEFT] = false;
	}

	if (g_aGamepadState[nJoy].rgdwPOV[0] >= 45 * 100 && g_aGamepadState[nJoy].rgdwPOV[0] <= 135 * 100)
	{
		// 十字キー[右]が押されている
		g_aJoyStateGamepad[nJoy][JOYPADKEY_RIGHT] = true;
	}

	else
	{
		// 十字キー[右]が押されている
		g_aJoyStateGamepad[nJoy][JOYPADKEY_RIGHT] = false;
	}

	if ((g_aGamepadState[nJoy].rgdwPOV[0] >= 315 * 100 && g_aGamepadState[nJoy].rgdwPOV[0] <= 360 * 100) ||
		(g_aGamepadState[nJoy].rgdwPOV[0] >= 0 * 100 && g_aGamepadState[nJoy].rgdwPOV[0] <= 45 * 100))
	{
		// 十字キー[上]が押されている
		g_aJoyStateGamepad[nJoy][JOYPADKEY_UP] = true;
	}

	else
	{
		// 十字キー[上]が押されている
		g_aJoyStateGamepad[nJoy][JOYPADKEY_UP] = false;
	}

	if (g_aGamepadState[nJoy].rgdwPOV[0] >= 135 * 100 && g_aGamepadState[nJoy].rgdwPOV[0] <= 225 * 100)
	{
		// 十字キー[下]が押されている
		g_aJoyStateGamepad[nJoy][JOYPADKEY_DOWN] = true;
	}

	else
	{
		// 十字キー[下]が押されている
		g_aJoyStateGamepad[nJoy][JOYPADKEY_DOWN] = false;
	}

	for (nKey = JOYPADKEY_X; nKey <= JOYPADKEY_START; nKey++)
	{
		if (g_aGamepadState[nJoy].rgbButtons[nKey])
		{
			// ボタンが押されている
			g_aJoyStateGamepad[nJoy][nKey] = true;
		}

		else
		{
			g_aJoyStateGamepad[nJoy][nKey] = false;
		}
	}
}