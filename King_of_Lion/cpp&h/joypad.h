// ------------------------------------------------------------------------------------------
//
// joypad���̓w�b�_�[���� [joypad.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _JOYPAD_H_
#define _JOYPAD_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------
#define MAX_TRUN (32768.0f)

// ------------------------------------------------------------------------------------------
//
// �񋓌^
//
// ------------------------------------------------------------------------------------------

// joypad�̃{�^���ԍ��Œ�
typedef enum
{
	JOYPADKEY_X = 0,
	JOYPADKEY_Y,
	JOYPADKEY_A,
	JOYPADKEY_B,
	JOYPADKEY_LEFT_1,
	JOYPADKEY_RIGHT_1,
	JOYPADKEY_LEFT_2,
	JOYPADKEY_RIGHT_2,
	JOYPADKEY_LEFT_STICK,
	JOYPADKEY_RIGHT_STICK,
	JOYPADKEY_BACK,
	JOYPADKEY_START,
	JOYPADKEY_LEFT,
	JOYPADKEY_RIGHT,
	JOYPADKEY_UP,
	JOYPADKEY_DOWN,
	JOYPADKEY_MAX,
}JOYPADKEY;

// ------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ------------------------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);
BOOL CALLBACK EnumAxesCallback(const LPCDIDEVICEOBJECTINSTANCE pdidoi, LPVOID pContext);

HRESULT InitJoyboard(HINSTANCE hInstance, HWND hWnd);
void UninitJoyboard(void);
void UpdateJoyboard(void);

bool GetJoyboardPress(int nJoy,JOYPADKEY key);
bool GetJoyboardTrigger(int nJoy,JOYPADKEY key);
bool GetJoyboardRelease(int nJoy, JOYPADKEY key);
bool GetJoyboardRepeat(int nJoy, JOYPADKEY key);

int GetGamepadTriggerLeft(int nJoy, JOYPADKEY key);
int GetGamepadTriggerRight(int nJoy, JOYPADKEY key);

void GetGamepadStickLeft(int nJoy, int *pValueH, int *pValueV);
void GetGamepadStickRight(int nJoy, int *pValueH, int *pValueV);
void SetJoyStateGamepad(int nJoy);

#endif
