//*************************************************************************************************************
//
// オブジェクトウィンドウ管理[objwindow.h]
// Author : Koki Nishiyama
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "objwindow.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define EDITWINDOWCLASS_NAME	"PlayerEditorWindow"
#define EDITWINDOW_NAME			"PlayerEditor"
#define EDITWINDOW_SIZEX		(300)
#define EDITWINDOW_SIZEY		(500)

#define EDITITEM_SIZEX			(125)
#define EDITITEM_SIZEY			(20)

#define EDITBUTTON_ID			(1000)


//-------------------------------------------------------------------------------------------------------------
// 構造体
//-------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 *pos = NULL;
	D3DXVECTOR3 *size = NULL;
	int	*nBlock_H = NULL;
	int *nBlock_W = NULL;
} OBJWINDOWINFO;

//-------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK EditWindowProcedure(
	HWND hWnd,
	UINT uMsg, 
	WPARAM wParam,
	LPARAM lParam
);
HWND SetWindowEditDisabled(
	HWND *hWndthis, 
	HWND hWnd, 
	int nID,
	INTEGER2 pos,
	char* name
);
HWND SetWindowEdit(
	HWND *hWndthis, 
	HWND hWnd, 
	int nID,
	INTEGER2 pos, 
	int *nValue = NULL,
	float *fValue = NULL
);

//-------------------------------------------------------------------------------------------------------------
// グローバル変数
//-------------------------------------------------------------------------------------------------------------
WNDCLASSEX	g_objwindWcex;
HWND		g_objwindhWnd;
RECT		g_objwindrect;
OBJWINDOWINFO	g_objwind;

//-------------------------------------------------------------------------------------------------------------
// 詳細設定
//-------------------------------------------------------------------------------------------------------------
void SetObjWindow(
	D3DXVECTOR3 *pos,	// 位置
	D3DXVECTOR3 *size,	// サイズ
	int *nBlock_H,		// 縦ブロック
	int *nBlock_W		// 横ブロック
)
{
	g_objwind.pos = pos;
	g_objwind.size = size;
	g_objwind.nBlock_H = nBlock_H;
	g_objwind.nBlock_W = nBlock_W;
}

//-------------------------------------------------------------------------------------------------------------
// ウィンドウの登録
//-------------------------------------------------------------------------------------------------------------
void EditWindowRegister(HINSTANCE hInstance)
{
	g_objwindWcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEXのメモリサイズの指定
		CS_CLASSDC,						// 表示するウィンドウのスタイルを設定
		EditWindowProcedure,			// ウィンドウプロシージャのアドレス（関数名）を指定
		0,								// 通常は使用は使用しないので0指定
		0,								// 通常は使用は使用しないので0指定
		hInstance,						// Windowsの引数の院スタンドハンドル指定
		LoadIcon(NULL,IDI_APPLICATION),	// タスクバーに使用するアイコン指定
		LoadCursor(NULL,IDC_ARROW),		// 使用するマウスカーソル指定
		(HBRUSH)BLACK_PEN,				// ウィンドウクライアント領域の色指定
		NULL,							// メニューを指定
		EDITWINDOWCLASS_NAME,			// ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	// 小さいアイコン指定
	};

	// ウィンドウクラスの登録
	RegisterClassEx(&g_objwindWcex);
}

//-------------------------------------------------------------------------------------------------------------
// ウィンドウの作成
//-------------------------------------------------------------------------------------------------------------
void CreateEditWindow(HINSTANCE hInstance, int CmdShow)
{
	g_objwindrect = { 0,0,EDITWINDOW_SIZEX,EDITWINDOW_SIZEY };
	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&g_objwindrect, WS_OVERLAPPEDWINDOW, true);

	// ウィンドウの生成
	g_objwindhWnd = CreateWindowEx(0,			// 拡張ウィンドウスタイル
		EDITWINDOWCLASS_NAME,					// ウィンドウクラス名
		EDITWINDOW_NAME,				// ウィンドウの名前
		WS_EX_DLGMODALFRAME | WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME,		// ウィンドウスタイル
		CW_USEDEFAULT,								// ウィンドウ左上X座標
		CW_USEDEFAULT,								// ウィンドウ左上Y座標
		(g_objwindrect.right - g_objwindrect.left),					// ウィンドウの幅
		(g_objwindrect.bottom - g_objwindrect.top),					// ウィンドウの高さ
		NULL,							// 親ウィンドウのハンドル
		NULL,							// メニューもしくは子ウィンドウのハンドル
		hInstance,						// インスタンスハンドル
		NULL);							// ウィンドウ作成データ

										// ウィンドウの表示
	ShowWindow(g_objwindhWnd, CmdShow);		// 指定されたウィンドウの表示設定
	UpdateWindow(g_objwindhWnd);				// ウィンドウのクライアントの更新
}

