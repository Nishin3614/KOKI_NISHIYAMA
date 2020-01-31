// ----------------------------------------
//
// 軌跡処理の説明[meshobit.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "meshobit.h"
#include "game.h"
#include "debugproc.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define MESHOBIT_ROTMOVE (0.01f)		// 角度の移動量
#define MESHOBIT_SIZE (100.0f)			// サイズ
#define MESHOBIT_ANGLE (D3DX_PI / 10)	// 湾曲の角度

// ----------------------------------------
//
// グローバル変数
//
// ----------------------------------------

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------
LPDIRECT3DTEXTURE9 CMeshobit::m_pTex[TEX_MAX] = {};
CMeshobit::MESHOBIT CMeshobit::m_meshobit[MESHOBIT_MAX] = {};

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CMeshobit::CMeshobit() : CScene(ACTOR_MESHOBIT, LAYER_3DPARTICLE)
{
	m_tex = TEX_0;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CMeshobit::~CMeshobit()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CMeshobit::Init(void)
{
	// 軌跡1番目から2番目の描画
	for (int nCntMesh = 0; nCntMesh < MESHOBIT_MAX; nCntMesh++)
	{
		// 設定
		m_meshobit[nCntMesh].othermtx = NULL;					// 行列
		m_meshobit[nCntMesh].m_aOffset[0] = D3DVECTOR3_ZERO;	// オフセット1
		m_meshobit[nCntMesh].m_aOffset[1] = D3DVECTOR3_ZERO;	// オフセット2
		m_meshobit[nCntMesh].col = D3DXCOLOR_INI;				// 色
		m_meshobit[nCntMesh].type = CMeshobit::TYPE_0;			// タイプ
		m_meshobit[nCntMesh].fSize = 0.0f;						// サイズ
		m_meshobit[nCntMesh].nMaxFrame = 0;						// 最大フレーム数
		m_meshobit[nCntMesh].bUse = false;						// 使用状態
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CMeshobit::Uninit(void)
{
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CMeshobit::Update(void)
{
	for (int nCntMesh = 0; nCntMesh < MESHOBIT_MAX; nCntMesh++)
	{
		// 使用状態
		// フレーム数カウントが最大フレーム数未満
		if (m_meshobit[nCntMesh].bUse == true &&
			m_meshobit[nCntMesh].nCntFrame < m_meshobit[nCntMesh].nMaxFrame)
		{
			// 変数宣言
			VERTEX_3D * pVtx;
			D3DXVec3TransformCoord(
				&m_meshobit[nCntMesh].m_aPosVerTex[0],
				&m_meshobit[nCntMesh].m_aOffset[0],
				m_meshobit[nCntMesh].othermtx
			);
			// 1番目の頂点
			m_meshobit[nCntMesh].obit[0].pos[0] = m_meshobit[nCntMesh].m_aPosVerTex[0];
			D3DXVec3TransformCoord(
				&m_meshobit[nCntMesh].m_aPosVerTex[1],
				&m_meshobit[nCntMesh].m_aOffset[1],
				m_meshobit[nCntMesh].othermtx
			);
			// 2番目の頂点
			m_meshobit[nCntMesh].obit[0].pos[1] =				// ②
				m_meshobit[nCntMesh].m_aPosVerTex[1];
			for (int nCntObit = m_meshobit[nCntMesh].nLine - 1; nCntObit > 0; nCntObit--)
			{
				m_meshobit[nCntMesh].obit[nCntObit].pos[0] = m_meshobit[nCntMesh].obit[nCntObit - 1].pos[0];
				m_meshobit[nCntMesh].obit[nCntObit].pos[1] = m_meshobit[nCntMesh].obit[nCntObit - 1].pos[1];
			}
			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			m_meshobit[nCntMesh].pVtxBuff->Lock(
				0,
				0,
				(void **)&pVtx,
				0);
			//頂点設定 //
			//行ループ
			for (int nCntMeshObit = 0; nCntMeshObit < m_meshobit[nCntMesh].nLine; nCntMeshObit++)
			{
				// 頂点座標の設定
				pVtx[0].pos = m_meshobit[nCntMesh].obit[nCntMeshObit].pos[0];
				// 頂点座標の設定
				pVtx[1].pos = m_meshobit[nCntMesh].obit[nCntMeshObit].pos[1];
				// ポイント合わせ
				pVtx += 2;
			}
			// アンロック
			m_meshobit[nCntMesh].pVtxBuff->Unlock();
			// フレーム数カウントアップ
			m_meshobit[nCntMesh].nCntFrame++;
		}
		// それ以外
		else
		{
			// フレーム数カウント初期化
			m_meshobit[nCntMesh].nCntFrame = 0;
			m_meshobit[nCntMesh].othermtx = NULL;					// 行列
			m_meshobit[nCntMesh].m_aOffset[0] = D3DVECTOR3_ZERO;	// オフセット1
			m_meshobit[nCntMesh].m_aOffset[1] = D3DVECTOR3_ZERO;	// オフセット2
			m_meshobit[nCntMesh].col = D3DXCOLOR_INI;				// 色
			m_meshobit[nCntMesh].type = CMeshobit::TYPE_0;			// タイプ
			m_meshobit[nCntMesh].fSize = 0.0f;						// サイズ
			m_meshobit[nCntMesh].bUse = false;
		}
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CMeshobit::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;
	// 両面を描画
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ライティングモード無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Zバッファ　無効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);	
	// Z方向　無効
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	// 軌跡描画
	for (int nCntMeshObit = 0; nCntMeshObit < MESHOBIT_MAX; nCntMeshObit++)
	{
		if (m_meshobit[nCntMeshObit].bUse == true)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&m_meshobit[nCntMeshObit].matrix);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &m_meshobit[nCntMeshObit].matrix);
			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_meshobit[nCntMeshObit].pVtxBuff, 0, sizeof(VERTEX_3D));
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);
			// テクスチャの設定
			pDevice->SetTexture(
				0,
				m_pTex[m_tex]);
			// 軌跡の描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,										// 描画を始める頂点
				m_meshobit[nCntMeshObit].nNumPolygon	// ポリゴン数
			);
		}
	}
	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// ライティングモード有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// 裏面(左回り)でカリングする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// Zバッファ　有効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	// Z方向　有効
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}

