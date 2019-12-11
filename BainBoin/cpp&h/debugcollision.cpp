// ----------------------------------------
//
// デバッグ用の当たり判定表示処理の説明[debugcollision.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "debugcollision.h"
#include "3Dline.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

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

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CDebugcollision::CDebugcollision() : CScene::CScene(ACTOR_3DCOLLISION,LAYER_3DOBJECT)
{
	m_pos = NULL;
	m_size = NULL;
	m_type = COLLISIONTYPE_BOX;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CDebugcollision::~CDebugcollision()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CDebugcollision::Init(void)
{
	switch (m_type)
	{
	case COLLISIONTYPE_BOX:
		BoxCollision();
		break;
	default:
		break;
	}
}

// ----------------------------------------
// 箱の当たり判定処理
// ----------------------------------------
void CDebugcollision::BoxCollision(void)
{
	// 1つ目
	C3DLine::Create(
		*m_pos,
		D3DXVECTOR3(0.0f,0.0f,0.0f),
		D3DXVECTOR3(-m_size->x * 0.5f, m_size->y * 0.5f,-m_size->z * 0.5f),
		D3DXVECTOR3(m_size->x * 0.5f, m_size->y * 0.5f, -m_size->z * 0.5f)
	);
	// 2つ目
	C3DLine::Create(
		*m_pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_size->x * 0.5f, m_size->y * 0.5f, -m_size->z * 0.5f),
		D3DXVECTOR3(m_size->x * 0.5f, -m_size->y * 0.5f, -m_size->z * 0.5f)
	);
	// 3つ目
	C3DLine::Create(
		*m_pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_size->x * 0.5f, -m_size->y * 0.5f, -m_size->z * 0.5f),
		D3DXVECTOR3(-m_size->x * 0.5f, -m_size->y * 0.5f, -m_size->z * 0.5f)
	);
	// 4つ目
	C3DLine::Create(
		*m_pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-m_size->x * 0.5f, -m_size->y * 0.5f, -m_size->z * 0.5f),
		D3DXVECTOR3(-m_size->x * 0.5f, m_size->y * 0.5f, -m_size->z * 0.5f)
	);
	// 5つ目
	C3DLine::Create(
		*m_pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-m_size->x * 0.5f, m_size->y * 0.5f, -m_size->z * 0.5f),
		D3DXVECTOR3(-m_size->x * 0.5f, m_size->y * 0.5f, m_size->z * 0.5f)
	);
	// 6つ目
	C3DLine::Create(
		*m_pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_size->x * 0.5f, m_size->y * 0.5f, -m_size->z * 0.5f),
		D3DXVECTOR3(m_size->x * 0.5f, m_size->y * 0.5f, m_size->z * 0.5f)
	);
	// 7つ目
	C3DLine::Create(
		*m_pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_size->x * 0.5f, -m_size->y * 0.5f, -m_size->z * 0.5f),
		D3DXVECTOR3(m_size->x * 0.5f, -m_size->y * 0.5f, m_size->z * 0.5f)
	);
	// 8つ目
	C3DLine::Create(
		*m_pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-m_size->x * 0.5f, -m_size->y * 0.5f, -m_size->z * 0.5f),
		D3DXVECTOR3(-m_size->x * 0.5f, -m_size->y * 0.5f, m_size->z * 0.5f)
	);
	// 9つ目
	C3DLine::Create(
		*m_pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-m_size->x * 0.5f, m_size->y * 0.5f, m_size->z * 0.5f),
		D3DXVECTOR3(m_size->x * 0.5f, m_size->y * 0.5f, m_size->z * 0.5f)
	);
	// 10つ目
	C3DLine::Create(
		*m_pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_size->x * 0.5f, m_size->y * 0.5f, m_size->z * 0.5f),
		D3DXVECTOR3(m_size->x * 0.5f, -m_size->y * 0.5f, m_size->z * 0.5f)
	);
	// 11つ目
	C3DLine::Create(
		*m_pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_size->x * 0.5f, -m_size->y * 0.5f, m_size->z * 0.5f),
		D3DXVECTOR3(-m_size->x * 0.5f, -m_size->y * 0.5f, m_size->z * 0.5f)
	);
	// 12つ目
	C3DLine::Create(
		*m_pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-m_size->x * 0.5f, -m_size->y * 0.5f, m_size->z * 0.5f),
		D3DXVECTOR3(-m_size->x * 0.5f, m_size->y * 0.5f, m_size->z * 0.5f)
	);

}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CDebugcollision::Uninit(void)
{
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CDebugcollision::Update(void)
{
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CDebugcollision::Draw(void)
{
}

// ----------------------------------------
// 生成処理
// ----------------------------------------
CDebugcollision * CDebugcollision::Create(
	D3DXVECTOR3 * pos,
	D3DXVECTOR3 * size,
	COLLISIONTYPE type
)
{
	// 変数宣言
	CDebugcollision * pDebugcollision;	// デバッグ当たり判定
	// メモリ確保
	pDebugcollision = new CDebugcollision;
	pDebugcollision->m_pos = pos;
	pDebugcollision->m_size = size;
	pDebugcollision->m_type = type;
	// 初期化
	pDebugcollision->Init();
	// 返す
	return pDebugcollision;
}
