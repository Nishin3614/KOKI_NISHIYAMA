// ----------------------------------------
//
// モデル処理 [player.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------
#include "player.h"
#include "scene_X.h"
#include "floor.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "3Dparticle.h"
#include "gauge.h"
#include "game.h"
#include "item.h"
#include "score.h"
#include "Waypoint.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define PLAYER_KEYMOVE			(2)						// キー移動
#define PLAYER_COLLISIONSIZE	(50.0f)					// 当たり判定の大きさ

#define PLAYER_FEXTEND_TIME		(4)						// 1段階伸びる時間
#define PLAYER_SEXTEND_TIME		(8)						// 2段階伸びる時間
#define PLAYER_TEXTEND_TIME		(12)					// 3段階伸びる時間

#define PLAYER_SHRINK_TIME		(30)					// 縮む時間

#define PLAYER_SHRINK_DEFAU		(1.0f)					// 通常の伸び
#define PLAYER_FIRST_EXTEND		(5.0f)					// 1段階の伸び
#define PLAYER_SECOND_EXTEND	(10.0f)					// 2段階の伸び
#define PLAYER_THIRD_EXTEND		(15.0f)					// 3段階の伸び

#define PLAYER_FSHRINK_TIME		(PLAYER_FEXTEND_TIME + PLAYER_SHRINK_TIME)	// 1段階縮む時間
#define PLAYER_SSHRINK_TIME		(PLAYER_SEXTEND_TIME + PLAYER_SHRINK_TIME)	// 2段階縮む時間
#define PLAYER_TSHRINK_TIME		(PLAYER_TEXTEND_TIME + PLAYER_SHRINK_TIME)	// 3段階縮む時間

