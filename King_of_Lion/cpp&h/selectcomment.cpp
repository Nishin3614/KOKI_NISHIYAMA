// ------------------------------------------------------------------------------------------
//
// 選択コメント処理の説明[selectcoment.cpp]
// Author : Koki Nishiyama 
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "title.h"

#include "input.h"

#include "joypad.h"

#include "fade.h"

#include "sound.h"

#include "selectcomment.h"

#include "arrow.h"

#include "game.h"

#include "tutorial.h"

#include "range.h"

#include "gamefade.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define SELECTCOMMENT_TEX (SELECTCOMMENTTYPE_MAX + SELECTCOMMENTSCENE_AREA)	// テクスチャーの数

#define SELECTUI_SIZE_X (300)												// 選択コメント名サイズ（ｘ）
#define SELECTUI_SIZE_Y (300)												// 選択コメント名サイズ（ｙ）

#define SELECTCOMMENTUI_SIZE_X (1200)										// 選択コメント名サイズ（ｘ）
#define SELECTCOMMENTUI_SIZE_Y (300)										// 選択コメント名サイズ（ｙ）

#define UI_SIZE_X (200)														// エンター名サイズ（ｘ）
#define UI_SIZE_Y (200)														// エンター名サイズ（ｙ）

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void Update_SelectSelectComment(void);
void Cnt_SelectComment(int nCnt);
void Update_ColChangeSelectComment(void);
void Update_VertexSelectComment(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureSelectComment[SELECTCOMMENT_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelectComment = NULL;												// テクスチャへのポインタ
SELECTCOMENT g_aSelectComment[SELECTCOMMENTTYPE_MAX];												// 選択コメント
int g_nCntScoment = 0;
int g_SelectCommentScene = 0;																		// シーン

// ------------------------------------------------------------------------------------------
// 選択コメント初期化処理
// ------------------------------------------------------------------------------------------
void InitSelectComment(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	VERTEX_2D *pVtx;							// 頂点情報の番地

	int nCntSelectComment;

	// テクスチャーファイル名定義
	char *paTextureFile[SELECTCOMMENT_TEX] =
	{
		{ "data/TEXTURE/00_Yes.png" },					// はい
		{ "data/TEXTURE/01_No.png" },					// いいえ
		{ "data/TEXTURE/TUTORIALS/02_Tutorial.png" },	// チュートリアル
		{ "data/TEXTURE/CONTINUE/03_continue.png" },	// コンティニュー
		{ "data/TEXTURE/AREA/04_area.png" },			// エリア移動
	};

	// 初期化
	for (nCntSelectComment = 0; nCntSelectComment < SELECTCOMMENTTYPE_MAX; nCntSelectComment++)
	{
		// 選択コメントの位置
		if (nCntSelectComment == SELECTCOMMENTTYPE_YES)
		{
			g_aSelectComment[nCntSelectComment].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.25f, 500.0f, 0.0f);
		}

		// ゲームコメントの位置
		else if(nCntSelectComment == SELECTCOMMENTTYPE_NO)
		{
			g_aSelectComment[nCntSelectComment].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, 500.0f, 0.0f);
		}
		// タイトルコメントの位置
		else if (nCntSelectComment == SELECTCOMMENTTYPE_COME)
		{
			g_aSelectComment[nCntSelectComment].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 200.0f, 0.0f);
		}

		g_aSelectComment[nCntSelectComment].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSelectComment[nCntSelectComment].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aSelectComment[nCntSelectComment].nType = SELECTCOMMENTTYPE_COME;
		g_aSelectComment[nCntSelectComment].nCntCol = 0;
		g_aSelectComment[nCntSelectComment].bUse = false;
	}

	// テクスチャの読み込み
	for (int nCntSelectTex = 0;
		nCntSelectTex < SELECTCOMMENT_TEX;
		nCntSelectTex++)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			paTextureFile[nCntSelectTex],
			&g_pTextureSelectComment[nCntSelectTex]);
	}

	// 選択コメントカウントの初期化
	g_nCntScoment = SELECTCOMMENTTYPE_NO;
	g_SelectCommentScene = 0;				// シーン

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * SELECTCOMMENTTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffSelectComment,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffSelectComment->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntSelectComment = 0; nCntSelectComment < SELECTCOMMENTTYPE_MAX; nCntSelectComment++)
	{
		if (nCntSelectComment == 2)
		{
			// 頂点の設定
			pVtx[0].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x - SELECTCOMMENTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y - SELECTCOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// 一つ目の頂点

			pVtx[1].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x + SELECTCOMMENTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y - SELECTCOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// 二つ目の頂点

			pVtx[2].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x - SELECTCOMMENTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y + SELECTCOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// 三つ目の頂点

			pVtx[3].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x + SELECTCOMMENTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y + SELECTCOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// 四つ目の頂点
		}

		else
		{
			// 頂点の設定
			pVtx[0].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x - SELECTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y - SELECTUI_SIZE_Y * 0.5f,
				0.0f);	// 一つ目の頂点

			pVtx[1].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x + SELECTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y - SELECTUI_SIZE_Y * 0.5f,
				0.0f);	// 二つ目の頂点

			pVtx[2].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x - SELECTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y + SELECTUI_SIZE_Y * 0.5f,
				0.0f);	// 三つ目の頂点

			pVtx[3].pos = D3DXVECTOR3(
				g_aSelectComment[nCntSelectComment].pos.x + SELECTUI_SIZE_X * 0.5f,
				g_aSelectComment[nCntSelectComment].pos.y + SELECTUI_SIZE_Y * 0.5f,
				0.0f);	// 四つ目の頂点
		}
		
		// 頂点作成
		pVtx[0].rhw = 1.0f;									// 一つ目の頂点作成
		pVtx[1].rhw = 1.0f;									// 二つ目の頂点作成
		pVtx[2].rhw = 1.0f;									// 三つ目の頂点作成
		pVtx[3].rhw = 1.0f;									// 四つ目の頂点作成

		// カラー設定
		pVtx[0].col = g_aSelectComment[nCntSelectComment].col;	// 一つ目の色設定
		pVtx[1].col = g_aSelectComment[nCntSelectComment].col;	// 二つ目の色設定
		pVtx[2].col = g_aSelectComment[nCntSelectComment].col;	// 三つ目の色設定
		pVtx[3].col = g_aSelectComment[nCntSelectComment].col;	// 四つ目の色設定

		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				// 一つ目のテクスチャ
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				// 二つ目のテクスチャ
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				// 三つ目のテクスチャ
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				// 四つ目のテクスチャ

		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffSelectComment->Unlock();
}