//-------------------------------------------------------------------------------------------------------------
// 設定
// 1行列、2オフセット始点、3オフセット終点、4色、5タイプ、6サイズ、7線の個数、8フレーム数
//-------------------------------------------------------------------------------------------------------------
void CMeshobit::Set(
	D3DXMATRIX	*pMtx,		// マトリックス情報
	D3DXVECTOR3	&offset1,	// オフセット1
	D3DXVECTOR3 &offset2,	// オフセット2
	D3DXCOLOR	&col,		// 色
	TYPE		type,		// タイプ
	TEX			tex,		// テクスチャー
	float		fSize,		// サイズ情報
	int			nLine,		// 線
	int			&nFrame		// フレーム数
)
{
	// 軌跡1番目から2番目の描画
	for (int nCntMesh = 0; nCntMesh < MESHOBIT_MAX; nCntMesh++)
	{
		if (m_meshobit[nCntMesh].bUse == false)
		{
			// 変数宣言
			VERTEX_3D * pVtx;
			// 設定
			m_meshobit[nCntMesh].othermtx = pMtx;			// 行列
			m_meshobit[nCntMesh].m_aOffset[0] = offset1;	// オフセット1
			m_meshobit[nCntMesh].m_aOffset[1] = offset2;	// オフセット2
			m_meshobit[nCntMesh].col = col;					// 色
			m_meshobit[nCntMesh].type = type;				// タイプ
			m_meshobit[nCntMesh].fSize = fSize;				// サイズ
			m_meshobit[nCntMesh].nMaxFrame = nFrame;		// 最大フレーム数
			// 上限を超えていたら
			if (nLine > OBITLINE_MAX)
			{
#ifdef _DEBUG
				CCalculation::Messanger("軌跡の線の上限が超えています。\n");
#endif // _DEBUG
				// 代わりに軌跡の線の最大数を代入
				m_meshobit[nCntMesh].nLine = OBITLINE_MAX;
			}
			// それ以外
			else
			{
				m_meshobit[nCntMesh].nLine = nLine;	// 線
			}
			m_meshobit[nCntMesh].nNumPolygon =		// ポリゴン数
				m_meshobit[nCntMesh].nLine * 2 - 2;

			// 行列からの位置の頂点算出
			D3DXVec3TransformCoord(
				&m_meshobit[nCntMesh].m_aPosVerTex[0],
				&m_meshobit[nCntMesh].m_aOffset[0],
				m_meshobit[nCntMesh].othermtx
			);
			// 行列からの位置の頂点算出
			D3DXVec3TransformCoord(
				&m_meshobit[nCntMesh].m_aPosVerTex[1],
				&m_meshobit[nCntMesh].m_aOffset[1],
				m_meshobit[nCntMesh].othermtx
			);
			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			m_meshobit[nCntMesh].pVtxBuff->Lock(
				0,
				0,
				(void **)&pVtx,
				0);
			//頂点設定 //
			//行ループ
			for (int nCntMeshObit = 0; nCntMeshObit < m_meshobit[nCntMesh].nLine; nCntMeshObit++)
			{
				// 頂点を代入
				m_meshobit[nCntMesh].obit[nCntMeshObit].pos[0] = 
					m_meshobit[nCntMesh].m_aPosVerTex[0];
				// 頂点を代入
				m_meshobit[nCntMesh].obit[nCntMeshObit].pos[1] =				// ②
					m_meshobit[nCntMesh].m_aPosVerTex[1];
				// 頂点座標の設定
				pVtx[0].pos = m_meshobit[nCntMesh].obit[0].pos[0];
				float fCol;
				fCol = (float)nCntMeshObit / m_meshobit[nCntMesh].nLine;
				// カラーの設定
				pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, 0.0f - (float)nCntMeshObit / m_meshobit[nCntMesh].nLine);
				// テクスチャーの設定
				pVtx[0].tex = D3DXVECTOR2(1.0f * nCntMeshObit / m_meshobit[nCntMesh].nLine, 0.0f);
				// 法線の設定
				pVtx[0].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
				// 頂点座標の設定
				pVtx[1].pos = m_meshobit[nCntMesh].obit[0].pos[1];
				// カラーの設定
				pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, 1.0f - (float)nCntMeshObit / m_meshobit[nCntMesh].nLine);
				// テクスチャーの設定
				pVtx[1].tex = D3DXVECTOR2(1.0f * nCntMeshObit / m_meshobit[nCntMesh].nLine, 1.0f);
				// 法線の設定
				pVtx[1].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
				// ポイント合わせ
				pVtx += 2;
			}

			// アンロック
			m_meshobit[nCntMesh].pVtxBuff->Unlock();
			// 使用状態
			m_meshobit[nCntMesh].bUse = true;
			return;
		}
	}
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// デバッグ表示
//-------------------------------------------------------------------------------------------------------------
void CMeshobit::Debug(void)
{
	if (m_meshobit[1].othermtx != NULL)
	{
		CDebugproc::Print("行列位置:(%.3f,%.3f,%.3f)\n",
			m_meshobit[1].othermtx->_41,
			m_meshobit[1].othermtx->_42,
			m_meshobit[1].othermtx->_43);
	}

	CDebugproc::Print("頂点加算位置:(%.3f,%.3f,%.3f)\n",
		m_meshobit[1].m_aPosVerTex[0].x,
		m_meshobit[1].m_aPosVerTex[0].y,
		m_meshobit[1].m_aPosVerTex[0].z);


}
#endif // _DEBUG

