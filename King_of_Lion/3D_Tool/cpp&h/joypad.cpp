// ------------------------------------------------------------------------------------------
//
// joypad���͏��� [joypad.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "joypad.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------
#define COUNT_WAIT_REPEAT (20)
#define INTERVAL_REPEAT (1)
#define MAX_GAMEPAD (4)
#define GAMEPAD_MAX_RANGE (1024)

// ------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ------------------------------------------------------------------------------------------
LPDIRECTINPUT8 g_pJoyput = NULL;							// DirectInput�I�u�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevJoyboard[MAX_GAMEPAD] = {};		// ���̓f�o�C�X�ւ̃|�C���^
DIJOYSTATE2 g_aGamepadState[MAX_GAMEPAD];					// �Q�[���p�b�h��Ԃ��󂯎�郏�[�N
D3DXVECTOR3 g_aJoyStateAxis[MAX_GAMEPAD];					// �X�e�B�b�N�̈ʒu���
int g_nCntWaitRepeat=0;										// �J�E���g�}�b�N�X
int g_nIntervalRepeat=0;									// �����̊�
int g_nCntpad = 0;											// �J�E���g
int g_aJoyStateRepeatCntGamepad[MAX_GAMEPAD][JOYPADKEY_MAX];	// ���s�[�g��
bool g_aJoyStateGamepad[MAX_GAMEPAD][JOYPADKEY_MAX];			// �Q�[���p�b�h��Ԃ̕ۑ��ꏊ
bool g_aJoyStateTriggerGamepad[MAX_GAMEPAD][JOYPADKEY_MAX];	// �Q�[���p�b�h��Ԃ̃I���E�I�t
bool g_aJoyStateReleaseGamepad[MAX_GAMEPAD][JOYPADKEY_MAX];	// �Q�[���p�b�h��Ԃ̕Ԃ�
bool g_aJoyStateRepeatGamepad[MAX_GAMEPAD][JOYPADKEY_MAX];	// �Q�[���p�b�h��Ԃ��J��Ԃ�

