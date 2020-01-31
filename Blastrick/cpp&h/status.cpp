//*************************************************************************************************************
//
// ステータス管理[status.h]
// Author : Nishiyama koki
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "status.h"
#include "player.h"

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
#define DISPLAY_MAX				(36)


//-------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK EditWindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
WNDCLASSEX CStatus::m_wcex;
HWND CStatus::m_hWnd;
RECT CStatus::m_rect;

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CStatus::CStatus()
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CStatus::~CStatus()
{
}

//-------------------------------------------------------------------------------------------------------------
// ウィンドウの登録
//-------------------------------------------------------------------------------------------------------------
void CStatus::EditWindowRegister(HINSTANCE hInstance)
{
	m_wcex =
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
		EDITWINDOWCLASS_NAME,					// ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	// 小さいアイコン指定
	};

	// ウィンドウクラスの登録
	RegisterClassEx(&m_wcex);
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CStatus * CStatus::Create(void)
{
	return nullptr;
}

//-------------------------------------------------------------------------------------------------------------
// ウィンドウの作成
//-------------------------------------------------------------------------------------------------------------
void CStatus::CreateEditWindow(HINSTANCE hInstance, int CmdShow)
{
	m_rect = { 0,0,EDITWINDOW_SIZEX,EDITWINDOW_SIZEY };
	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&m_rect, WS_OVERLAPPEDWINDOW, true);

	// ウィンドウの生成
	m_hWnd = CreateWindowEx(0,			// 拡張ウィンドウスタイル
		EDITWINDOWCLASS_NAME,					// ウィンドウクラス名
		EDITWINDOW_NAME,				// ウィンドウの名前
		WS_EX_DLGMODALFRAME | WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME,		// ウィンドウスタイル
		CW_USEDEFAULT,								// ウィンドウ左上X座標
		CW_USEDEFAULT,								// ウィンドウ左上Y座標
		(m_rect.right - m_rect.left),					// ウィンドウの幅
		(m_rect.bottom - m_rect.top),					// ウィンドウの高さ
		NULL,							// 親ウィンドウのハンドル
		NULL,							// メニューもしくは子ウィンドウのハンドル
		hInstance,						// インスタンスハンドル
		NULL);							// ウィンドウ作成データ

										// ウィンドウの表示
	ShowWindow(m_hWnd, CmdShow);		// 指定されたウィンドウの表示設定
	UpdateWindow(m_hWnd);				// ウィンドウのクライアントの更新
}

//-------------------------------------------------------------------------------------------------------------
// ウィンドウの登録解除
//-------------------------------------------------------------------------------------------------------------
void CStatus::EditWindowUnregister(void)
{
	// ウィンドウクラスの登録を解除
	UnregisterClass(EDITWINDOWCLASS_NAME, m_wcex.hInstance);

}

//-------------------------------------------------------------------------------------------------------------
// ステータスの読み込み
//-------------------------------------------------------------------------------------------------------------
void CStatus::StatusLoad(STATUS_INFO *Status)
{
	// 変数宣言
	FILE *pFile;		// ファイルのポインタ
	int nCntError = 0;	// エラー表示カウント

	// 開けたかどうか
	if ((pFile = fopen("data/status_manager.txt", "r")) != NULL)
	{
		// データの取得
		while (fscanf(
			pFile,
			"%[^,],%d,%d,%f,%f,%f",
			&Status->cName,		// 名前
			&Status->nID,		// 番号
			&Status->nType,		// タイプ
			&Status->fHp,		// HP
			&Status->fSpeed,	// スピード
			&Status->fAttack	// 攻撃力
		) != EOF)
		{

		}
		fclose(pFile);

	}
	else
	{
		MessageBox(NULL, "ステータス情報ファイルを開けませんでした", "警告！", MB_ICONWARNING);
	}
}

//-------------------------------------------------------------------------------------------------------------
// ステータス出力
//-------------------------------------------------------------------------------------------------------------
void CStatus::StatusSave(void)
{
	/*
	// 変数宣言
	FILE *pFile;		// ファイルのポインタ
	char cRead[128];	// 読み込み用
	char cComp[128];	// 比較用
	char cEmpty[128];	// 要らないもの用
	int nCntError = 0;	// エラー表示カウント

						// 開けたかどうか
	if ((pFile = fopen("data/status_manager.txt", "r")) != NULL)
	{
		// データの取得
		while (fscanf(pFile, "%[^,],%d,%d,%.3f,%.3f,%.3f",
			Status->cName,		// 名前
			Status->nID,		// 番号
			Status->nType,		// タイプ
			Status->fHp,		// HP
			Status->fSpeed,		// スピード
			Status->fAttack		// 攻撃力
		) != EOF)
		{

		}
	}
	else
	{
		MessageBox(NULL, "プレイヤー情報ファイルを開けませんでした", "警告！", MB_ICONWARNING);
	}
	fclose(pFile);
	*/
}

