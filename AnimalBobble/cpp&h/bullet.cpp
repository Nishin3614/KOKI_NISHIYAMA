// ------------------------------------------------------------------------------------------
//
// 弾処理の説明[bullet.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------
#include "bullet.h"
#include "effect.h"
#include "explosion.h"
#include "score.h"
#include "player.h"
#include "frame.h"
#include "array.h"
#include "box.h"
#include "fade.h"
#include "game.h"
#include "ceiling.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------
#define BULLETSIZE (50.0f)
#define BULLETSIX (D3DX_PI * 2 / CBullet::DIRECTION_MAX)
#define BULLETSIX_ONE (BULLETSIX / 2)
#define BULLET_CONNECT (3)
#define BULLET_MOVE (15)
#define BULLET_CNTMOVE (50)

// ------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CBullet::m_pTex[CBullet::TYPE_MAX] = {};
int CBullet::m_nCntJudg = 0;
int CBullet::m_nCntIdMax = 0;
//----- 一人用 -----//
char CBullet::m_cFileName[ONEPLAYER_STAGE][128] =		// ファイル名
{
	"data/LOAD/Single/stage1.txt",
	"data/LOAD/Single/stage2.txt",
	"data/LOAD/Single/stage3.txt",
	"data/LOAD/Single/stage4.txt",
	"data/LOAD/Single/stage5.txt"
};
int CBullet::m_nLineLoad[ONEPLAYER_STAGE][MAX_ARRAY_LINE * MAX_ARRAY_COLUMN] = {};
int CBullet::m_nColumnLoad[ONEPLAYER_STAGE][MAX_ARRAY_LINE * MAX_ARRAY_COLUMN] = {};
int CBullet::m_nType[ONEPLAYER_STAGE][MAX_ARRAY_LINE * MAX_ARRAY_COLUMN] = {};
int CBullet::m_nAllLoad[ONEPLAYER_STAGE] = {};
int CBullet::m_nDownCntLoad[ONEPLAYER_STAGE] = { 10,10,10,10,10 };
int CBullet::m_nDownCnt[CManager::PLAYER_MAX] = {};

// ------------------------------------------------------------------------------------------
// コンストラクタ処理
// ------------------------------------------------------------------------------------------
CBullet::CBullet() : CScene_TWO::CScene_TWO(ACTOR_BULLET, LAYER_BULLET)
{
	//----- 変数の初期化 */
	// 位置
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 目的位置
	m_posgoal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 移動量
	m_move = D3DXVECTOR3(BULLET_MOVE, BULLET_MOVE, 0.0f);
	// タイプ
	m_type = TYPE_BEE;
	// 状態
	m_state = STATE_BOX;
	// 回転量
	m_fRot = D3DX_PI;
	// 体力
	m_nLife = 50;
	// 行番号
	m_nLine = -1;
	// 列番号
	m_nColumn = 9;
	// 移動カウント
	m_nCntJudg = 0;
	// つながっている判定
	m_bJudg = false;
	// 落ちてる判定
	m_bDown = false;
	// 移動しているかどうか
	m_bMove = false;
	// プレイヤー番号
	m_PlayerId = CManager::PLAYER_1;
}

// ------------------------------------------------------------------------------------------
// デストラクタ処理
// ------------------------------------------------------------------------------------------
CBullet::~CBullet()
{
}

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void CBullet::Init(void)
{
	// サイズ設定
	CScene_TWO::SetSize(D3DXVECTOR2(BULLETSIZE, BULLETSIZE));
	// 初期化
	CScene_TWO::Init();
	// 現在の位置設定
	CScene_TWO::SetPosition(m_pos);
	// テクスチャー設定
	CScene_TWO::BindTexture(m_pTex[m_type]);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void CBullet::Uninit(void)
{
	if (m_state == STATE_OUT || m_state == STATE_END)
	{
		m_nCntIdMax--;
	}
	CScene_TWO::Uninit();
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void CBullet::Update(void)
{
	// 状態によって
	if (m_state == STATE_OUT)			// 放出後
	{
		Goout();
	}
	else if (m_state == STATE_PLAYER)	// プレイヤー待機中
	{
		Player();
	}
	else if (m_state == STATE_BOX)		// ボックス待機中
	{
		Box();
	}
	else if (m_state == STATE_END)		// 落ちたアニメーション
	{
		DownAnim();
	}
	// 現在の位置設定
	CScene_TWO::SetPosition(m_pos);
	// 更新
	CScene_TWO::Update();
}

// ------------------------------------------------------------------------------------------
// ボックス待機中
// ------------------------------------------------------------------------------------------
void CBullet::Box(void)
{
}

// ------------------------------------------------------------------------------------------
// プレイヤー待機中
// ------------------------------------------------------------------------------------------
void CBullet::Player(void)
{
	// 移動カウントが20未満の場合
	if (m_nCntMove < BULLET_CNTMOVE)
	{
		m_pos.x += (m_posgoal.x - m_pos.x) / BULLET_CNTMOVE;	// 目標地点に近づく
		m_nCntMove++;								// 移動カウントアップ
	}
	// 移動カウントが20の場合
	else if (m_nCntMove == BULLET_CNTMOVE)
	{
		// 変数宣言
		CPlayer * pPlayer = NULL;	// プレイヤー
		// 情報取得
		for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_OBJ); nCntLayer++)
		{
			if (pPlayer == NULL)	pPlayer = (CPlayer*)CScene::GetActor(ACTOR_PLAYER, LAYER_OBJ, nCntLayer);	// プレイヤー
		}
		pPlayer->ShotState(false);																				// 弾打っているかの状態
		m_pos = m_posgoal;																						// 目標地点に代入
	}
}

