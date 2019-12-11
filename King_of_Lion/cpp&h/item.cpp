// ------------------------------------------------------------------------------------------
//
// アイテム処理 [item.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "item.h"

#include "input.h"

#include "shadow.h"

#include "explosion.h"

#include "effect.h"

#include "wall.h"

#include "floor.h"

#include "materials.h"

#include "player.h"

#include "game.h"

#include "hpgauge.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define ITEM_MAX (128)
#define ITEM_SIZE (20)

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------アイテムの種類---------- //
typedef enum
{
	ITEMTYPE_MEAT = 0,
	ITEMTYPE_MATATABI,
	ITEMTYPE_MAX
} ITEMTYPE;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void Init_MakeVertexItem(LPDIRECT3DDEVICE9 pDevice);
void Init_LoadItem(void);
void Update_MoveItem(int nCntItem);
void Update_ColisionItem(int nCntItem);
void Update_PickUpItem(int nCntItem);
void Update_EffectItem(int nCntItem);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_paTextureItem[ITEMTYPE_MAX] = {};	// 頂点テクスチャーのポインタ
ITEM g_aItem[ITEM_MAX];									// アイテム

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	char *paItemTexture[ITEMTYPE_MAX] =
	{
		{ "data/TEXTURE/item/000_meat.png" },		// 肉
		{ "data/TEXTURE/item/01_matatabi.png" },	// マタタビ
	};

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		// 位置・回転の初期設定
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].posold = g_aItem[nCntItem].pos;
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].fRadius = 50.0f;
		g_aItem[nCntItem].nLife = 0;
		g_aItem[nCntItem].nIdxShadow = 0;
		g_aItem[nCntItem].nType = 0;
		g_aItem[nCntItem].nAttack = 0;
		g_aItem[nCntItem].bUse = false;
	}

	// テクスチャー情報読み込み
	for (int nCntText = 0; nCntText < ITEMTYPE_MAX; nCntText++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, paItemTexture[nCntText], &g_paTextureItem[nCntText]);
	}

	// アイテムの読み込み
	Init_LoadItem();

	// 頂点情報の作成
	Init_MakeVertexItem(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitItem(void)
{
	for (int nCntItem = 0; nCntItem < ITEMTYPE_MAX; nCntItem++)
	{
		// テクスチャの開放
		if (g_paTextureItem[nCntItem] != NULL)
		{
			g_paTextureItem[nCntItem]->Release();
			g_paTextureItem[nCntItem] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateItem(void)
{
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse)
		{
			g_aItem[nCntItem].posold = g_aItem[nCntItem].pos;

			// ライフ減少
			//g_aItem[nCntItem].nLife--;
			
			// 位置情報の更新
			Update_MoveItem(nCntItem);

			// ライフが０になったら
			if (g_aItem[nCntItem].nLife <= 0)
			{
				// 未使用状態へ
				g_aItem[nCntItem].bUse = false;

				// 影の削除
				DeleteShadow(g_aItem[nCntItem].nIdxShadow);
			}

			// 拾った時の処理
			Update_PickUpItem(nCntItem);

			// 当たり判定 
			Update_ColisionItem(nCntItem);

			// エフェクト生成
			Update_EffectItem(nCntItem);

			// 影の位置を設定
			SetPositionShadow(g_aItem[nCntItem].nIdxShadow, g_aItem[nCntItem].pos);
		}
	}
}

// ------------------------------------------------------------------------------------------
// 位置情報の更新処理
// ------------------------------------------------------------------------------------------
void Update_MoveItem(int nCntItem)
{
	// 移動量
	g_aItem[nCntItem].pos += g_aItem[nCntItem].move;

	// 重力
	g_aItem[nCntItem].move.y -= 1.0f;
}

// ------------------------------------------------------------------------------------------
// エフェクト処理
// ------------------------------------------------------------------------------------------
void Update_EffectItem(int nCntItem)
{
	// 変数宣言
	float fAngle;		// xの角度
	float fYAngle;		// yの角度
	D3DXVECTOR3 move;	// 移動量
	D3DXCOLOR col;		// カラー

						// X・Zの角度
	fAngle =
		float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

	// Yの角度
	fYAngle =
		float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

	// 移動方向
	move =
		D3DXVECTOR3(sinf(fAngle) * 5.0f,
			cosf(fYAngle) * 5.0f,
			cosf(fAngle) * 5.0f);

	// カラー
	col =
		D3DXCOLOR(
			0.5f + float(rand() % 50) / 100.0f,
			0.5f - float(rand() % 50) / 100.0f,
			0.0f,
			1.0f);

	// エフェクトの設定
	SetEffect(
		D3DXVECTOR3(													// 位置
			sinf(fAngle) * g_aItem[nCntItem].size.x + g_aItem[nCntItem].pos.x,
			cosf(fYAngle) * g_aItem[nCntItem].size.y + g_aItem[nCntItem].pos.y,
			cosf(fAngle) * g_aItem[nCntItem].size.z + g_aItem[nCntItem].pos.z),
		move,															// 移動量
		col,															// 色
		float(rand() % 10),												// サイズ
		-0,																// 減少サイズ
		0.0f,															// 重力
		10,																// エフェクトライフ
		0,																// タイプ
		0);
}

// ------------------------------------------------------------------------------------------
// 当たった処理
// ------------------------------------------------------------------------------------------
void Update_ColisionItem(int nCntItem)
{
	// 変数宣言
	int nCntFloor;
	FLOOR * pFloor = GetFLOOR();

	// 床の当たり判定 //
	// ブロックループ
	for (nCntFloor = 0; nCntFloor < FLOOR_MAX; nCntFloor++, pFloor++)
	{
		// 使用状態
		if (pFloor->bUse == true)
		{
			// ブロックのX範囲
			if (g_aItem[nCntItem].pos.x > pFloor->pos.x - pFloor->size.x * pFloor->nBlock_Width&&
				g_aItem[nCntItem].pos.x < pFloor->pos.x + pFloor->size.x * pFloor->nBlock_Width&&
				g_aItem[nCntItem].pos.z > pFloor->pos.z - pFloor->size.z * pFloor->nBlock_Depth&&
				g_aItem[nCntItem].pos.z < pFloor->pos.z + pFloor->size.z * pFloor->nBlock_Depth)
			{
				// 当たり判定(上)

				if (g_aItem[nCntItem].pos.y - g_aItem[nCntItem].size.y - 10.0f< pFloor->pos.y &&
					g_aItem[nCntItem].posold.y - g_aItem[nCntItem].size.y - 10.0f >= pFloor->pos.y)
				{
					// ブロック状の上に
					g_aItem[nCntItem].pos.y = pFloor->pos.y + g_aItem[nCntItem].size.y + 10.0f;

					// 移動量の初期化
					g_aItem[nCntItem].move.y = 0.0f;
				}

				// 当たり判定(下)
				if (g_aItem[nCntItem].pos.y + g_aItem[nCntItem].size.y > pFloor->pos.y &&
					g_aItem[nCntItem].posold.y + g_aItem[nCntItem].size.y <= pFloor->pos.y)
				{
					// ブロック状の下に
					g_aItem[nCntItem].pos.y = pFloor->pos.y - g_aItem[nCntItem].size.y;

					// 移動量の初期化
					g_aItem[nCntItem].move.y = 0.0f;
				}
			}
		}
	}

	// 素材の当たり判定 //
	if (ColisionMaterials(&g_aItem[nCntItem].pos,
		&g_aItem[nCntItem].posold,
		&g_aItem[nCntItem].move,
		&g_aItem[nCntItem].size,
		&g_aItem[nCntItem].pMaterials))
	{

	}

	// 壁の当たり判定
	if (ColisionWall(&g_aItem[nCntItem].pos,
		&g_aItem[nCntItem].posold,
		&g_aItem[nCntItem].move,
		&g_aItem[nCntItem].size,
		&g_aItem[nCntItem].pWall))
	{

	}
}

// ------------------------------------------------------------------------------------------
// アイテム拾ったとき
// ------------------------------------------------------------------------------------------
void Update_PickUpItem(int nCntItem)
{
	// プレイヤーの位置情報
	D3DXVECTOR3 Player_pos = GetPlayer_Pos();

	// プレイヤーの半径情報
	float Player_Radius = GetPlayer_Radius();

	// アイテムの当たり判定
	if ((g_aItem[nCntItem].fRadius * g_aItem[nCntItem].fRadius) +
		(Player_Radius * Player_Radius) >=
		(g_aItem[nCntItem].pos.x - Player_pos.x) * (g_aItem[nCntItem].pos.x - Player_pos.x) +
		(g_aItem[nCntItem].pos.y - Player_pos.y) * (g_aItem[nCntItem].pos.y - Player_pos.y) +
		(g_aItem[nCntItem].pos.z - Player_pos.z) * (g_aItem[nCntItem].pos.z - Player_pos.z))
	{
		switch (g_aItem[nCntItem].nType)
		{
		case ITEMTYPE_MEAT:
			StatusUpPlayer(
				10,			// HPアップ
				0,			// MPアップ
				0);			// 攻撃力アップ

			break;

		case ITEMTYPE_MATATABI:
			StatusUpPlayer(
				0,			// HPアップ
				10,			// MPアップ
				0);			// 攻撃力アップ

			break;

		default:
			break;
		}

		g_aItem[nCntItem].bUse = false;
		DeleteShadow(g_aItem[nCntItem].nIdxShadow);
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	ITEM *pItem = &g_aItem[0];

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;

	// ビューマトリックスの代入用
	D3DXMATRIX mtxView;

	// アルファテストの設定に変更
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテスト有効
	pDevice->SetRenderState(D3DRS_ALPHAREF,200);				// アルファ値の基準設定(基準より大きいと描画する)
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 状況に応じてアルファテストを実行する(新ピクセル > 現在のピクセル)
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				// ライティングモード有効

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++, pItem++)
	{
		if (pItem->bUse)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&pItem->mtxWorldItem);

			// 現在のビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// 逆行列
			g_aItem[nCntItem].mtxWorldItem._11 = mtxView._11;
			g_aItem[nCntItem].mtxWorldItem._12 = mtxView._21;
			g_aItem[nCntItem].mtxWorldItem._13 = mtxView._31;
			g_aItem[nCntItem].mtxWorldItem._21 = mtxView._12;
			g_aItem[nCntItem].mtxWorldItem._22 = mtxView._22;
			g_aItem[nCntItem].mtxWorldItem._23 = mtxView._32;
			//g_aItem[nCntItem].mtxWorldItem._31 = mtxView._13;
			//g_aItem[nCntItem].mtxWorldItem._32 = mtxView._23;
			//g_aItem[nCntItem].mtxWorldItem._33 = mtxView._33;

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
				pItem->rot.y, pItem->rot.x, pItem->rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&pItem->mtxWorldItem,	// 1
				&pItem->mtxWorldItem,					// 2
				&mtxRot);							// 3

			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
				pItem->pos.x, pItem->pos.y, pItem->pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&pItem->mtxWorldItem,	// 1
				&pItem->mtxWorldItem,					// 2
				&mtxTrans);							// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pItem->mtxWorldItem);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_paTextureItem[g_aItem[nCntItem].nType]);

			// アイテムの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);
		}
	}

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	// アルファテストの設定を戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			// ライティングモード有効
}