//-------------------------------------------------------------------------------------------------------------
// ウィンドウの登録解除
//-------------------------------------------------------------------------------------------------------------
void EditWindowUnregister(void)
{
	// ウィンドウクラスの登録を解除
	UnregisterClass(EDITWINDOWCLASS_NAME, g_objwindWcex.hInstance);
}

//-------------------------------------------------------------------------------------------------------------
// 入力無効エディタの設定
//-------------------------------------------------------------------------------------------------------------
HWND SetWindowEditDisabled(
	HWND * hWndthis,
	HWND hWnd, 
	int nID, 
	INTEGER2 pos,
	char * name
)
{
	*hWndthis = CreateWindowEx(0,
		"EDIT",
		name,
		(WS_CHILD | WS_VISIBLE | WS_DISABLED | ES_CENTER | WS_BORDER),
		pos.nMax,
		pos.nMin,
		EDITITEM_SIZEX,
		EDITITEM_SIZEY,
		hWnd,
		(HMENU)nID,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	return *hWndthis;
}

//-------------------------------------------------------------------------------------------------------------
// 入力有効エディタの設定
//-------------------------------------------------------------------------------------------------------------
HWND SetWindowEdit(
	HWND * hWndthis, 
	HWND hWnd, 
	int nID, 
	INTEGER2 pos,
	int * nValue, 
	float * fValue
)
{
	char string[24] = { "" };
	if (nValue != NULL)
	{
		snprintf(string, 24, "%d", *nValue);
	}
	else if (fValue != NULL)
	{
		snprintf(string, 24, "%.3f", *fValue);
	}
	*hWndthis = CreateWindowEx(0,
		"EDIT",
		string,
		(WS_CHILD | WS_VISIBLE | ES_RIGHT | WS_BORDER),
		pos.nMax,
		pos.nMin,
		EDITITEM_SIZEX,
		EDITITEM_SIZEY,
		hWnd,
		(HMENU)nID,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	return *hWndthis;
}

//-------------------------------------------------------------------------------------------------------------
// ウィンドウプロシージャ
//-------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK EditWindowProcedure(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	// 変数宣言


	static HWND hWndJumpName;			// ジャンプ力
	static HWND hWndJump;				// ジャンプ力
	static char aJumpData[64];			// ジャンプ力

	static HWND hWndJumpRateName;		// ジャンプ力上昇値
	static HWND hWndJumpRate;			// ジャンプ力上昇値
	static char aJumpRateData[64];		// ジャンプ力上昇値

	static HWND hWndRotName;			// 回転力
	static HWND hWndRot;				// 回転力
	static char aRotData[64];			// 回転力

	static HWND hWndRotRateName;		// 回転力上昇値
	static HWND hWndRotRate;			// 回転力上昇値
	static char aRotRateData[64];		// 回転力上昇値

	static HWND hWndOutputButton;		// 出力ボタン

	if (uMsg == WM_CREATE)
	{

		/* 出力ボタン */
		hWndOutputButton = CreateWindowEx(0,
			"BUTTON",
			" ",
			(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON),
			10, 10 + (EDITITEM_SIZEY * 5),
			70, 30,								// X、Y、幅、高さ
			hWnd,											// 親ウィンドウ
			(HMENU)EDITBUTTON_ID,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE)	// インスタンスハンドル（アプリの識別子）
			, NULL);
		// ジャンプ力の設定
		SetWindowEditDisabled(&hWndJumpName, hWnd, 100,
			INTEGER2(10, 10), "位置");
		SetWindowEdit(&hWndJump, hWnd, 101,
			INTEGER2(10 + EDITITEM_SIZEX, 10), NULL, NULL);

		SetWindowEdit(&hWndJumpRate, hWnd, 103,
			INTEGER2(10 + EDITITEM_SIZEX, 10 + EDITITEM_SIZEY), NULL, NULL);

		SetWindowEdit(&hWndRot, hWnd, 105,
			INTEGER2(10 + EDITITEM_SIZEX, 10 + EDITITEM_SIZEY * 2), NULL, NULL);

		// 出力ボタンに表示する文字列
		SetWindowText(hWndOutputButton, "出力");
	}
	GetWindowText(hWndJump, &aJumpData[0], 256);
	GetWindowText(hWndJumpRate, &aJumpRateData[0], 256);
	GetWindowText(hWndRot, &aRotData[0], 256);
	GetWindowText(hWndRotRate, &aRotRateData[0], 256);


	if (g_objwind.pos != NULL)
	{
		g_objwind.pos->x = (float)atof(aJumpData);		// ジャンプ力
		g_objwind.pos->y = (float)atof(aJumpRateData);	// ジャンプ力の上昇率
		g_objwind.pos->z = (float)atof(aRotRateData);	// 回転力
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 既定の処理を提供
}