// ------------------------------------------------------------------------------------------
// すでに打った時の場合
// ------------------------------------------------------------------------------------------
void CBullet::Goout(void)
{
	// 移動
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	//----- 当たり判定 -----//
	Collision();
	// 削除処理
	Delete();
	// ゲームオーバー
	Dead();
	// 一段落ちる処理
	DownOne();
}

// ------------------------------------------------------------------------------------------
// 落ちるアニメーション
// ------------------------------------------------------------------------------------------
void CBullet::DownAnim(void)
{
	// 変数宣言
	CArray * pArray = NULL;	// 配列
	// 移動量
	m_move.y += float(rand() % 10) / 10;
	// 位置更新
	m_pos.y += m_move.y;
	// 情報取得
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_UI); nCntLayer++)
	{
		if (pArray == NULL)	pArray = (CArray*)CScene::GetActor(ACTOR_ARRAY, LAYER_UI, nCntLayer);	// 配列
	}

	if (m_pos.y > 700.0f)
	{
		pArray->DeleteState(m_nLine, m_nColumn);
		// リリース
		Release();
		return;
	}
}

// ------------------------------------------------------------------------------------------
// 当たり判定
// ------------------------------------------------------------------------------------------
void CBullet::Collision(void)
{
	// 変数宣言
	CBullet * pBullet = NULL;	// 弾
	CFrame * pFrame = NULL;		// フレーム
	CArray * pArray = NULL;		// 配列
	CPlayer * pPlayer = NULL;	// プレイヤー
	CCeiling * pCeiling = NULL;	// 天井

	// 情報取得
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_OBJ); nCntLayer++)
	{
		if(pFrame == NULL) pFrame = (CFrame*)CScene::GetActor(ACTOR_FRAME, LAYER_OBJ, nCntLayer);			// フレーム
		if (pArray == NULL)	pArray = (CArray*)CScene::GetActor(ACTOR_ARRAY, LAYER_UI, nCntLayer);			// 配列
		if (pPlayer == NULL)	pPlayer = (CPlayer*)CScene::GetActor(ACTOR_PLAYER, LAYER_OBJ, nCntLayer);	// プレイヤー
		if (pCeiling == NULL) pCeiling = (CCeiling*)CScene::GetActor(ACTOR_CEILING, LAYER_OBJ, nCntLayer);	// 天井
	}
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_UI); nCntLayer++)
	{
		if (pArray == NULL)	pArray = (CArray*)CScene::GetActor(ACTOR_ARRAY, LAYER_UI, nCntLayer);			// 配列
	}

	// 移動状態であれば
	if (m_bMove)
	{
		//----- 泡同士の当たり判定の処理 -----//
		for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
		{
			// 情報取得
			pBullet = (CBullet*)CScene::GetActor(ACTOR_BULLET, LAYER_BULLET, nCnt);	// 弾
																					// 条件判定
			if (BulletJudg(this, pBullet, m_PlayerId, STATE_OUT, false) == true)
			{
				// 円の当たり判定
				if (CCalculation::Collision_Circle(
					m_pos + D3DXVECTOR3(sinf(m_fRot) * GetLengh(), cosf(m_fRot) * GetLengh(), 0.0f),	// 現在のオブジェクトの位置
					0,																					// 現在のオブジェクトの半径
					pBullet->m_pos,																		// 目的のオブジェクトの位置
					GetLengh()))																		// 目的のオブジェクトの半径
				{
					// ヌルチェック
					if (pArray != NULL)
					{
						// 近い位置情報を返す
						m_pos = pArray->AroundPosCorrection(
							m_pos,					// 位置情報
							pBullet->m_nLine,		// 当たった弾の行情報
							pBullet->m_nColumn,		// 当たった弾の列情報
							&m_nLine,				// 当たりに行った弾の行情報
							&m_nColumn);			// 当たりに行った弾の列情報
					}
					// 当たった後の処理
					Collision_After();
					// 関数を抜ける
					return;
				}

			}
		}
		//----- 天井と泡の当たり判定 -----//
		// ヌルチェック
		if (pCeiling != NULL &&
			pCeiling->GetPlayerId() == m_PlayerId &&
			pArray != NULL)
		{
			// yの限界値を超えたとき
			if (m_pos.y - CScene_TWO::GetSize().y / 2 <
				pCeiling->GetPos().y + pCeiling->GetSize().y)
			{
				// 近い配列の位置情報を返す
				m_pos = pArray->TopPosCorrection(m_pos, 0, &m_nColumn);
				// 行は初期のまま
				m_nLine = 0;
				// 当たった後の処理
				Collision_After();
			}
		}
		// ヌルチェック
		if (pFrame != NULL &&
			pFrame->GetPlayerId() == m_PlayerId)
		{
			// xの限界値を超えたとき
			if (m_pos.x - CScene_TWO::GetSize().x / 2 <=
				pFrame->GetPos().x - pFrame->GetSize().x / 2 ||
				m_pos.x + CScene_TWO::GetSize().x / 2 >=
				pFrame->GetPos().x + pFrame->GetSize().x / 2)
			{
				m_move.x *= -1;
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 当たった後の処理
// ------------------------------------------------------------------------------------------
void CBullet::Collision_After(void)
{
	// 移動量を初期化する
	m_move.x = 0;
	m_move.y = 0;
	// 判定をオンにする
	m_bJudg = true;
	// 移動状態をオフにする
	m_bMove = false;
	// プレイヤーに位置を移動
	BoxToPlayer();
	// カウントアップ
	m_nCntJudg++;
	//----- 消える処理 -----//
	ConectDelete(this);
	// 落ちる設定処理
	m_nDownCnt[m_PlayerId]++;
}

// ------------------------------------------------------------------------------------------
// 連鎖削除処理
// ------------------------------------------------------------------------------------------
void CBullet::ConectDelete(CBullet * bulletCom)	// 比較するもの
{
	// 変数宣言
	int Direction;		// 現在の方向
	CBullet * pBullet;	// 弾

	// 方向指定
	Direction = DIRECTION_NORTHWEST;	// 北の方向

	while (1)
	{
		//----- 隣り合ったオブジェクトがあるかないか -----//
		for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
		{
			// 情報取得
			pBullet = (CBullet*)CScene::GetActor(CScene::ACTOR_BULLET, CScene::LAYER_BULLET, nCnt);	// 泡
			// 条件判定
			if (BulletJudg(bulletCom, pBullet, bulletCom->m_PlayerId, STATE_OUT, false) &&					// 泡の条件
				pBullet->m_type == bulletCom->m_type &&												// タイプ
				Existence(pBullet, bulletCom, (DIRECTION)Direction))								// 存在しているかどうか
			{
				// 消える判定
				pBullet->m_bJudg = true;
				// カウントアップ
				m_nCntJudg++;
				//----- 連鎖削除処理 -----//
				ConectDelete(pBullet);
			}
		}
		// 方向インクリメント
		Direction++;
		// 方向全部確認したらループを抜ける
		if (Direction == DIRECTION_MAX)
		{
			return;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 連鎖落ち処理
// ------------------------------------------------------------------------------------------
void CBullet::ConectDown(CBullet * bulletCom)
{
	// 変数宣言
	int Direction;		// 現在の方向
	CBullet * pBullet;

	// 方向指定
	Direction = DIRECTION_NORTHWEST;	// 北の方向

	while (1)
	{
		// 隣り合ったオブジェクトがあるかないか
		for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
		{
			pBullet = (CBullet*)CScene::GetActor(CScene::ACTOR_BULLET, CScene::LAYER_BULLET, nCnt);	// 情報取得
																									// 条件判定
			if (BulletJudg(bulletCom, pBullet, bulletCom->m_PlayerId, STATE_OUT, false) &&					// 泡の条件
				Existence(pBullet, bulletCom, (DIRECTION)Direction))								// 存在しているかどうか
			{
				// 消える判定
				pBullet->m_bJudg = true;
				//----- 連鎖削除処理 -----//
				ConectDown(pBullet);
			}
		}
		// 方向インクリメント
		Direction++;

		// 方向全部確認したらループを抜ける
		if (Direction == DIRECTION_MAX)
		{
			return;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 削除処理
// ------------------------------------------------------------------------------------------
void CBullet::Delete(void)
{
	// 変数宣言
	CBullet * pBullet = NULL;	// 情報
	CScore * pScore = NULL;	// スコア
	CArray * pArray = NULL;	// 配列
	CManager::PLAYER plyerid = m_PlayerId;	// プレイヤーのID
	bool bTopDelete = false;				// 上に配置している泡が消えているかどうか

	// 情報取得
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_OBJ); nCntLayer++)
	{
		if (pScore == NULL)	pScore = (CScore*)CScene::GetActor(ACTOR_SCORE, LAYER_OBJ, nCntLayer);	// スコア
	}
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_UI); nCntLayer++)
	{
		if (pArray == NULL)	pArray = (CArray*)CScene::GetActor(ACTOR_ARRAY, LAYER_UI, nCntLayer);	// 配列
	}
	//----- 連鎖削除 -----//
	for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
	{
		// 情報取得
		pBullet = (CBullet*)GetActor(ACTOR_BULLET, LAYER_BULLET, nCnt);	// 泡
		if (BulletJudg(nullptr, pBullet, plyerid, STATE_OUT, true))	// 泡の条件
		{
			if (m_nCntJudg >= BULLET_CONNECT)
			{
				// エフェクト
				for (int nCntEffect = 0; nCntEffect < 10; nCntEffect++)
				{
					// 変数宣言
					CEffect *pEffect;	// effect
					D3DXCOLOR Nowcol;	// 出現時の色
					D3DXCOLOR Goalcol;	// 消える時の色
					float frot =		// 角度
						rand() % 314 * 0.01f - rand() % 314 * 0.01f;	

					switch (m_type)
					{
						// ハチ
					case CBullet::TYPE_BEE:
						Nowcol = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
						Goalcol = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
						break;
						// 猫
					case CBullet::TYPE_CAT:
						Nowcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						Goalcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
						break;
						// 魚
					case CBullet::TYPE_FISH:
						Nowcol = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
						Goalcol = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.0f);
						break;
						// 鷹
					case CBullet::TYPE_HAWK:
						Nowcol = D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f);
						Goalcol = D3DXCOLOR(1.0f, 0.5f, 0.2f, 0.0f);
						break;
						// 蛇
					case CBullet::TYPE_SNAKE:
						Nowcol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						Goalcol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f);
						break;
					default:
						break;
					}
					pEffect = CEffect::Create(
						pBullet->GetPos(),	// 位置
						Nowcol,				// カラー
						Goalcol,			// 目標のカラー
						frot,				// 初期角度
						float(rand() % 5),	// 速さ
						30.0f,				// サイズ
						1.0f,				// 目標サイズ
						40					// ライフ(分割数)
					);
					pEffect->Fountain(frot, 50);
				}
				
				// 上に配置している
				bTopDelete = true;
				pBullet->m_state = STATE_END;
				pArray->DeleteState(pBullet->m_nLine, pBullet->m_nColumn);
				// リリース
				pBullet->Release();
				// スコア設定
				pScore->SetScore(100);
			}
			else
			{
				pBullet->m_bJudg = false;
			}
		}
	}
	//----- 連鎖落ち -----//
	if (bTopDelete)
	{
		//----- 連鎖落ち選択 -----//
		for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
		{
			// 情報取得
			pBullet = (CBullet*)GetActor(ACTOR_BULLET, LAYER_BULLET, nCnt);	// 泡
			// 条件判定
			if (BulletJudg(this, pBullet, plyerid, STATE_OUT, false) &&	// 条件判定
				pBullet->m_nLine == 0)	// 行0の時
			{
				// つながっている状態
				pBullet->m_bJudg = true;
				// 連鎖落ち処理
				ConectDown(pBullet);
			}
		}
		//----- 連鎖落ち -----//
		for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
		{
			// 情報取得
			pBullet = (CBullet*)GetActor(ACTOR_BULLET, LAYER_BULLET, nCnt);	// 泡
																			// 条件判定
			if (BulletJudg(nullptr, pBullet, plyerid, STATE_OUT, false))	// 泡の条件
			{
				// 状態
				pBullet->m_state = STATE_END;
				// スコア設定
				pScore->SetScore(100);
				// カウントアップ
				m_nCntJudg++;
			}
		}
	}
	//----- 合計ポイント -----//
	if (m_nCntJudg >= BULLET_CONNECT)
	{
		// 投げる時の音
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_DELETE);
		pScore->SetScore(500 * m_nCntJudg);
	}
	// つながった個数を初期化
	m_nCntJudg = 0;
}

// ------------------------------------------------------------------------------------------
// 落ちる処理
// ------------------------------------------------------------------------------------------
void CBullet::DownOne(void)
{
	// 変数宣言
	CBullet * pBullet = NULL;	// 泡
	CArray * pArray = NULL;		// 配列
	CCeiling * pCeiling = NULL;	// プレイヤー
	// 情報取得
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_OBJ); nCntLayer++)
	{
		if (pCeiling == NULL) pCeiling = (CCeiling*)CScene::GetActor(ACTOR_CEILING, LAYER_OBJ, nCntLayer);	// 天井
	}
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_UI); nCntLayer++)
	{
		if (pArray == NULL)	pArray = (CArray*)CScene::GetActor(ACTOR_ARRAY, LAYER_UI, nCntLayer);	// 配列
	}

	// 放出した泡とき
	if (m_state == STATE_OUT)
	{
		// 揺らす演出
		if (m_nDownCnt[m_PlayerId] == m_nDownCntLoad[CManager::GetGame()->GetStage()] - 1 &&	// 落ちる前
			!m_bMove)																			// 移動していない泡
		{
			float frot = rand() % 314 * 0.01f - rand() % 314 * 0.01f;
			m_pos.x = pArray->GetPos(m_nLine, m_nColumn).x + sinf(frot) * 2.0f;
			m_pos.y = pArray->GetPos(m_nLine, m_nColumn).y + cosf(frot) * 2.0f;
		}
		// 落ちる
		if (m_nDownCnt[m_PlayerId] == m_nDownCntLoad[CManager::GetGame()->GetStage()])			// 落ちる時
		{
			// 投げる時の音
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_DOWN);
			// 初期位置を一つ下に下げる
			pArray->OriginPosChange();
			// 天井を下げる
			pCeiling->SetPosSize(
				pCeiling->GetPos() +
				D3DXVECTOR3(0.0f, BULLETSIZE / 2, 0.0f),
				pCeiling->GetSize() + D3DXVECTOR2(0.0, BULLETSIZE / 2));

			//----- 再配置 -----//
			for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
			{
				// 情報取得
				pBullet = (CBullet*)GetActor(ACTOR_BULLET, LAYER_BULLET, nCnt);	// 泡
																				// 条件判定
				if (BulletJudg(nullptr, pBullet, m_PlayerId, STATE_OUT, false))
				{
					// 再配置
					pBullet->m_pos = pArray->TargetPosCorrection(pBullet->m_nLine, pBullet->m_nColumn);
				}
			}
			// カウントを初期化
			m_nDownCnt[m_PlayerId] = 0;
		}
	}
}