// ------------------------------------------------------------------------------------------
// アイテムの設定
// ------------------------------------------------------------------------------------------
void SetItem(
	D3DXVECTOR3 pos,	// 位置
	D3DXVECTOR3 rot,	// 回転
	D3DXVECTOR3 move,	// 移動量
	D3DXVECTOR3 size,	// サイズ
	float	fRadius,	// 半径
	int nType,			// タイプ
	int nLife			// ライフ
)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffItem->Lock(0, 0, (void **)&pVtx, 0);

	// 壁ループ
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		// 使用していない状態
		if (!g_aItem[nCntItem].bUse)
		{
			// 情報更新 //
			// 位置情報
			g_aItem[nCntItem].pos = pos;

			// 移動量
			g_aItem[nCntItem].move = move;

			// サイズ
			g_aItem[nCntItem].size = size;

			// 半径
			g_aItem[nCntItem].fRadius = fRadius;

			// タイプ
			g_aItem[nCntItem].nType = nType;

			// ライフ量
			g_aItem[nCntItem].nLife = nLife;

			// 使用状態へ
			g_aItem[nCntItem].bUse = true;

			// 影の作成
			g_aItem[nCntItem].nIdxShadow = SetShadow(
				g_aItem[nCntItem].pos,
				D3DXVECTOR3(20.0f, 0.0f, 20.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// ポイント合わせ
			pVtx += nCntItem * 4;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aItem[nCntItem].size.x, g_aItem[nCntItem].size.y, 0);	// 一つ目の頂点
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].size.x, g_aItem[nCntItem].size.y, 0);		// 二つ目の頂点
			pVtx[2].pos = D3DXVECTOR3(-g_aItem[nCntItem].size.x, -g_aItem[nCntItem].size.y, 0);	// 三つ目の頂点
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].size.x, -g_aItem[nCntItem].size.y, 0);	// 四つ目の頂点

			break;
		}
	}

	// アンロック
	g_pVtxBuffItem->Unlock();
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void Init_MakeVertexItem(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * ITEM_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffItem, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffItem->Lock(0, 0, (void **)&pVtx, 0);
			
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aItem[nCntItem].size.x, g_aItem[nCntItem].size.y, 0);	// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].size.x, g_aItem[nCntItem].size.y, 0);		// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(-g_aItem[nCntItem].size.x, -g_aItem[nCntItem].size.y, 0);	// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].size.x, -g_aItem[nCntItem].size.y, 0);	// 四つ目の頂点


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
	g_pVtxBuffItem->Unlock();
}

