// ----------------------------------------
//
// 棒人間処理 [rothuman.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------
#include "rothuman.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define ROTHUMAN_MOTIONFILE "data/LOAD/CHARACTER/giraffe_motion.txt"	// モーションのファイル名

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------

// ----------------------------------------
// オーバーロードコンストラクタ(自身用)
// ----------------------------------------
CRothuman::CRothuman() : CEnemy::CEnemy(ACTOR_ROTHUMAN)
{
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CRothuman::~CRothuman()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CRothuman::Init(void)
{
	CCharacter::SetCharacter(CHARACTER_ROTHUMAN);
	CEnemy::Init();
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CRothuman::Uninit(void)
{
	CEnemy::Uninit();
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CRothuman::Update(void)
{
	// エネミー更新
	CEnemy::Update();
	// 攻撃時は入らない
	if (!CCharacter::GetAttack() ||
		!(CCharacter::GetState() == CCharacter::STATE_DAMAGE ||
		CCharacter::GetState() == CCharacter::STATE_DIE))
	{
		// 行動処理
		AiAction();
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CRothuman::Draw(void)
{
	CEnemy::Draw();
}

// ----------------------------------------
// 生成処理
// ----------------------------------------
CRothuman * CRothuman::Create(
	D3DXVECTOR3 pos			// 位置
)
{
	// 変数宣言
	CRothuman * pRothuman;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pRothuman = new CRothuman();
	// 初期化処理
	pRothuman->Init();
	// 位置設定
	pRothuman->SetPos(pos);
	// 生成したオブジェクトを返す
	return pRothuman;
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CRothuman::LoadCreate(void)
{
	// 変数宣言
	D3DXVECTOR3 pos;
	for (int nCntCreate = 0; nCntCreate < 10; nCntCreate++)
	{
		// 変数宣言
		CRothuman * pRothuman;
		// メモリの生成(初め->基本クラス,後->派生クラス)
		pRothuman = new CRothuman();
		// 位置設定
		pos.x = (float)(rand() % 900) - (float)(rand() % 900);
		pos.z = (float)(rand() % 900) - (float)(rand() % 900);

		pRothuman->SetPos(pos);
		// 初期化処理
		pRothuman->Init();
		pRothuman = NULL;
	}
	return E_NOTIMPL;
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CRothuman::Load(void)
{
	CCharacter::Load(
		CCharacter::CHARACTER_ROTHUMAN,
		MOTIONTYPE_MAX, 
		ROTHUMAN_MOTIONFILE);
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄処理
// ----------------------------------------
void CRothuman::UnLoad(void)
{
	CCharacter::UnLoad(
		CCharacter::CHARACTER_ROTHUMAN,
		MOTIONTYPE_MAX);
}

// ----------------------------------------
// 行動処理処理
// ----------------------------------------
void CRothuman::AiAction(void)
{
	switch (m_Ai)
	{
	case AI_NORMALACTION:
		NormalAction();
		break;
	case AI_ATTACKACTION:
		AttackAction();
		break;
	default:
		break;
	}
}

// ----------------------------------------
// 攻撃行動処理
// ----------------------------------------
void CRothuman::AttackAction(void)
{
	if (!m_bAttackCollision)
	{
		return;
	}
	// 変数宣言
	int nParcent;
	D3DXVECTOR3 move;
	// 情報取得
	move = CCharacter::GetMove();								// 移動量

	nParcent = rand() % 100;

	if (nParcent > 20)
	{
		CCharacter::SetMotion(MOTIONTYPE_ATTACK);
	}
	else if (nParcent > 100)
	{
		CCharacter::SetMotion(MOTIONTYPE_ATTACK);
	}
	CCharacter::SetAttack(true);
	move = D3DVECTOR3_ZERO;
	CCharacter::SetMove(move);
}

// ----------------------------------------
// 普段の行動処理
// ----------------------------------------
void CRothuman::NormalAction(void)
{
	CCharacter::SetMotion(MOTIONTYPE_NEUTRAL);
}

// ----------------------------------------
// 移動処理
// ----------------------------------------
void CRothuman::move(void)
{
	// 変数宣言
	D3DXVECTOR3 pos;	// 位置
}

// ----------------------------------------
// 回転処理
// ----------------------------------------
void CRothuman::rot(void)
{
	// 変数宣言
	D3DXVECTOR3 rot;	// 回転
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// デバッグ表示
//-------------------------------------------------------------------------------------------------------------
void CRothuman::Debug(void)
{
}
#endif // _DEBUG
