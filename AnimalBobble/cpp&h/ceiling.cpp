// ----------------------------------------
//
// 天井処理の説明[ceiling.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "ceiling.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define CEILING_TEXTRUE "data/TEXTURE/Ceiling.png"

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
LPDIRECT3DTEXTURE9 CCeiling::m_pTex = NULL;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CCeiling::CCeiling() : CScene_TWO::CScene_TWO(ACTOR_CEILING,LAYER_OBJ)
{
	m_PlayerId = CManager::PLAYER_1;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CCeiling::~CCeiling()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CCeiling::Init(void)
{	
	CScene_TWO::SetSize(D3DXVECTOR2(0.0f, 0.0f));
	CScene_TWO::Init();
	CScene_TWO::SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	CScene_TWO::BindTexture(m_pTex);
	CScene_TWO::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CCeiling::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CCeiling::Update(void)
{
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CCeiling::Draw(void)
{
	CScene_TWO::Draw();
}

// ----------------------------------------
// プレイヤー番号取得処理
// ----------------------------------------
CManager::PLAYER CCeiling::GetPlayerId(void)
{
	return m_PlayerId;
}

// ----------------------------------------
// 位置情報取得処理
// ----------------------------------------
D3DXVECTOR3 CCeiling::GetPos(void)
{
	return CScene_TWO::GetPosition();
}

// ----------------------------------------
// サイズ情報取得処理
// ----------------------------------------
D3DXVECTOR2 CCeiling::GetSize(void)
{
	return CScene_TWO::GetSize();
}

// ----------------------------------------
// 位置・サイズ設定処理
// ----------------------------------------
void CCeiling::SetPosSize(
	D3DXVECTOR3 pos,	// 位置
	D3DXVECTOR2 size	// サイズ
)
{
	CScene_TWO::SetSize(size);
	CScene_TWO::SetPosition(pos);
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CCeiling::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// テクスチャー設定
	D3DXCreateTextureFromFile(pDevice, CEILING_TEXTRUE, &m_pTex);

	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CCeiling::UnLoad(void)
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
CCeiling * CCeiling::Create(
	CManager::PLAYER playerId,	// プレイヤー番号
	D3DXVECTOR3 pos,	// 位置
	D3DXVECTOR2 size	// サイズ
)
{
	// 変数宣言
	CCeiling * pCeiling;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pCeiling = new CCeiling();
	// 初期化処理
	pCeiling->Init();
	// プレイヤー番号代入
	pCeiling->m_PlayerId = playerId;
	// サイズ
	pCeiling->SetSize(size);
	// 位置代入
	pCeiling->SetPosition(pos);
	// 生成したオブジェクトを返す
	return pCeiling;
}