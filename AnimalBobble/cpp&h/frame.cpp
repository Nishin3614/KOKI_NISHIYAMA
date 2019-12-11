// ----------------------------------------
//
// シーン処理の説明[frame.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "frame.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define FRAME_TEXTRUE "data/TEXTURE/UI/Frame.png"

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
LPDIRECT3DTEXTURE9 CFrame::m_pTex = NULL;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CFrame::CFrame() : CScene_TWO::CScene_TWO(ACTOR_FRAME,LAYER_OBJ)
{
	m_PlayerId = CManager::PLAYER_1;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CFrame::~CFrame()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CFrame::Init(void)
{	
	CScene_TWO::SetSize(D3DXVECTOR2(500.0f, 600.0f));
	CScene_TWO::Init();
	CScene_TWO::SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	CScene_TWO::BindTexture(m_pTex);
	CScene_TWO::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CFrame::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CFrame::Update(void)
{
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CFrame::Draw(void)
{
	CScene_TWO::Draw();
}

// ----------------------------------------
// プレイヤー番号取得処理
// ----------------------------------------
CManager::PLAYER CFrame::GetPlayerId(void)
{
	return m_PlayerId;
}

// ----------------------------------------
// 位置情報取得処理
// ----------------------------------------
D3DXVECTOR3 CFrame::GetPos(void)
{
	return CScene_TWO::GetPosition();
}

// ----------------------------------------
// サイズ情報取得処理
// ----------------------------------------
D3DXVECTOR2 CFrame::GetSize(void)
{
	return CScene_TWO::GetSize();
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CFrame::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// テクスチャー設定
	D3DXCreateTextureFromFile(pDevice, FRAME_TEXTRUE, &m_pTex);

	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CFrame::UnLoad(void)
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
CFrame * CFrame::Create(CManager::PLAYER playerId, D3DXVECTOR3 pos)
{
	// 変数宣言
	CFrame * pFrame;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pFrame = new CFrame();
	// 初期化処理
	pFrame->Init();
	// プレイヤー番号代入
	pFrame->m_PlayerId = playerId;
	// 位置代入
	pFrame->SetPosition(pos);
	// 生成したオブジェクトを返す
	return pFrame;
}