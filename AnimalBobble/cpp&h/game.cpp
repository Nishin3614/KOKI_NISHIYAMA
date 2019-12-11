// ------------------------------------------------------------------------------------------
//
// ゲーム処理 [game.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "game.h"
/* 描画 */
#include "effect.h"
#include "tlybg.h"
#include "frame.h"
#include "ceiling.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "number.h"
#include "box.h"
#include "array.h"
#include "fade.h"
#include "game_ui.h"
#include "play_element_ui.h"
#include "topscore.h"

/* ポーズ */
#include "pause.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------------------------------------------------------
CGame::STATE CGame::m_state = CGame::STATE_NORMAL;	// ゲーム状態
int CGame::m_nStage = 0;							// ステージ

// ------------------------------------------------------------------------------------------
// コンストラクタ
// ------------------------------------------------------------------------------------------
CGame::CGame()
{
	m_state = CGame::STATE_NORMAL;
	m_pause = NULL;
}

// ------------------------------------------------------------------------------------------
// デストラクタ
// ------------------------------------------------------------------------------------------
CGame::~CGame()
{
}

// ------------------------------------------------------------------------------------------
// 初期化
// ------------------------------------------------------------------------------------------
void CGame::Init(void)
{
	// 変数宣言
	CFrame * pFrame;								// フレーム
	D3DXVECTOR3 FramePos;							// フレーム位置
	D3DXVECTOR2 FrameSize;							// フレームサイズ
	CManager::PLAYER playerID = CManager::PLAYER_1;	// プレイヤー番号

	/* 初期化 */
	// 状態
	m_state = STATE_NORMAL;
	// ステージ
	m_nStage = 0;

	/* 作成 */
	// 背景
	CTlyBg::Create(CManager::MODE_GAME);
	// 枠
	pFrame = CFrame::Create(playerID, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50.0f, 0.0f));
	// 情報取得
	FramePos = pFrame->GetPos();	// 位置
	FrameSize = pFrame->GetSize();	// サイズ
	// 天井
	CCeiling::Create(
		playerID,
		D3DXVECTOR3(FramePos.x, FramePos.y - FrameSize.y / 2, 0.0f),
		D3DXVECTOR2(FrameSize.x / 2, 1.0f));
	// スコア
	CScore::Create(
		playerID,
		D3DXVECTOR3(FramePos.x,FramePos.y - FrameSize.y / 2 -10.0f,0.0f));
	// 箱
	CBox::Create(playerID, D3DXVECTOR3(FramePos.x - 200.0f, FramePos.y + FrameSize.y / 2 - 50.0f, 0.0f));
	// プレイヤー
	CPlayer::Create(playerID,D3DXVECTOR3(FramePos.x, FramePos.y + FrameSize.y / 2 - 50.0f,0.0f));
	// 配列
	CArray::Create(playerID,FramePos,FrameSize);
	// 弾の読み込み
	CBullet::LoadStage();
	// 弾の生成
	CBullet::LoadCreate(playerID, 0, m_nStage);
	// プレイヤー用UI
	CPlay_element_ui::Create(playerID);
	// ゲーム用UI
	CGame_ui::Create();
	// ポーズの生成
	m_pause = new CPause();
	// ポーズの初期化
	m_pause->Init();
}

// ------------------------------------------------------------------------------------------
// 終了
// ------------------------------------------------------------------------------------------
void CGame::Uninit(void)
{
	// ポーズ
	if (m_pause != NULL)
	{
		CPause::SetPause(false);
		m_pause->Uninit();
		delete m_pause;
		m_pause = NULL;
	}

	CScene::ReleaseAll();
}

// ------------------------------------------------------------------------------------------
// 更新
// ------------------------------------------------------------------------------------------
void CGame::Update(void)
{
	// 変数宣言
	CGame_ui * pGame_ui = NULL;
	CScore * pScore = NULL;
	// 情報取得
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_OBJ); nCntLayer++)
	{
		if (pGame_ui == NULL) pGame_ui = (CGame_ui*)CScene::GetActor(CScene::ACTOR_GAME_UI, CScene::LAYER_UI, nCntLayer);	// ゲームui
		if (pScore == NULL) pScore = (CScore*)CScene::GetActor(CScene::ACTOR_SCORE, CScene::LAYER_OBJ, nCntLayer);			// スコア
	}

	/* ポーズ用 */
	// ゲーム状態の変更
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_P))
	{
		// ゲーム状態をポーズに
		if (m_state != STATE_PAUSE)
		{
			m_state = STATE_PAUSE;
			CPause::SetPause(true);
		}
		// ゲーム状態をノーマルに
		else
		{
			m_state = STATE_NORMAL;
			CPause::SetPause(false);
		}
	}
	/* クリア */
	// 総数がなくなったら
	if (CBullet::GetAllBullet() <= 0)
	{
		// クリア状態じゃないとき
		if (m_state != STATE_CLEAR)
		{
			// クリア時の音
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_CLEAR);
			m_state = STATE_CLEAR;
		}
	}
	// クリア状態ならば
	if (m_state == STATE_CLEAR)
	{
		// エンター押したら
		if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
		{
			// ステージアップ
			m_nStage++;
			pGame_ui->GetNumber()->SetNum(m_nStage + 1);

			// ネクストステージ
			if (m_nStage < ONEPLAYER_STAGE)
			{
				// 弾の生成
				CBullet::LoadCreate(CManager::PLAYER_1, 0, m_nStage);
				// ノーマル状態へ
				m_state = STATE_START;
			}
			// 全部クリア
			else
			{
				// フェイド中じゃない場合
				if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
				{
					// ノーマル状態へ
					m_state = STATE_NORMAL;
					m_nStage = 0;
					// MYスコアを設定
					CTopscore::SetScore(pScore->GetScore());
					CManager::GetFade()->SetFade(CManager::MODE_RANKING);	// ランキング
				}
			}
		}
	}
	/* スタート */
	else if (m_state == STATE_START)
	{
		// エンター押したら
		if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
		{
			// ノーマル状態へ
			m_state = STATE_NORMAL;
		}
	}
	// ゲームオーバー
	else if (m_state == STATE_GAMEOVER)
	{
		// エンター押したら
		if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
		{
			// フェイド中じゃない場合
			if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				// ノーマル状態へ
				m_state = STATE_NORMAL;
				m_nStage = 0;
				// MYスコアを設定
				CTopscore::SetScore(pScore->GetScore());
				// エンター音
				CManager::GetSound()->PlaySound(CSound::LABEL_SE_ENTER);
				CManager::GetFade()->SetFade(CManager::MODE_RANKING);	// ランキング
			}
		}
	}
	// ポーズ状態ならば
	else if (m_state == STATE_PAUSE)
	{
		if (m_pause != NULL)
		{
			m_pause->Update();
		}
	}
}

// ------------------------------------------------------------------------------------------
// 描画
// ------------------------------------------------------------------------------------------
void CGame::Draw(void)
{
	if (m_state == STATE_PAUSE)
	{
		if (m_pause != NULL)
		{
			m_pause->Draw();
		}
	}
}

// ------------------------------------------------------------------------------------------
// 状態取得
// ------------------------------------------------------------------------------------------
CGame::STATE CGame::GetState(void)
{
	return m_state;
}

// ------------------------------------------------------------------------------------------
// ステージ取得
// ------------------------------------------------------------------------------------------
int CGame::GetStage(void)
{
	return m_nStage;
}

// ------------------------------------------------------------------------------------------
// 状態設定
// ------------------------------------------------------------------------------------------
void CGame::SetState(STATE state)
{
	m_state = state;
}
