// ----------------------------------------
//
// 3Dシーン処理の説明[scene_three.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene_three.h"
#include "manager.h"

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CScene_THREE::CScene_THREE()
{
	// 変数の初期化
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_fLengh = 0;
	m_fAngle = 0;
	m_bBillboard = false;
}

// ----------------------------------------
// イニシャライザコンストラクタ
// ----------------------------------------
CScene_THREE::CScene_THREE(ACTOR actor, LAYER layer) : CScene::CScene(actor,layer)
{
	// 変数の初期化
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(100, 0.0f, 100);
	m_fLengh = 0;
	m_fAngle = 0;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CScene_THREE::~CScene_THREE()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CScene_THREE::Init(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポイント
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	D3DXVECTOR3 VecA, VecB;					// ベクトル
	D3DXVECTOR3 Cross0, Cross1_2, Cross3;	// 外積
	int nCntBlock = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	pVtx[0].pos = D3DXVECTOR3(-100, 0, 100);
	pVtx[1].pos = D3DXVECTOR3(100, -50, 100);
	pVtx[2].pos = D3DXVECTOR3(-100, -50,-100);
	pVtx[3].pos = D3DXVECTOR3(0, 0, -0);

	// ベクトル
	VecA = pVtx[1].pos - pVtx[0].pos;
	VecB = pVtx[2].pos - pVtx[0].pos;
	// 外積計算
	Cross0 = CCalculation::Cross_product(VecA, VecB);
	// ベクトル
	VecA = pVtx[2].pos - pVtx[3].pos;
	VecB = pVtx[1].pos - pVtx[3].pos;
	// 外積計算
	Cross3 = CCalculation::Cross_product(VecA, VecB);
	// 正規化
	D3DXVec3Normalize(&Cross0, &Cross0);
	D3DXVec3Normalize(&Cross3, &Cross3);
	// 外積計算
	Cross1_2 = (Cross0 + Cross3) / 2;

	pVtx[0].nor = Cross0;
	pVtx[1].nor = Cross1_2;
	pVtx[2].nor = Cross1_2;
	pVtx[3].nor = Cross3;

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CScene_THREE::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
 		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CScene_THREE::Update(void)
{
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CScene_THREE::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化(単位行列にするため)
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映 //
	// ヨーピッチロールによる回転行列作成(y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
		m_rot.y, m_rot.x, m_rot.z);	// それぞれの回転量

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,					// 2
		&mtxRot);							// 3


	// 位置を反映 //
	// 平行移動行列作成(オフセット)
	D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
		m_pos.x, m_pos.y, m_pos.z);	// それぞれの移動量

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,					// 2
		&mtxTrans);							// 3

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(
		0,
		m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2
		);
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// デバッグ表示
//-------------------------------------------------------------------------------------------------------------
void CScene_THREE::Debug(void)
{
}
#endif // _DEBUG

// ----------------------------------------
// 作成処理
// ----------------------------------------
CScene_THREE * CScene_THREE::Create()
{
	// 変数宣言
	CScene_THREE * pScene_Two;		// シーン3Dクラス

	// メモリの生成(初め->基本クラス,後->派生クラス)
	pScene_Two = new CScene_THREE(ACTOR_SCENE_THREE,LAYER_3DOBJECT);

	// 初期化処理
	pScene_Two->Init();
	
	// 生成したオブジェクトを返す
	return pScene_Two;
}

