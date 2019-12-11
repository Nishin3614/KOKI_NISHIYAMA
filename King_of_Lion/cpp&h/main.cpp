// ------------------------------------------------------------------------------------------
//
// メイン処理 [main.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "main.h"
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "clear.h"
#include "gameover.h"
#include "rank.h"
#include "input.h"
#include "joypad.h"
#include "mouse.h"
#include "sound.h"
#include "statistics.h"
#include "Load_Save.h"

// デバッグモード用
#include "camera.h"
#include "player.h"
#include "shadow.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define CLASS_NAME		"WindowClass"	// ウインドウのクラス名
#define WINDOW_NAME		"King_Of_Lion"	// ウインドウのキャプション名
#define DEBUG_FONT (2)					// デバッグ用フォント数
#define MODE_STATE (MODE_TITLE)			// 初期のモード状態

// ------------------------------------------------------------------------------------------
// 構造体定義
// ------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

#ifdef _DEBUG
void DrawFPS(void);
#endif

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3D9			g_pD3D = NULL;			// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Direct3Dデバイスへのポインタ(描画に必要)
MODE g_mode = MODE_NONE;					// 現在のモード

#ifdef _DEBUG
LPD3DXFONT			g_pFont = NULL;			// フォントへのポインタ
int					g_nCountFPS = 0;		// FPSカウンタ
#endif

// ------------------------------------------------------------------------------------------
// メイン関数
// ------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL, IDI_APPLICATION)
	};
	HWND hWnd;
	MSG msg;
	RECT rect =	{0,	0, SCREEN_WIDTH, SCREEN_HEIGHT};

	DWORD dwCurrentTime;		// 現在時刻
	DWORD dwExecLastTime;		// 最後に処理した時刻	

	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウインドウを作成
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						(rect.right - rect.left),
						(rect.bottom - rect.top),
						NULL,
						NULL,
						hInstance,
						NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if (FAILED(Init(hInstance, hWnd,true)))
	{
		return -1;
	}
	
	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();		// システム時刻をミリ秒単位で取得

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();		// システム時刻をミリ秒単位で取得

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// メッセージを取得しなかった場合 0 を返す
		{
			// Windowsの処理
			if (msg.message == WM_QUIT)
			{
				// WM_QUITメッセージが来たらループを抜ける
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// DirectXの処理
			dwCurrentTime = timeGetTime();		// システム時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);		// FPSを測定
#endif

				dwFPSLastTime = dwCurrentTime;		// FPSを測定した時刻を保存
				dwFrameCount = 0;					// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))		// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;		// 処理した時刻を保存

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;			// カウントを加算
			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

// ------------------------------------------------------------------------------------------
// ウインドウプロシージャ
// ------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	
	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ(Ｚバッファ)とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート(現在の速度に合わせる)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// クライアント領域を直ちに更新する

	// Direct3Dデバイスの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,					// ディスプレイアダプタ
									D3DDEVTYPE_HAL,						// デバイスタイプ
									hWnd,								// フォーカスするウインドウへのハンドル
									D3DCREATE_HARDWARE_VERTEXPROCESSING,// デバイス作成制御の組み合わせ
									&d3dpp,								// デバイスのプレゼンテーションパラメータ
									&g_pD3DDevice)))					// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp,
										&g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp,
											&g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}
	// レンダーステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面(左回り)をカリングする
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);			// 塗りつぶしモードの設定

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数
	
#ifdef _DEBUG
	// デバッグ表示用フォントを設定
	D3DXCreateFont(g_pD3DDevice, 24, 10, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "ＭＳ ゴシック", &g_pFont);
