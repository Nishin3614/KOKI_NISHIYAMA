// ------------------------------------------------------------------------------------------
//
// ポリゴン処理 [billboard.cpp]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "Billboard.h"
#include "input.h"
#include "arrangement.h"
#include "mouse.h"
#include "game.h"

// 当たり判定用	//
#include "giraffe.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define BILLBOARD_SIZE (10)
#define BILLBOARD_WOOD "data/TEXTURE/materials/green_tree000.png"

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------状態---------- //
typedef enum
{
	BILLBOARDTYPE_WOOD = 0,
	BILLBOARDTYPE_MAX
} BILLBOARDTYPE;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice);
void Update_ChangeBillboard(void);
void Update_DispBillboard(void);
void Update_DestroyBillboard(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_paTextureBillboard[BILLBOARDTYPE_MAX] = {};	// 頂点テクスチャーのポインタ
BILLBOARD g_aBillboard[ALL_MAX];									// ビルボード
BILLBOARDSET g_BillboardSet;											// ビルボードの設定

// 3D用のセーブファイル
const char g_cBillboardFIleName_3D[5][128] =
{
	"data/SAVE/3D/Stage_1/billboard.txt",
	"data/SAVE/3D/Stage_2/billboard.txt",
	"data/SAVE/3D/Stage_3/billboard.txt",
	"data/SAVE/3D/Stage_4/billboard.txt",
	"data/SAVE/3D/Stage_5/billboard.txt"
};

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++)
	{
		// 位置・回転の初期設定
		g_aBillboard[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].nType = BILLBOARDTYPE_WOOD;
		g_aBillboard[nCntBillboard].bUse = false;
		g_aBillboard[nCntBillboard].bDisp = false;
	}

	// 配置情報の初期化
	g_BillboardSet.nType = 0;
	g_BillboardSet.size = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BILLBOARD_WOOD, &g_paTextureBillboard[0]);

	// ビルボードを読み込む
	LoadBillboard();

	// 頂点情報の作成
	MakeVertexBillboard(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitBillboard(void)
{
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARDTYPE_MAX; nCntBillboard++)
	{
		// テクスチャの開放
		if (g_paTextureBillboard[nCntBillboard] != NULL)
		{
			g_paTextureBillboard[nCntBillboard]->Release();
			g_paTextureBillboard[nCntBillboard] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateBillboard(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	LONG		pMouseX = 0;

	// マウスの情報取得
	pMouseX = GetMouseX();

	// 削除処理
	Update_DestroyBillboard();

	// 床の変化処理
	Update_ChangeBillboard();

	// 配置時の場所把握
	Update_DispBillboard();

	// 画面内を左クリックで、カーソルの位置に敵配置
	if (GetMousePressTrigger(0))
	{
		SetBillboard(
			pArrangement->pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			g_BillboardSet.size,
			g_BillboardSet.nType);
	}
}

// ------------------------------------------------------------------------------------------
// 床の変化処理
// ------------------------------------------------------------------------------------------
void Update_ChangeBillboard(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	// 縦のサイズを増減
	if (GetKeyboardSpeacePress(DIK_O))
	{
		g_BillboardSet.size.y += 50.0f;
	}

	else if (GetKeyboardSpeacePress(DIK_L))
	{
		g_BillboardSet.size.y -= 50.0f;
	}

	// 横のサイズを増減
	if (GetKeyboardSpeacePress(DIK_I))
	{
		g_BillboardSet.size.x += 50.0f;
	}

	else if (GetKeyboardSpeacePress(DIK_K))
	{
		g_BillboardSet.size.x -= 50.0f;
	}

	// タイプの変化
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_BillboardSet.nType++;

		// 限界値まで行ったら
		if (g_BillboardSet.nType >= BILLBOARDTYPE_MAX)
		{
			// 初期化
			g_BillboardSet.nType = 0;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 配置の時の場所把握処理
// ------------------------------------------------------------------------------------------
void Update_DispBillboard(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 変数宣言
	int nCntBillboard;

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == false)
		{
			g_aBillboard[nCntBillboard].nType = g_BillboardSet.nType;
			g_aBillboard[nCntBillboard].size = g_BillboardSet.size;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(
				-g_aBillboard[nCntBillboard].size.x,
				g_aBillboard[nCntBillboard].size.y, 0);	// 一つ目の頂点
			pVtx[1].pos = D3DXVECTOR3(
				g_aBillboard[nCntBillboard].size.x,
				g_aBillboard[nCntBillboard].size.y, 0);	// 二つ目の頂点
			pVtx[2].pos = D3DXVECTOR3(
				-g_aBillboard[nCntBillboard].size.x,
				-g_aBillboard[nCntBillboard].size.y, 0);	// 三つ目の頂点
			pVtx[3].pos = D3DXVECTOR3(
				g_aBillboard[nCntBillboard].size.x,
				-g_aBillboard[nCntBillboard].size.y, 0);	// 四つ目の頂点

			g_aBillboard[nCntBillboard].bDisp = true;

			// 配置の位置情報を取得
			g_aBillboard[nCntBillboard].pos = pArrangement->pos;

			break;

		}
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffBillboard->Unlock();

}

// ------------------------------------------------------------------------------------------
// 削除処理
// ------------------------------------------------------------------------------------------
void Update_DestroyBillboard(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	for (int nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == true && g_aBillboard[nCntBillboard].bDisp == true)
		{
			// ブロックのX範囲
			if (pArrangement->pos.x >
				g_aBillboard[nCntBillboard].pos.x - g_aBillboard[nCntBillboard].size.x * 0.5f &&
				pArrangement->pos.x <														  
				g_aBillboard[nCntBillboard].pos.x + g_aBillboard[nCntBillboard].size.x * 0.5f &&
				pArrangement->pos.z >														  
				g_aBillboard[nCntBillboard].pos.z - g_aBillboard[nCntBillboard].size.z * 0.5f &&
				pArrangement->pos.z <														  
				g_aBillboard[nCntBillboard].pos.z + g_aBillboard[nCntBillboard].size.z * 0.5f)
			{
				// 削除処理
				if (GetKeyboardSpeacePress(DIK_B))
				{
					g_aBillboard[nCntBillboard].bUse = false;
					g_aBillboard[nCntBillboard].bDisp = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	BILLBOARD *pBillboard = &g_aBillboard[0];

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;

	// ビューマトリックスの代入用
	D3DXMATRIX mtxView;

	// アルファテストの設定に変更
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテスト有効
	pDevice->SetRenderState(D3DRS_ALPHAREF, 200);				// アルファ値の基準設定(基準より大きいと描画する)
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 状況に応じてアルファテストを実行する(新ピクセル > 現在のピクセル)

	for (int nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++, pBillboard++)
	{
		if (pBillboard->bDisp == true)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&pBillboard->mtxWorldBillboard);

			// 現在のビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// 逆行列
			pBillboard->mtxWorldBillboard._11 = mtxView._11;
			pBillboard->mtxWorldBillboard._12 = mtxView._21;
			pBillboard->mtxWorldBillboard._13 = mtxView._31;
			pBillboard->mtxWorldBillboard._21 = mtxView._12;
			pBillboard->mtxWorldBillboard._22 = mtxView._22;
			pBillboard->mtxWorldBillboard._23 = mtxView._32;

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
				pBillboard->rot.y, pBillboard->rot.x, pBillboard->rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&pBillboard->mtxWorldBillboard,	// 1
				&pBillboard->mtxWorldBillboard,					// 2
				&mtxRot);							// 3

			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
				pBillboard->pos.x, pBillboard->pos.y, pBillboard->pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&pBillboard->mtxWorldBillboard,	// 1
				&pBillboard->mtxWorldBillboard,					// 2
				&mtxTrans);							// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pBillboard->mtxWorldBillboard);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_paTextureBillboard[g_aBillboard[nCntBillboard].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);
		}
	}

	// アルファテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void SetBillboard(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nType)
{
	VERTEX_3D * pVtx;	// 頂点情報

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);

	// 弾ループ
	for (int nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++)
	{
		// 使用していない状態
		if (!g_aBillboard[nCntBillboard].bUse)
		{
			// 情報更新
			g_aBillboard[nCntBillboard].pos = pos;

			// サイズ
			g_aBillboard[nCntBillboard].size = size;

			// タイプ
			g_aBillboard[nCntBillboard].nType = nType;

			// 使用状態へ
			g_aBillboard[nCntBillboard].bUse = true;

			// 頂点情報の更新
			pVtx += nCntBillboard * 4;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(
				-g_aBillboard[nCntBillboard].size.x,
				g_aBillboard[nCntBillboard].size.y, 0);	// 一つ目の頂点
			pVtx[1].pos = D3DXVECTOR3(
				g_aBillboard[nCntBillboard].size.x, 
				g_aBillboard[nCntBillboard].size.y, 0);	// 二つ目の頂点
			pVtx[2].pos = D3DXVECTOR3(
				-g_aBillboard[nCntBillboard].size.x,
				-g_aBillboard[nCntBillboard].size.y, 0);	// 三つ目の頂点
			pVtx[3].pos = D3DXVECTOR3(
				g_aBillboard[nCntBillboard].size.x, 
				-g_aBillboard[nCntBillboard].size.y, 0);	// 四つ目の頂点

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * ALL_MAX, 
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED, 
		&g_pVtxBuffBillboard,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);
			
	for (int nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(
			-g_aBillboard[nCntBillboard].size.x,
			g_aBillboard[nCntBillboard].size.y, 0);	// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(
			g_aBillboard[nCntBillboard].size.x,
			g_aBillboard[nCntBillboard].size.y, 0);	// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(
			-g_aBillboard[nCntBillboard].size.x,
			-g_aBillboard[nCntBillboard].size.y, 0);	// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(
			g_aBillboard[nCntBillboard].size.x,
			-g_aBillboard[nCntBillboard].size.y, 0);	// 四つ目の頂点

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 一つ目の頂点
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 二つ目の頂点
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 三つ目の頂点
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 四つ目の頂点

		// カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 一つ目の色設定
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 二つ目の色設定
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 三つ目の色設定
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 四つ目の色設定

		// テクスチャー
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// 一つ目の色設定
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);	// 二つ目の色設定
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);	// 三つ目の色設定
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	// 四つ目の色設定

		// ポイント合わせ
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffBillboard->Unlock();
}

