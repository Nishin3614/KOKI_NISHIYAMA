// ----------------------------------------
//
// 当たり判定処理の説明[collider.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "collider.h"
#include "character.h"
#include "meshsphere.h"

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CCollider::CCollider()
{
	// 変数の初期化
	m_mtx = NULL;				// 行列
	m_offset = D3DVECTOR3_ZERO;	// オフセット位置
	m_pos = D3DVECTOR3_ZERO;	// 中心座標
	m_fRadius = 0;				// 半径
	m_nStartFrame = 0;			// スタート
	m_nEndFrame = 0;			// エンド
	m_nCntFrame = 0;			// フレームカウント
	m_nAttack = 0;				// 攻撃力
	m_nCharacter = 0;			// キャラクタータイプ
}

// ----------------------------------------
// イニシャライザコンストラクタ
// ----------------------------------------
CCollider::CCollider(ACTOR actor, LAYER layer) : CScene::CScene(actor,layer)
{
	// 変数の初期化
	m_mtx = NULL;				// 行列
	m_offset = D3DVECTOR3_ZERO;	// オフセット位置
	m_pos = D3DVECTOR3_ZERO;	// 中心座標
	m_fRadius = 0;				// 半径
	m_nStartFrame = 0;			// スタート
	m_nEndFrame = 0;			// エンド
	m_nCntFrame = 0;			// フレームカウント
	m_nAttack = 0;				// 攻撃力
	m_nCharacter = 0;			// キャラクタータイプ
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CCollider::~CCollider()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CCollider::Init(void)
{
	/*
	CMeshsphere *pSphere;
	pSphere = CMeshsphere::Create();
	pSphere->Set(m_pos, m_fRadius);
	*/
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CCollider::Uninit(void)
{
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CCollider::Update(void)
{
	switch (m_owner)
	{
	case OWNER_MYSELF:
		OwnerMyself();
		break;
	case OWNER_YOU:
		OwnerYou();
		break;
	}
}

// ----------------------------------------
// 管理者自分処理
// ----------------------------------------
void CCollider::OwnerMyself(void)
{
	// フレーム外
	if (m_nCntFrame < m_nStartFrame)
	{
		return;
	}
	else if (m_nCntFrame >= m_nEndFrame)
	{
		m_nCntFrame = 0;
		m_mtx = NULL;
		m_bUse = false;
		Release();
		return;
	}
	// 位置
	D3DXVec3TransformCoord(
		&m_pos,
		&m_offset,
		m_mtx
	);
	// 変数宣言
	CCharacter * pCharacter = NULL;	// 床
	// 情報取得
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntLayer++)
	{
		pCharacter = (CCharacter*)CScene::GetScene(CScene::LAYER_CHARACTER, nCntLayer);
		// 存在しているかどうか
		if (pCharacter == NULL)
		{
			continue;
		}
		// 判定を適用するかどうか
		else if (this->m_nCharacter == pCharacter->GetCharacter() ||		// 同族か
			pCharacter->GetStatus().bInvincible)							// 無敵状態ではないか
		{
			continue;
		}
		// 球の当たり判定
		if (CCalculation::Collision_Circle(
			m_pos,					// 位置
			m_fRadius,				// 範囲
			pCharacter->GetPos(),	// キャラクター位置
			100.0f					// 範囲
		))
		{
			// ダメージ処理
			pCharacter->AplayDamage(
				m_pos,
				CCharacter::BLUST_STOP,
				m_nAttack
			);
		}
	}
	// フレームカウント
	m_nCntFrame++;
}

// ----------------------------------------
// 管理者使用しオブジェクトている処理
// ----------------------------------------
void CCollider::OwnerYou(void)
{
	// 使用状態がfalseの場合抜ける
	if (!m_bUse)
	{
		return;
	}
	// 変数宣言
	CCharacter * pCharacter = NULL;	// 床
	// 情報取得
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntLayer++)
	{
		pCharacter = (CCharacter*)CScene::GetScene(CScene::LAYER_CHARACTER, nCntLayer);
		// 存在しているかどうか
		if (pCharacter == NULL)
		{
			continue;
		}
		// 判定を適用するかどうか
		else if (this->m_nCharacter == pCharacter->GetCharacter() &&		// 同族か
			pCharacter->GetStatus().bInvincible)							// 無敵状態ではないか
		{
			continue;
		}
		// 球の当たり判定
		if (CCalculation::Collision_Circle(
			m_pos,					// 位置
			m_fRadius,				// 範囲
			pCharacter->GetPos(),	// キャラクター位置
			100.0f					// 範囲
		))
		{
			// ダメージ処理
			pCharacter->AplayDamage(
				m_pos,
				CCharacter::BLUST_STOP,
				m_nAttack
			);
		}
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CCollider::Draw(void)
{
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// デバッグ表示
//-------------------------------------------------------------------------------------------------------------
void CCollider::Debug(void)
{
}
#endif // _DEBUG

// ----------------------------------------
// 位置からの作成処理
// ----------------------------------------
CCollider * CCollider::Create(
	D3DXVECTOR3	const &pos,			// 位置
	float		const &fRadius,		// 半径
	int			const &nAttack,		// 攻撃力
	int			const &nCharacter,	// キャラクター
	OWNER		const &owner		// 管理者
)
{
	// 変数宣言
	CCollider * pCollider;		// 当たり判定クラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pCollider = new CCollider(ACTOR_COLLIDER,LAYER_UI);
	// 設定
	pCollider->m_pos = pos;					// 位置
	pCollider->m_fRadius = fRadius;			// 半径
	pCollider->m_nAttack = nAttack;			// 攻撃力
	pCollider->m_nCharacter = nCharacter;	// キャラクター
	pCollider->m_owner = owner;				// 管理者
	// 初期化処理
	pCollider->Init();
	// 生成したオブジェクトを返す
	return pCollider;
}

// ----------------------------------------
// 行列情報からの作成処理
// ----------------------------------------
CCollider * CCollider::Create(
	D3DXMATRIX	*mtx,				// 行列
	D3DXVECTOR3 const &offset,		// オフセット位置
	float		const &fRadius,		// 半径
	int			const &nStartFrame,	// スタート
	int			const &nEndFrame,	// エンド
	int			const &nAttack,		// 攻撃力
	int			const &nCharacter,	// キャラクター
	OWNER		const &owner		// 管理者
)
{
	// 変数宣言
	CCollider * pCollider;		// 当たり判定クラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pCollider = new CCollider(ACTOR_COLLIDER, LAYER_UI);
	// 設定
	pCollider->m_nCharacter = nCharacter;
	pCollider->m_owner = owner;
	pCollider->Set(
		mtx,
		offset,		// オフセット位置
		fRadius,		// 半径
		nStartFrame,	// スタート
		nEndFrame,	// エンド
		nAttack		// 攻撃力
	);
	// 初期化処理
	pCollider->Init();
	// 生成したオブジェクトを返す
	return pCollider;
}

// ----------------------------------------
// 当たり判定設定設定処理
// ----------------------------------------
void CCollider::Set(
	D3DXMATRIX	*mtx,				// 行列
	D3DXVECTOR3 const &offset,		// オフセット位置
	float		const &fRadius,		// 半径
	int			const &nStartFrame,	// スタート
	int			const &nEndFrame,	// エンド
	int			const &nAttack		// 攻撃力
)
{
	// 行列がない場合抜ける
	if (mtx == NULL) return;
	// 設定
	m_mtx = mtx;					// 行列
	m_offset = offset;				// オフセット位置
	D3DXVec3TransformCoord(
		&m_pos,
		&m_offset,
		m_mtx
	);
	m_fRadius = fRadius;			// 半径
	m_nStartFrame = nStartFrame;	// スタート
	m_nEndFrame = nEndFrame;		// エンド
	m_nAttack = nAttack;			// 攻撃力
}

// ----------------------------------------
// 使用状態設定処理
// ----------------------------------------
void CCollider::SetUse(bool const bUse)
{
	// 変数宣言
	CCharacter * pCharacter = NULL;	// 床
	// 情報取得
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntLayer++)
	{
		pCharacter = (CCharacter*)CScene::GetScene(CScene::LAYER_CHARACTER, nCntLayer);
		// 存在しているかどうか
		if (pCharacter == NULL)
		{
			continue;
		}
		// 判定を適用するかどうか
		else if (this->m_nCharacter == pCharacter->GetCharacter())		// 同族か
		{
			continue;
		}
		// キャラクターの当たり判定状態
		pCharacter->SetInvi(bUse);
	}
	// 使用状態
	m_bUse = bUse;
}

// ----------------------------------------
// 位置設定処理
// ----------------------------------------
void CCollider::SetPos(D3DXVECTOR3 const & pos)
{
	m_pos = pos;
}

// ----------------------------------------
// 位置情報取得処理
// ----------------------------------------
D3DXVECTOR3 CCollider::GetPos(void)
{
	return m_pos;
}

// ----------------------------------------
// キャラクター情報取得処理
// ----------------------------------------
int CCollider::GetCharacter(void)
{
	return m_nCharacter;
}
