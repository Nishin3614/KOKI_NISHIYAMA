// ------------------------------------------------------------------------------------------
//
// 弾処理 [bullet.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "bullet.h"
#include "input.h"
#include "shadow.h"
#include "explosion.h"
#include "effect.h"
#include "sound.h"

// 当たり判定用	//
#include "player.h"
#include "giraffe.h"
#include "buffalo.h"
#include "king_rion.h"
#include "shogira.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define BULLET_MAX (128)							// 玉の最大数
#define BULLET_SIZE (10)							// サイズ
#define BULLET_TEXTURE (3)							// テクスチャー数
#define BULLET_PLAYER "data/TEXTURE/sunani.png"		// プレイヤー
#define BULLET_ENEMY "data/TEXTURE/bullet000.png"	// 敵
#define BULLET_KING "data/TEXTURE/sunani.png"		// ボス

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------状態---------- //
typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_ENEMY,
	BULLETTYPE_BOSS,
	BULLETTYPE_MAX
} BULLETTYPE;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);	// 頂点情報生成
// 更新
void Update_DecisionBullet(int nCntBullet);			// 攻撃の当たり判定
void Update_ColisionBullet(int nCntBullet);			// 当たり判定
void Update_EndEffectBullet(int nCntBullet);		// effect

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;				// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_paTextureBullet[BULLET_TEXTURE] = {};	// 頂点テクスチャーのポインタ
BULLET g_aBullet[BULLET_MAX];									// 弾

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		// 位置・回転の初期設定
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].nIdxShadow = 0;
		g_aBullet[nCntBullet].nPattern = 0;
		g_aBullet[nCntBullet].nCntAnim = 0;
		g_aBullet[nCntBullet].nType = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].nAttack = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BULLET_PLAYER, &g_paTextureBullet[0]);
	D3DXCreateTextureFromFile(pDevice, BULLET_ENEMY, &g_paTextureBullet[1]);
	D3DXCreateTextureFromFile(pDevice, BULLET_KING, &g_paTextureBullet[2]);

	// 頂点情報の作成
	MakeVertexBullet(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < BULLET_TEXTURE; nCntBullet++)
	{
		// テクスチャの開放
		if (g_paTextureBullet[nCntBullet] != NULL)
		{
			g_paTextureBullet[nCntBullet]->Release();
			g_paTextureBullet[nCntBullet] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateBullet(void)
{
	// 変数宣言
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffBullet->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{
			// タイプ別
			switch (g_aBullet[nCntBullet].nType)
			{
				// プレイヤー弾
			case BULLETTYPE_PLAYER:
			case BULLETTYPE_BOSS:
				// カウントアニメ
				g_aBullet[nCntBullet].nCntAnim++;
				if (g_aBullet[nCntBullet].nCntAnim % 20 == 0)
				{		
					g_aBullet[nCntBullet].nPattern = (g_aBullet[nCntBullet].nPattern++) % ANIMATION_FOUR;	// 8枚パターン創造
					// テクスチャー設定
					SetTexBullet(pVtx,
						ANIMATION_FOUR_FTEX * g_aBullet[nCntBullet].nPattern,
						ANIMATION_FOUR_FTEX + ANIMATION_FOUR_FTEX * g_aBullet[nCntBullet].nPattern);
				}
				// 移動方向
				move =
					D3DXVECTOR3(0.0f,
						0.0f,
						0.0f);

				// カラー
				col =
					D3DXCOLOR(
						1.0f,
						0.3f,
						0.5f,
						1.0f);

				// エフェクトの設定
				SetEffect(
					g_aBullet[nCntBullet].pos,				// 位置
					move,									// 移動量
					col,									// 色
					g_aBullet[nCntBullet].size.x + 10,	// サイズ
					-1,										// 減少サイズ
					0.0f,									// 重力
					5,										// エフェクトライフ
					0,										// タイプ
					0);
				break;
			case BULLETTYPE_ENEMY:
				// X・Zの角度
				fAngle =
					float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

				// Yの角度
				fYAngle =
					float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

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
						sinf(fAngle) * BULLET_SIZE + g_aBullet[nCntBullet].pos.x,
						cosf(fYAngle) * BULLET_SIZE + g_aBullet[nCntBullet].pos.y,
						cosf(fAngle) * BULLET_SIZE + g_aBullet[nCntBullet].pos.z),

					move,															// 移動量
					col,															// 色
					float(rand() % 10),												// サイズ
					-0,																// 減少サイズ
					0.0f,															// 重力
					10,																// エフェクトライフ
					0,																// タイプ
					0);
			default:
				break;
			}
			// 現在の位置保存
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;
			// 移動
			g_aBullet[nCntBullet].pos.z +=g_aBullet[nCntBullet].move.z;
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;

			// ライフ減少
			g_aBullet[nCntBullet].nLife--;
			// 影の位置を設定
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos);

			// ライフが０になったら
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				// 未使用状態へ
				g_aBullet[nCntBullet].bUse = false;
				// 影の削除
				DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
				// タイプ別
				switch (g_aBullet[nCntBullet].nType)
				{
					// プレイヤー弾
				case BULLETTYPE_PLAYER:
				case BULLETTYPE_BOSS:
					// 爆発effect
					Update_EndEffectBullet(nCntBullet);
					break;
				case BULLETTYPE_ENEMY:
					break;
				}
				// 初期化
				g_aBullet[nCntBullet].nLife = 200;
			}

			// キャラクターの当たり判定
			Update_DecisionBullet(nCntBullet);

			// 当たり判定
			Update_ColisionBullet(nCntBullet);
		}
		// ポイントアップ
		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffBullet->Unlock();
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	BULLET *pBullet = &g_aBullet[0];

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;

	// ビューマトリックスの代入用
	D3DXMATRIX mtxView;

	// アルファテストの設定に変更
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテスト有効
	pDevice->SetRenderState(D3DRS_ALPHAREF,200);				// アルファ値の基準設定(基準より大きいと描画する)
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 状況に応じてアルファテストを実行する(新ピクセル > 現在のピクセル)

	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&pBullet->mtxWorldBullet);

			// 現在のビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// 逆行列
			g_aBullet[nCntBullet].mtxWorldBullet._11 = mtxView._11;
			g_aBullet[nCntBullet].mtxWorldBullet._12 = mtxView._21;
			g_aBullet[nCntBullet].mtxWorldBullet._13 = mtxView._31;
			g_aBullet[nCntBullet].mtxWorldBullet._21 = mtxView._12;
			g_aBullet[nCntBullet].mtxWorldBullet._22 = mtxView._22;
			g_aBullet[nCntBullet].mtxWorldBullet._23 = mtxView._32;

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
				pBullet->rot.y, pBullet->rot.x, pBullet->rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&pBullet->mtxWorldBullet,	// 1
				&pBullet->mtxWorldBullet,					// 2
				&mtxRot);							// 3

			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
				pBullet->pos.x, pBullet->pos.y, pBullet->pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&pBullet->mtxWorldBullet,	// 1
				&pBullet->mtxWorldBullet,					// 2
				&mtxTrans);							// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pBullet->mtxWorldBullet);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_paTextureBullet[g_aBullet[nCntBullet].nType]);

			// 弾の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}

	// アルファテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

