//*************************************************************************************************************
//
// ステータス管理[status.h]
// Author : Sekine Ikuto
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
		EDITWINDOWCLASS_NAME,			// ウィンドウクラスの名前
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
	char cRead[128];	// 読み込み用
	char cComp[128];	// 比較用
	char cEmpty[128];	// 要らないもの用
	int nCntError = 0;	// エラー表示カウント



	// 開けたかどうか
	if ((pFile = fopen("DATA/TEXT/PlayerStatus.txt", "r")) != NULL)
	{

		while (strcmp(cComp, "SCRIPT") != 0)
		{
			// 1行読み込む
			fgets(cRead, sizeof(cRead), pFile);
			// 読み込んど文字列代入
			sscanf(cRead, "%s", &cComp);

			// エラーカウントをインクリメント
			nCntError++;
			if (nCntError > 1048576)
			{// エラー
				nCntError = 0;
				MessageBox(NULL, "[SCRIP]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
			}
		}

		if (strcmp(cComp, "SCRIPT") == 0)
		{
			// END_SCRIPTまでループ
			while (strcmp(cComp, "END_SCRIPT") != 0)
			{
				// 1行読み込む
				fgets(cRead, sizeof(cRead), pFile);
				// 読み込んど文字列代入
				sscanf(cRead, "%s", &cComp);

				// エラーカウントをインクリメント
				nCntError++;
				if (nCntError > 1048576)
				{// エラー
					nCntError = 0;
					MessageBox(NULL, "[END_SCRIPT]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
				}
				if (strcmp(cComp, "SETSTATUS") == 0)
				{
					do
					{// ループ開始
					 // 1行読み込む
						fgets(cRead, sizeof(cRead), pFile);
						// 読み込んど文字列代入
						sscanf(cRead, "%s", &cComp);
						// エラーカウントをインクリメント
						nCntError++;
						if (nCntError > 1048576)
						{// エラー
							nCntError = 0;
						}
						if (strcmp(cComp, "JUMPFORCE") == 0)
						{
							sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty, &Status->fJumpforce);
							cComp[0] = '\0';
						}
						else if (strcmp(cComp, "ROTFORCE") == 0)
						{
							sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty, &Status->fRotforce);
							cComp[0] = '\0';
						}
						else if (strcmp(cComp, "RATE_JUMPFORCE") == 0)
						{
							sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty, &Status->fRateJumpforce);
							cComp[0] = '\0';
						}
						else if (strcmp(cComp, "RATE_ROTFORCE") == 0)
						{
							sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty, &Status->fRateRotforce);
							cComp[0] = '\0';
						}
					} while ((strcmp(cComp, "END_SETSTATUS") != 0) && (strcmp(cComp, "}") != 0));

				}
			}
		}
	}
	else
	{
		MessageBox(NULL, "プレイヤー情報ファイルを開けませんでした", "警告！", MB_ICONWARNING);
	}
	fclose(pFile);
}

//-------------------------------------------------------------------------------------------------------------
// ステータス出力
//-------------------------------------------------------------------------------------------------------------
void CStatus::StatusSave(void)
{
	CPlayer *pPlayer = NULL;

	for (int nCntlayer = 0; nCntlayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntlayer++)
	{
		if (pPlayer = (CPlayer *)CScene::GetActor(CScene::ACTOR_PLAYER, CScene::LAYER_3DOBJECT, nCntlayer)) break;
	}

	if (pPlayer != NULL)
	{
		// セーブ
		pPlayer->SaveStatus();
	}
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
HWND CStatus::SetWindowEdit(HWND * hWndthis, HWND hWnd, int nID, INTEGER2 pos, int * nValue, float * fValue)
{
	char string[12] = { "" };
	if (nValue != NULL)
	{
		snprintf(string, 12, "%d", *nValue);
	}
	else if (fValue != NULL)
	{
		snprintf(string, 12, "%.3f", *fValue);
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

		// ジャンプ力の設定
		CStatus::SetWindowEditDisabled(&hWndJumpName, hWnd, 100,
			INTEGER2(10, 10), "ジャンプ力");
		CStatus::SetWindowEdit(&hWndJump, hWnd, 101,
			INTEGER2(10 + EDITITEM_SIZEX, 10), NULL, &Status.fJumpforce);

			// ジャンプ力上昇値の設定
		CStatus::SetWindowEditDisabled(&hWndJumpRateName, hWnd, 102,
			INTEGER2(10, 10+ EDITITEM_SIZEY), "ジャンプ上昇値");
		CStatus::SetWindowEdit(&hWndJumpRate, hWnd, 103,
			INTEGER2(10 + EDITITEM_SIZEX, 10 + EDITITEM_SIZEY), NULL, &Status.fRateJumpforce);


		// 回転力の設定
		CStatus::SetWindowEditDisabled(&hWndJumpName, hWnd, 104,
			INTEGER2(10, 10 + EDITITEM_SIZEY * 2), "回転力");
		CStatus::SetWindowEdit(&hWndRot, hWnd, 105,
			INTEGER2(10 + EDITITEM_SIZEX, 10 + EDITITEM_SIZEY*2), NULL, &Status.fRotforce);

		// 回転力上昇値の設定
		CStatus::SetWindowEditDisabled(&hWndJumpRateName, hWnd, 106,
			INTEGER2(10, 10 + EDITITEM_SIZEY*3), "回転力上昇値");
		CStatus::SetWindowEdit(&hWndRotRate, hWnd, 107,
			INTEGER2(10 + EDITITEM_SIZEX, 10 + EDITITEM_SIZEY*3), NULL, &Status.fRateRotforce);
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
		//pPlayer->SetStatus(StatusInfo);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 既定の処理を提供
}
