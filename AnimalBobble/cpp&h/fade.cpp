// ----------------------------------------
//
// 画面遷移処理の説明[fade.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "fade.h"
#include "manager.h"

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
LPDIRECT3DTEXTURE9 CFade::m_pTex = NULL;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CFade::CFade()
{
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CFade::~CFade()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CFade::Init(CManager::MODE Mode)
{	
	// 変数宣言
	VERTEX_2D *pVtx;				// 頂点情報
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	m_fade = FADE_OUT;
	m_modeNext = Mode;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 黒い画面（不透明）

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
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	// 頂点の大きさ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
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
void CFade::Uninit(void)
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
void CFade::Update(void)
{
	VERTEX_2D *pVtx;

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{
			m_col.a -= 0.05f;
			if (m_col.a <= 0.0f)
			{
				// フェード終了
				m_col.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}

		else if (m_fade == FADE_OUT)
		{
			// フェードアウト
			m_col.a += 0.05f;
			if (m_col.a >= 1.0f)
			{
				// フェード終了
				m_col.a = 1.0f;
				m_fade = FADE_IN;
				CManager::SetMode(m_modeNext);
			}
		}

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		// 頂点データをアンロックする
		m_pVtxBuff->Unlock();
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CFade::Draw(void)
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
	pDevice->SetTexture(0, m_pTex);
	// ポリゴン描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);
}

// ----------------------------------------
// 画面遷移設定処理
// ----------------------------------------
void CFade::SetFade(CManager::MODE Mode)
{
	m_fade = FADE_OUT;
	m_modeNext = Mode;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// 黒い画面（透明)
}

// ----------------------------------------
// 画面遷移取得処理
// ----------------------------------------
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CFade::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	// テクスチャーの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/fade000.png", &m_pTex);

	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CFade::UnLoad(void)
{
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}