// ------------------------------------------------------------------------------------------
// テクスチャー設定
// ------------------------------------------------------------------------------------------
void SetTexBullet(VERTEX_3D *pVtx,float fBeginTex,float fEndTex)
{
	// テクスチャー
	pVtx[0].tex = D3DXVECTOR2(fBeginTex, 0.0f);	// 一つ目の色設定
	pVtx[1].tex = D3DXVECTOR2(fEndTex, 0.0f);	// 二つ目の色設定
	pVtx[2].tex = D3DXVECTOR2(fBeginTex, 1.0f);	// 三つ目の色設定
	pVtx[3].tex = D3DXVECTOR2(fEndTex, 1.0f);	// 四つ目の色設定
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void SetBullet(
	D3DXVECTOR3 pos,	// 位置
	D3DXVECTOR3 move ,	// 移動量
	D3DXVECTOR3 rot,	// 回転量
	D3DXVECTOR3 size,	// サイズ
	int nLife,			// ライフ
	int nType,			// タイプ
	int nAttack)		// 攻撃力
{
	VERTEX_3D * pVtx;	// 頂点情報

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffBullet->Lock(0, 0, (void **)&pVtx, 0);

	// 弾ループ
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		// 使用していない状態
		if (!g_aBullet[nCntBullet].bUse)
		{
			// SE
			PlaySound(SOUND_LABEL_SE_BEAM);

			g_aBullet[nCntBullet].nAttack = 0;

			// 情報更新
			g_aBullet[nCntBullet].pos = pos;

			// 移動量
			g_aBullet[nCntBullet].move =
				D3DXVECTOR3(-sinf(rot.y) * move.x, move.y, -cosf(rot.y) * move.z);

			// サイズ
			g_aBullet[nCntBullet].size = size;

			// ライフ量
			g_aBullet[nCntBullet].nLife = nLife;

			// タイプ
			g_aBullet[nCntBullet].nType = nType;

			// 攻撃力
			// プレイヤー
			if (g_aBullet[nCntBullet].nType == BULLETTYPE_PLAYER)
			{
				g_aBullet[nCntBullet].nAttack += 10 + nAttack;
			}

			// 敵
			else if (g_aBullet[nCntBullet].nType == BULLETTYPE_ENEMY)
			{
				g_aBullet[nCntBullet].nAttack += 5 + nAttack;
			}

			// 使用状態へ
			g_aBullet[nCntBullet].bUse = true;

			// 影の作成
			g_aBullet[nCntBullet].nIdxShadow = SetShadow(
				g_aBullet[nCntBullet].pos,
				D3DXVECTOR3(BULLET_SIZE, 0.0f, BULLET_SIZE),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// 頂点情報の更新
			pVtx += nCntBullet * 4;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(
				-g_aBullet[nCntBullet].size.x,
				g_aBullet[nCntBullet].size.y, 0);	// 一つ目の頂点
			pVtx[1].pos = D3DXVECTOR3(
				g_aBullet[nCntBullet].size.x, 
				g_aBullet[nCntBullet].size.y, 0);	// 二つ目の頂点
			pVtx[2].pos = D3DXVECTOR3(
				-g_aBullet[nCntBullet].size.x,
				-g_aBullet[nCntBullet].size.y, 0);	// 三つ目の頂点
			pVtx[3].pos = D3DXVECTOR3(
				g_aBullet[nCntBullet].size.x, 
				-g_aBullet[nCntBullet].size.y, 0);	// 四つ目の頂点
			// テクスチャー設定
			// プレイヤー
			if (g_aBullet[nCntBullet].nType == BULLETTYPE_PLAYER)
			{
				SetTexBullet(pVtx,
					ANIMATION_FOUR_FTEX * g_aBullet[nCntBullet].nPattern,
					ANIMATION_FOUR_FTEX + ANIMATION_FOUR_FTEX * g_aBullet[nCntBullet].nPattern);
			}
			// それ以外
			else
			{
				SetTexBullet(pVtx,
					0.0f,
					1.0f);
			}
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * BULLET_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffBullet->Lock(0, 0, (void **)&pVtx, 0);
			
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(
			-g_aBullet[nCntBullet].size.x,
			g_aBullet[nCntBullet].size.y, 0);	// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(
			g_aBullet[nCntBullet].size.x,
			g_aBullet[nCntBullet].size.y, 0);	// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(
			-g_aBullet[nCntBullet].size.x,
			-g_aBullet[nCntBullet].size.y, 0);	// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(
			g_aBullet[nCntBullet].size.x,
			-g_aBullet[nCntBullet].size.y, 0);	// 四つ目の頂点

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
	g_pVtxBuffBullet->Unlock();
}