#define PLAYER_FJUMP_MAGNI		(1.0f)					// 1段階のジャンプ力の倍率
#define PLAYER_SJUMP_MAGNI		(2.0f)					// 2段階のジャンプ力の倍率
#define PLAYER_TJUMP_MAGNI		(3.0f)					// 3段階のジャンプ力の倍率
#define PLAYER_KEYMOVE (2)
#define PLAYER_G (0.8f)					// 重力
#define PLAYER_RESISTANCE (0.9f)		// 抵抗力
#define PLAYER_COLLISIONSIZE (50.0f)	// 当たり判定の大きさ
#define PLAYER_REST_X (1300.0f)			// xの範囲
#define PLAYER_REST_Z (200.0f)			// zの範囲
#define	PLAYER_JUMP_UP			(0.1f)	// ジャンプのステータス上昇率
#define	PLAYER_ROT_UP			(0.1f)	// 回転のステータス上昇率
#define	PLAYER_JUMP_MOVE			(0.2f)	// 回転のステータス上昇率
#define PLAYER_KEYMOVE			(2)

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------
CModel_info		*CPlayer::m_pModel_info = NULL;		// モデル情報
MODEL_OFFSET	*CPlayer::m_pModel_offset = NULL;	// モデルの初期配置
MOTION			*CPlayer::m_pMotion = NULL;			// モーション情報の保存

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CPlayer::CPlayer() : CScene::CScene(ACTOR_PLAYER, LAYER_3DOBJECT)
{
	m_pos = D3DXVECTOR3(0.0f,200.0f,0.0f);		// 位置
	m_posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前の位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 現在回転量
	m_rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向きたい方向
	m_rotbetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転の差分
	m_size = D3DXVECTOR3(							// モデルのサイズ
		PLAYER_COLLISIONSIZE,
		PLAYER_COLLISIONSIZE,
		PLAYER_COLLISIONSIZE);
	m_bJump = false;								// ジャンプ

	m_pCollision = NULL;

	m_JumpRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	// モデルの設定
	for (int nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
	{
		// モデル生成
		m_pModel[nCntModel] = CModel::Create();
		// モデル情報渡し
		m_pModel[nCntModel]->BindModel(
			m_pModel_info[nCntModel].GetModel_info(),
			m_pModel_offset[nCntModel]
		);
		// すべての親以外
		if (nCntModel != 0)
		{
			// 親情報設定
			m_pModel[nCntModel]->SetParent(m_pModel[m_pModel_info[nCntModel].GetParent()]);
		}
	}
	// ステータス読み込み
	LoadStatus();
	// 当たり判定生成
	m_pCollision = CCollision::Create();
	m_pCollision->SetPos(&m_pos);
	m_pCollision->SetSize(m_size);
	m_pCollision->SetMove(&m_move);
	m_pCollision->SetType(CCollision::OBJTYPE_PLAYER);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CPlayer::Uninit(void)
{
	// モデル
	for (int nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
	{
		// ヌルチェック
		if (m_pModel[nCntModel] != NULL)
		{
			delete m_pModel[nCntModel];
			m_pModel[nCntModel] = NULL;
		}
	}
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CPlayer::Update(void)
{
	// 変数宣言
	CScore * pScore = NULL;	// 床
							// 情報取得
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_UI); nCntLayer++)
	{
		if (pScore == NULL) pScore = (CScore*)CScene::GetActor(CScene::ACTOR_SCORE, CScene::LAYER_UI, nCntLayer);	// 床
	}

	// 移動
	Move();
	// フレームカウントアップ
	m_nFrame++;
	// モーションタイプが変化
	// 初期化
	if (m_nMotiontype != m_nMotiontypeOld)
	{
		m_nFrame = 0;		// フレームキー情報のカウント
		m_keyinfoCnt = 0;	// キー情報のカウント
	}
	// モーションの保存
	m_nMotiontypeOld = m_nMotiontype;
	// フレーム数が同じになったら
	if (m_nFrame == m_pMotion[m_nMotiontype].KeyInfo[m_keyinfoCnt].nFrame)
	{
		// 初期化
		m_nFrame = 0;	// フレーム
		// カウントアップ
		m_keyinfoCnt++;
		// キー情報が超えたら
		if (m_keyinfoCnt >= m_pMotion[m_nMotiontype].nNumKey)
		{
			// ループしないとき
			if (m_pMotion[m_nMotiontype].nLoop == 0)
			{
				// 初期化
				m_keyinfoCnt = 0;				// キー情報
				m_nMotiontype = 0;	// モーションタイプ
			}

			// ループする時
			else
			{
				// 初期化
				m_keyinfoCnt = 0;				// キー情報
			}
		}
	}
	// モデル
	for (int nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
	{
		// ヌルチェック
		if (m_pModel[nCntModel] != NULL)
		{
			m_pModel[nCntModel]->SetMotion(m_pMotion[m_nMotiontype].KeyInfo[m_keyinfoCnt].Key[nCntModel], m_pMotion[m_nMotiontype].KeyInfo[m_keyinfoCnt].nFrame);
			m_pModel[nCntModel]->Update();
		}
	}
	CDebugproc::Print("ステータス情報\n");
	CDebugproc::Print("ジャンプ力		: %.2f\n", m_Status.fJumpforce);
	CDebugproc::Print("ジャンプ力上昇率	: %.2f\n", m_Status.fRateJumpforce);
	CDebugproc::Print("回転力			: %.2f\n", m_Status.fRotforce);
	CDebugproc::Print("回転力上昇率		: %.2f\n", m_Status.fRateRotforce);

	m_pCollision->SetPos(&m_pos);

	MODEL_INFO *pModelInfo = m_pModel[0]->GetModelInfo();
	CDebugproc::Print("スケールY軸 -> [%.4f]\n", pModelInfo->scal.y);
}

// ----------------------------------------
// 移動処理
// ----------------------------------------
void CPlayer::Move(void)
{
	// 変数宣言
	float fRot;
	CFloor * pFloor = NULL;	// 床

	int nValueH = 0;//スティックの横
	int nValueV = 0;//スティックの縦

	// 情報取得
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntLayer++)
	{
		if (pFloor == NULL) pFloor = (CFloor*)CScene::GetActor(CScene::ACTOR_FLOOR, CScene::LAYER_3DOBJECT, nCntLayer);	// 床
	}
	fRot = CManager::GetRenderer()->GetCamera()->GetRot().y;	// 回転情報取得
	// ジャンプ
	if (CManager::GetKeyboard()->GetKeyboardDetach(DIK_SPACE) &&
		m_bJump == false)
	{
		CGauge *pGauge = CGame::GetGauge();

		m_bJump = true;
		m_move.x = 0;	// 移動量
		m_move.y = 0;	// 移動量
		m_move.z = 0;	// 移動量
		m_nJumpCharge = pGauge->GetGaugeValue();
		//m_move.y += 20.0f;
		//m_move.y += CPlayer::GetResultJumpforce();
		C3DParticle::Create(C3DParticle::OFFSET_ID_EXPLOSION, m_pos);
		C3DParticle::Create(C3DParticle::OFFSET_ID_EXPLOSIONWAVE, m_pos);

		m_move.x -= sinf(m_rot.z) * GetJumpforce();
		m_move.y += cosf(m_rot.z) * GetJumpforce();

		pGauge->ResetJump();
	}

	if (CManager::GetJoy()->GetRelease(0, CJoypad::KEY_A) &&
		m_bJump == false)
	{
		CGauge *pGauge = CGame::GetGauge();

		m_bJump = true;
		m_move.x = 0;	// 移動量
		m_move.y = 0;	// 移動量
		m_move.z = 0;	// 移動量
		m_nJumpCharge = pGauge->GetGaugeValue();
		//m_move.y += 20.0f;
		//m_move.y += CPlayer::GetResultJumpforce();
		C3DParticle::Create(C3DParticle::OFFSET_ID_EXPLOSION, m_pos);
		C3DParticle::Create(C3DParticle::OFFSET_ID_EXPLOSIONWAVE, m_pos);

		m_move.x -= sinf(m_rot.z) * GetJumpforce() / m_Status.fRateJumpforce;
		m_move.y += cosf(m_rot.z) * GetJumpforce();

		pGauge->ResetJump();
	}

	if (m_bJump == false)
	{//ジャンプしていない時
		//キーボードでの処理
		// 回転

		if (CManager::GetKeyboard()->GetKeyboardPress(DIK_J))
		{//左回転
			m_rot.z -= 0.05f;
		}
		else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_L))
		{//右回転
			m_rot.z += 0.05f;
		}

		//ジョイパッドでの処理
		CManager::GetJoy()->GetStickLeft(0, &nValueH, &nValueV);

		if (nValueH != 0 || nValueV != 0)
		{
			if (nValueH <= -1)
			{
				m_rot.z -= 0.05f;
			}

			else if (nValueH >= 1)
			{
				m_rot.z += 0.05f;
			}
		}

		// 回転上限
		if (m_rot.z > D3DX_PI / 3)
		{
			m_rot.z = D3DX_PI / 3;
		}
		if (m_rot.z < -D3DX_PI / 3)
		{
			m_rot.z = -D3DX_PI / 3;
		}
	}
	else
	{//ジャンプしている時
		//キーボードの処理
		// 回転
		if (CManager::GetKeyboard()->GetKeyboardPress(DIK_J))
		{//左回転
			m_rot.z -= m_Status.fRotforce * m_Status.fRateRotforce;
			m_fCntRot += m_Status.fRotforce * m_Status.fRateRotforce;
			m_move.x += PLAYER_JUMP_MOVE;
		}
		else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_L))
		{//右回転
			m_rot.z += m_Status.fRotforce * m_Status.fRateRotforce;
			m_fCntRot += m_Status.fRotforce * m_Status.fRateRotforce;
			m_move.x -= PLAYER_JUMP_MOVE;
		}

		//ジョイパッドでの処理
		CManager::GetJoy()->GetStickLeft(0, &nValueH, &nValueV);

		if (nValueH != 0 || nValueV != 0)
		{
			if (nValueH <= -1)
			{
				//m_move.x = 0;
				m_rot.z -= m_Status.fRotforce * m_Status.fRateRotforce;
				m_fCntRot += m_Status.fRotforce * m_Status.fRateRotforce;
				m_move.x += PLAYER_JUMP_MOVE;
			}

			else if (nValueH >= 1)
			{
				//m_move.x = 0;
				m_rot.z += m_Status.fRotforce * m_Status.fRateRotforce;
				m_fCntRot += m_Status.fRotforce * m_Status.fRateRotforce;
				m_move.x -= PLAYER_JUMP_MOVE;
			}
		}
		// 限度調整
		m_rot.z = CCalculation::Rot_One_Limit(m_rot.z);
	}

	// 限度調整
	m_rot.y = CCalculation::Rot_One_Limit(m_rot.y);


	if (m_bJump == false)
	{
		// 抵抗力
		m_move.x *= PLAYER_RESISTANCE;
		//m_move.y *= PLAYER_RESISTANCE;
		m_move.z *= PLAYER_RESISTANCE;
	}
	// 重力
	m_move.y -= PLAYER_G;	// 移動量

	m_pos += m_move;

	CScore * pScore = NULL;	// 床
							// 情報取得
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_UI); nCntLayer++)
	{
		if (pScore == NULL) pScore = (CScore*)CScene::GetActor(CScene::ACTOR_SCORE, CScene::LAYER_UI, nCntLayer);	// 床
	}

	// 床の高さを代入
	if (pFloor != NULL)
	{
		if (m_pos.y < pFloor->GetHeight(m_pos))
		{
			// 着地した瞬間
			if (m_bJump == true)
			{
				// スコア加算
				pScore->AddScore((int)(m_fCntRot / D3DX_PI / 2) * 100 * (int)(m_fCntRot / D3DX_PI / 2));
			}
			m_bJump = false;
			m_move.x = 0;
			m_move.y = 0;
			m_move.z = 0;
			
			m_pos.y = pFloor->GetHeight(m_pos);
			MODEL_INFO *pModelInfo = m_pModel[0]->GetModelInfo();
			pModelInfo->scal.y = 0;
			m_fCntRot = 0;
		}
	}
	// 伸縮処理
	ExpanAndContraProc();
	// 当たり判定に乗ったら
	if (m_pCollision->Collision_Come() == true)
	{
		if (m_bJump == true)
		{
			// スコア加算
			pScore->AddScore((int)(m_fCntRot / D3DX_PI / 2) * 100 * (int)(m_fCntRot / D3DX_PI / 2));
		}

		m_bJump = false;
		m_move.x = 0;
		m_move.y = 0;
		m_move.z = 0;
		MODEL_INFO *pModelInfo = m_pModel[0]->GetModelInfo();
		pModelInfo->scal.y = 0;
		m_fCntRot = 0;
	}

	// 制限区域
	Restriction();
}

