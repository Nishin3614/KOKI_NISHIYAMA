// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// カメラ処理 [camera.cpp]
// Author : KOKI_NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "camera.h"
#include "input.h"
#include "joypad.h"
#include "manager.h"
#include "debugproc.h"
#include "scene_X.h"
#include "player.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// マクロ定義
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define CAMERA_FILE ("data/LOAD/camerainfo.txt")

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 静的変数宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCamera::LOAD	CCamera::m_load[TYPE_MAX] = {};	// 情報保存

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCamera::CCamera()
{
	// カメラの初期化
	m_posV = D3DXVECTOR3(1.0f, 100.0f, 100.0f);
	m_posVDest = m_posV;
	m_posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = m_posR;
	m_posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_move = D3DXVECTOR3(1.0f, 3.0f, 3.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = TYPE_FOLLOW;
	m_fHeight = 500.0f;
	m_fLength = 500.0f;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCamera::~CCamera()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Init(void)
{
	// カメラの初期化
	m_posV = D3DXVECTOR3(1.0f, 100.0f, 100.0f);
	m_posVDest = m_posV;
	m_posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = m_posR;
	m_posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_move = D3DXVECTOR3(1.0f, 3.0f, 3.0f);
	m_rot = D3DXVECTOR3(-0.1f,0.0f,0.0f);
	m_rotDest = m_rot;
	m_rotDiff = D3DVECTOR3_ZERO;
	m_rotOrigin = D3DVECTOR3_ZERO;
	m_type = TYPE_FOLLOW;
	m_fHeight = 350.0f;
	m_fLength = 400.0f;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Uninit(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Update(void)
{
	// 回転
	Rot();
	// タイプ変化
	Type();
	// 注視点軸に回転 //
	// 目的の角度から現在の角度の差
	m_rotDiff =
		m_rotDest - m_rot;
	// 回転上限
	CCalculation::Rot_One_Limit(m_rotDiff.x);
	CCalculation::Rot_One_Limit(m_rotDiff.y);
	CCalculation::Rot_One_Limit(m_rotDiff.z);

	// 角度に慣性
	m_rot += 
		m_rotDiff * 0.1f;
	// 回転上限
	CCalculation::Rot_One_Limit(m_rot.x);
	CCalculation::Rot_One_Limit(m_rot.y);
	CCalculation::Rot_One_Limit(m_rot.z);

	//-------------------- 視点移動 --------------------//
	// 目的地から現在地の距離(視点)
	m_posRDiff =
		m_posRDest - m_posR;

	// 注視点に慣性
	m_posR.x += m_posRDiff.x * 0.1f;
	m_posR.y += m_posRDiff.y * 0.1f;
	m_posR.z += m_posRDiff.z * 0.1f;
	
	//-------------------- 視点移動 --------------------//
	// 目的の視点
	m_posVDest.x =
		m_posR.x + sinf(m_rot.y) * m_fLength;
	m_posVDest.y =
		m_posR.y + cosf(m_rot.x) * m_fHeight;
	m_posVDest.z =
		m_posR.z + cosf(m_rot.y) * m_fLength;

	// 目的地から現在地の距離(注視点)
	m_posVDiff =
		m_posVDest - m_posV;

	// 注視点に慣性
	m_posV.x += m_posVDiff.x * 0.1f;
	m_posV.y += m_posVDiff.y * 0.1f;
	m_posV.z += m_posVDiff.z * 0.1f;

#ifdef _DEBUG
	CDebugproc::Print("回転量X%.3f\n", m_rot.x);
	CDebugproc::Print("回転量X%.3f\n", m_posVDest.y);
	// CDebugproc::Print("カメラの視点x:%f,y:%f,z:%f\n", m_posVDest.x, m_posVDest.y, m_posVDest.z);
	// CDebugproc::Print("カメラの注視点x:%f,y:%f,z:%f\n", m_posR.x, m_posR.y, m_posR.z);
	// CDebugproc::Print("カメラの角度x:%f,y:%f,z:%f\n", m_rot.x, m_rot.y, m_rot.z);
#endif // _DEBUG
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Draw(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CCamera::Load(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntObj = 0;		// シャドウマッピングカウント
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	pFile = fopen(CAMERA_FILE, "r");

	// 開けた
	if (pFile != NULL)
	{
		// エンドスクリプトが来るまでループ
		while (strcmp(cHeadText, "END") != 0)
		{
			// 初期化
			cHeadText[0] = '\0';
			fgets(cRaedText, sizeof(cRaedText), pFile);
			sscanf(cRaedText, "%s", &cHeadText);
			// マテリアルセット来たら
			if (strcmp(cHeadText, "TYPE") == 0)
			{
				sscanf(cRaedText, "%s %d",
					&cDie,
					&m_load[nCntObj].nType
				);
				// エンドマテリアルセットが来るまでループ
				while (strcmp(cHeadText, "END_TYPE") != 0)
				{
					// 初期化
					cHeadText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);
					// 回転情報読み込み
					if (strcmp(cHeadText, "ROT") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&m_load[nCntObj].rot.x,
							&m_load[nCntObj].rot.y,
							&m_load[nCntObj].rot.z);
					}
					// オフセット情報読み込み
					else if (strcmp(cHeadText, "OFFSET") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f", &cDie, &cDie,
							&m_load[nCntObj].offset.x,
							&m_load[nCntObj].offset.y
						);
					}
					// 長さ情報読み込み
					else if (strcmp(cHeadText, "LENGH") == 0)
					{
						sscanf(cRaedText, "%s %s %f", &cDie, &cDie,
							&m_load[nCntObj].fLengh);
					}
					// 長さ情報読み込み
					else if (strcmp(cHeadText, "HEIGHT") == 0)
					{
						sscanf(cRaedText, "%s %s %f", &cDie, &cDie,
							&m_load[nCntObj].fHeight);
					}
				}
				// 上限より下なら
				if (nCntObj < TYPE_MAX)
				{
					// オブジェクトの更新
					nCntObj++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "カメラの読み込みに失敗しました", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 破棄
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Unload(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 情報初期化
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::InfoInit(
	D3DXVECTOR3 const &pos,	// 位置情報
	D3DXVECTOR3 const &rot	// 回転情報
)
{
	// 注視点
	m_posR = pos;
	m_posRDest = m_posR;
	// 視点
	m_posV.x =
		m_posR.x + sinf(m_rot.y) * m_fLength;
	m_posV.y =
		m_posR.y + cosf(m_rot.x) * m_fHeight;
	m_posV.z =
		m_posR.z + cosf(m_rot.y) * m_fLength;
	m_posVDest = m_posV;
	// 回転
	m_rot = rot;
	m_rotDest = m_rot;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 取得
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCamera * CCamera::Get(void)
{
	return nullptr;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
		10000.0f);									// FarZ値(遠いビュー平面のZ値)初期値は1000

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックス //
	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(
		&m_mtxView,	// (カメラマン視点)
		&m_posV,	// カメラの視点
		&m_posR,	// カメラの注目点
		&m_posU);	// カメラの上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラの注視点設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetPosR(
	D3DXVECTOR3 const & pos,	// 位置情報 
	D3DXVECTOR3 const & rot		// 回転情報 
)
{
	// ゲームのカメラの回転の目的地
	m_posRDest.x = pos.x + sinf(rot.y + D3DX_PI) * m_load[m_type].offset.x;
	m_posRDest.y = pos.y + m_load[m_type].offset.y;
	m_posRDest.z = pos.z + cosf(rot.y + D3DX_PI) * m_load[m_type].offset.x;

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラのタイプ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::SetType(TYPE type)
{
	if (m_type == TYPE_FOLLOW)
	{
		m_rotOrigin = m_rotDest;
	}
	if (type == TYPE_FOLLOW)
	{
		m_rotDest = m_rotOrigin;
	}
	if (m_type > TYPE_MAX ||
		m_type < 0)
	{
		CCalculation::Messanger("タイプがおかしいです。");
	}
	m_type = type;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// カメラの方向
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}
//
//// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//// プレイヤー追従
//// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//void CCamera::PlayerFoll(void)
//{
//	/* 変数宣言 */
//	CPlayer * pPlayer = NULL;	// プレイヤー
//	// 情報取得
//	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntLayer++)
//	{
//		if (pPlayer == NULL)
//		{
//			pPlayer = (CPlayer*)CScene::GetActor(CScene::ACTOR_PLAYER, CScene::LAYER_CHARACTER, nCntLayer);	// プレイヤー
//		}
//	}
//	if (pPlayer != NULL)
//	{
//		// ゲームのカメラの回転の目的地
//		m_posR = pPlayer->GetPos();
//		m_posR.y += 50.0f;
//	}
//#ifdef _DEBUG
//	else
//	{
//		CCalculation::Messanger("カメラ内のプレイヤーの情報がありません");
//	}
//#endif // _DEBUG
//	//-------------------- 視点移動 --------------------//
//}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// タイプ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Type(void)
{
	// 長さ
	m_fLength = m_load[m_type].fLengh;
	m_fHeight = m_load[m_type].fHeight;

	/* 変数宣言 */
	CPlayer * pPlayer = NULL;	// プレイヤー
	// 情報取得
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntLayer++)
	{
		if (pPlayer == NULL)
		{
			pPlayer = (CPlayer*)CScene::GetActor(CScene::ACTOR_PLAYER, CScene::LAYER_CHARACTER, nCntLayer);	// プレイヤー
		}
	}
	// プレイヤーがない場合関数を抜ける
	if (pPlayer == NULL)
	{
		return;
	}

	switch (m_type)
	{
	case TYPE_FOLLOW:
		break;
	case TYPE_FRONT:
		// 回転を設定
		m_rotDest = m_load[m_type].rot + pPlayer->GetRot();

		break;
	case TYPE_BACK:
		// 回転を設定
		m_rotDest = m_load[m_type].rot + pPlayer->GetRot();

		break;
	case TYPE_RIGHT:
		// 回転を設定
		m_rotDest = m_load[m_type].rot + pPlayer->GetRot();

		break;
	case TYPE_LEFT:
		// 回転を設定
		m_rotDest = m_load[m_type].rot + pPlayer->GetRot();

		break;
	case TYPE_SLASH:
		// 回転を設定
		m_rotDest = m_load[m_type].rot + pPlayer->GetRot();

		break;
	case TYPE_SLASH_NEAR:
		// 回転を設定
		m_rotDest = m_load[m_type].rot + pPlayer->GetRot();

		break;
	case TYPE_SLASH_FAR:
		// 回転を設定
		m_rotDest = m_load[m_type].rot + pPlayer->GetRot();

		break;

	default:
		break;
	}

	// 回転上限
	CCalculation::Rot_One_Limit(m_rotDest.x);
	CCalculation::Rot_One_Limit(m_rotDest.y);
	CCalculation::Rot_One_Limit(m_rotDest.z);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 回転
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Rot(void)
{
	// ジョイパッド用 //
	// パッド用 //
	int nValueH, nValueV;	// ゲームパッドのスティック情報の取得用
	float fRot;			// 移動速度

	// ジョイパッドの処理 //
	if (CManager::GetJoy() != NULL)
	{
	// ゲームパッドのスティック情報を取得
		CManager::GetJoy()->GetStickRight(0, &nValueH, &nValueV);
		if (nValueH != 0 || nValueV != 0)
		{
			// y回転の角度を計算
			if (nValueH < 0)
			{
				fRot = abs(nValueH) / 1024.0f * 0.03f;
				m_rotDest.y -= fRot;

			}
			else if (nValueH > 0)
			{
				fRot = abs(nValueH) / 1024.0f * 0.03f;
				m_rotDest.y += fRot;

			}
			// x回転の角度を計算
			if (nValueV < 0)
			{
				fRot = abs(nValueV) / 1024.0f * 0.03f;
				m_rotDest.x -= fRot;

			}
			else if (nValueV > 0)
			{
				fRot = abs(nValueV) / 1024.0f * 0.03f;
				m_rotDest.x += fRot;

			}
			// -3.14〜3.14に回転差分を固定させる
			CCalculation::Rot_One_Limit(m_rotDest.y);
			// -3.14〜3.14に回転差分を固定させる
			CCalculation::Rot_One_Limit(m_rotDest.x);
		}
	}
	// 注視点軸に回転 //
	// 半時計回り
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_RIGHT))
	{
		// 加算
		m_rotDest.y += 0.03f;
		// -3.14〜3.14に回転差分を固定させる
		CCalculation::Rot_One_Limit(m_rotDest.y);
	}

	// 時計回り
	else if (CManager::GetKeyboard()->GetKeyboardPress(DIK_LEFT))
	{
		// 減算
		m_rotDest.y -= 0.03f;
		// -3.14〜3.14に回転差分を固定させる
		CCalculation::Rot_One_Limit(m_rotDest.y);
	}
	// 上
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_UP))
	{
		// 加算
		m_rotDest.x += 0.03f;
		// -3.14〜3.14に回転差分を固定させる
		CCalculation::Rot_One_Limit(m_rotDest.x);
	}
	// 下
	if (CManager::GetKeyboard()->GetKeyboardPress(DIK_DOWN))
	{
		// 加算
		m_rotDest.x -= 0.03f;
		// -3.14〜3.14に回転差分を固定させる
		CCalculation::Rot_One_Limit(m_rotDest.x);
	}
	// x回転の限界地点
	if (m_rotDest.x > -0.1f)
	{
		m_rotDest.x = -0.1f;
	}
	if (m_rotDest.x < -3)
	{
		m_rotDest.x = -3;
	}

}