// ------------------------------------------------------------------------------------------
// キャラクターの当たり判定
// ------------------------------------------------------------------------------------------
void Update_DecisionBullet(int nCntBullet)
{
	// 変数宣言
	// 情報取得
	PLAYER * pPlayer = GetPlayer();			// プレイヤー
	GIRAFFE * pGiraffe = GetGiraffe();		// キリン
	SHOGIRA * pShogira = GetShogira();		// 弾キリン
	BUFFALO * pBuffalo = GetBuffalo();		// バッファロー
	KING_RION *pKing_Rion = GetKing_Rion();	// 百獣の王

	// 敵の弾
	if (g_aBullet[nCntBullet].nType == BULLETTYPE_ENEMY)
	{
		if (pPlayer->status.nState != PLAYER_STATE_DIE)
		{
			// キリンの弾の弾とプレイヤーの当たり判定
			if ((pPlayer->status.fLength * pPlayer->status.fLength) +
				(g_aBullet[nCntBullet].size.x * g_aBullet[nCntBullet].size.x) >=
				(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x) *
				(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x) +
				(pPlayer->pos.y - g_aBullet[nCntBullet].pos.y) *
				(pPlayer->pos.y - g_aBullet[nCntBullet].pos.y) +
				(pPlayer->pos.z - g_aBullet[nCntBullet].pos.z) *
				(pPlayer->pos.z - g_aBullet[nCntBullet].pos.z))
			{
				// プレイヤーのダメージ処理
				HitPlayer(g_aBullet[nCntBullet].nAttack);

				// 未使用状態へ
				g_aBullet[nCntBullet].bUse = false;

				//// 爆発処理
				//SetExplosion(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].nAttack);

				// 影の削除
				DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
			}
		}
	}

	// プレイヤーの弾
	if (g_aBullet[nCntBullet].nType == BULLETTYPE_PLAYER)
	{
		// カウントキリン
		for (int nCntGiraffe = 0; nCntGiraffe < MAX_GIRAFFE; nCntGiraffe++, pGiraffe++)
		{
			// キリンが使用状態の場合
			if (pGiraffe->bUse == true)
			{
				if (pGiraffe->status.nStatus != GIRAFFESTATUS_DIE)
				{
					// プレイヤーの弾の弾とキリンの当たり判定
					if ((pGiraffe->fLength * pGiraffe->fLength) +
						(g_aBullet[nCntBullet].size.x * 0.5 * g_aBullet[nCntBullet].size.x * 0.5) >=
						(pGiraffe->pos.x - g_aBullet[nCntBullet].pos.x) *
						(pGiraffe->pos.x - g_aBullet[nCntBullet].pos.x) +
						(pGiraffe->pos.y - g_aBullet[nCntBullet].pos.y) *
						(pGiraffe->pos.y - g_aBullet[nCntBullet].pos.y) +
						(pGiraffe->pos.z - g_aBullet[nCntBullet].pos.z) *
						(pGiraffe->pos.z - g_aBullet[nCntBullet].pos.z))
					{
						// プレイヤーダメージ処理
						HitGiraffe(nCntGiraffe, g_aBullet[nCntBullet].nAttack);

						// 未使用状態へ
						g_aBullet[nCntBullet].bUse = false;

						// タイプ別
						switch (g_aBullet[nCntBullet].nType)
						{
							// プレイヤー弾
						case BULLETTYPE_PLAYER:
						case BULLETTYPE_BOSS:
							// 爆発effect
							Update_EndEffectBullet(nCntBullet);
							break;
						case BULLETTYPE_ENEMY:
							break;
						}

						// 影の削除
						DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
					}
				}
			}
		}

		// カウント弾キリン
		for (int nCntShogira = 0; nCntShogira < MAX_SHOGIRA; nCntShogira++, pShogira++)
		{
			// 弾キリンが使用状態の場合
			if (pShogira->bUse == true)
			{
				if (pShogira->status.nStatus != GIRAFFESTATUS_DIE)
				{
					// プレイヤーの弾の弾と弾キリンの当たり判定
					if ((pShogira->fLength * pShogira->fLength) +
						(g_aBullet[nCntBullet].size.x * 0.5 * g_aBullet[nCntBullet].size.x * 0.5) >=
						(pShogira->pos.x - g_aBullet[nCntBullet].pos.x) *
						(pShogira->pos.x - g_aBullet[nCntBullet].pos.x) +
						(pShogira->pos.y - g_aBullet[nCntBullet].pos.y) *
						(pShogira->pos.y - g_aBullet[nCntBullet].pos.y) +
						(pShogira->pos.z - g_aBullet[nCntBullet].pos.z) *
						(pShogira->pos.z - g_aBullet[nCntBullet].pos.z))
					{
						// プレイヤーダメージ処理
						HitShogira(nCntShogira, g_aBullet[nCntBullet].nAttack);

						// 未使用状態へ
						g_aBullet[nCntBullet].bUse = false;

						// タイプ別
						switch (g_aBullet[nCntBullet].nType)
						{
							// プレイヤー弾
						case BULLETTYPE_PLAYER:
						case BULLETTYPE_BOSS:
							// 爆発effect
							Update_EndEffectBullet(nCntBullet);
							break;
						case BULLETTYPE_ENEMY:
							break;
						}

						// 影の削除
						DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
					}
				}
			}
		}

		// カウントバッファロー
		for (int nCntBuffalo = 0; nCntBuffalo < MAX_GIRAFFE; nCntBuffalo++, pBuffalo++)
		{
			// バッファローが使用状態の場合
			if (pBuffalo->bUse == true)
			{
				if (pBuffalo->status.nStatus != GIRAFFESTATUS_DIE)
				{
					// プレイヤーの弾の弾とバッファローの当たり判定
					if ((pBuffalo->fLength * pBuffalo->fLength) +
						(g_aBullet[nCntBullet].size.x * 0.5 * g_aBullet[nCntBullet].size.x * 0.5) >=
						(pBuffalo->pos.x - g_aBullet[nCntBullet].pos.x) *
						(pBuffalo->pos.x - g_aBullet[nCntBullet].pos.x) +
						(pBuffalo->pos.y - g_aBullet[nCntBullet].pos.y) *
						(pBuffalo->pos.y - g_aBullet[nCntBullet].pos.y) +
						(pBuffalo->pos.z - g_aBullet[nCntBullet].pos.z) *
						(pBuffalo->pos.z - g_aBullet[nCntBullet].pos.z))
					{
						// プレイヤーダメージ処理
						HitBuffalo(nCntBuffalo, g_aBullet[nCntBullet].nAttack);

						// 未使用状態へ
						g_aBullet[nCntBullet].bUse = false;

						// タイプ別
						switch (g_aBullet[nCntBullet].nType)
						{
							// プレイヤー弾
						case BULLETTYPE_PLAYER:
						case BULLETTYPE_BOSS:
							// 爆発effect
							Update_EndEffectBullet(nCntBullet);
							break;
						case BULLETTYPE_ENEMY:
							break;
						}
						// 影の削除
						DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
					}
				}
			}
		}

		// カウントキリン
		for (int nCntKing_Rion = 0; nCntKing_Rion < MAX_KING_RION; nCntKing_Rion++, pKing_Rion++)
		{
			// キリンが使用状態の場合
			if (pKing_Rion->bUse == true)
			{
				if (pKing_Rion->status.nStatus != KING_RIONSTATUS_DIE)
				{
					// プレイヤーの弾の弾とキリンの当たり判定
					if ((pKing_Rion->fLength * pKing_Rion->fLength) +
						(g_aBullet[nCntBullet].size.x * 0.5 * g_aBullet[nCntBullet].size.x * 0.5) >=
						(pKing_Rion->pos.x - g_aBullet[nCntBullet].pos.x) *
						(pKing_Rion->pos.x - g_aBullet[nCntBullet].pos.x) +
						(pKing_Rion->pos.y - g_aBullet[nCntBullet].pos.y) *
						(pKing_Rion->pos.y - g_aBullet[nCntBullet].pos.y) +
						(pKing_Rion->pos.z - g_aBullet[nCntBullet].pos.z) *
						(pKing_Rion->pos.z - g_aBullet[nCntBullet].pos.z))
					{
						// プレイヤーダメージ処理
						HitKing_Rion(nCntKing_Rion, g_aBullet[nCntBullet].nAttack);

						// 未使用状態へ
						g_aBullet[nCntBullet].bUse = false;

						// タイプ別
						switch (g_aBullet[nCntBullet].nType)
						{
							// プレイヤー弾
						case BULLETTYPE_PLAYER:
						case BULLETTYPE_BOSS:
							// 爆発effect
							Update_EndEffectBullet(nCntBullet);
							break;
						case BULLETTYPE_ENEMY:
							break;
						}

						// 影の削除
						DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
					}
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 弾の当たり判定
// ------------------------------------------------------------------------------------------
void Update_ColisionBullet(int nCntBullet)
{
	// 素材の当たり判定 //
	if (ColisionMaterials(&g_aBullet[nCntBullet].pos,
		&g_aBullet[nCntBullet].posOld,
		&g_aBullet[nCntBullet].move,
		&g_aBullet[nCntBullet].size,
		&g_aBullet[nCntBullet].pMaterials) != -1)
	{
		// 未使用状態へ
		g_aBullet[nCntBullet].bUse = false;

		// タイプ別
		switch (g_aBullet[nCntBullet].nType)
		{
			// プレイヤー弾
		case BULLETTYPE_PLAYER:
		case BULLETTYPE_BOSS:
			// 爆発effect
			Update_EndEffectBullet(nCntBullet);
			break;
		case BULLETTYPE_ENEMY:
			break;
		}

		// 影の削除
		DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
	}

	// 壁の当たり判定
	if (ColisionWall(&g_aBullet[nCntBullet].pos,
		&g_aBullet[nCntBullet].posOld,
		&g_aBullet[nCntBullet].move,
		&g_aBullet[nCntBullet].size,
		&g_aBullet[nCntBullet].pWall))
	{
		// 未使用状態へ
		g_aBullet[nCntBullet].bUse = false;

		// タイプ別
		switch (g_aBullet[nCntBullet].nType)
		{
			// プレイヤー弾
		case BULLETTYPE_PLAYER:
		case BULLETTYPE_BOSS:
			// 爆発effect
			Update_EndEffectBullet(nCntBullet);
			break;
		case BULLETTYPE_ENEMY:
			break;
		}

		// 影の削除
		DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
	}
}

// ------------------------------------------------------------------------------------------
// 爆発effect情報
// ------------------------------------------------------------------------------------------
void Update_EndEffectBullet(int nCntBullet)
{
		// 変数宣言
	float fAngle;
	float fYAngle;
	D3DXVECTOR3 move;
	D3DXCOLOR col;

	// タイプ別
	switch (g_aBullet[nCntBullet].nType)
	{
		// プレイヤー弾
	case BULLETTYPE_PLAYER:
	case BULLETTYPE_BOSS:
		// 移動方向
		move =
			D3DXVECTOR3(0.0f,
				1.0f,
				0.0f);

		// カラー
		col =
			D3DXCOLOR(
				1.0f,
				0.0f,
				0.0f,
				1.0f);

		for (int nCntEffect = 0; nCntEffect < 30; nCntEffect++)
		{
			// X・Zの角度
			fAngle =
				float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

			// Yの角度
			fYAngle =
				float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

			// エフェクトの設定
			SetEffect(
				g_aBullet[nCntBullet].pos +
				D3DXVECTOR3(sinf(fAngle) * BULLET_SIZE,
					cosf(fYAngle) * BULLET_SIZE,
					cosf(fAngle) * BULLET_SIZE),// 位置
				move,									// 移動量
				col,									// 色
				70,		// サイズ
				-1,										// 減少サイズ
				0.0f,									// 重力
				65,										// エフェクトライフ
				0,										// タイプ
				0);
		}
		// カラー
		col =
			D3DXCOLOR(
				1.0f,
				0.3f,
				0.1f,
				1.0f);


		for (int nCntEffect = 0; nCntEffect < 30; nCntEffect++)
		{
			// X・Zの角度
			fAngle =
				float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

			// Yの角度
			fYAngle =
				float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

			// エフェクトの設定
			SetEffect(
				g_aBullet[nCntBullet].pos +
				D3DXVECTOR3(sinf(fAngle) * BULLET_SIZE,
					cosf(fYAngle) * BULLET_SIZE,
					cosf(fAngle) * BULLET_SIZE),// 位置
				move,									// 移動量
				col,									// 色
				50,		// サイズ
				-1,										// 減少サイズ
				0.0f,									// 重力
				65,										// エフェクトライフ
				0,										// タイプ
				0);
		}

		break;
	case BULLETTYPE_ENEMY:
		// X・Zの角度
		fAngle =
			float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

		// Yの角度
		fYAngle =
			float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;

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
				sinf(fAngle) * BULLET_SIZE + g_aBullet[nCntBullet].pos.x,
				cosf(fYAngle) * BULLET_SIZE + g_aBullet[nCntBullet].pos.y,
				cosf(fAngle) * BULLET_SIZE + g_aBullet[nCntBullet].pos.z),

			move,															// 移動量
			col,															// 色
			float(rand() % 10),												// サイズ
			-0,																// 減少サイズ
			0.0f,															// 重力
			10,																// エフェクトライフ
			0,																// タイプ
			0);
	default:
		break;
	}
}

// ------------------------------------------------------------------------------------------
// 弾情報
// ------------------------------------------------------------------------------------------
BULLET *GetBullet(void) 
{
	return &g_aBullet[0];
}