// ----------------------------------------
// ジャンプ力の計算結果
// ----------------------------------------
float CPlayer::GetResultJumpforce(void)
{
	return m_Status.fJumpforce + (m_Status.fRateJumpforce * m_nNumJumpItem);
}

// ----------------------------------------
// 回転力の計算結果
// ----------------------------------------
float CPlayer::GetResultRotforce(void)
{
	return m_Status.fRotforce + (m_Status.fRateRotforce * m_nNumRotItem);
}

// ----------------------------------------
// 伸縮処理
// ----------------------------------------
void CPlayer::ExpanAndContraProc(void)
{

	// ジャンプしている時の時間
	if (m_bJump == false && m_JumpTime != 0)
	{
		m_JumpTime = 0;
		return;
	}
	if (m_bJump == false)
	{
		return;
	}

	// ジャンプ時間をインクリメント
	m_JumpTime++;

	// ヌルチェック
	if (m_pModel[0]->GetModelInfo() == NULL)
	{
		return;
	}
	MODEL_INFO *pModelInfo = m_pModel[0]->GetModelInfo();
	// 伸びる時
	if (GetExtendTime() >= m_JumpTime)
	{
		float fValue = (GetExpanAndContraMagni() - PLAYER_SHRINK_DEFAU) / (float)GetExtendTime();
		pModelInfo->scal.y += fValue;
	}
	// 縮む時
	else if (GetShrinkTime()+ GetExtendTime() >= m_JumpTime)
	{
		float fValue = (GetExpanAndContraMagni() - PLAYER_SHRINK_DEFAU) / (float)GetShrinkTime();
		pModelInfo->scal.y -= fValue;
	}
}

