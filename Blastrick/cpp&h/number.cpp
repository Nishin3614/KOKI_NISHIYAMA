// ----------------------------------------
//
// 番号処理の説明[number.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "number.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

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
LPDIRECT3DTEXTURE9 CNumber::m_pTex[CNumber::TEX_MAX] = {};

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CNumber::CNumber()
{
	m_pVtxBuff = NULL;
	m_rot = 0;
	m_fLengh = 0;
	m_fAngle = 0;
	m_texID = TEX_SCORE;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CNumber::~CNumber()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CNumber::Init(void)
{	
	// 変数宣言
	VERTEX_2D *pVtx;				// 頂点情報
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	// 頂点の大きさ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// 頂点テクスチャー
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CNumber::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CNumber::Update(void)
{
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CNumber::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(
		0,
		m_pVtxBuff,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0, m_pTex[m_texID]);

	// ポリゴン描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);
}

// ----------------------------------------
// 番号設定処理
// ----------------------------------------
void CNumber::SetNum(int nNum)
{
	// 変数宣言
	VERTEX_2D *pVtx;	// 頂点情報
	float fTex;			// テクスチャー範囲

	// テクスチャー範囲の計算
	fTex = (float)nNum / 10.0f;

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// 頂点テクスチャー
	pVtx[0].tex = D3DXVECTOR2(fTex, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTex + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fTex, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fTex + 0.1f, 1.0f);
	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 位置設定処理
// ----------------------------------------
void CNumber::SetPos(D3DXVECTOR3 pos,float fsize)
{
	// 変数宣言
	VERTEX_2D *pVtx;	// 頂点情報

	// 三角関数を使い斜めの長さを求める
	m_fLengh = sqrtf((fsize / 2) * (fsize / 2) + (fsize / 2) * (fsize / 2));
	m_fAngle = atan2f((fsize / 2), (fsize / 2));

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// 頂点座標
	pVtx[0].pos = pos + D3DXVECTOR3(sinf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, cosf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(sinf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, cosf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(sinf(-m_fAngle + m_rot) * m_fLengh, cosf(-m_fAngle + m_rot) * m_fLengh, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(sinf(m_fAngle + m_rot) * m_fLengh, cosf(m_fAngle + m_rot) * m_fLengh, 0.0f);
	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// テクスチャー処理
// ----------------------------------------
void CNumber::SetTex(TEX tex)
{
	m_texID = tex;
}

// ----------------------------------------
// カラー設定処理
// ----------------------------------------
void CNumber::SetCol(D3DXCOLOR col)
{
	// 変数宣言
	VERTEX_2D *pVtx;	// 頂点情報

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// 頂点カラー
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CNumber::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// テクスチャー情報代入
	char TexData[TEX_MAX][72] =
	{
		{ "data/TEXTURE/number000.png" },
		{ "data/TEXTURE/UI/stage_number.png" },
	};
	// テクスチャーの読み込み
	for (int nCnt = 0; nCnt < TEX_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice, TexData[nCnt], &m_pTex[nCnt]);
	}

	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CNumber::UnLoad(void)
{
	// テクスチャーの読み込み
	for (int nCnt = 0; nCnt < TEX_MAX; nCnt++)
	{
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CNumber * CNumber::Create()
{
	// 変数宣言
	CNumber * pNumber;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pNumber = new CNumber();
	// 初期化処理
	pNumber->Init();
	// 生成したオブジェクトを返す
	return pNumber;
}