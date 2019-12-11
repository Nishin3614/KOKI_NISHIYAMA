// ----------------------------------------
//
// ゴール処理の説明[goal.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "goal.h"
#include "player.h"
#include "meshdome.h"
#include "fade.h"
#include "debugproc.h"
#include "score.h"
#include "topscore.h"
#include "time.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define GOAL_ROTMOVE (0.01f)				// 角度の移動量
#define GOAL_SIZE (100.0f)					// サイズ
#define GOAL_TEX "data/TEXTURE/Line.png"	// テクスチャー
#define GOAL_ANGLE (D3DX_PI / 1)			// 湾曲の角度

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
LPDIRECT3DTEXTURE9 CGoal::m_pTex = NULL;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CGoal::CGoal() : CScene(ACTOR_GOAL, LAYER_3DOBJECT)
{
	/* 変数の初期化 */
	// 回転量
	m_pos = D3DXVECTOR3(-100.0f, 3900.0f, 0.0f);
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CGoal::~CGoal()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CGoal::Init(void)
{
	// メッシュドームの生成
	CMeshdome::Create(
		m_pos,
		D3DXVECTOR3(50.0f, 1000.0f, 50.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(0.0f, 0.5f, 1.0f, 0.5f),
		20, 1,
		CMeshdome::TYPE_WAVE
	);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CGoal::Uninit(void)
{
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CGoal::Update(void)
{
	CPlayer *pPlayer = NULL;
	CTopscore * pTopscore = NULL;
	CScore	* pScore = NULL;
	CTime * pTime = NULL;

	for (int nCntlayer = 0; nCntlayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntlayer++)
	{
		if (pPlayer = (CPlayer *)CScene::GetActor(CScene::ACTOR_PLAYER, CScene::LAYER_3DOBJECT, nCntlayer)) break;
	}
	for (int nCntlayer = 0; nCntlayer < CScene::GetMaxLayer(CScene::LAYER_UI); nCntlayer++)
	{
		if (pTopscore == NULL) pTopscore = (CTopscore *)CScene::GetActor(CScene::ACTOR_TOPSCORE, CScene::LAYER_UI, nCntlayer);
		if (pScore == NULL) pScore = (CScore *)CScene::GetActor(CScene::ACTOR_SCORE, CScene::LAYER_UI, nCntlayer);
		if (pTime == NULL) pTime = (CTime *)CScene::GetActor(CScene::ACTOR_TIME, CScene::LAYER_UI, nCntlayer);
	}


	// ゲーム遷移
	// ランキングへ
	if (CCalculation::Collision_Circle(
		m_pos,	// 現在のオブジェクトの位置
		50.0f,				// 現在のオブジェクトの半径
		pPlayer->GetPos(),	// 目的のオブジェクトの位置
		20.0f))					// 目的のオブジェクトの半径
	{
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			pTopscore->SetScore(pScore->GetScore() + 20000);
			CManager::GetFade()->SetFade(CManager::MODE_RANKING);
		}
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CGoal::Draw(void)
{
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CGoal::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice =		
		CManager::GetRenderer()->GetDevice();
	// テクスチャー設定
	D3DXCreateTextureFromFile(pDevice, GOAL_TEX, &m_pTex);
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CGoal::UnLoad(void)
{
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CGoal * CGoal::Create(
	D3DXVECTOR3 pos
)
{
	// 変数宣言
	CGoal * pGoal;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pGoal = new CGoal();
	// 位置情報
	pGoal->m_pos = pos;
	// 初期化処理
	pGoal->Init();
	// 生成したオブジェクトを返す
	return pGoal;
}