// ----------------------------------------
// 伸縮の倍率の取得
// ----------------------------------------
float CPlayer::GetExpanAndContraMagni(void)
{
	if (m_nJumpCharge <= GAGE_VALUE_FAST)
	{
		return PLAYER_FIRST_EXTEND;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_SECOND)
	{
		return PLAYER_SECOND_EXTEND;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_THIRD)
	{
		return PLAYER_THIRD_EXTEND;
	}
	return 0.0f;
}

// ----------------------------------------
// 伸びる時間の取得
// ----------------------------------------
int CPlayer::GetExtendTime(void)
{
	if (m_nJumpCharge <= GAGE_VALUE_FAST)
	{
		return PLAYER_FEXTEND_TIME;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_SECOND)
	{
		return PLAYER_SEXTEND_TIME;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_THIRD)
	{
		return PLAYER_TEXTEND_TIME;
	}
	return 0;
}

// ----------------------------------------
// 縮む時間の取得
// ----------------------------------------
int CPlayer::GetShrinkTime(void)
{
	if (m_nJumpCharge <= GAGE_VALUE_FAST)
	{
		return PLAYER_FSHRINK_TIME;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_SECOND)
	{
		return PLAYER_SSHRINK_TIME;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_THIRD)
	{
		return PLAYER_TSHRINK_TIME;
	}
	return 0;
}