// ------------------------------------------------------------------------------------------
// ゲームオーバー処理
// ------------------------------------------------------------------------------------------
void CBullet::Dead(void)
{
	// 変数宣言
	CArray * pArray = NULL;	// 配列

	// 情報取得
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_OBJ); nCntLayer++)
	{
		if (pArray == NULL)	pArray = (CArray*)CScene::GetActor(ACTOR_ARRAY, LAYER_UI, nCntLayer);	// 配列
	}

	// 上限に達したら
	if (m_nLine == MAX_ARRAY_LINE - 1 - pArray->GetDownCnt())
	{
		if (m_state != STATE_END)
		{
			// ゲームモード
			if (CManager::GetMode() == CManager::MODE_GAME)
			{
				// ゲームオーバージャない場合
				if (CManager::GetGame()->GetState() != CGame::STATE_GAMEOVER)
				{
					CManager::GetGame()->SetState(CGame::STATE_GAMEOVER);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 箱からプレイヤーへ処理
// ------------------------------------------------------------------------------------------
void CBullet::BoxToPlayer(void)
{
	// 変数宣言
	CBox *pBox = NULL;			// 箱
	CPlayer * pPlayer = NULL;	// プレイヤー
	// 情報取得
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_OBJ); nCntLayer++)
	{
		if (pBox == NULL) pBox = (CBox*)CScene::GetActor(ACTOR_BOX, LAYER_OBJ, nCntLayer);				// 箱
		if (pPlayer == NULL) pPlayer = (CPlayer*)CScene::GetActor(ACTOR_PLAYER, LAYER_OBJ, nCntLayer);	// プレイヤー
	}

	//----- 箱からプレイヤーへの移動 -----//
	for (int nCnt = 0; nCnt < GetMaxLayer(LAYER_BULLET); nCnt++)
	{
		// 情報取得
		CBullet *pBullet = (CBullet *)CScene::GetActor(CScene::ACTOR_BULLET, CScene::LAYER_BULLET, nCnt);	// 弾
		// ヌルチェック
		if (pBullet != NULL)
		{
			// 箱からプレイヤーへ
			if (pBullet->GetPlayerId() == m_PlayerId &&
				pBullet->GetState() == CBullet::STATE_BOX)
			{
				pBullet->SetPosGoal(pPlayer->GetPos());				// 位置設定
				pBullet->SetState(CBullet::STATE_PLAYER);	// 状態設定
			}
		}
	}
	// 弾生成
	CBullet::Create(m_PlayerId, pBox->GetPos(), (CBullet::TYPE)(rand() % (int)CBullet::TYPE_MAX));

	//----- 箱のアニメーションパターン -----//
	// 垂直のアニメーションパターン設定
	pBox->SetVirticalAnim(1);
	// 水平のアニメーションパターン設定
	pBox->SetHorizonAnim(1);
}

// ------------------------------------------------------------------------------------------
// 存在するかしないか処理
// ------------------------------------------------------------------------------------------
bool CBullet::Existence(CBullet * bulletOri, CBullet * bulletCom, DIRECTION direction)
{
	// 変数宣言
	bool bExist = false;	// 存在しているかいないか

	//----- 方向に応じて -----//
	switch (direction)
	{
		// 北西
	case DIRECTION_NORTHWEST:
		// 列が偶数列
		if (bulletCom->m_nLine % 2 == 0)
		{
			// 配列番号が一緒だったら
			if (bulletOri->m_nLine == bulletCom->m_nLine - 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn - 1)
			{
				// 存在している
				bExist = true;
			}
		}
		// 列が奇数列
		else
		{
			// 配列番号が一緒だったら
			if (bulletOri->m_nLine == bulletCom->m_nLine - 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn)
			{
				// 存在している
				bExist = true;
			}
		}
		break;
		// 北東
	case DIRECTION_NORTHEAST:
		// 列が偶数列
		if (bulletCom->m_nLine % 2 == 0)
		{
			// 配列番号が一緒だったら
			if (bulletOri->m_nLine == bulletCom->m_nLine - 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn)
			{
				// 存在している
				bExist = true;
			}
		}
		// 列が奇数列
		else
		{
			// 配列番号が一緒だったら
			if (bulletOri->m_nLine == bulletCom->m_nLine - 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn + 1)
			{
				// 存在している
				bExist = true;
			}
		}
		break;
		// 東
	case DIRECTION_EAST:
		// 配列番号が一緒だったら
		if (bulletOri->m_nLine == bulletCom->m_nLine &&
			bulletOri->m_nColumn == bulletCom->m_nColumn + 1)
		{
			// 存在している
			bExist = true;
		}

		break;
		// 南東
	case DIRECTION_SOUTHEAST:
		// 列が偶数列
		if (bulletCom->m_nLine % 2 == 0)
		{
			// 配列番号が一緒だったら
			if (bulletOri->m_nLine == bulletCom->m_nLine + 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn)
			{
				// 存在している
				bExist = true;
			}
		}
		// 列が奇数列
		else
		{
			// 配列番号が一緒だったら
			if (bulletOri->m_nLine == bulletCom->m_nLine + 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn + 1)
			{
				// 存在している
				bExist = true;
			}
		}
		break;
		// 南西
	case DIRECTION_SOUTHWEST:
		// 列が偶数列
		if (bulletCom->m_nLine % 2 == 0)
		{
			// 配列番号が一緒だったら
			if (bulletOri->m_nLine == bulletCom->m_nLine + 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn - 1)
			{
				// 存在している
				bExist = true;
			}
		}
		// 列が奇数列
		else
		{
			// 配列番号が一緒だったら
			if (bulletOri->m_nLine == bulletCom->m_nLine + 1 &&
				bulletOri->m_nColumn == bulletCom->m_nColumn)
			{
				// 存在している
				bExist = true;
			}
		}
		break;
		// 西
	case DIRECTION_WEST:
		// 配列番号が一緒だったら
		if (bulletOri->m_nLine == bulletCom->m_nLine &&
			bulletOri->m_nColumn == bulletCom->m_nColumn - 1)
		{
			// 存在している
			bExist = true;
		}
		break;

	default:
		break;
	}
	return bExist;
}

// ------------------------------------------------------------------------------------------
// 条件判定処理
// ------------------------------------------------------------------------------------------
bool CBullet::BulletJudg(
	CBullet * pBulletOrigin,	// 弾の比較元
	CBullet * pBulletComp,		// 弾の比較先
	CManager::PLAYER plyerid,	// プレイヤー番号
	CBullet::STATE state,		// 弾の状態
	bool bJubg					// 判定
)
{
	// 条件
	if (pBulletComp != NULL &&					// ヌルチェック
		pBulletComp != pBulletOrigin &&			// プレイヤー番号
		pBulletComp->m_PlayerId == plyerid &&	// プレイヤー番号
		pBulletComp->m_state == state &&		// 状態
		pBulletComp->m_bJudg == bJubg)			// つながっていない状態の時
	{
		return true;
	}
	return false;
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void CBullet::Draw(void)
{
	CScene_TWO::Draw();
}

// ------------------------------------------------------------------------------------------
// 位置情報設定処理
// ------------------------------------------------------------------------------------------
void CBullet::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
	m_posgoal = pos;
}

// ------------------------------------------------------------------------------------------
// 目標地点の設定処理
// ------------------------------------------------------------------------------------------
void CBullet::SetPosGoal(D3DXVECTOR3 posgoal)
{
	m_posgoal = posgoal;
	m_posdif = (m_posgoal - m_pos) / 2 + m_pos;
}

// ------------------------------------------------------------------------------------------
// 状態設定処理
// ------------------------------------------------------------------------------------------
void CBullet::SetRot(float frot)
{
	// 移動量の設定
	m_move.x *= sinf(frot);
	m_move.y *= cosf(frot);
}

// ------------------------------------------------------------------------------------------
// 状態設定処理
// ------------------------------------------------------------------------------------------
void CBullet::SetState(STATE state)
{
	// 状態設定
	m_state = state;
	// 移動状態に
	m_bMove = true;
	// 弾のステータスが出力した時
	if (m_state == STATE_OUT)
	{
		// 総数加算
		m_nCntIdMax++;
	}
}

// ------------------------------------------------------------------------------------------
// プレイヤー番号取得処理
// ------------------------------------------------------------------------------------------
CManager::PLAYER CBullet::GetPlayerId(void)
{
	return m_PlayerId;
}

// ------------------------------------------------------------------------------------------
// 状態取得処理
// ------------------------------------------------------------------------------------------
CBullet::STATE CBullet::GetState(void)
{
	return m_state;
}

// ------------------------------------------------------------------------------------------
// 位置情報取得処理
// ------------------------------------------------------------------------------------------
D3DXVECTOR3 CBullet::GetPos(void)
{
	return m_pos;
}

// ------------------------------------------------------------------------------------------
// 読み込み処理
// ------------------------------------------------------------------------------------------
HRESULT CBullet::Load(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =					// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	char cTex[CBullet::TYPE_MAX][128] =			// テクスチャー名
	{
		"data/TEXTURE/Bullet/Bee.png",
		"data/TEXTURE/Bullet/Cat.png",
		"data/TEXTURE/Bullet/Fish.png",
		"data/TEXTURE/Bullet/Hawk.png",
		"data/TEXTURE/Bullet/Snake.png",
	};

	// テクスチャー設定
	for (int nCnt = 0; nCnt < CBullet::TYPE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice, cTex[nCnt], &m_pTex[nCnt]);
	}

	// ステージ読み込み
	LoadStage();

	return S_OK;
}

// ------------------------------------------------------------------------------------------
// 読み込んだ情報を破棄
// ------------------------------------------------------------------------------------------
void CBullet::UnLoad(void)
{
	for (int nCnt = 0; nCnt < CBullet::TYPE_MAX; nCnt++)
	{
		// テクスチャの開放
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 弾の総数処理
// ------------------------------------------------------------------------------------------
int CBullet::GetAllBullet(void)
{
	return m_nCntIdMax;
}

// ------------------------------------------------------------------------------------------
// 作成処理
// ------------------------------------------------------------------------------------------
CBullet * CBullet::Create(CManager::PLAYER PlayerId, D3DXVECTOR3 pos, TYPE type)
{
	// 変数宣言
	CBullet * pBullet;		// 弾2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pBullet = new CBullet();
	// 現在の位置
	pBullet->m_pos = pos;
	pBullet->m_posgoal = pos;
	// タイプの設定
	pBullet->m_type = type;
	// プレイヤー番号
	pBullet->m_PlayerId = PlayerId;
	// 初期化処理
	pBullet->Init();
	// 生成したオブジェクトを返す
	return pBullet;
}

// ------------------------------------------------------------------------------------------
// 読み込み作成処理
// ------------------------------------------------------------------------------------------
CBullet * CBullet::LoadCreate(CManager::PLAYER PlayerId, int nMode, int nStage)
{
	// 変数宣言
	CBullet * pBullet = NULL;	// 弾2Dクラス
	CArray * pArray = NULL;		// 列クラス
	CFrame * pFrame = NULL;		// フレームクラス
	CCeiling * pCeiling = NULL;	// 天井クラス

	// 情報取得
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_OBJ); nCntLayer++)
	{
		if (pFrame == NULL) pFrame = (CFrame*)CScene::GetActor(ACTOR_FRAME, LAYER_OBJ, nCntLayer);			// フレーム
		if (pCeiling == NULL) pCeiling = (CCeiling*)CScene::GetActor(ACTOR_CEILING, LAYER_OBJ, nCntLayer);	// 天井
	}
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_UI); nCntLayer++)
	{
		if (pArray == NULL)	pArray = (CArray*)CScene::GetActor(ACTOR_ARRAY, LAYER_UI, nCntLayer);	// 配列
	}

	// 生成されたときの初期値に変更
	pArray->BiginPos();
	// ヌルチェック
	if (pCeiling != NULL)
	{
		pCeiling->SetPosSize(D3DXVECTOR3(pFrame->GetPos().x, pFrame->GetPos().y - pFrame->GetSize().y / 2, 0.0f), D3DXVECTOR2(pFrame->GetSize().x / 2, 1.0f));
	}
	// 
	m_nDownCnt[PlayerId] = 0;
	m_nCntIdMax = 0;

	for (int nCntBullet = 0; nCntBullet < m_nAllLoad[nStage]; nCntBullet++)
	{
		// メモリの生成(初め->基本クラス,後->派生クラス)
		pBullet = new CBullet();
		// タイプの設定
		pBullet->m_type = (CBullet::TYPE)m_nType[nStage][nCntBullet];
		// プレイヤー番号
		pBullet->m_PlayerId = PlayerId;
		// 行設定
		pBullet->m_nLine = m_nLineLoad[nStage][nCntBullet];
		// 列設定
		pBullet->m_nColumn = m_nColumnLoad[nStage][nCntBullet];
		// 位置設定
		pBullet->m_pos = pArray->GetPos(pBullet->m_nLine, pBullet->m_nColumn);
		// 位置設定
		pBullet->m_posgoal = pBullet->m_pos;
		// 移動量設定
		pBullet->m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 状態設定
		pBullet->m_state = STATE_OUT;
		// 初期化処理
		pBullet->Init();
		// 近い位置情報を返す
		pArray->SetState(pBullet->m_nLine, pBullet->m_nColumn, true);
		// 総数加算
		m_nCntIdMax++;
	}
	// 生成したオブジェクトを返す
	return NULL;
}