//-------------------------------------------------------------------------------------------------------------
// 入力無効エディタの設定
//-------------------------------------------------------------------------------------------------------------
HWND CStatus::SetWindowEditDisabled(HWND * hWndthis, HWND hWnd, int nID, INTEGER2 pos, char * name)
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
HWND CStatus::SetWindowEdit(
	HWND * hWndthis,
	HWND hWnd, 
	int nID, 
	INTEGER2 pos, 
	int * nValue, 
	float * fValue,
	char * cValue
)
{
	// 変数宣言
	char string[DISPLAY_MAX] = { "" };	// 文字列格納
	if (nValue != nullptr)
	{
		snprintf(string, DISPLAY_MAX, "%d", *nValue);
	}
	else if (fValue != nullptr)
	{
		snprintf(string, DISPLAY_MAX, "%.3f", *fValue);
	}
	else if (cValue != nullptr)
	{
		strcpy(string, cValue);
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
LRESULT CALLBACK EditWindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		STATUS_INFO Status;
		// ステータスの読み込み
		CStatus::StatusLoad(&Status);

		/* 出力ボタン */
		hWndOutputButton = CreateWindowEx(0,
			"BUTTON",
			" ",
			(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON),
			10, 10 + (EDITITEM_SIZEY*5),
			70, 30,								// X、Y、幅、高さ
			hWnd,											// 親ウィンドウ
			(HMENU)EDITBUTTON_ID,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE)	// インスタンスハンドル（アプリの識別子）
			, NULL);

		// 出力ボタンに表示する文字列
		SetWindowText(hWndOutputButton, "出力");

		// 名前の設定
		CStatus::SetWindowEditDisabled(
			&hWndJumpName,					// 特定の枠
			hWnd, 							// すべての枠
			100,							// 番号
			INTEGER2(10, 10),				// 位置
			"名前");						// 名前
		CStatus::SetWindowEdit(&hWndJump, hWnd, 101,
			INTEGER2(10 + EDITITEM_SIZEX, 10), nullptr, nullptr,&Status.cName);

			// ID上昇値の設定
		CStatus::SetWindowEditDisabled(&hWndJumpRateName, hWnd, 102,
			INTEGER2(10, 10+ EDITITEM_SIZEY), "ID");
		CStatus::SetWindowEdit(&hWndJumpRate, hWnd, 103,
			INTEGER2(10 + EDITITEM_SIZEX, 10 + EDITITEM_SIZEY), &Status.nID, nullptr,nullptr);


		// タイプの設定
		CStatus::SetWindowEditDisabled(&hWndJumpName, hWnd, 104,
			INTEGER2(10, 10 + EDITITEM_SIZEY * 2), "タイプ");
		CStatus::SetWindowEdit(&hWndRot, hWnd, 105,
			INTEGER2(10 + EDITITEM_SIZEX, 10 + EDITITEM_SIZEY*2), &Status.nType, nullptr);

		// HPの設定
		CStatus::SetWindowEditDisabled(&hWndJumpRateName, hWnd, 106,
			INTEGER2(10, 10 + EDITITEM_SIZEY*3), "HP");
		CStatus::SetWindowEdit(&hWndRotRate, hWnd, 107,
			INTEGER2(10 + EDITITEM_SIZEX, 10 + EDITITEM_SIZEY*3), NULL, &Status.fHp,nullptr);
	}

	switch (uMsg)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == EDITBUTTON_ID)
		{
			// メッセージボックス
			int nID = MessageBox(hWnd, "出力しますか？", "出力", MB_YESNO);
			// もしYESだったら
			if (nID == IDYES)
			{
				CStatus::StatusSave();
			}
		}
		break;
	}

	GetWindowText(hWndJump, &aJumpData[0], 256);
	GetWindowText(hWndJumpRate, &aJumpRateData[0], 256);
	GetWindowText(hWndRot, &aRotData[0], 256);
	GetWindowText(hWndRotRate, &aRotRateData[0], 256);

	/*
	CPlayer *pPlayer = NULL;

	for (int nCntlayer = 0; nCntlayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntlayer++)
	{
		if (pPlayer = (CPlayer *)CScene::GetActor(CScene::ACTOR_PLAYER, CScene::LAYER_3DOBJECT, nCntlayer)) break;
	}

	if (pPlayer != NULL)
	{// 変数定義
		STATUS_INFO StatusInfo;										// ステータスの情報

		StatusInfo.fJumpforce		= (float)atof(aJumpData);		// ジャンプ力
		StatusInfo.fRateJumpforce	= (float)atof(aJumpRateData);	// ジャンプ力の上昇率
		StatusInfo.fRateRotforce	= (float)atof(aRotRateData);	// 回転力
		StatusInfo.fRotforce		= (float)atof(aRotData);		// 回転力の上昇率

		// ステータスの設定
		pPlayer->SetStatus(StatusInfo);
	}
	*/
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 既定の処理を提供
}