//----------------------------------------
// ゲージ量の倍率の取得
// ----------------------------------------
float CPlayer::GetJumpMagni(void)
{
	if (m_nJumpCharge <= GAGE_VALUE_FAST)
	{
		return PLAYER_FJUMP_MAGNI;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_SECOND)
	{
		return PLAYER_SJUMP_MAGNI;
	}
	else if (m_nJumpCharge <= GAGE_VALUE_THIRD)
	{
		return PLAYER_TJUMP_MAGNI;
	}
	return 0;
}

//----------------------------------------
// ジャンプ力の取得
// ----------------------------------------
float CPlayer::GetJumpforce(void)
{
	return m_Status.fJumpforce * GetJumpMagni()*m_Status.fRateJumpforce;
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CPlayer::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld, &mtxRot);

	// 位置を反映 //
	// 平行移動行列作成(オフセット)
	D3DXMatrixTranslation(&mtxTrans,	// 総合の入れ物
		m_pos.x,
		m_pos.y,
		m_pos.z);

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3

	// モデル
	for (int nCntModel = 0; nCntModel < PLAYERTYPE_MAX; nCntModel++)
	{
		// ヌルチェック
		if (m_pModel[nCntModel] != NULL)
		{
			m_pModel[nCntModel]->Draw(m_mtxWorld);
		}
	}
}

// ----------------------------------------
// 位置取得処理
// ----------------------------------------
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}

// ----------------------------------------
// 回転取得処理
// ----------------------------------------
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_rot;
}

