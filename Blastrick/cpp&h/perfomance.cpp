// ------------------------------------------
//
// 演出の処理[perfomance.h]
// Author : Nishiyama koki
//
// ------------------------------------------
// ------------------------------------------
// インクルードファイル
// ------------------------------------------
#include "perfomance.h"
#include "trajectory.h"
#include "3Dparticle.h"
#include "meshsphere.h"
#include "bgdome.h"

// ------------------------------------------
// 静的メンバ変数の初期化
// ------------------------------------------

// ------------------------------------------
// コンストラクタ
// ------------------------------------------
CPerfomance::CPerfomance()
{
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CPerfomance::~CPerfomance()
{
}

// ------------------------------------------
// 設定
// 1行列、2タイプ
// ------------------------------------------
void CPerfomance::Set(
	D3DXMATRIX *pmtx,	// 行列
	TYPE type,			// タイプ
	D3DXVECTOR3 &pos,	// 位置
	D3DXVECTOR3 &rot,	// 回転
	int nFrame			// フレーム数
	)
{
	/*
	switch (type)
	{
	case TYPE_0:
		// 軌跡の設定
		CTrajectory::Set(
			pmtx,					// 位置
			CTrajectory::TYPE_0,	// タイプ
			nFrame					// フレーム数
		);
		// パーティクル生成
		C3DParticle::Create(
			C3DParticle::OFFSET_ID_STAR,
			pos
		);
		/*
		// パーティクル生成
		C3DParticle::Create(
			C3DParticle::OFFSET_ID_CROSSLINE,
			D3DXVECTOR3(
				pmtx->_41,
				pmtx->_42,
				pmtx->_43
			)
		);
		// 位置
		D3DXVec3TransformCoord(
			&m_pos,
			&m_offset,
			m_mtx
		);

		// パーティクル生成
		C3DParticle::Create(
			C3DParticle::OFFSET_ID_RANDAMLINE,
			D3DXVECTOR3(
				pmtx->_41,
				pmtx->_42,
				pmtx->_43
			)
		);
		// メッシュスフィア
		CMeshsphere::Set(
			pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f),
			500.0f,
			nFrame
		);
		/*
		// 背景のドーム
		CBgdome::Set(
			pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			nFrame
		);
		break;
	case TYPE_1:
		// 軌跡の設定
		CTrajectory::Set(
			pmtx,					// 位置
			CTrajectory::TYPE_0,	// タイプ
			nFrame					// フレーム数
		);
		// パーティクル生成
		C3DParticle::Create(
			C3DParticle::OFFSET_ID_STAR,
			pos
		);

		// メッシュスフィア
		CMeshsphere::Set(
			pos + D3DXVECTOR3(0.0f, 100.0f, 0.0f),
			500.0f,
			nFrame
		);
		/*
		// 背景のドーム
		CBgdome::Set(
			pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			nFrame
		);
		break;

	default:
		break;
	}
*/
}

#ifdef _DEBUG
// ------------------------------------------
// デバッグ表示
// ------------------------------------------
void CPerfomance::Debug(void)
{
}
#endif // _DEBUG
