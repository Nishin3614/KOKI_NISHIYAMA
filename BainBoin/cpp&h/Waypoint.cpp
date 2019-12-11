// ----------------------------------------
//
// 中間地点処理の説明[waypoint.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "waypoint.h"
#include "player.h"
#include "meshdome.h"
#include "fade.h"
#include "debugproc.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define WAYPOINT_ROTMOVE (0.01f)				// 角度の移動量
#define WAYPOINT_SIZE (100.0f)					// サイズ
#define WAYPOINT_TEX "data/TEXTURE/Line.png"	// テクスチャー
#define WAYPOINT_ANGLE (D3DX_PI / 1)			// 湾曲の角度

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
LPDIRECT3DTEXTURE9 CWaypoint::m_pTex = NULL;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CWaypoint::CWaypoint() : CScene(ACTOR_WAYPOINT, LAYER_3DOBJECT)
{
	/* 変数の初期化 */
	// 回転量
	m_pos = D3DXVECTOR3(0.0f, 2600.0f, 0.0f);
	m_bWaypoint = false;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CWaypoint::~CWaypoint()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CWaypoint::Init(void)
{
	// メッシュドームの生成
	m_dome = CMeshdome::Create(
		m_pos,
		D3DXVECTOR3(50.0f, 500.0f, 50.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),
		20, 1,
		CMeshdome::TYPE_WAVE
	);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CWaypoint::Uninit(void)
{
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CWaypoint::Update(void)
{
	CPlayer *pPlayer = NULL;

	for (int nCntlayer = 0; nCntlayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntlayer++)
	{
		if (pPlayer = (CPlayer *)CScene::GetActor(CScene::ACTOR_PLAYER, CScene::LAYER_3DOBJECT, nCntlayer)) break;
	}

	// ゲーム遷移
	// ランキングへ
	if (CCalculation::Collision_Circle(
		m_pos,	// 現在のオブジェクトの位置
		50.0f,				// 現在のオブジェクトの半径
		pPlayer->GetPos(),	// 目的のオブジェクトの位置
		1))					// 目的のオブジェクトの半径
	{
		m_bWaypoint = true;
		m_dome->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f));
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CWaypoint::Draw(void)
{
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CWaypoint::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice =		
		CManager::GetRenderer()->GetDevice();
	// テクスチャー設定
	D3DXCreateTextureFromFile(pDevice, WAYPOINT_TEX, &m_pTex);
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CWaypoint::UnLoad(void)
{
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CWaypoint * CWaypoint::Create(
	D3DXVECTOR3 pos
)
{
	// 変数宣言
	CWaypoint * pWaypoint;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pWaypoint = new CWaypoint();
	// 位置情報
	pWaypoint->m_pos = pos;
	// 初期化処理
	pWaypoint->Init();
	// 生成したオブジェクトを返す
	return pWaypoint;
}

// ----------------------------------------
// 中間地点取得処理
// ----------------------------------------
bool CWaypoint::GetWaypoint(void)
{
	return m_bWaypoint;
}

// ----------------------------------------
// 中間地点の位置取得処理
// ----------------------------------------
D3DXVECTOR3 CWaypoint::GetPos(void)
{
	return m_pos;
}
