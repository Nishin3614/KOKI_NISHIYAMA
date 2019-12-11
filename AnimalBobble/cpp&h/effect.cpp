// ----------------------------------------
//
// エフェクト処理の説明[effect.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "effect.h"

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
LPDIRECT3DTEXTURE9 CEffect::m_pTex = NULL;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CEffect::CEffect() : CScene_TWO::CScene_TWO(ACTOR_EFFECT, LAYER_PARTICLE)
{
	// 変数の初期化
	m_fSize = 0.0f;
	m_fRot = 0;
	m_fGoalrot = 0;
	m_fDiffrot = 0;
	m_Speed = 0;
	m_fSize = 0;
	m_fGoalsize = 0;
	m_fDiffsize = 0;
	m_nLife = 0;
	m_nSplit = 0;
	m_nCntSplit = 0;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CEffect::~CEffect()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CEffect::Init(void)
{
	// サイズ設定
	CScene_TWO::SetSize(D3DXVECTOR2(m_fSize, m_fSize));
	// 初期化
	CScene_TWO::Init();
	// 現在の位置設定
	CScene_TWO::SetPosition(m_pos);
	// カラー設定
	CScene_TWO::SetCol(m_col);
	// テクスチャー設定
	CScene_TWO::BindTexture(m_pTex);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CEffect::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CEffect::Update(void)
{
	// ライフの減少
	m_nLife--;
	// 分割カウントアップ
	m_nCntSplit++;
	// カラーの変更
	m_col -= m_Diffcol;
	// サイズの変更
	m_fSize -= m_fDiffsize;
	// 角度の変更
	if (m_nCntSplit <= m_nSplit)
	{
		m_fRot -= m_fDiffrot;
	}
	m_pos.x += sinf(m_fRot) * m_Speed;
	m_pos.y += cosf(m_fRot) * m_Speed;

	// 角度変更
	CScene_TWO::SetRot(m_fRot);
	// サイズ変更
	CScene_TWO::SetSize(D3DXVECTOR2(m_fSize, m_fSize));
	// 現在の位置設定
	CScene_TWO::SetPosition(m_pos);
	// カラー設定
	CScene_TWO::SetCol(m_col);
	// 更新
	CScene_TWO::Update();

	// 終了
	if (m_nLife <= 0)
	{
		Release();
		return;
	}

}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CEffect::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene_TWO::Draw();

	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CEffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	// テクスチャーの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bobble.png", &m_pTex);

	return S_OK;
	//"data/TEXTURE/bobble.png"
	//"data/TEXTURE/Effect/effect.jpg"
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CEffect::UnLoad(void)
{
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}


// ----------------------------------------
// 噴水処理
// ----------------------------------------
void CEffect::Fountain(
	float fGoalrot,
	int nSplit)
{
	m_fGoalrot = fGoalrot;
	m_nSplit = nSplit;
	m_fDiffrot = (m_fGoalrot - m_fRot) / m_nSplit;
}

// ----------------------------------------
// 円処理
// ----------------------------------------
void CEffect::Circle(
	int nSplit
)
{
	m_nSplit = nSplit;
	m_fDiffrot = D3DX_PI / m_nSplit;
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CEffect * CEffect::Create(
	D3DXVECTOR3 pos,		// 位置
	D3DXCOLOR col,			// カラー
	D3DXCOLOR goalcol,		// 目標のカラー
	float fRot,				// 初期角度
	float fSpeed,			// 速さ
	float fSize,			// サイズ
	float fGoalsize,		// 目標サイズ
	int nLife				// ライフ(分割数)
)
{
	// 変数宣言
	CEffect * pEffect;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pEffect = new CEffect();
	/* 変数代入 */
	pEffect->m_pos = pos;
	pEffect->m_col = col;
	pEffect->m_Goalcol = goalcol;
	pEffect->m_fRot = fRot;
	pEffect->m_Speed = fSpeed;
	pEffect->m_fSize = fSize;
	pEffect->m_fGoalsize = fGoalsize;
	pEffect->m_nLife = nLife;
	pEffect->m_Diffcol = (pEffect->m_Goalcol - pEffect->m_col);
	pEffect->m_Diffcol /= (float)pEffect->m_nLife;
	pEffect->m_fDiffsize = (pEffect->m_fGoalsize - pEffect->m_fSize) / pEffect->m_nLife;

	// 初期化処理
	pEffect->Init();
	// 生成したオブジェクトを返す
	return pEffect;
}