#endif

	// モードの初期化
	g_mode = MODE_NONE;

	// キー入力初期化処理
	InitKeyboard(hInstance,hWnd);

	// ジョイパッド初期化処理
	InitJoyboard(hInstance,hWnd);

	// マウス初期化処理
	InitMouse(hInstance,hWnd);

	// サウンド初期化処理
	InitSound(hWnd);

	// ロードセーブの初期化処理
	InitLoad_Save();

	// フェード初期化
	InitFade(MODE_STATE);
	return S_OK;
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void Uninit(void)
{
	// キー入力初期化
	UninitKeyboard();

	// ジョイパッド終了処理
	UninitJoyboard();

	// マウス終了処理
	UninitMouse();

	// サウンド終了処理
	UninitSound();

	// タイトル終了処理
	UninitTitle();

	// チュートリアル
	UninitTutorial();

	// ゲーム終了処理
	UninitGame();

	// クリア終了処理
	UninitClear();

	// リザルト終了処理
	UninitGameover();

	// 統計終了処理
	UninitStatistics();

	// フェード終了処理
	UninitFade();

	// ランキング背景終了処理
	UninitRank();
	
	// Direct3Dデバイスの開放
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの開放
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
 		g_pD3D = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void Update(void)
{
	// キー入力更新処理
	UpdateKeyboard();

	// ジョイパッド更新処理
	UpdateJoyboard();

	// マウス更新処理
	UpdateMouse();

	switch (g_mode)
	{
	case MODE_TITLE:
		// タイトル更新
		UpdateTitle();
		break;

	case MODE_TUTORIAL:
		// チュートリアル
		UpdateTutorial();
		break;

	case MODE_GAME:
		UpdateGame();
		break;

	case MODE_CLEAR:
		UpdateClear();
		break;

	case MODE_GAMEOVER:
		UpdateGameover();
		break;
	case MODE_STATI:
		UpdateStatistics();
		break;
	case MODE_RANK:
		UpdateRank();
		break;

	default:
		break;
	}

	// フェード更新処理
	UpdateFade();

#ifdef _DEBUG
	// ワイヤーフレームをかける
	if (GetKeyboardPress(DIK_F2))
	{
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);		// 塗りつぶしモードの設定

	}
	
	// 通常モード
	if (GetKeyboardPress(DIK_F3))
	{
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);			// 塗りつぶしモードの設定
	}

	// 頂点だけの表示
	if (GetKeyboardPress(DIK_F4))
	{
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);			// 塗りつぶしモードの設定
	}
#endif // DEBUG
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(255, 255, 255, 255), 1.0f, 0);

	// 描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();
			break;

		case MODE_TUTORIAL:
			DrawTutorial();
			break;

		case MODE_GAME:
			DrawGame();
			break;

		case MODE_CLEAR:
			DrawClear();
			break;

		case MODE_GAMEOVER:
			DrawGameover();
			break;
		case MODE_STATI:
			DrawStatistics();
			break;
		case MODE_RANK:
			DrawRank();
			break;
		default:
			break;
		}

		DrawFade();

#ifdef _DEBUG
		DrawFPS();

#endif // _DEBUG


		// 描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

// ------------------------------------------------------------------------------------------
// デバイスの取得
// ------------------------------------------------------------------------------------------
void SetMode(MODE mode)
{
	// 前のモード終了
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		StopSound(SOUND_LABEL_BGM_TITLE);
		StopSound();
		break;

	case MODE_TUTORIAL:
		UninitTutorial();
		StopSound(SOUND_LABEL_BGM_GAME);
		StopSound();

		break;

	case MODE_GAME:
		UninitGame();
		StopSound(SOUND_LABEL_BGM_GAME);
		StopSound();
		break;

	case MODE_CLEAR:
		UninitClear();
		StopSound(SOUND_LABEL_BGM_CLEAR);
		StopSound();
		break;

	case MODE_GAMEOVER:
		UninitGameover();
		StopSound(SOUND_LABEL_BGM_GAMEOVER);
		StopSound();
		break;

	case MODE_STATI:
		UninitStatistics();

		StopSound(SOUND_LABEL_BGM_RANKINGMOVE);
		StopSound();
		break;

	case MODE_RANK:
		UninitRank();
		StopSound(SOUND_LABEL_BGM_RANKINGMOVE);
		StopSound();
		break;

	default:
		break;
	}

	g_mode = mode;	// 現在のモードを代入

	// 現在のモード初期化
	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();
		PlaySound(SOUND_LABEL_BGM_TITLE);

		break;

	case MODE_TUTORIAL:
		InitTutorial();
		PlaySound(SOUND_LABEL_BGM_GAME);
		break;

	case MODE_GAME:
		InitGame();
		PlaySound(SOUND_LABEL_BGM_GAME);

		break;

	case MODE_CLEAR:
		InitClear();
		PlaySound(SOUND_LABEL_BGM_CLEAR);
		break;

	case MODE_GAMEOVER:
		InitGameover();
		PlaySound(SOUND_LABEL_BGM_GAMEOVER);
		break;

	case MODE_STATI:
		InitStatistics();

		PlaySound(SOUND_LABEL_BGM_TITLE);
		break;

	case MODE_RANK:
		InitRank();

		PlaySound(SOUND_LABEL_BGM_TITLE);
		break;

	default:
		break;
	}
}

// ------------------------------------------------------------------------------------------
// デバイスの取得
// ------------------------------------------------------------------------------------------
MODE GetMode(void)
{
	return g_mode;
}

// ------------------------------------------------------------------------------------------
// デバイスの取得
// ------------------------------------------------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------
// フォントの取得
// ------------------------------------------------------------------------------------------
LPD3DXFONT GetFont(void)
{
	return g_pFont;
}

// ------------------------------------------------------------------------------------------
// FPS表示処理
// ------------------------------------------------------------------------------------------
void DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// 表示場所・範囲
	char aStr[256];

	// 変数に文字を当てえる
	sprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	// テキスト描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
#endif