// ------------------------------------------------------------------------------------------
// ビルボード読み込み処理
// ------------------------------------------------------------------------------------------
void LoadBillboard(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad = 0;		// カウントロード
	int nCntBillboard = 0;		// カウント床
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

							// もし表示しているものがあったら
	for (nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == true)
		{
			g_aBillboard[nCntBillboard].bUse = false;
			g_aBillboard[nCntBillboard].bDisp = false;
		}
	}

	// カウント初期化
	nCntBillboard = 0;

	// ファイル開
	pFile = fopen(g_cBillboardFIleName_3D[GetGameStage()], "r");

	// 開けた
	if (pFile != NULL)
	{
		// スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);	// 一文を読み込む
			sscanf(cRaedText, "%s", &cHeadText);		// 比較用テクストに文字を代入
		}

		// スクリプトだったら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// エンドスクリプトが来るまでループ
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);

				// モデルセットが来たら
				if (strcmp(cHeadText, "BILLBOARDSET") == 0)
				{

					// エンドモデルセットが来るまでループ
					while (strcmp(cHeadText, "END_BILLBOARDSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 種類情報読み込み
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, 
								&g_aBillboard[nCntBillboard].nType);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, 
								&g_aBillboard[nCntBillboard].pos.x, 
								&g_aBillboard[nCntBillboard].pos.y,
								&g_aBillboard[nCntBillboard].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_aBillboard[nCntBillboard].rot.x,
								&g_aBillboard[nCntBillboard].rot.y,
								&g_aBillboard[nCntBillboard].rot.z);
						}

						// サイズ情報読み込み
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, 
								&g_aBillboard[nCntBillboard].size.x, 
								&g_aBillboard[nCntBillboard].size.y,
								&g_aBillboard[nCntBillboard].size.z);
						}
					}

					// 使用状態・表示状態
					g_aBillboard[nCntBillboard].bUse = true;
					g_aBillboard[nCntBillboard].bDisp = true;
					// 床カウントの更新
					nCntBillboard++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "モデルデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// 配置場所がわかるために描画していたものを非表示に処理
