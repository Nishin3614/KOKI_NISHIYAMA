// ----------------------------------------
//
// 線処理の説明[line.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "line.h"

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
LPDIRECT3DTEXTURE9 CLine::m_pTex = NULL;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CLine::CLine() : CScene::CScene(ACTOR_LINE, LAYER_3DOBJECT)
{
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CLine::~CLine()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CLine::Init(void)
{	
	// 変数宣言
	VERTEX_2D *pVtx;							// 頂点情報
	LPDIRECT3DDEVICE9 pDevice =					// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 2,
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
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	// 頂点の大きさ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	// 頂点テクスチャー
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CLine::Uninit(void)
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
void CLine::Update(void)
{
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CLine::Draw(void)
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
	// テクスチャの設定
	pDevice->SetTexture(0, m_pTex);
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);						// 頂点数
}

// ----------------------------------------
// 位置設定処理
// ----------------------------------------
void CLine::SetPos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	// 変数宣言
	VERTEX_2D *pVtx;							// 頂点情報

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	// 頂点座標
	pVtx[0].pos = pos1;
	pVtx[1].pos = pos2;

	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CLine * CLine::Create(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	// 変数宣言
	CLine * pLine;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pLine = new CLine();
	// 初期化処理
	pLine->Init();
	// 位置設定
	pLine->SetPos(pos1, pos2);
	// 生成したオブジェクトを返す
	return pLine;
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CLine::Load(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =					// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	D3DXCreateTextureFromFile(pDevice,"png", &m_pTex);
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CLine::UnLoad(void)
{
		// テクスチャの開放
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}
