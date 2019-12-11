//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : KOKI_NISHIYAMA
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "joypad.h"
#include "manager.h"
#include "debugproc.h"
#include "scene_X.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CCamera::CCamera()
{
	m_fx = 1.0f;
	m_fz = 1.0f;
	m_fY = 500.0f;

	// カメラの初期化
	m_posV = D3DXVECTOR3(m_fx, m_fY, m_fz);
	m_posVDest = m_posV;
	m_posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_move = D3DXVECTOR3(1.0f, 3.0f, 3.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0;
	m_nCntTime = 0;
	m_bfront = false;

	// 視点と注視点との距離計算
	D3DXVECTOR3 diffPos = m_posV - m_posR;
	// 長さ計算
	m_fLength = sqrtf(diffPos.x*diffPos.x + diffPos.z * diffPos.z);
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CCamera::~CCamera()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CCamera::Init(void)
{
	m_fx = 1.0f;
	m_fz = -700.0f;
	m_fY = 400.0f;

	// カメラの初期化
	m_posV = D3DXVECTOR3(m_fx, m_fY, m_fz);
	m_posVDest = m_posV;
	m_posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = m_posR;
	m_posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_move = D3DXVECTOR3(1.0f, 3.0f, 3.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0;
	m_nCntTime = 0;
	m_bfront = false;
	// 視点と注視点との距離計算
	D3DXVECTOR3 diffPos = m_posV - m_posR;

	// 長さ計算
	m_fLength = sqrtf(diffPos.x*diffPos.x + diffPos.z * diffPos.z);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CCamera::Uninit(void)
{
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CCamera::Update(void)
{
	// プレイヤー追従
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		PlayerFoll();
	}
	// 回転
	Rot();
	// 注視点軸に回転 //
	//-------------------- 視点移動 --------------------//
	// 目的の視点
	m_posVDest.x =
		m_posR.x + sinf(m_rot.y) * m_fLength;
	m_posVDest.y =
		m_posR.y + m_fY;
	m_posVDest.z =
		m_posR.z + cosf(m_rot.y) * m_fLength;

	// 目的地から現在地の距離(注視点)
	m_posVDiff =
		m_posVDest - m_posV;

	// 注視点に慣性
	m_posV.x += m_posVDiff.x * 0.1f;
	m_posV.y += m_posVDiff.y * 0.1f;
	m_posV.z += m_posVDiff.z * 0.1f;
	/*
	CDebugproc::Print("カメラの視点x:%f,y:%f,z:%f\n", m_posVDest.x, m_posVDest.y, m_posVDest.z);
	CDebugproc::Print("カメラの注視点x:%f,y:%f,z:%f\n", m_posR.x, m_posR.y, m_posR.z);
	CDebugproc::Print("カメラの角度x:%f,y:%f,z:%f\n", m_rot.x, m_rot.y, m_rot.z);
	*/
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CCamera::Draw(void)
{
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CCamera * CCamera::Create(void)
{
	// 変数宣言
	CCamera * pCamera;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pCamera = new CCamera();
	// 初期化処理
	pCamera->Init();
	// 生成したオブジェクトを返す
	return pCamera;
}

// ----------------------------------------
// 取得
// ----------------------------------------
CCamera * CCamera::Get(void)
{
	return nullptr;
}

// ----------------------------------------
// 設定
// ----------------------------------------
void CCamera::Set(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	// プロジェクションマトリックス //
	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(
		&m_mtxProjection,					// (テレビ視点)
		D3DXToRadian(45.0f),						// 視野角(画角)
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// アスペクト比
		10.0f,										// MearZ値(近いビュー平面のZ値)
		20000.0f);									// FarZ値(遠いビュー平面のZ値)初期値は1000

													// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックス //
	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(
		&m_mtxView,	// (カメラマン視点)
		&m_posV,		// カメラの視点
		&m_posR,		// カメラの注目点
		&m_posU);	// カメラの上方向ベクトル

							// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

// ----------------------------------------
// カメラの方向
// ----------------------------------------
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}

// ----------------------------------------
// プレイヤー追従
// ----------------------------------------
void CCamera::PlayerFoll(void)
{
	/* 変数宣言 */
	CPlayer * pPlayer = NULL;	// プレイヤー
	// 情報取得
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntLayer++)
	{
		if (pPlayer == NULL) pPlayer = (CPlayer*)CScene::GetActor(CScene::ACTOR_PLAYER, CScene::LAYER_3DOBJECT, nCntLayer);	// プレイヤー
	}

	m_nCntTime++;				// カウントタイム
	//-------------------- 視点移動 --------------------//
	// ゲームのカメラの回転の目的地
	m_posR = pPlayer->GetPos();
}

// ----------------------------------------
// 回転
// ----------------------------------------
void CCamera::Rot(void)
{
	// 注視点軸に回転 //
	// 半時計回り
	//if (CManager::GetKeyboard()->GetKeyboardPress(DIK_RIGHT))
	//{
	//	m_rot.y += 0.1f;

	//	// -3.14～3.14に回転差分を固定させる
	//	m_rot.y = CCalculation::Rot_One_Limit(m_rot.y);

	//	// 視点更新
	//	m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * m_fLength;
	//	m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * m_fLength;

	//	// カウントタイム初期化
	//	m_nCntTime = 0;

	//	// 正面を向くをoffへ
	//	m_bfront = false;
	//}

	//// 時計回り
	//else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_LEFT))
	//{
	//	m_rot.y -= 0.1f;

	//	// -3.14～3.14に回転差分を固定させる
	//	m_rot.y = CCalculation::Rot_One_Limit(m_rot.y);

	//	// 視点更新
	//	m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * m_fLength;
	//	m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * m_fLength;

	//	// カウントタイム初期化
	//	m_nCntTime = 0;

	//	// 正面を向くをoffへ
	//	m_bfront = false;
	//}
	// 上
	//if (CManager::GetKeyboard()->GetKeyboardPress(DIK_UP))
	//{
	//	m_fY += 10.0f;
	//}
	//// 下
	//if (CManager::GetKeyboard()->GetKeyboardPress(DIK_DOWN))
	//{
	//	m_fY -= 10.0f;
	//}
}
