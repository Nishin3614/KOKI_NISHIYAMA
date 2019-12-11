// ----------------------------------------
//
// シーン処理の説明[player.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "player.h"
#include "bullet.h"
#include "box.h"
#include "effect.h"
#include "game.h"
#include "debugproc.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define PLAYER_ROTMOVE (0.01f)
#define PLAYER_SIZE (100.0f)
#define PLAYER_TEX "data/TEXTURE/Player/BobChanAnimation.png"
#define PLAYER_ANIMCOUNT (4)
#define PLAYER_HORIZONANIM (1.0f/PLAYER_ANIMCOUNT)
#define PLAYER_VERTICALANIM (1.0f/PLAYER_ANIMCOUNT)
#define PLAYER_SHOTCOUNT (10)									// プレイヤーの発射カウント

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
LPDIRECT3DTEXTURE9 CPlayer::m_pTex = NULL;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CPlayer::CPlayer() : CScene_TWO::CScene_TWO(ACTOR_PLAYER,LAYER_OBJ)
{
	/* 変数の初期化 */
	// 回転量
	m_rot = 0.0f;
	// 弾状態
	m_bShot = false;
	// 水平のアニメーションパターン
	m_nHorizonAnim = 0;
	// 垂直のアニメーションパターン
	m_nVirticalAnim = 0;
	// アニメーションカウント
	m_nCntAnim = 0;
	// 発射カウント
	m_nShotCnt = PLAYER_SHOTCOUNT;
	// 発射タイム
	m_nShotTime = 0;
	// プレイヤー番号
	m_PlayerId = CManager::PLAYER_1;		
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CPlayer::~CPlayer()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CPlayer::Init(void)
{
	// 変数宣言
	CBullet *pBullet = NULL;	// 弾
	CBox *pBox = NULL;	// 箱
	// サイズ設定
	CScene_TWO::SetSize(D3DXVECTOR2(PLAYER_SIZE, PLAYER_SIZE));
	// 初期化
	CScene_TWO::Init();
	// 位置設定
	CScene_TWO::SetPosition(m_pos);
	// テクスチャー設定
	CScene_TWO::BindTexture(m_pTex);
	// 頂点カラー設定
	CScene_TWO::SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	// 表示テクスチャー設定
	CScene_TWO::SetTex(
		D3DXVECTOR2(0.0f, 0.0f), 
		D3DXVECTOR2(0.25f, 0.25f));
	// 弾の生成
	pBullet = CBullet::Create(m_PlayerId,m_pos, (CBullet::TYPE)(rand() % (int)CBullet::TYPE_MAX));
	// プレイヤーの位置へ
	pBullet->SetPos(m_pos);																// 位置設定
	pBullet->SetState(CBullet::STATE_PLAYER);											// 状態設定
	// 情報取得
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_OBJ); nCntLayer++)
	{
		if (pBox == NULL)	pBox = (CBox*)CScene::GetActor(ACTOR_BOX, LAYER_OBJ, nCntLayer);	// 箱
	}

	// 弾の生成
	pBullet = CBullet::Create(m_PlayerId,pBox->GetPos(), (CBullet::TYPE)(rand() % (int)CBullet::TYPE_MAX));
	// 予測線の生成
	m_pprediction = CPrediction::Create(m_PlayerId, m_pos, CPrediction::TYPE_BEE);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CPlayer::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CPlayer::Update(void)
{
	// 変数宣言
	D3DXVECTOR3 pos;
	/* コントローラー用変数 */
	int nValueH = 0;		// 水平方向
	int nValueV = 0;		// 垂直方向
	CKeyboard * pKeyboard;	// キーボード情報
	CJoypad * pJoypad;		// ジョイパッド情報
							
	// 情報取得
	pKeyboard = CManager::GetKeyboard();	// キーボード
	pJoypad = CManager::GetJoy();		   	// ジョイパッド
	// 現在の位置情報取得
	pos = CScene_TWO::GetPosition();

	/*
	pos = D3DXVECTOR3(
		(float)CManager::GetMouse()->GetX(),
		(float)CManager::GetMouse()->GetY(),
		0.0f);
	*/

	// コントローラーの左スティックの情報取得
	pJoypad->GetStickLeft(0, &nValueH, &nValueV);
	/*
	// 左スティックを動かしているかどうか
	if (nValueH != 0 || nValueV != 0)
	{
		// 角度
		m_rot = -atan2f((float)nValueH, (float)nValueV);
	}
	*/
	if (CManager::GetGame()->GetState() == CGame::STATE_NORMAL)
	{
		// プレイヤー番号１
		if (m_PlayerId == CManager::PLAYER_1)
		{
			/* 回転 */
			// 左
			if (pKeyboard->GetKeyboardPress(DIK_A))
			{
				m_rot += PLAYER_ROTMOVE;
			}
			// 右
			else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_D))
			{
				m_rot -= PLAYER_ROTMOVE;
			}

			/* 回転量制限 */
			if (m_rot >= D3DX_PI / 2 - 0.1f)
			{
				m_rot = -D3DX_PI / 2 + 0.2f;
			}
			else if (m_rot <= -D3DX_PI / 2 + 0.1f)
			{
				m_rot = D3DX_PI / 2 - 0.2f;
			}


			/* 弾発射 */
			// 弾状態がOFFの場合
			if (!m_bShot)
			{
				if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) ||
					pJoypad->GetTrigger(0, pJoypad->KEY_B))
				{
					// 投げる時の音
					CManager::GetSound()->PlaySound(CSound::LABEL_SE_THROW);

					for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
					{
						// 情報取得
						CBullet *pBullet = (CBullet *)CScene::GetActor(CScene::ACTOR_BULLET, CScene::LAYER_BULLET, nCnt);	// 弾
						// ヌルチェック
						if (pBullet != NULL)
						{
							if (pBullet->GetPlayerId() == m_PlayerId)
							{
								// プレイヤーからの放出
								if (pBullet->GetState() == CBullet::STATE_PLAYER)
								{
									pBullet->SetState(CBullet::STATE_OUT);		// 状態設定
									pBullet->SetRot(D3DX_PI + m_rot);			// 回転情報設定
								}
							}
						}
					}
					// 弾状態をONにする
					m_bShot = true;
					// 垂直のアニメーションパターン
					m_nVirticalAnim = 2;
					// 発射カウント初期化
					m_nShotCnt = PLAYER_SHOTCOUNT;

				}
				else if (m_nShotCnt <= 0)
				{
					for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
					{
						// 情報取得
						CBullet *pBullet = (CBullet *)CScene::GetActor(CScene::ACTOR_BULLET, CScene::LAYER_BULLET, nCnt);	// 弾
																															// ヌルチェック
						if (pBullet != NULL)
						{
							if (pBullet->GetPlayerId() == m_PlayerId)
							{
								// プレイヤーからの放出
								if (pBullet->GetState() == CBullet::STATE_PLAYER)
								{
									pBullet->SetState(CBullet::STATE_OUT);		// 状態設定
									pBullet->SetRot(D3DX_PI + m_rot);			// 回転情報設定
								}
							}
						}
					}
					// 弾状態をONにする
					m_bShot = true;
					// 垂直のアニメーションパターン
					m_nVirticalAnim = 2;
					// 発射カウント初期化
					m_nShotCnt = PLAYER_SHOTCOUNT;
				}
			}
		}
		// プレイヤー番号１
		if (m_PlayerId == CManager::PLAYER_2)
		{
			/* 回転 */
			// 左
			if (pKeyboard->GetKeyboardPress(DIK_LEFT))
			{
				m_rot += PLAYER_ROTMOVE;
			}
			// 右
			else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_RIGHT))
			{
				m_rot -= PLAYER_ROTMOVE;
			}

			/* 回転量制限 */
			if (m_rot >= D3DX_PI / 2 - 0.1f)
			{
				m_rot = -D3DX_PI / 2 + 0.2f;
			}
			else if (m_rot <= -D3DX_PI / 2 + 0.1f)
			{
				m_rot = D3DX_PI / 2 - 0.2f;
			}

			/* 弾発射 */
			// 弾状態がOFFの場合
			if (!m_bShot)
			{
				if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) ||
					pJoypad->GetTrigger(0, pJoypad->KEY_B))
				{
					// 投げる時の音
					CManager::GetSound()->PlaySound(CSound::LABEL_SE_THROW);
					for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
					{
						// 情報取得
						CBullet *pBullet = (CBullet *)CScene::GetActor(CScene::ACTOR_BULLET, CScene::LAYER_BULLET, nCnt);	// 弾
																															// ヌルチェック
						if (pBullet != NULL)
						{
							if (pBullet->GetPlayerId() == m_PlayerId)
							{
								// プレイヤーからの放出
								if (pBullet->GetState() == CBullet::STATE_PLAYER)
								{
									pBullet->SetState(CBullet::STATE_OUT);		// 状態設定
									pBullet->SetRot(D3DX_PI + m_rot);			// 回転情報設定
								}
							}
						}
					}
					// 弾状態をONにする
					m_bShot = true;
					// 垂直のアニメーションパターン
					m_nVirticalAnim = 2;
					// 発射カウント初期化
					m_nShotCnt = PLAYER_SHOTCOUNT;
				}
				else if (m_nShotCnt <= 0)
				{
					for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
					{
						// 情報取得
						CBullet *pBullet = (CBullet *)CScene::GetActor(CScene::ACTOR_BULLET, CScene::LAYER_BULLET, nCnt);	// 弾
																															// ヌルチェック
						if (pBullet != NULL)
						{
							if (pBullet->GetPlayerId() == m_PlayerId)
							{
								// プレイヤーからの放出
								if (pBullet->GetState() == CBullet::STATE_PLAYER)
								{
									pBullet->SetState(CBullet::STATE_OUT);		// 状態設定
									pBullet->SetRot(D3DX_PI + m_rot);			// 回転情報設定
								}
							}
						}
					}
					// 弾状態をONにする
					m_bShot = true;
					// 垂直のアニメーションパターン
					m_nVirticalAnim = 2;
					// 発射カウント初期化
					m_nShotCnt = PLAYER_SHOTCOUNT;
				}
			}
		}

		// 発射タイムアップ
		m_nShotTime++;
		// 発射カウントダウン
		if (m_nShotTime % 60 == 0)
		{
			m_nShotCnt--;
		}
	}
	// 予測線の回転設定
	m_pprediction->SetRot(m_rot);
	// 位置設定
	CScene_TWO::SetPosition(pos);
	// 回転設定
	CScene_TWO::SetRot(m_rot);
	// アニメーション変化
	Animation();
	// 更新
	CScene_TWO::Update();
}

