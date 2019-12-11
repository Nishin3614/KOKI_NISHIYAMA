// ----------------------------------------
//
// 箱処理の説明[box.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "box.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define BOX_TEX "data/TEXTURE/HossanAnimation.png"
#define BOX_SIZE (100.0f)
#define BOX_ANIMCOUNT (2)
#define BOX_HORIZONANIM (1.0f/BOX_ANIMCOUNT)
#define BOX_VERTICALANIM (1.0f/BOX_ANIMCOUNT)

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
LPDIRECT3DTEXTURE9 CBox::m_pTex = NULL;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CBox::CBox() : CScene_TWO::CScene_TWO(ACTOR_BOX,LAYER_OBJ)
{
	// 水平のアニメーションパターン
	m_nHorizonAnim = 0;
	// 垂直のアニメーションパターン
	m_nVirticalAnim = 1;
	// アニメーションカウント
	m_nCntAnim = 0;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CBox::~CBox()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CBox::Init(void)
{	
	// サイズ設定
	CScene_TWO::SetSize(D3DXVECTOR2(BOX_SIZE, BOX_SIZE));
	// 初期化
	CScene_TWO::Init();
	// 位置設定
	CScene_TWO::SetPosition(D3DXVECTOR3(500.0f, 600.0f, 0.0f));
	// 頂点テクスチャー設定
	CScene_TWO::SetTex(
		D3DXVECTOR2(m_nHorizonAnim * BOX_HORIZONANIM, m_nVirticalAnim * BOX_VERTICALANIM),
		D3DXVECTOR2(m_nHorizonAnim * BOX_HORIZONANIM + BOX_HORIZONANIM, m_nVirticalAnim * BOX_VERTICALANIM + BOX_VERTICALANIM));
	// テクスチャー設定
	CScene_TWO::BindTexture(m_pTex);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CBox::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CBox::Update(void)
{
	CScene_TWO::Update();
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CBox::Draw(void)
{
	CScene_TWO::Draw();
}

// ----------------------------------------
// 垂直のアニメーションパターン設定処理
// ----------------------------------------
void CBox::SetVirticalAnim(int nVirtical)
{
	m_nVirticalAnim = nVirtical;
	// 頂点テクスチャー設定
	CScene_TWO::SetTex(
		D3DXVECTOR2(m_nHorizonAnim * BOX_HORIZONANIM, m_nVirticalAnim * BOX_VERTICALANIM),
		D3DXVECTOR2(m_nHorizonAnim * BOX_HORIZONANIM + BOX_HORIZONANIM, m_nVirticalAnim * BOX_VERTICALANIM + BOX_VERTICALANIM));
}

// ----------------------------------------
// 水平のアニメーションパターン設定処理
// ----------------------------------------
void CBox::SetHorizonAnim(int nHorizon)
{
	m_nHorizonAnim = nHorizon;
	// 頂点テクスチャー設定
	CScene_TWO::SetTex(
		D3DXVECTOR2(m_nHorizonAnim * BOX_HORIZONANIM, m_nVirticalAnim * BOX_VERTICALANIM),
		D3DXVECTOR2(m_nHorizonAnim * BOX_HORIZONANIM + BOX_HORIZONANIM, m_nVirticalAnim * BOX_VERTICALANIM + BOX_VERTICALANIM));
}

// ----------------------------------------
// プレイヤー番号取得処理
// ----------------------------------------
CManager::PLAYER CBox::GetPlayerId(void)
{
	return m_PlayerId;
}

// ----------------------------------------
// 位置情報取得処理
// ----------------------------------------
D3DXVECTOR3 CBox::GetPos(void)
{
	return CScene_TWO::GetPosition();
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CBox::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice =		
		CManager::GetRenderer()->GetDevice();
	// テクスチャー設定
	D3DXCreateTextureFromFile(pDevice, BOX_TEX, &m_pTex);
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CBox::UnLoad(void)
{
	// テクスチャの開放
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CBox * CBox::Create(CManager::PLAYER playerId, D3DXVECTOR3 pos)
{
	// 変数宣言
	CBox * pBox;		// 箱2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pBox = new CBox();
	// 初期化処理
	pBox->Init();
	// プレイヤー番号
	pBox->m_PlayerId = playerId;
	// 位置設定
	pBox->SetPosition(pos);
	// 生成したオブジェクトを返す
	return pBox;
}
// ----------------------------------------
// アニメーション処理
// ----------------------------------------
void CBox::Animation(void)
{
	// アニメーションカウントアップ
	m_nCntAnim++;
	// 20フレームずつアニメーションする
	if (m_nCntAnim % 20 == 0)
	{
		// アニメーション変化
		m_nHorizonAnim = (m_nHorizonAnim + 1) % BOX_ANIMCOUNT;
	}
}
