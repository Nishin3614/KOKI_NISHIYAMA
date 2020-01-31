//=============================================================================
//
// ライト処理 [light.cpp]
// Author : KOKI_NISHIYAMA
//
//=============================================================================
#include "light.h"
#include "manager.h"

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CLight::CLight()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	// カウントライト
	for (int nCntLight = 0; nCntLight < TYPE_MAX; nCntLight++)
	{
		// ライトをクリアする
		ZeroMemory(&m_light[nCntLight], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ライトの拡散光を設定
		m_light[nCntLight].Diffuse = m_col[nCntLight];

		// ライトの方向の設定
		D3DXVec3Normalize(&m_VecDir[nCntLight], &m_VecDir[nCntLight]);		// 正規化
		m_light[nCntLight].Direction = m_VecDir[nCntLight];					// ライトに代入

		// ライトを設定する
		pDevice->SetLight(nCntLight, &m_light[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CLight::~CLight()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CLight::Init(void)
{
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CLight::Uninit(void)
{
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CLight::Update(void)
{
	/*
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	// 秘奥義1
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_U))
	{
		m_VecDir[0].x += 0.01f;
	}
	// 秘奥義1
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_I))
	{
		m_VecDir[0].y += 0.01f;
	}
	// 秘奥義1
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_O))
	{
		m_VecDir[0].z += 0.01f;
	}


	// ライトの方向の設定
	D3DXVec3Normalize(&m_VecDir[0], &m_VecDir[0]);		// 正規化
	m_light[0].Direction = m_VecDir[0];					// ライトに代入

	// ライトを設定する
	pDevice->SetLight(0, &m_light[0]);

	CDebugproc::Print("ライトの方向(%.3f,%.3f,%.3f)\n", m_VecDir[0].x, m_VecDir[0].y, m_VecDir[0].z);
	*/
}

// ----------------------------------------
// 生成処理
// ----------------------------------------
CLight * CLight::Create(void)
{
	// 変数宣言
	CLight * pLight;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pLight = new CLight();
	// 初期化処理
	pLight->Init();
	// 生成したオブジェクトを返す
	return pLight;
}

// ----------------------------------------
// ライト情報取得処理
// ----------------------------------------
D3DLIGHT9 & CLight::GetLight(TYPE type)
{
	return m_light[type];
}

// ----------------------------------------
// 色情報取得処理
// ----------------------------------------
D3DXCOLOR & CLight::GetCol(TYPE type)
{
	return m_col[type];
}

// ----------------------------------------
// 方向情報取得処理
// ----------------------------------------
D3DVECTOR & CLight::GetVec(TYPE type)
{
	return 	m_light[type].Direction;
}
