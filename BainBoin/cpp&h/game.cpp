// ------------------------------------------
//
// ゲーム処理 [game.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#include "game.h"
/* 描画 */
#include "tlybg.h"
#include "score.h"
#include "number.h"
#include "fade.h"
#include "item.h"
#include "floor.h"
#include "player.h"
#include "item.h"
#include "play_element_ui.h"
#include "scene_X.h"
#include "3Deffect.h"
#include "meshsphere.h"
#include "block.h"
#include "goal.h"
#include "time.h"
#include "score.h"
#include "player.h"
#include "gauge.h"
#include "Waypoint.h"

/* 当たり判定 */
#include "collision.h"

/* ポーズ */
#include "pause.h"

/* イクミン追加 */
#include "GameUI.h"


// ------------------------------------------
//
// マクロ定義
//
// ------------------------------------------

// ------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------
CGame::STATE CGame::m_state = CGame::STATE_NORMAL;
int CGame::m_nStage = 0;	// ステージ
CPause *m_pPause[CPause::PAUSETYPE_MAX] = {};	// ポーズの状態
CPlayer *CGame::m_pPlayer = NULL;
CGauge *CGame::m_pGauge = NULL;
CTime *CGame::m_pTime = NULL;

// ------------------------------------------
// コンストラクタ
// ------------------------------------------
CGame::CGame()
{
	m_state = CGame::STATE_NORMAL;
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CGame::~CGame()
{
}

// ------------------------------------------
// 初期化
// ------------------------------------------
void CGame::Init(void)
{
	// 変数宣言

	/* 初期化 */
	// 状態
	m_state = STATE_NORMAL;
	// ステージ
	m_nStage = 0;
	CManager::PLAYER playerID = CManager::PLAYER_1;
	// フロアー
	CFloor::Create(
		D3DXVECTOR3(0.0f,50.0f,0.0f),
		D3DXVECTOR3(1000.0f, 0.0f, 1000.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		10, 10);
	// プレイヤー
	m_pPlayer = CPlayer::Create();
	// ブロックの生成
	CBlock::LoadCreate();
	// アイテムの生成
	CItem::LoadCreate();
	// effect
	C3DEffect::Create();
	// メッシュスフィア
	CMeshsphere::create();
	CMeshsphere::Set(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10000.0f);
	CMeshsphere::SetRadius(0, 10000.0f);
	// ゴール地点
	CGoal::Create(
		D3DXVECTOR3(-100.0f, 3900.0f, 0.0f)
	);
	/*
	// 中間地点地点
	CWaypoint::Create(
		D3DXVECTOR3(0.0f, 2050.0f, 0.0f)
	);
	*/
	// タイム
	m_pTime = CTime::Create();
	// スコア
	CScore::Create();
	/* イクミン追加 */
	//CGameUI::Load();
	//CGameUI::Create();
	// 当たり判定
	CCollision::LoadCreate();
	m_pGauge = CGauge::Create();

}

// ------------------------------------------
// 終了
// ------------------------------------------
void CGame::Uninit(void)
{
	/* イクミン追加 */
	CGameUI::Unload();
	// シーン情報の削除
	CScene::ReleaseAll();
	// 当たり判定の削除
	CCollision::DeleteAll();
}

// ------------------------------------------
// 更新
// ------------------------------------------
void CGame::Update(void)
{
	// 画面遷移の状態が遷移していない状態だったら
	if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		if (CGame::GetTime()->GetTimeOverFlag())
		{
			CManager::GetFade()->SetFade(CManager::MODE_RANKING);
		}
	}

	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_P) || CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_START))
	{
		for (int nCntPause = 0; nCntPause < 4; nCntPause++)
		{
			if (m_pPause[nCntPause] != NULL)
			{

				m_pPause[nCntPause]->Release();
				m_pPause[nCntPause] = NULL;
				CPause::SetPause(false);
			}
			else if (m_pPause[nCntPause] == NULL)
			{
				m_pPause[nCntPause] = m_pPause[nCntPause]->Create((CPause::PAUSETYPE)nCntPause);
				CPause::SetPause(true);
			}
		}
	}

	// 当たり判定
	CCollision::UpdateAll();
}

// ------------------------------------------
// 描画
// ------------------------------------------
void CGame::Draw(void)
{
}

// ------------------------------------------
// 状態取得
// ------------------------------------------
CGame::STATE CGame::GetState(void)
{
	return m_state;
}

// ------------------------------------------
// ステージ取得
// ------------------------------------------
int CGame::GetStage(void)
{
	return m_nStage;
}

// ------------------------------------------
// 状態設定
// ------------------------------------------
void CGame::SetState(STATE state)
{
	m_state = state;
}

CPlayer * CGame::GetPlayer(void)
{
	return m_pPlayer;
}

CGauge * CGame::GetGauge(void)
{
	return m_pGauge;
}

CTime * CGame::GetTime(void)
{
	return m_pTime;
}