// ------------------------------------------------------------------------------------------
// アイテム情報を取得
// ------------------------------------------------------------------------------------------
void Init_LoadItem(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad = 0;
	int nCntItem = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

							// ファイル開
	pFile = fopen("data/SAVE/ITEM/item.txt", "r");

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

				// 改行だったら
				if (strcmp(cHeadText, "\n") == 0)
				{
				}

				// アイテムセットが来たら
				else if (strcmp(cHeadText, "ITEMSET") == 0)
				{
					// 使用状態へ
					g_aItem[nCntItem].bUse = true;
					// エンドアイテムセットが来るまでループ
					while (strcmp(cHeadText, "END_ITEMSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 位置情報読み込み
						if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_aItem[nCntItem].pos.x,
								&g_aItem[nCntItem].pos.y,
								&g_aItem[nCntItem].pos.z);
						}

						// サイズ情報読み込み
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_aItem[nCntItem].size.x,
								&g_aItem[nCntItem].size.y,
								&g_aItem[nCntItem].size.z);
						}

						// 種類情報読み込み
						else if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_aItem[nCntItem].nType);
						}

						// HP情報読み込み
						else if (strcmp(cHeadText, "LIFE") == 0)
						{
							sscanf(cRaedText, "%s %s %d",
								&cDie, &cDie,
								&g_aItem[nCntItem].nLife);
						}
					}
					// アイテムの更新
					nCntItem++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "アイテムデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}

}

// ------------------------------------------------------------------------------------------
// アイテム情報表示
// ------------------------------------------------------------------------------------------
void InforItem(void)
{

}

// ------------------------------------------------------------------------------------------
// アイテム情報
// ------------------------------------------------------------------------------------------
ITEM *GetItem(void) 
{
	return &g_aItem[0];
}