// ------------------------------------------------------------------------------------------
void NotDispBillboard(void)
{
	int nCntBillboard;

	// 床のカウント
	for (nCntBillboard = 0; nCntBillboard < ALL_MAX; nCntBillboard++)
	{
		// 配置用の床
		if (g_aBillboard[nCntBillboard].bUse == false &&
			g_aBillboard[nCntBillboard].bDisp == true)
		{
			g_aBillboard[nCntBillboard].bDisp = false;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// セーブ処理
// ------------------------------------------------------------------------------------------
void SaveBillboard(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int		nCntBillboard = 0;
	int		nCntDataLoad = 0;
	int		nCntBug = 0;			// バグ対策カウント
	char	cWriteText[512];		// 文字として書き込み用
	char	cHeadText[128];			// 文字の格納
	char	cEqual[8] = { "=" };	// イコール用

	char	cSharpEnter[8];			// コメント１
	char	cComment[64];			// コメント２
	char	cEnter[8];				// コメント３

									// よく使われる文字
	strcpy(cSharpEnter, "#\n");
	strcpy(cComment, "#=============================\n");
	strcpy(cEnter, "\n");

	// 床の種類種類
	char *cBillboardType[BILLBOARDTYPE_MAX] =
	{
		{ "    ----------[00]:木１----------" },				// 00_道１
	};

	// 床データの種類
	char *cBillboardData[] =
	{
		{ "TYPE" },			// タイプ
		{ "POS" },			// 位置
		{ "ROT" },			// 回転
		{ "SIZE" },		// Ｘサイズ
		{ "END_BILLBOARDSET" }	// エンドフロアー
	};

	// ファイル開
	pFile = fopen(g_cBillboardFIleName_3D[GetGameStage()], "w");

	// 開けた
	if (pFile != NULL)
	{
		//------------------ コメント ------------------//
		strcpy(cWriteText, cComment);

		strcat(cWriteText, cSharpEnter);

		strcat(cWriteText, "# セーブモデル\n");

		strcat(cWriteText, "# Author : koki nishiyama\n");

		strcat(cWriteText, cSharpEnter);

		strcat(cWriteText, cComment);

		strcat(cWriteText, cEnter);

		fputs(cWriteText, pFile);

		//------------------ コメント終了 ------------------//

		strcpy(cWriteText, "SCRIPT");

		fputs(cWriteText, pFile);

		// スクリプトが通った
		if (strcmp(cWriteText, "SCRIPT") == 0)
		{
			// エンドスクリプトが来るまでループ
			while (strcmp(cWriteText, "END_SCRIPT") != 0)
			{
				// 床の最大数ならセーブ
				if (nCntBillboard < ALL_MAX - 1)
				{
					if (g_aBillboard[nCntBillboard].bUse == true)
					{
						fputs(cEnter, pFile);

						// モデルのパーツの名前を代入
						strcpy(cWriteText, cBillboardType[g_aBillboard[nCntBillboard].nType]);
						fputs(cWriteText, pFile);
						fputs(cEnter, pFile);

						// エンドスクリプトではない
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    BILLBOARDSET\n");
							fputs(cWriteText, pFile);

							// エンドモデルセットが来るまでループ
							while (strcmp(cWriteText, "END_BILLBOARDSET") != 0)
							{
								// 文字列をコピーする
								strcpy(cWriteText, cBillboardData[nCntDataLoad]);

								// タイプ情報保存
								if (strcmp(cWriteText, "TYPE") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual, g_aBillboard[nCntBillboard].nType);
									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 位置情報保存
								else if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_aBillboard[nCntBillboard].pos.x,
										g_aBillboard[nCntBillboard].pos.y,
										g_aBillboard[nCntBillboard].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 回転情報保存
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_aBillboard[nCntBillboard].rot.x,
										g_aBillboard[nCntBillboard].rot.y,
										g_aBillboard[nCntBillboard].rot.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// サイズ情報保存
								else if (strcmp(cWriteText, "SIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_aBillboard[nCntBillboard].size.x,
										g_aBillboard[nCntBillboard].size.y,
										g_aBillboard[nCntBillboard].size.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// エンド床セット
								else if (strcmp(cWriteText, "END_BILLBOARDSET") == 0)
								{
									sprintf(cHeadText, "    %s", cWriteText);
									fputs(cHeadText, pFile);

									// カウントデータ初期化
									nCntDataLoad = 0;
								}
							}
						}
					}
					// モデルパーツ変更
					nCntBillboard++;
				}

				// エンドスクリプト
				else
				{
					fputs(cEnter, pFile);

					strcpy(cWriteText, "END_SCRIPT");

					fputs(cWriteText, pFile);
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	else
	{
		MessageBox(NULL, "モデルデータの書き込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// 弾情報
// ------------------------------------------------------------------------------------------
BILLBOARD *GetBillboard(void) 
{
	return &g_aBillboard[0];
}