// ----------------------------------------
// メッシュ処理
// ----------------------------------------
HRESULT CMeshobit::LoadMesh(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	for (int nCntMesh = 0; nCntMesh < MESHOBIT_MAX; nCntMesh++)
	{
		// 設定
		m_meshobit[nCntMesh].nNumberVertex =			// 頂点
			OBITLINE_MAX * 2;
		m_meshobit[nCntMesh].nNumPolygon =				// ポリゴン
			MESHOBIT_MAX * 2 - 2;
		// 設定
		m_meshobit[nCntMesh].bUse = false;
		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(
			sizeof(VERTEX_3D) * m_meshobit[nCntMesh].nNumberVertex,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_meshobit[nCntMesh].pVtxBuff,
			NULL);
		// 頂点データの範囲をロックし、頂点バッファへのポインタ
		m_meshobit[nCntMesh].pVtxBuff->Lock(
			0,
			0,
			(void **)&pVtx,
			0);
		//頂点設定 //
		//行ループ
		for (int nCntMeshObit = 0; nCntMeshObit < OBITLINE_MAX; nCntMeshObit++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			// テクスチャーの設定
			pVtx[0].tex = D3DXVECTOR2(1.0f * nCntMeshObit / OBITLINE_MAX, 0.0f);
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// 頂点座標の設定
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// カラーの設定
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
			// テクスチャーの設定
			pVtx[1].tex = D3DXVECTOR2(1.0f * nCntMeshObit / OBITLINE_MAX, 1.0f);
			// 法線の設定
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// ポイント合わせ
			pVtx += 2;
		}
		// アンロック
		m_meshobit[nCntMesh].pVtxBuff->Unlock();
	}
	return S_OK;
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CMeshobit::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice =
		CManager::GetRenderer()->GetDevice();
	// テクスチャー情報代入
	char TexData[TEX_MAX][72] =
	{
		//{ "data/TEXTURE/trajectory.png" },
		//{ "data/TEXTURE/kiseki2.jpg" }
		{ "data/TEXTURE/meshobit_10.png" },
		{ "data/TEXTURE/meshobit_10.png" }
	};
	// あらかじめ軌跡の生成をする
	LoadMesh();

	// テクスチャー設定
	for (int nCntTex = 0; nCntTex < TEX_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, TexData[nCntTex], &m_pTex[nCntTex]);
	}
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CMeshobit::UnLoad(void)
{
	// テクスチャの開放
	for (int nCntTex = 0; nCntTex < TEX_MAX; nCntTex++)
	{
		if (m_pTex[nCntTex] != NULL)
		{
			m_pTex[nCntTex]->Release();
			m_pTex[nCntTex] = NULL;
		}
	}
	// 頂点バッファの開放
	for (int nCntMesh = 0; nCntMesh < MESHOBIT_MAX; nCntMesh++)
	{
		if (m_meshobit[nCntMesh].pVtxBuff != NULL)
		{
			m_meshobit[nCntMesh].pVtxBuff->Release();
			m_meshobit[nCntMesh].pVtxBuff = NULL;
		}
	}
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CMeshobit * CMeshobit::Create(void)
{
	// 変数宣言
	CMeshobit * pMeshobit;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pMeshobit = new CMeshobit();
	// 初期化処理
	pMeshobit->Init();
	// 生成したオブジェクトを返す
	return pMeshobit;
}