// ------------------------------------------------------------------------------------------
//
// �L�[�{�[�h�̏�����
//
// ------------------------------------------------------------------------------------------
HRESULT InitJoyboard(HINSTANCE hInstance, HWND hWnd)
{
	// �ϐ��錾
	HRESULT hr;	// ����

	// DirectInput�I�u�W�F�N�g�̍쐬
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pJoyput, NULL);
	if (FAILED(hr))
	{
		return hr;
	}

	g_nCntWaitRepeat = COUNT_WAIT_REPEAT;
	g_nIntervalRepeat = INTERVAL_REPEAT;

	for (g_nCntpad = 0; g_nCntpad < MAX_GAMEPAD; g_nCntpad++)
	{
		// ���̓f�o�C�X�i�Q�[���p�b�h�j�̍쐬
		hr = g_pJoyput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
		if (FAILED(hr) || g_pDevJoyboard[g_nCntpad] == NULL)
		{ 
			return hr;
		}

		// �f�[�^�t�H�[�}�b�g��ݒ�
		hr = g_pDevJoyboard[g_nCntpad]->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			return hr;
		}

		// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		hr = g_pDevJoyboard[g_nCntpad]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
		if (FAILED(hr))
		{
			return hr;
		}

		/* -------------------�p�b�h�̐ݒ�------------------- */
		// �ϐ��錾
		DIPROPRANGE diprg;			// �f�o�C�X�̐ݒ�
		// �\���̂̒��g��NULL�ɂ���
		ZeroMemory(&diprg, sizeof(diprg));

		/* ----------diprg�̒��g---------- */
		/* diph�̒��g */
		diprg.diph.dwSize = sizeof(diprg);				// diprg�\���̂̃��������i�[
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);	// diph�\���̂̃��������i�[
		diprg.diph.dwHow = DIPH_BYOFFSET;				// ���̏��Ȃ̂���

		/* �p�b�h�̍ŏ��l�E�ő�l*/
		diprg.lMin = -GAMEPAD_MAX_RANGE;
		diprg.lMax = GAMEPAD_MAX_RANGE;

		/* ���ꂼ��̃p�b�h����ݒ� */
		// X���̐ݒ�
		diprg.diph.dwObj = DIJOFS_X;	// �p�b�h��x�̏��
		g_pDevJoyboard[g_nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���̐ݒ�
		diprg.diph.dwObj = DIJOFS_Y;	// �p�b�h��y�̏��
		g_pDevJoyboard[g_nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z���̐ݒ�
		diprg.diph.dwObj = DIJOFS_Z;	// �p�b�h��z�̏��
		g_pDevJoyboard[g_nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// RZ���̐ݒ�
		diprg.diph.dwObj = DIJOFS_RZ;	// �p�b�h��rz�̏��
		g_pDevJoyboard[g_nCntpad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ���͐���J�n
		g_pDevJoyboard[g_nCntpad]->Acquire();

		// �N���A
		memset(&g_aGamepadState[g_nCntpad], 0, sizeof(g_aGamepadState[g_nCntpad]));
		memset(&g_aJoyStateGamepad[g_nCntpad], 0, sizeof(g_aJoyStateGamepad[g_nCntpad]));
		memset(&g_aJoyStateTriggerGamepad[g_nCntpad], 0, sizeof(g_aJoyStateTriggerGamepad[g_nCntpad]));
		memset(&g_aJoyStateReleaseGamepad[g_nCntpad], 0, sizeof(g_aJoyStateReleaseGamepad[g_nCntpad]));
		memset(&g_aJoyStateRepeatGamepad[g_nCntpad], 0, sizeof(g_aJoyStateRepeatGamepad[g_nCntpad]));
	}
	// ����������
	return S_OK;
}

// ------------------------------------------------------------------------------------------
//
// �W���C�p�b�h�\�͎擾
//
// ------------------------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	// �ϐ��錾
	static GUID pad_discrimination[2];
	DIDEVCAPS diDevCaps;

	if (FAILED(g_pJoyput->CreateDevice(pdidInstance->guidInstance, &g_pDevJoyboard[g_nCntpad], NULL)))
	{
		return DIENUM_CONTINUE;
	}

	// �f�o�C�X�̔\�͎擾
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(g_pDevJoyboard[g_nCntpad]->GetCapabilities(&diDevCaps)))
	{
		g_pDevJoyboard[g_nCntpad]->Release();
		g_pDevJoyboard[g_nCntpad] = NULL;
		return DIENUM_CONTINUE;
	}

	// �f�o�C�X�̎��ʎq��ۑ�
	pad_discrimination[g_nCntpad] = pdidInstance->guidInstance;

	return DIENUM_STOP;
}
/*
// ------------------------------------------------------------------------------------------
//
// ���̗͂L��
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
// �L�[�{�[�h�̏I������
//
// ------------------------------------------------------------------------------------------
void UninitJoyboard(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	for (g_nCntpad = 0; g_nCntpad < MAX_GAMEPAD; g_nCntpad++)
	{
		if (g_pDevJoyboard[g_nCntpad] != NULL)
		{
			//g_pDevJoyboard[g_nCntpad]->Unacquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�����J��
			g_pDevJoyboard[g_nCntpad]->Release();
			g_pDevJoyboard[g_nCntpad] = NULL;

		}
	}
	// DirectInput�I�u�W�F�N�g�̊J��
	if (g_pJoyput != NULL)
	{
		g_pJoyput->Release();
		g_pJoyput = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// �L�[�{�[�h�̍X�V����
//
// ------------------------------------------------------------------------------------------
void UpdateJoyboard(void)
{
	bool aJoyState[JOYPADKEY_MAX];	// �L�[�{�[�h�̓��͏��
	int nCntPad;
	int nCntJoy;

	for (nCntPad = 0; nCntPad < MAX_GAMEPAD; nCntPad++)
	{
		if (g_pDevJoyboard[nCntPad] == NULL)
		{
			return;
		}

		// �f�o�C�X����f�[�^���擾
		if (SUCCEEDED(g_pDevJoyboard[nCntPad]->GetDeviceState(sizeof(g_aGamepadState[nCntPad]), &g_aGamepadState[nCntPad])))
		{
			// �L�[���ݒ�
			SetJoyStateGamepad(nCntPad);

			// ���ʒu���L�^
			g_aJoyStateAxis[nCntPad].y = (float)g_aGamepadState[nCntPad].lX / GAMEPAD_MAX_RANGE;
			g_aJoyStateAxis[nCntPad].x = (float)g_aGamepadState[nCntPad].lY / GAMEPAD_MAX_RANGE;
			g_aJoyStateAxis[nCntPad].z = (float)g_aGamepadState[nCntPad].lZ / GAMEPAD_MAX_RANGE;

			for (nCntJoy = 0; nCntJoy < JOYPADKEY_MAX; nCntJoy++)
			{
				// �L�[�v���X���ۑ�
				aJoyState[nCntJoy] = g_aJoyStateGamepad[nCntPad][nCntJoy];	

				// �g���K�[�E�����[�X�E���s�[�g���̍쐬
				g_aJoyStateTriggerGamepad[nCntPad][nCntJoy] = (aJoyState[nCntJoy] ^ g_aJoyStateGamepad[nCntPad][nCntJoy]) & g_aJoyStateGamepad[nCntPad][nCntJoy];
				g_aJoyStateReleaseGamepad[nCntPad][nCntJoy] = (aJoyState[nCntJoy] ^ g_aJoyStateGamepad[nCntPad][nCntJoy]) & !g_aJoyStateGamepad[nCntPad][nCntJoy];
				g_aJoyStateRepeatGamepad[nCntPad][nCntJoy] = g_aJoyStateTriggerGamepad[nCntPad][nCntJoy];

				// ON�̏��
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
// ����������
//
// ------------------------------------------------------------------------------------------

bool GetJoyboardPress(int nJoy, JOYPADKEY key)
{
	return g_aJoyStateGamepad[nJoy][key];
}

// ------------------------------------------------------------------------------------------
//
// �����ꂽ�Ƃ��̏���
//
// ------------------------------------------------------------------------------------------

bool GetJoyboardTrigger(int nJoy, JOYPADKEY key)
{
	return g_aJoyStateTriggerGamepad[nJoy][key];
}

// ------------------------------------------------------------------------------------------
//
// �߂��Ă����E���g���}������
//
// ------------------------------------------------------------------------------------------

bool GetJoyboardRelease(int nJoy, JOYPADKEY key)
{
	return g_aJoyStateReleaseGamepad[nJoy][key];
}

// ------------------------------------------------------------------------------------------
//
// ��������
//
// ------------------------------------------------------------------------------------------

bool GetJoyboardRepeat(int nJoy, JOYPADKEY key)
{
	return g_aJoyStateRepeatGamepad[nJoy][key];
}

// ------------------------------------------------------------------------------------------
//
// L�{�^�������ꂽ����
//
// ------------------------------------------------------------------------------------------

int GetGamepadTriggerLeft(int nJoy, JOYPADKEY key)
{
	return g_aGamepadState[nJoy].rgbButtons[JOYPADKEY_LEFT_2];
}

// ------------------------------------------------------------------------------------------
//
// R�{�^�������ꂽ����
//
// ------------------------------------------------------------------------------------------

int GetGamepadTriggerRight(int nJoy, JOYPADKEY key)
{
	return g_aGamepadState[nJoy].rgbButtons[JOYPADKEY_RIGHT_2];
}

// ------------------------------------------------------------------------------------------
//
// ���X�e�B�b�N�����ꂽ����
//
// ------------------------------------------------------------------------------------------

void GetGamepadStickLeft(int nJoy, int *pValueH, int *pValueV)
{
	*pValueH = g_aGamepadState[nJoy].lX;
	*pValueV = -g_aGamepadState[nJoy].lY;
}

// ------------------------------------------------------------------------------------------
//
// �E�X�e�B�b�N�����ꂽ����
//
// ------------------------------------------------------------------------------------------

void GetGamepadStickRight(int nJoy, int * pValueH, int * pValueV)
{
	*pValueH = g_aGamepadState[nJoy].lZ;
	*pValueV = -g_aGamepadState[nJoy].lRz;
}

// ------------------------------------------------------------------------------------------
//
// �L�[���ݒ�
//
// ------------------------------------------------------------------------------------------
void SetJoyStateGamepad(int nJoy)
{
	// �ϐ��錾
	int nKey;

	if (g_aGamepadState[nJoy].rgdwPOV[0] >= 225 * 100 && g_aGamepadState[nJoy].rgdwPOV[0] <= 315 * 100)
	{
		// �\���L�[[��]��������Ă���
		g_aJoyStateGamepad[nJoy][JOYPADKEY_LEFT] = true;
	}

	else
	{
		// �\���L�[[��]��������Ă���
		g_aJoyStateGamepad[nJoy][JOYPADKEY_LEFT] = false;
	}

	if (g_aGamepadState[nJoy].rgdwPOV[0] >= 45 * 100 && g_aGamepadState[nJoy].rgdwPOV[0] <= 135 * 100)
	{
		// �\���L�[[�E]��������Ă���
		g_aJoyStateGamepad[nJoy][JOYPADKEY_RIGHT] = true;
	}

	else
	{
		// �\���L�[[�E]��������Ă���
		g_aJoyStateGamepad[nJoy][JOYPADKEY_RIGHT] = false;
	}

	if ((g_aGamepadState[nJoy].rgdwPOV[0] >= 315 * 100 && g_aGamepadState[nJoy].rgdwPOV[0] <= 360 * 100) ||
		(g_aGamepadState[nJoy].rgdwPOV[0] >= 0 * 100 && g_aGamepadState[nJoy].rgdwPOV[0] <= 45 * 100))
	{
		// �\���L�[[��]��������Ă���
		g_aJoyStateGamepad[nJoy][JOYPADKEY_UP] = true;
	}

	else
	{
		// �\���L�[[��]��������Ă���
		g_aJoyStateGamepad[nJoy][JOYPADKEY_UP] = false;
	}

	if (g_aGamepadState[nJoy].rgdwPOV[0] >= 135 * 100 && g_aGamepadState[nJoy].rgdwPOV[0] <= 225 * 100)
	{
		// �\���L�[[��]��������Ă���
		g_aJoyStateGamepad[nJoy][JOYPADKEY_DOWN] = true;
	}

	else
	{
		// �\���L�[[��]��������Ă���
		g_aJoyStateGamepad[nJoy][JOYPADKEY_DOWN] = false;
	}

	for (nKey = JOYPADKEY_X; nKey <= JOYPADKEY_START; nKey++)
	{
		if (g_aGamepadState[nJoy].rgbButtons[nKey])
		{
			// �{�^����������Ă���
			g_aJoyStateGamepad[nJoy][nKey] = true;
		}

		else
		{
			g_aJoyStateGamepad[nJoy][nKey] = false;
		}
	}
}