// ------------------------------------------------------------------------------------------
// ステージ読み込み
// ------------------------------------------------------------------------------------------
void CBullet::LoadStage(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad = 0;		// カウントロード
	int nCntBullet = 0;		// カウント弾
	char cReadText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

							// ステージカウント
	for (int nCntStage = 0; nCntStage < ONEPLAYER_STAGE; nCntStage++)
	{
		m_nAllLoad[nCntStage] = 0;

		// ファイル開
		pFile = fopen(m_cFileName[nCntStage], "r");

		// 開けた
		if (pFile != NULL)
		{
			// スクリプトが来るまでループ
			while (strcmp(cHeadText, "SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);	// 一文を読み込む
				sscanf(cReadText, "%s", &cHeadText);		// 比較用テクストに文字を代入
			}

			// スクリプトだったら
			if (strcmp(cHeadText, "SCRIPT") == 0)
			{
				// エンドスクリプトが来るまでループ
				while (strcmp(cHeadText, "END_SCRIPT") != 0)
				{
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);

					// セットが来たら
					if (strcmp(cHeadText, "BULLETSET") == 0)
					{
						// エンドセットが来るまでループ
						while (strcmp(cHeadText, "END_BULLETSET") != 0)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);

							// 種類情報読み込み
							if (strcmp(cHeadText, "TYPE") == 0)
							{
								sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_nType[nCntStage][m_nAllLoad[nCntStage]]);
							}

							// 行情報読み込み
							else if (strcmp(cHeadText, "LINE") == 0)
							{
								sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_nLineLoad[nCntStage][m_nAllLoad[nCntStage]]);
							}

							// 列情報読み込み
							else if (strcmp(cHeadText, "COLUMN") == 0)
							{
								sscanf(cReadText, "%s %s %d",
									&cDie, &cDie,
									&m_nColumnLoad[nCntStage][m_nAllLoad[nCntStage]]);
							}

						}

						// 総合数カウントアップ
						m_nAllLoad[nCntStage]++;
					}
				}
			}
			// ファイル閉
			fclose(pFile);
		}

		// 開けない
		else
		{
		}
	}
}