// ----------------------------------------
// 生成処理
// ----------------------------------------
CPlayer * CPlayer::Create()
{
	// 変数宣言
	CPlayer * pPlayer;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pPlayer = new CPlayer();
	// 初期化処理
	pPlayer->Init();
	// 生成したオブジェクトを返す
	return pPlayer;
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
void CPlayer::Load(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();
	// モデル情報の生成
	m_pModel_info = new CModel_info[PLAYERTYPE_MAX];
	// モデルのオフセットの生成
	m_pModel_offset = new MODEL_OFFSET[PLAYERTYPE_MAX];
	// モーションの生成
	m_pMotion = new MOTION[MOTIONTYPE_MAX];
	// テクスチャー情報代入
	char ModelData[PLAYERTYPE_MAX][72] =
	{
		"data/MODEL/bane/01_bane.x",			// [頭]01
		"data/MODEL/bane/00_hed.x",			// [体]00
	};
	// モデルのオフセット設定
	CModel_info::ModelOffset(
		m_pModel_offset,					// モデルのオフセット
		"data/MOTION/motion_banekun.txt"	// ファイル名
	);
	// モーションの設定
	CMotion::Load(
		m_pMotion,							// モーション
		m_pModel_offset,					// オフセット
		PLAYERTYPE_MAX,						// キー
		"data/MOTION/motion_banekun.txt"	// ファイル名
	);
	// モデルの保存
	for (int nCntModelLoad = 0; nCntModelLoad < PLAYERTYPE_MAX; nCntModelLoad++)
	{
		// モデル情報を読み込み
		m_pModel_info[nCntModelLoad].Load(
			pDevice,
			ModelData[nCntModelLoad]);
	}
}

// ----------------------------------------
// 読み込んだ情報を破棄処理
// ----------------------------------------
void CPlayer::UnLoad(void)
{
	// モデルの描画情報破棄
	for (int nCntModelLoad = 0; nCntModelLoad < PLAYERTYPE_MAX; nCntModelLoad++)
	{
		// モデル情報の開放
		m_pModel_info[nCntModelLoad].Unload();
	}
	// モーションの破棄
	CMotion::UnLoad(
		m_pMotion,					// モーション
		MOTIONTYPE_MAX
	);
	// モデル情報の破棄
	delete[] m_pModel_info;
	// モデルのオフセットの破棄
	delete[] m_pModel_offset;
}

// ----------------------------------------
// ステータスの読み込み
// ----------------------------------------
void CPlayer::LoadStatus(void)
{
	// 変数宣言
	FILE *pFile;		// ファイルのポインタ
	char cRead[128];	// 読み込み用
	char cComp[128];	// 比較用
	char cEmpty[128];	// 要らないもの用
	int nCntError = 0;	// エラー表示カウント

	// 開けたかどうか
	if ((pFile = fopen("DATA/TEXT/PlayerStatus.txt", "r")) != NULL)
	{

		while (strcmp(cComp, "SCRIPT") != 0)
		{
			// 1行読み込む
			fgets(cRead, sizeof(cRead), pFile);
			// 読み込んど文字列代入
			sscanf(cRead, "%s", &cComp);

			// エラーカウントをインクリメント
			nCntError++;
			if (nCntError > 1048576)
			{// エラー
				nCntError = 0;
				MessageBox(NULL, "[SCRIP]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
			}
		}

		if (strcmp(cComp, "SCRIPT") == 0)
		{
			// END_SCRIPTまでループ
			while (strcmp(cComp, "END_SCRIPT") != 0)
			{
				// 1行読み込む
				fgets(cRead, sizeof(cRead), pFile);
				// 読み込んど文字列代入
				sscanf(cRead, "%s", &cComp);

				// エラーカウントをインクリメント
				nCntError++;
				if (nCntError > 1048576)
				{// エラー
					nCntError = 0;
					MessageBox(NULL, "[END_SCRIPT]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
				}
				if (strcmp(cComp, "SETSTATUS") == 0)
				{
					do
					{// ループ開始
					 // 1行読み込む
						fgets(cRead, sizeof(cRead), pFile);
						// 読み込んど文字列代入
						sscanf(cRead, "%s", &cComp);
						// エラーカウントをインクリメント
						nCntError++;
						if (nCntError > 1048576)
						{// エラー
							nCntError = 0;
						}
						if (strcmp(cComp, "JUMPFORCE") == 0)
						{
							sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty, &m_Status.fJumpforce);
							cComp[0] = '\0';
						}
						else if (strcmp(cComp, "ROTFORCE") == 0)
						{
							sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty, &m_Status.fRotforce);
							cComp[0] = '\0';
						}
						else if (strcmp(cComp, "RATE_JUMPFORCE") == 0)
						{
							sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty, &m_Status.fRateJumpforce);
							cComp[0] = '\0';
						}
						else if (strcmp(cComp, "RATE_ROTFORCE") == 0)
						{
							sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty, &m_Status.fRateRotforce);
							cComp[0] = '\0';
						}
					} while ((strcmp(cComp, "END_SETSTATUS") != 0) && (strcmp(cComp, "}") != 0));

				}
			}
		}
	}
	else
	{
		MessageBox(NULL, "プレイヤー情報ファイルを開けませんでした", "警告！", MB_ICONWARNING);
	}
	fclose(pFile);
}

