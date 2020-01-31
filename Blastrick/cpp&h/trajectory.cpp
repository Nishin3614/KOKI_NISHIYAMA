// ------------------------------------------
//
// 軌跡の処理[trajectory.h]
// Author : Nishiyama koki
//
// ------------------------------------------
// ------------------------------------------
// インクルードファイル
// ------------------------------------------
#include "trajectory.h"
#include "meshobit.h"

// ------------------------------------------
// 静的メンバ変数の初期化
// ------------------------------------------

// ------------------------------------------
// コンストラクタ
// ------------------------------------------
CTrajectory::CTrajectory()
{
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CTrajectory::~CTrajectory()
{
}

// ------------------------------------------
// 初期化
// ------------------------------------------
void CTrajectory::Init(void)
{
}

// ------------------------------------------
// 終了
// ------------------------------------------
void CTrajectory::Uninit(void)
{

}

// ------------------------------------------
// 更新
// ------------------------------------------
void CTrajectory::Update(void)
{
}

// ------------------------------------------
// 描画
// ------------------------------------------
void CTrajectory::Draw(void)
{
}

// ------------------------------------------
// 設定
// 1行列、2タイプ
// ------------------------------------------
void CTrajectory::Set(
	D3DXMATRIX *pmtx,	// 行列
	TYPE type,			// タイプ
	int &nFrame		// フレーム数
)
{
	switch (type)
	{
	case TYPE_NORMAL:
		// 白
		CMeshobit::Set(
			pmtx, 
			D3DXVECTOR3(0.0f, 0.0f,0.0f),
			D3DXVECTOR3(0.0f, 80.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CMeshobit::TYPE_0,
			CMeshobit::TEX_0,
			100.0f,
			40,
			nFrame
		);
		CMeshobit::Set(
			pmtx,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 85.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			CMeshobit::TYPE_0, 
			CMeshobit::TEX_0,
			100.0f,
			40,
			nFrame
		);
		CMeshobit::Set(
			pmtx,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(5.0f, 80.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CMeshobit::TYPE_0,
			CMeshobit::TEX_1,
			100.0f,
			40,
			nFrame
		);
		CMeshobit::Set(
			pmtx,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 80.0f, 5.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CMeshobit::TYPE_0,
			CMeshobit::TEX_1,
			100.0f,
			40,
			nFrame
		);
		break;
	case TYPE_TRICK1:
		// 白
		CMeshobit::Set(
			pmtx,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 150.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CMeshobit::TYPE_0,
			CMeshobit::TEX_0,
			100.0f,
			80,
			nFrame
		);
		CMeshobit::Set(
			pmtx,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 155.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			CMeshobit::TYPE_0,
			CMeshobit::TEX_0,
			100.0f,
			80,
			nFrame
		);
			break;
	case TYPE_TRICK2:
		// 白
		CMeshobit::Set(
			pmtx,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 200.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			CMeshobit::TYPE_0,
			CMeshobit::TEX_0,
			100.0f,
			80,
			nFrame
		);
		CMeshobit::Set(
			pmtx,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 205.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			CMeshobit::TYPE_0,
			CMeshobit::TEX_0,
			100.0f,
			80,
			nFrame
		);
		break;
	default:
		break;
	}
}

// ------------------------------------------
// 生成
// ------------------------------------------
CTrajectory * CTrajectory::Create(void)
{
	CTrajectory *pTrajectory = new CTrajectory;
	return pTrajectory;
}

// ------------------------------------------
// 読み込み
// ------------------------------------------
HRESULT CTrajectory::Load(void)
{
	CMeshobit::Load();
	return S_OK;
}

// ------------------------------------------
// あらかじめ軌跡の生成をする
// ------------------------------------------
HRESULT CTrajectory::LoadCreate(void)
{
	return S_OK;
}

// ------------------------------------------
// 開放
// ------------------------------------------
void CTrajectory::Unload(void)
{
	CMeshobit::UnLoad();
}

#ifdef _DEBUG
// ------------------------------------------
// デバッグ表示
// ------------------------------------------
void CTrajectory::Debug(void)
{
}
#endif // _DEBUG