// ------------------------------------------------------------------------------------------
// 選択コメント終了処理
// ------------------------------------------------------------------------------------------
void UninitSelectComment(void)
{
	int nCntSelectComment;

	// テクスチャの開放
	for (nCntSelectComment = 0; nCntSelectComment < SELECTCOMMENTTYPE_MAX; nCntSelectComment++)
	{
		if (g_pTextureSelectComment[nCntSelectComment] != NULL)
		{
			g_pTextureSelectComment[nCntSelectComment]->Release();
			g_pTextureSelectComment[nCntSelectComment] = NULL;
		}
	}
	// 頂点バッファの開放
	if (g_pVtxBuffSelectComment != NULL)
	{
		g_pVtxBuffSelectComment->Release();
		g_pVtxBuffSelectComment = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 選択コメント更新処理
// ------------------------------------------------------------------------------------------

void UpdateSelectComment(void)
{
	// 選択処理
	Update_SelectSelectComment();

	// カラーの変化処理
	Update_ColChangeSelectComment();

	// 頂点情報処理
	Update_VertexSelectComment();
}

// ------------------------------------------------------------------------------------------
// 選択処理
// ------------------------------------------------------------------------------------------
void Update_SelectSelectComment(void)
{
	// スティックでの操作
	static int nCntStickLeft = 0;
	static int nCntStickRight = 0;

	// ジョイパッド用 //
	int nValueH, nValueV;	// ゲームパッドのスティック情報の取得用

	// ゲームパッドのスティック情報を取得
	GetGamepadStickLeft(0, &nValueH, &nValueV);

	// 速度の計算
	if (nValueH < 0)
	{
		nCntStickLeft++;
	}
	// 速度の計算
	else if (nValueH > 0)
	{
		nCntStickRight++;
	}
	// 押されていないときは初期化
	else
	{
		nCntStickLeft = 0;
		nCntStickRight = 0;
	}
	// 規定値が超えたら初期化
	if (nCntStickLeft == 20 || nCntStickRight == 20)
	{
		nCntStickLeft = 0;
		nCntStickRight = 0;
	}

	// カウント増減
	if (GetKeyboardTrigger(DIK_D) ||
		GetJoyboardTrigger(0, JOYPADKEY_RIGHT) ||
		nCntStickRight == 1)
	{
		// 選択
		Cnt_SelectComment(1);
	}
	else if (GetKeyboardTrigger(DIK_A) ||
		GetJoyboardTrigger(0, JOYPADKEY_LEFT) ||
		nCntStickLeft == 1)
	{
		// 選択
		Cnt_SelectComment(-1);
	}

	if (GetKeyboardTrigger(DIK_RETURN) ||
		GetKeyboardTrigger(DIK_J) ||
		GetJoyboardTrigger(0, JOYPADKEY_B))
	{
		// はい選択時
		if (g_nCntScoment == SELECTCOMMENTTYPE_YES)
		{
			// 決定音
			PlaySound(SOUND_LABEL_SE_ENTER);

			// チュートリアルの場合
			if (g_SelectCommentScene == SELECTCOMMENTSCENE_TUTORIALS)
			{
				SetFade(MODE_GAME);
			}

			// コンティニューの場合
			else if (g_SelectCommentScene == SELECTCOMMENTSCENE_CONTINUE)
			{
				SetFade(MODE_GAME);
			}

			// エリア移動の場合
			else if (g_SelectCommentScene == SELECTCOMMENTSCENE_AREA)
			{
				SetGameFade(GAMESTATE_BOSS);
			}
		}

		// いいえ選択時
		else if (g_nCntScoment == SELECTCOMMENTTYPE_NO ||
			GetJoyboardTrigger(0, JOYPADKEY_A))
		{
			// 決定音
			PlaySound(SOUND_LABEL_SE_ENTER);

			// チュートリアルの場合
			if (g_SelectCommentScene == SELECTCOMMENTSCENE_TUTORIALS)
			{
				SetTutorialState(TUTORIALSSTATE_NORMAL);
				PlayerPos_Range();
			}

			// コンティニューの場合
			else if (g_SelectCommentScene == SELECTCOMMENTSCENE_CONTINUE)
			{
				SetFade(MODE_TITLE);
			}

			// エリア移動の場合
			else if (g_SelectCommentScene == SELECTCOMMENTSCENE_AREA)
			{
				SetGameState(GAMESTATE_NORMAL);
				PlayerPos_Range();
			}
		}
	}

	// 矢印の位置情報更新
	ArrowPosition(g_aSelectComment[g_nCntScoment].pos);
}

// ------------------------------------------------------------------------------------------
// カウント変化処理
// ------------------------------------------------------------------------------------------
void Cnt_SelectComment(int nCnt)
{
	// 決定音
	PlaySound(SOUND_LABEL_SE_ENTER);

	// 前回のα情報を通常へ
	g_aSelectComment[g_nCntScoment].col.a = 1.0f;

	// 前回のカラーカウントを通常へ
	g_aSelectComment[g_nCntScoment].nCntCol = 0;

	// 頂点情報処理
	Update_VertexSelectComment();

	// カウントダウン
	g_nCntScoment += nCnt;

	// カウントが限界点を下回ったら
	if (g_nCntScoment < SELECTCOMMENTTYPE_YES)
	{
		g_nCntScoment = SELECTCOMMENTTYPE_COME - 1;
	}

	// カウントが限界点を超えたら
	else if (g_nCntScoment >= SELECTCOMMENTTYPE_COME)
	{
		g_nCntScoment = SELECTCOMMENTTYPE_YES;
	}
}

// ------------------------------------------------------------------------------------------
// カラー変化処理
// ------------------------------------------------------------------------------------------
void Update_ColChangeSelectComment(void)
{
	// カウントアップ
	g_aSelectComment[g_nCntScoment].nCntCol++;

	if (g_aSelectComment[g_nCntScoment].nCntCol % 20 == 0)
	{
		// カラー設定
		g_aSelectComment[g_nCntScoment].col.a = 1.0f;	// 一つ目の色設定
	}

	else if (g_aSelectComment[g_nCntScoment].nCntCol % 10 == 0)
	{
		// カラー設定
		g_aSelectComment[g_nCntScoment].col.a = 0.0f;	// 一つ目の色設定
	}
}

// ------------------------------------------------------------------------------------------
// 頂点情報処理
// ------------------------------------------------------------------------------------------
void Update_VertexSelectComment(void)
{
	VERTEX_2D *pVtx;			// 頂点情報

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffSelectComment->Lock(0, 0, (void **)&pVtx, 0);

	// ポイント合わせ
	pVtx += 4 * g_nCntScoment;

	// カラー設定
	pVtx[0].col = g_aSelectComment[g_nCntScoment].col;	// 一つ目の色設定
	pVtx[1].col = g_aSelectComment[g_nCntScoment].col;	// 二つ目の色設定
	pVtx[2].col = g_aSelectComment[g_nCntScoment].col;	// 三つ目の色設定
	pVtx[3].col = g_aSelectComment[g_nCntScoment].col;	// 四つ目の色設定

	// アンロック
	g_pVtxBuffSelectComment->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// 選択コメント描画処理
//
// ------------------------------------------------------------------------------------------
void DrawSelectComment(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	int nCntSelectComment;

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffSelectComment, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	for (nCntSelectComment = 0; nCntSelectComment < SELECTCOMMENTTYPE_MAX; nCntSelectComment++)
	{
		// コメント
		if (nCntSelectComment == SELECTCOMMENTTYPE_COME)
		{
			pDevice->SetTexture(0, g_pTextureSelectComment[g_aSelectComment[nCntSelectComment].nType]);
		}

		// はい・いいえ
		else
		{
			pDevice->SetTexture(0, g_pTextureSelectComment[nCntSelectComment]);
		}

		// 選択コメント描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSelectComment * 4, 2);
	}
}

// ------------------------------------------------------------------------------------------
// 選択コメントのシーン処理
// ------------------------------------------------------------------------------------------
void SceneSelectComment(int nScene)
{
	g_SelectCommentScene = nScene;
	g_aSelectComment[SELECTCOMMENTTYPE_COME].nType = SELECTCOMMENTTYPE_COME;
	g_aSelectComment[SELECTCOMMENTTYPE_COME].nType += nScene;
}

// ------------------------------------------------------------------------------------------
// 選択コメントの番号取得処理
// ------------------------------------------------------------------------------------------
int GetSelectCommentSelect(void)
{
	return g_nCntScoment;
}

// ------------------------------------------------------------------------------------------
//
// 選択コメント取得処理
//
// ------------------------------------------------------------------------------------------
SELECTCOMENT *GetSelectComment(void)
{
	return &g_aSelectComment[0];
}