// ----------------------------------------
// ステータスの保存
// ----------------------------------------
void CPlayer::SaveStatus(void)
{
	FILE *pFile;		// ファイルのポインタ

	if ((pFile = fopen("DATA/TEXT/PlayerStatus.txt", "w")) != NULL)
	{
		fprintf(pFile, "//---------------------------------------------\n");
		fprintf(pFile, "// ステータスのの情報\n");
		fprintf(pFile, "//---------------------------------------------\n");
		fprintf(pFile, "SCRIPT\n\n");

		fprintf(pFile, "SETSTATUS\n");
		fprintf(pFile, "{\n");
		fprintf(pFile, "	JUMPFORCE = %.2f\n", m_Status.fJumpforce);
		fprintf(pFile, "	ROTFORCE = %.2f\n", m_Status.fRotforce);
		fprintf(pFile, "	RATE_JUMPFORCE = %.2f\n", m_Status.fRateJumpforce);
		fprintf(pFile, "	RATE_ROTFORCE = %.2f\n", m_Status.fRateRotforce);
		fprintf(pFile, "}\n\n");

		fprintf(pFile, "END_SCRIPT\n\n");
	}
	else
	{
		MessageBox(NULL, "プレイヤー情報ファイルを保存できませんでたwww", "警告！", MB_ICONWARNING);
	}

	fclose(pFile);

}

// ----------------------------------------
// ステータスの設定
// ----------------------------------------
void CPlayer::SetStatus(STATUS_INFO Status)
{
	m_Status = Status;
}

// ----------------------------------------
// ジャンプ上昇
// ----------------------------------------
void CPlayer::AddJump(void)
{
	// 変数宣言
	CItem * pItem = NULL;	// アイテム
	int nMaxItem = 0;		// 最大アイテム
							// 情報取得
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntLayer++)
	{
		if (pItem == NULL) pItem = (CItem*)CScene::GetActor(CScene::ACTOR_ITEM, CScene::LAYER_3DOBJECT, nCntLayer);	// 床
	}
	// 最大数取得
	nMaxItem = pItem->GetLoad()->GetJump();
	// ジャンプのアイテムアップ
	m_nNumJumpItem++;
	// ジャンプ率アップ
	m_Status.fRateJumpforce += (PLAYER_JUMP_UP / nMaxItem);
}

// ----------------------------------------
// 回転上昇
// ----------------------------------------
void CPlayer::AddRot(void)
{
	// 変数宣言
	CItem * pItem = NULL;	// アイテム
	int nMaxItem = 0;		// 最大アイテム
	// 情報取得
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntLayer++)
	{
		if (pItem == NULL) pItem = (CItem*)CScene::GetActor(CScene::ACTOR_ITEM, CScene::LAYER_3DOBJECT, nCntLayer);	// 床
	}
	// 最大数取得
	nMaxItem = pItem->GetLoad()->GetRot();
	// 回転率アップ
	m_nNumRotItem++;
	// 回転率アップ
	m_Status.fRateRotforce += (PLAYER_ROT_UP / nMaxItem);
}

// ----------------------------------------
// ステータスの設定
// ----------------------------------------
void CPlayer::Restriction(void)
{
	// Xの制限
	if (m_pos.x > PLAYER_REST_X)
	{
		m_pos.x = PLAYER_REST_X;
	}
	else if(m_pos.x < -PLAYER_REST_X)
	{
		m_pos.x = -PLAYER_REST_X;
	}
	// Zの制限
	if (m_pos.z > PLAYER_REST_Z)
	{
		m_pos.z = PLAYER_REST_Z;
	}
	else if (m_pos.z < -PLAYER_REST_Z)
	{
		m_pos.z = -PLAYER_REST_Z;
	}
}

// ----------------------------------------
//
// ----------------------------------------
bool CPlayer::GetbJump(void)
{
	return m_bJump;
}