// ----------------------------------------
// 接している面の高さを取得
// ----------------------------------------
float CScene_THREE::GetHeight(D3DXVECTOR3 pos)
{
	// 変数宣言
	VERTEX_3D *pVtx;						// 頂点情報へのポイント
	D3DXVECTOR3 VecA, VecB;					// ベクトル
	D3DXVECTOR3 Cross0, Cross1, Cross2;	// 外積
	D3DXVECTOR3 CrossPolygon;			// ポリゴンの法線
	D3DXVECTOR3 CrossAns;				// 位置の外積

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	// ベクトル
	VecA = pVtx[1].pos - pVtx[0].pos;
	VecB = pos - pVtx[0].pos;
	// 外積計算
	Cross0 = CCalculation::Cross_product(VecA, VecB);
	// ベクトル
	VecA = pVtx[2].pos - pVtx[1].pos;
	VecB = pos - pVtx[1].pos;
	// 外積計算
	Cross1 = CCalculation::Cross_product(VecA, VecB);
	// ベクトル
	VecA = pVtx[0].pos - pVtx[2].pos;

	VecB = pos - pVtx[2].pos;
	// 外積計算
	Cross2 = CCalculation::Cross_product(VecA, VecB);
	// ポリゴンの法線
	CrossPolygon = pVtx[0].nor;
	VecA = pos - pVtx[0].pos;

	CrossAns = CCalculation::Cross_product(VecA, CrossPolygon);
	// 正規化
	D3DXVec3Normalize(&Cross0, &Cross0);
	D3DXVec3Normalize(&Cross1, &Cross1);
	D3DXVec3Normalize(&Cross2, &Cross2);
	D3DXVec3Normalize(&CrossAns, &CrossAns);
	if (Cross0.y > 0 && Cross1.y > 0 && Cross2.y > 0)
	{
		// ポリゴンの法線
		D3DXVECTOR3 VecAB, VecBC;
		VecA = pos - (m_pos + pVtx[0].pos);
		//D3DXVec3Normalize(&VecA, &VecA);

		float Dot = 0;				// 内積
		float fY = 0;				// 高さ
		// 内積
		Dot = CCalculation::Dot_product(VecA, CrossPolygon);
		// ポリゴンの指定位置の高さ計算
		CrossPolygon = pVtx[0].nor;
		fY = pVtx[0].pos.y + ((-CrossPolygon.x * VecA.x) + (-CrossPolygon.z * VecA.z)) / CrossPolygon.y;
		//fY = pVtx[0].pos.y + ((-CrossPolygon.x * pVtx[0].nor.x) + (-CrossPolygon.z * pVtx[0].nor.z)) / CrossPolygon.y;
		//CDebugproc::Print("[高さ計算]:床の高さ = 床のポリゴンy - ((法線x * 方向x) - (法線z * 方向z)) / 法線y\n");
		//CDebugproc::Print("[高さ計算]:%.3f = %.3f - ((%.3f * %.3f) + (%.3f * %.3f)) / %.3f\n", fY, pVtx[0].pos.y, -CrossPolygon.x, VecA.x, -CrossPolygon.z, VecA.z, CrossPolygon.y);
		//CDebugproc::Print("[プレイヤーの位置]:(%.3f,%.3f,%.3f)\n", pos.x, pos.y, pos.z);
		//CDebugproc::Print("[0]:%.3f,[1]:%.3f,[2]:%.3f\n", m_pos.y + pVtx[0].pos.y, m_pos.y + pVtx[1].pos.y, m_pos.y + pVtx[2].pos.y);
		//CDebugproc::Print("[内積]:%.3f\n", Dot);
		//CDebugproc::Print("[高さ]:%.3f\n", fY);
		//CDebugproc::Print("[面の法線]:(%.3f,%.3f,%.3f)\n", CrossPolygon.x, CrossPolygon.y, CrossPolygon.z);
		//CDebugproc::Print("[位置の外積]:(%.3f,%.3f,%.3f)\n", CrossAns.x, CrossAns.y, CrossAns.z);
		// 頂点データの範囲をアンロック
		m_pVtxBuff->Unlock();
		return fY;
	}
	// 頂点データの範囲をアンロック
	m_pVtxBuff->Unlock();
	return 0.0f;
}

// ----------------------------------------
// 位置の設定処理
// ----------------------------------------
void CScene_THREE::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

// ----------------------------------------
// サイズ設定処理
// ----------------------------------------
void CScene_THREE::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

// ----------------------------------------
// 回転の設定処理
// ----------------------------------------
void CScene_THREE::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}
// ----------------------------------------
// 色の設定処理
// ----------------------------------------
void CScene_THREE::SetCol(D3DXCOLOR col)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// 頂点カラー
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 表示テクスチャー設定処理
// ----------------------------------------
void CScene_THREE::SetTex(D3DXVECTOR2 first, D3DXVECTOR2 last)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// 頂点テクスチャー
	pVtx[0].tex = D3DXVECTOR2(first.x, first.y);
	pVtx[1].tex = D3DXVECTOR2(last.x, first.y);
	pVtx[2].tex = D3DXVECTOR2(first.x, last.y);
	pVtx[3].tex = D3DXVECTOR2(last.x, last.y);
	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// ビルボード状態処理
// ----------------------------------------
void CScene_THREE::SetBillboard(bool bBillboard)
{
	m_bBillboard = bBillboard;
}

// ----------------------------------------
// サイズの取得処理
// ----------------------------------------
D3DXVECTOR3 CScene_THREE::GetSize(void)
{
	return m_size;
}

// ----------------------------------------
// 位置の取得処理
// ----------------------------------------
D3DXVECTOR3 CScene_THREE::GetPos(void)
{
	return m_pos;
}

// ----------------------------------------
// 回転取得
// ----------------------------------------
D3DXVECTOR3 CScene_THREE::GetRot(void)
{
	return m_rot;
}


// ----------------------------------------
// テクスチャー設定処理
// ----------------------------------------
void CScene_THREE::BindTexture(LPDIRECT3DTEXTURE9 p_Tex)
{
	m_pTexture = p_Tex;
}
