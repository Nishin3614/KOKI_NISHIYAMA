// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// モデル処理 [player.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "player.h"
#include "floor.h"
#include "input.h"
#include "camera.h"
#include "collider.h"

#include "trajectory.h"
#include "3Deffect.h"
#include "3Dparticle.h"
#include "meshsphere.h"
#include "bgdome.h"
#include "collision.h"


#include "2Dgauge.h"
#include "stategauge.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PLAYER_KEYMOVE (5)
#define PLAYER_G (0.5f)			// 重力
#define PLAYER_RESISTANCE (0.5f)// 抵抗力
#define PLAYER_MOTIONFILE "data/LOAD/PLAYER/Tricker.txt"	// モーションのファイル名

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::CPlayer() : CCharacter::CCharacter(ACTOR_PLAYER)
{
	m_posold = D3DVECTOR3_ZERO;		// 前の位置
	m_nConbo = 0;					// コンボ
	m_bTrick = false;				// 秘奥義中かどうか
	p2DHPGauge = NULL;				// HPゲージ
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Init(void)
{
	CCharacter::SetCharacter(CHARACTER_PLAYER);
	CCharacter::Init();
	// カメラの初期化
	CManager::GetRenderer()->GetCamera()->InfoInit(
		CCharacter::GetPos() +
		D3DXVECTOR3(0.0f, 100.0f, 0.0f),	// カメラ位置
		CCharacter::GetRot());				// カメラ回転
	
	D3DXVECTOR3 pos;
	pos = D3DXVECTOR3(50.0f, 660.0f, 0.0f);

	// HPゲージの生成
	p2DHPGauge = C2DGauge::Create(
		pos + D3DXVECTOR3(28.5f, 30.0f, 0.0f),
		D3DXVECTOR2(500.0f, 25.0f),
		D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)
	);
	// 秘奥義ゲージの変化定数を設定
	p2DHPGauge->SetConstance((float)CCharacter::GetStatus().nMaxLife);

	// MPゲージの生成
	p2DTrickGauge = C2DGauge::Create(
		pos + D3DXVECTOR3(40.0f, 10.0f, 0.0f),
		D3DXVECTOR2(180.0f, 15.0f),
		D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f)
	);
	// 秘奥義ゲージの変化定数を設定
	p2DTrickGauge->SetConstance((float)CCharacter::GetStatus().nMaxMP);
	// ステートゲージの生成
	CStateGauge::Create(D3DXVECTOR3(50.0f, 660.0f, 0.0f),
		D3DXVECTOR2(100.0f, 100.0f)
	);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
	CCharacter::Uninit();
	// HPゲージをヌルに
	if (p2DHPGauge != NULL)
	{
		p2DHPGauge = NULL;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Update(void)
{
	// ニュートラルモーションへ
	CCharacter::SetMotion(MOTIONTYPE_NEUTRAL);
	// カメラの注視点設定
	CManager::GetRenderer()->GetCamera()->SetPosR(
		CCharacter::GetPos(),
		CCharacter::GetRot()
	);
	// 攻撃
	Attack();
	// モーション状態
	MotionState();

	if (CCharacter::GetAttack() == false)
	{
		// 移動
		Move();
	}
	else
	{
		// 停止
		Stop();
	}
	if (m_bDamage)
	{
		// HPゲージの変化定数を設定
		p2DHPGauge->ChangeGauge((float)CCharacter::GetStatus().nLife);
		m_bDamage = false;
	}
	 // 更新
	CCharacter::Update();
	// カメラ設定
	CManager::GetRenderer()->GetCamera()->SetType((CCamera::TYPE)CCharacter::GetCameraType());
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 移動処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Move(void)
{
	// 変数宣言
	CFloor * pFloor = NULL;			// 床
	D3DXVECTOR3 move, rot;			// 移動量、回転
	bool bMove = false;				// 移動状態
	float fRot;						// 回転
	// 情報取得
	rot = CCharacter::GetRotDest();								// 目的回転量
	move = CCharacter::GetMove();								// 移動量
	fRot = CManager::GetRenderer()->GetCamera()->GetRot().y;	// カメラ回転

	// 移動 //
	/* ジョイパッド */
	// パッド用 //
	int nValueH, nValueV;	// ゲームパッドのスティック情報の取得用
	float fMove;			// 移動速度
	float fAngle;			// スティック角度の計算用変数
	fAngle = 0.0f;			// 角度

	if (CManager::GetJoy() != NULL)
	{
		// ゲームパッドのスティック情報を取得
		CManager::GetJoy()->GetStickLeft(0, &nValueH, &nValueV);

		/* プレイヤー移動 */
		// ゲームパッド移動
		if (nValueH != 0 || nValueV != 0)
		{
			// 角度の計算
			fAngle = atan2f((float)nValueH, (float)nValueV);

			if (fAngle > D3DX_PI)
			{
				fAngle -= D3DX_PI * 2;
			}
			else if (fAngle < -D3DX_PI)
			{
				fAngle += D3DX_PI * 2;
			}
			// 速度の計算
			if (abs(nValueH) > abs(nValueV))
			{
				fMove = (abs(nValueH) * PLAYER_KEYMOVE) / 1024.0f;
			}
			else
			{
				fMove = (abs(nValueV) * PLAYER_KEYMOVE) / 1024.0f;
			}
			rot.y = fAngle + fRot;

			// スティックの角度によってプレイヤー移動
			move.x -= sinf(fAngle + fRot) * (fMove);
			move.z -= cosf(fAngle + fRot) * (fMove);
			// 移動状態on
			bMove = true;
		}
	}
	/* キーボード */
	// 左
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_A))
	{
		// 移動状態on
		bMove = true;
		// 奥
		if (CManager::GetKeyboard()->GetKeyboardPress(DIK_W))
		{
			rot.y = -D3DX_PI * 0.25f + fRot;

			move.x += sinf(D3DX_PI * 0.75f + fRot) * PLAYER_KEYMOVE;
			move.z += cosf(D3DX_PI * 0.75f + fRot) * PLAYER_KEYMOVE;
		}
		// 手前
		else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_S))
		{
			rot.y = -D3DX_PI * 0.75f + fRot;

			move.x += sinf(D3DX_PI * 0.25f + fRot) * PLAYER_KEYMOVE;
			move.z += cosf(D3DX_PI * 0.25f + fRot) * PLAYER_KEYMOVE;
		}
		// 左
		else
		{
			rot.y = -D3DX_PI * 0.5f + fRot;
			move.x += sinf(D3DX_PI * 0.5f + fRot) * PLAYER_KEYMOVE;
			move.z += cosf(D3DX_PI * 0.5f + fRot) * PLAYER_KEYMOVE;
		}
	}
	// 右
	else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_D))
	{
		// 移動状態on
		bMove = true;

		// 奥
		if (CManager::GetKeyboard()->GetKeyboardPress(DIK_W))
		{
			rot.y = D3DX_PI * 0.25f + fRot;

			move.x += sinf(-D3DX_PI * 0.75f + fRot) * PLAYER_KEYMOVE;
			move.z += cosf(-D3DX_PI * 0.75f + fRot) * PLAYER_KEYMOVE;
		}
		// 手前
		else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_S))
		{
			rot.y = D3DX_PI * 0.75f + fRot;

			move.x += sinf(-D3DX_PI * 0.25f + fRot) * PLAYER_KEYMOVE;
			move.z += cosf(-D3DX_PI * 0.25f + fRot) * PLAYER_KEYMOVE;
		}
		// 右
		else
		{
			rot.y = D3DX_PI * 0.5f + fRot;

			move.x += sinf(-D3DX_PI * 0.5f + fRot) * PLAYER_KEYMOVE;
			move.z += cosf(-D3DX_PI * 0.5f + fRot) * PLAYER_KEYMOVE;
		}
	}
	// 奥に行く
	else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_W))
	{
		// 移動状態on
		bMove = true;
		rot.y = D3DX_PI * 0.0f + fRot;
		move.x += sinf(-D3DX_PI * 1.0f + fRot) * PLAYER_KEYMOVE;
		move.z += cosf(-D3DX_PI * 1.0f + fRot) * PLAYER_KEYMOVE;
	}
	// 手前に行く
	else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_S))
	{
		// 移動状態on
		bMove = true;
		rot.y = D3DX_PI * 1.0f + fRot;
		move.x += sinf(D3DX_PI * 0.0f + fRot) * PLAYER_KEYMOVE;
		move.z += cosf(D3DX_PI * 0.0f + fRot) * PLAYER_KEYMOVE;
	}
	// 移動状態なら
	if (bMove == true)
	{
		CCharacter::SetMotion(MOTIONTYPE_MOVE);
	}
	// 抵抗力
	move.x *= PLAYER_RESISTANCE;
	move.z *= PLAYER_RESISTANCE;

	CCharacter::SetMove(move);
	CCharacter::SetRotDest(rot);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 停止処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Stop(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モーション時の状態処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::MotionState(void)
{
	// 変数宣言
	D3DXVECTOR3 move;

	// モーション時の各処理
	switch (CCharacter::GetMotion())
	{
	case MOTIONTYPE_NEUTRAL:
		m_bTrick = false;

		break;
	case MOTIONTYPE_MOVE:
		break;
	case MOTIONTYPE_SLASHER:
		// パーティクル生成
		C3DParticle::Create(
			C3DParticle::OFFSET_ID_SPARKS,
			CCharacter::GetPos()
		);
		break;
	case MOTIONTYPE_TRICK1_FIREUPSIDE:
		// キーカウント8
		if (CCharacter::GetKeyInfoCnt() == 8)
		{
			move = CCharacter::GetMove();
			move.y += 1.03f;
			CCharacter::SetMove(move);
		}
		break;
	case MOTIONTYPE_TRICK2_FLASHSTRIKE:
		// キーカウント34,35,36

		break;
	default:
		break;
	}
	
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 攻撃処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Motion_Attack(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 攻撃処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Attack(void)
{
	// 秘奥義中は関数を抜ける
	if (m_bTrick)
	{
		return;
	}

	// 通常攻撃
	At_Normal();
	// 秘奥義
	Trick();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 通常攻撃処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::At_Normal(void)
{
	// 攻撃中だったら関数を抜ける
	if (CCharacter::GetAttack())
	{
		return;
	}
	// インターバルコンボカウントアップ
	m_nInvalConbo++;
	// コンボ初期化
	if (m_nInvalConbo >= 30)
	{
		m_nConbo = 0;
	}
	// コンボ攻撃
	// キーボード用 //
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		// コンボ攻撃
		ConboAttack();
		m_nInvalConbo = 0;
		m_nConbo++;
	}
	// ジョイパッド用 //
	if (CManager::GetJoy() == NULL)
	{
		return;
	}
	if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_B))
	{
		// コンボ攻撃
		ConboAttack();
		m_nInvalConbo = 0;
		m_nConbo++;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンボ攻撃処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::ConboAttack(void)
{
	// コンボ攻撃
	switch (m_nConbo)
	{
		// 初手コンボ
	case 0:
		At_DiagonalCut();
		break;
		// 1コンボ
	case 1:
		At_ReverseSide();
		break;
		// 2コンボ
	case 2:
		At_ReverseSide();
		break;
		// コンボの終了
	default:
		m_nConbo = 0;
		m_nInvalConbo = 0;
		// 秘奥義
		Trick1_FireUpside();
		break;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンボ1_追加入力なし(斜め切り)処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::At_DiagonalCut(void)
{
	// 変数宣言
	int nFrame;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 offset = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	// 移動設定
	CCharacter::SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	nFrame = CCharacter::GetMaxFrame(CCharacter::CHARACTER_PLAYER, MOTIONTYPE_TRICK1_FIREUPSIDE);

	// 軌跡の設定
	CTrajectory::Set(
		CCharacter::GetMatrix(15),		// 位置
		CTrajectory::TYPE_NORMAL,		// タイプ
		nFrame							// フレーム数
	);
	// パーティクル生成
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_STAR,
		pos
	);
	// モーション設定
	CCharacter::SetMotion(MOTIONTYPE_DIAGONALCUT);
	// 攻撃状態設定
	CCharacter::SetAttack(true);
	// 状態設定
	//CCharacter::SetState(STATE_;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンボ1_追加入力↑(突き)処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::At_Thrust(void)
{
	// 移動設定
	CCharacter::SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンボ2_追加入力(逆切り)処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::At_ReverseSide(void)
{
	// 変数宣言
	int nFrame;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 offset = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	// 移動設定
	CCharacter::SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	nFrame = CCharacter::GetMaxFrame(CCharacter::CHARACTER_PLAYER, MOTIONTYPE_TRICK1_FIREUPSIDE);

	// 軌跡の設定
	CTrajectory::Set(
		CCharacter::GetMatrix(15),		// 位置
		CTrajectory::TYPE_NORMAL,		// タイプ
		nFrame							// フレーム数
	);
	// パーティクル生成
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_STAR,
		pos
	);
	// モーション設定
	CCharacter::SetMotion(MOTIONTYPE_REVERSESIDE);
	// 攻撃状態設定
	CCharacter::SetAttack(true);
	// 状態設定
	//CCharacter::SetState(STATE_;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 秘奥義処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Trick(void)
{
	// 秘奥義1
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_1))
	{
		Trick1_FireUpside();
		m_bTrick = true;
	}
	// 秘奥義2
	else if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_2))
	{
		Trick2_FlashStrike();
		m_bTrick = true;
	}
	// 秘奥義3
	else if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_3))
	{
		Trick3_PainLightning();
		m_bTrick = true;
	}

	// ジョイパッドの処理 //
	if (CManager::GetJoy() == NULL)
	{
		return;
	}
	// 秘奥義1
	if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_LEFT_1))
	{
		Trick1_FireUpside();
		m_bTrick = true;
	}
	// 秘奥義2
	else if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_RIGHT_1))
	{
		Trick2_FlashStrike();
		m_bTrick = true;
	}
	// 秘奥義3
	else if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_RIGHT_2))
	{
		Trick3_PainLightning();
		m_bTrick = true;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 秘奥義1処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Trick1_FireUpside(void)
{
	// 変数宣言
	int nFrame;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 offset = D3DXVECTOR3(0.0f,100.0f,0.0f);

	nFrame = CCharacter::GetMaxFrame(CCharacter::CHARACTER_PLAYER, MOTIONTYPE_TRICK1_FIREUPSIDE);
	// モーション設定
	CCharacter::SetMotion(MOTIONTYPE_TRICK1_FIREUPSIDE);
	// 移動設定
	CCharacter::SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	// 攻撃状態設定
	CCharacter::SetAttack(true);
	// 軌跡の設定
	CTrajectory::Set(
		CCharacter::GetMatrix(15),		// 位置
		CTrajectory::TYPE_TRICK1,		// タイプ
		nFrame							// フレーム数
	);
	// 位置
	D3DXVec3TransformCoord(
		&pos,
		&offset,
		CCharacter::GetMatrix(15)
	);
	// パーティクル生成
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_STAR,
		pos
	);
	// パーティクル生成
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_WEAVELINE,
		pos
	);
	// メッシュスフィア
	CMeshsphere::Set(
		pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f),
		1000.0f,
		nFrame
	);
	/*
	// 背景のドーム
	CBgdome::Set(
	pos,
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	nFrame
	);
	*/
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 秘奥義2処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Trick2_FlashStrike(void)
{
	// 変数宣言
	int nFrame;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 offset = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	nFrame = CCharacter::GetMaxFrame(CCharacter::CHARACTER_PLAYER, MOTIONTYPE_TRICK2_FLASHSTRIKE);

	// モーション設定
	CCharacter::SetMotion(MOTIONTYPE_TRICK2_FLASHSTRIKE);
	// 移動設定
	CCharacter::SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	// 攻撃状態設定
	CCharacter::SetAttack(true);
	// 軌跡の設定
	CTrajectory::Set(
		CCharacter::GetMatrix(15),		// 位置
		CTrajectory::TYPE_TRICK2,		// タイプ
		nFrame							// フレーム数
	);
	// 位置
	D3DXVec3TransformCoord(
		&pos,
		&offset,
		CCharacter::GetMatrix(15)
	);
	// パーティクル生成
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_STAR,
		pos
	);
	// パーティクル生成
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_WEAVELINE,
		pos
	);
	// メッシュスフィア
	CMeshsphere::Set(
		pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f),
		1000.0f,
		nFrame
	);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 秘奥義3処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Trick3_PainLightning(void)
{
	// 変数宣言
	int nFrame;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 offset = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	nFrame = CCharacter::GetMaxFrame(CCharacter::CHARACTER_PLAYER, MOTIONTYPE_TRICK3_PAINLIGHTNING);

	// モーション設定 
	CCharacter::SetMotion(MOTIONTYPE_TRICK3_PAINLIGHTNING);
	// 移動設定
	CCharacter::SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	// 攻撃状態設定
	CCharacter::SetAttack(true);
	// 軌跡の設定
	CTrajectory::Set(
		CCharacter::GetMatrix(15),		// 位置
		CTrajectory::TYPE_TRICK1,		// タイプ
		nFrame							// フレーム数
	);
	// パーティクル生成
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_STAR,
		pos
	);
	// 位置
	D3DXVec3TransformCoord(
		&pos,
		&offset,
		CCharacter::GetMatrix(15)
	);

	// パーティクル生成
	C3DParticle::Create(
		C3DParticle::OFFSET_ID_CROSSLINE,
		pos
	);
	// メッシュスフィア
	CMeshsphere::Set(
		pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f),
		1000.0f,
		nFrame
	);

	/*
	// 演出設定
	CPerfomance::Set(CCharacter::GetMatrix(15),
		CPerfomance::TYPE_0,
		CCharacter::GetPos(),
		CCharacter::GetRot(),
		CCharacter::GetMaxFrame(CCharacter::CHARACTER_PLAYER, MOTIONTYPE_TRICK3_PAINLIGHTNING)
	);
	/*
	// 攻撃判定
	CCollider::Create(
		&CCharacter::GetPos(),							// 位置
		D3DXVECTOR3(									// 中心位置からの距離
			100.0f * -sinf(CCharacter::GetRot().y),
			100.0f,
			100.0f * -cosf(CCharacter::GetRot().y)),
		CCharacter::GetStatus().nAttack,				// 攻撃力
		0												// 味方か敵か
	);
	*/
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Draw(void)
{
	CCharacter::Draw();
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// デバッグ表示
//-------------------------------------------------------------------------------------------------------------
void CPlayer::Debug(void)
{
	CDebugproc::Print("キーカウント:%d\n",CCharacter::GetKeyInfoCnt());
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_N))
	{
		D3DXVECTOR3 pos;
		pos = p2DTrickGauge->GetPos();
		pos.x += 0.1f;
		p2DTrickGauge->SetPos(pos);
	}
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_M))
	{
		D3DXVECTOR3 pos;
		pos = p2DTrickGauge->GetPos();
		pos.x -= 0.1f;
		p2DTrickGauge->SetPos(pos);
	}
}
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 生成処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// リソース情報読み込み処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CPlayer::Load(void)
{
	CCharacter::Load(
		CCharacter::CHARACTER_PLAYER,
		MOTIONTYPE_MAX, 
		PLAYER_MOTIONFILE);
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだリソース情報を破棄処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::UnLoad(void)
{
	CCharacter::UnLoad(
		CCharacter::CHARACTER_PLAYER,
		MOTIONTYPE_MAX);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 位置取得処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return 	CCharacter::GetPos();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 回転取得処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return CCharacter::GetRot();
}