// ----------------------------------------
// アニメーション処理
// ----------------------------------------
void CPlayer::Animation(void)
{
	// アニメーションカウントアップ
	m_nCntAnim++;
	// 20フレームずつアニメーションする
	if (m_nCntAnim % 20 == 0)
	{
		// アニメーション変化
		m_nHorizonAnim = (m_nHorizonAnim + 1) % PLAYER_ANIMCOUNT;
	}
	// 頂点テクスチャー設定
	CScene_TWO::SetTex(
		D3DXVECTOR2(m_nHorizonAnim * PLAYER_HORIZONANIM, m_nVirticalAnim * PLAYER_VERTICALANIM),
		D3DXVECTOR2(m_nHorizonAnim * PLAYER_HORIZONANIM + PLAYER_HORIZONANIM, m_nVirticalAnim * PLAYER_VERTICALANIM + PLAYER_VERTICALANIM));
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CPlayer::Draw(void)
{
	CScene_TWO::Draw();
}

// ----------------------------------------
// 弾の状態処理
// ----------------------------------------
void CPlayer::ShotState(bool bShot)
{
	// 状態
	m_bShot = bShot;
	// 垂直のアニメーションパターン
	m_nVirticalAnim = 1;
}

// ----------------------------------------
// プレイヤー番号取得処理
// ----------------------------------------
CManager::PLAYER CPlayer::GetPlayerId(void)
{
	return m_PlayerId;
}

// ----------------------------------------
// 位置情報取得処理
// ----------------------------------------
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return GetPosition();
}

// ----------------------------------------
// 弾の発射カウント処理
// ----------------------------------------
int CPlayer::GetShotCnt(void)
{
	return m_nShotCnt;
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CPlayer::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice =		
		CManager::GetRenderer()->GetDevice();
	// テクスチャー設定
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEX, &m_pTex);
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CPlayer::UnLoad(void)
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
CPlayer * CPlayer::Create(CManager::PLAYER playerId, D3DXVECTOR3 pos)
{
	// 変数宣言
	CPlayer * pPlayer;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pPlayer = new CPlayer();
	// 位置情報
	pPlayer->m_pos = pos;
	// プレイヤー番号
	pPlayer->m_PlayerId = playerId;
	// 初期化処理
	pPlayer->Init();
	// 生成したオブジェクトを返す
	return pPlayer;
}