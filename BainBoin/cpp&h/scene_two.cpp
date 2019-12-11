// ----------------------------------------
//
// シーン処理の説明[scene_two.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene_two.h"
#include "manager.h"

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CScene_TWO::CScene_TWO()
{
	// 変数の初期化
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_rot = 0;
	m_fLengh = 0;
	m_fAngle = 0;
}

// ----------------------------------------
// イニシャライザコンストラクタ
// ----------------------------------------
CScene_TWO::CScene_TWO(ACTOR actor, LAYER layer) : CScene::CScene(actor,layer)
{
	// 変数の初期化
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	m_rot = 0;
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_fLengh = 0;
	m_fAngle = 0;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CScene_TWO::~CScene_TWO()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CScene_TWO::Init(void)
{
	// 変数宣言
	VERTEX_2D *pVtx;				// 頂点情報
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	// 三角関数を使い斜めの長さを求める
	m_fLengh = sqrtf((m_size.x / 2) * (m_size.x / 2) + (m_size.y / 2) * (m_size.y / 2));
	m_fAngle = atan2f((m_size.x / 2),(m_size.y / 2));

	// 頂点座標
	pVtx[0].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, cosf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, cosf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + m_rot) * m_fLengh, cosf(-m_fAngle + m_rot) * m_fLengh, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle + m_rot) * m_fLengh, cosf(m_fAngle + m_rot) * m_fLengh, 0.0f);
	// 頂点の大きさ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// 頂点テクスチャー
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CScene_TWO::Uninit(void)
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
void CScene_TWO::Update(void)
{
	// 三角関数を使い斜めの長さを求める
	m_fLengh = sqrtf((m_size.x / 2) * (m_size.x / 2) + (m_size.y / 2) * (m_size.y / 2));
	m_fAngle = atan2f((m_size.x / 2), (m_size.y / 2));
	/*
	float s = atanf(m_fLengh);
	float b = sinf(m_rot - D3DX_PI / 2 - D3DX_PI / 4);
	float c = sqrtf(1 * 1 + 2 * 2);
	D3DXVECTOR3 d = D3DXVECTOR3(sinf(m_rot - D3DX_PI / 2 - D3DX_PI / 4) * m_fLengh, cosf(m_rot - D3DX_PI / 2 - D3DX_PI / 4) * m_fLengh, 0.0f);
	*/

	// 変数宣言
	VERTEX_2D *pVtx;				// 頂点情報

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
 	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// 頂点座標
	pVtx[0].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, cosf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, cosf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + m_rot) * m_fLengh, cosf(-m_fAngle + m_rot) * m_fLengh, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle + m_rot) * m_fLengh, cosf(m_fAngle + m_rot) * m_fLengh, 0.0f);
	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CScene_TWO::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();


	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(
		0,
		m_pVtxBuff,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);
	// ポリゴン描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CScene_TWO * CScene_TWO::Create()
{
	// 変数宣言
	CScene_TWO * pScene_Two;		// シーン2Dクラス

	// メモリの生成(初め->基本クラス,後->派生クラス)
	pScene_Two = new CScene_TWO(ACTOR_SCENE_TWO,LAYER_UI);

	// 初期化処理
	pScene_Two->Init();
	
	// 生成したオブジェクトを返す
	return pScene_Two;
}

// ----------------------------------------
// 長さ処理
// ----------------------------------------
void CScene_TWO::SetLengh(D3DXVECTOR2 dist)
{
	// 変数宣言
	VERTEX_2D *pVtx;				// 頂点情報

	// 三角関数を使い斜めの長さを求める
	m_fLengh = sqrtf((dist.x) * (dist.x) + (dist.y) * (dist.y));
	m_fAngle = atan2f((dist.x), (dist.y));

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	// 頂点座標
	pVtx[0].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, cosf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, cosf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(sinf(-D3DX_PI / 2 + m_rot) * dist.x, cosf(-D3DX_PI / 2 + m_rot) * dist.x, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(sinf(D3DX_PI / 2 + m_rot) * dist.x, cosf(D3DX_PI / 2 + m_rot) * dist.x, 0.0f);

	// アンロック
	m_pVtxBuff->Unlock();

}

// ----------------------------------------
// サイズ設定処理
// ----------------------------------------
void CScene_TWO::SetSize(D3DXVECTOR2 size)
{
	m_size = size;
	// 三角関数を使い斜めの長さを求める
	m_fLengh = sqrtf((m_size.x) * (m_size.x) + (m_size.y) * (m_size.y));
	m_fAngle = atan2f((m_size.x), (m_size.y));
}

// ----------------------------------------
// 位置情報設定処理
// ----------------------------------------
void CScene_TWO::SetPosition(D3DXVECTOR3 pos)
{
	// 変数宣言
	VERTEX_2D *pVtx;	// 頂点情報
	// 位置情報の代入
	m_pos = pos;
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// 頂点座標
	pVtx[0].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, cosf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, cosf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + m_rot) * m_fLengh, cosf(-m_fAngle + m_rot) * m_fLengh, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle + m_rot) * m_fLengh, cosf(m_fAngle + m_rot) * m_fLengh, 0.0f);
	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// ゲージサイズ設定処理
// ----------------------------------------
void CScene_TWO::SetGaugeSize(D3DXVECTOR2 size)
{
	m_size = size;
	// 三角関数を使い斜めの長さを求める
	m_fLengh = sqrtf((m_size.x) * (m_size.x) + (m_size.y) * (m_size.y));
	m_fAngle = atan2f((m_size.x), (m_size.y));

	// 変数宣言
	VERTEX_2D *pVtx;				// 頂点情報

									// 三角関数を使い斜めの長さを求める

									// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	// 頂点座標
	pVtx[0].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, cosf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, cosf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + m_rot) * m_fLengh, 0.0f, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle + m_rot) * m_fLengh, 0.0f, 0.0f);

	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 回転情報取得処理
// ----------------------------------------
void CScene_TWO::SetRot(float frot)
{
	// 変数宣言
	VERTEX_2D *pVtx;	// 頂点情報
						// 回転情報の代入
	m_rot = frot;
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// 頂点座標
	pVtx[0].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, cosf(m_fAngle - D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, cosf(-m_fAngle + D3DX_PI + m_rot) * m_fLengh, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(sinf(-m_fAngle + m_rot) * m_fLengh, cosf(-m_fAngle + m_rot) * m_fLengh, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(sinf(m_fAngle + m_rot) * m_fLengh, cosf(m_fAngle + m_rot) * m_fLengh, 0.0f);
	// アンロック
	m_pVtxBuff->Unlock();

}

// ----------------------------------------
// 色の設定処理
// ----------------------------------------
void CScene_TWO::SetCol(D3DXCOLOR col)
{
	// 変数宣言
	VERTEX_2D *pVtx;	// 頂点情報

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
void CScene_TWO::SetTex(D3DXVECTOR2 first, D3DXVECTOR2 last)
{
	// 変数宣言
	VERTEX_2D *pVtx;	// 頂点情報

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
// サイズ情報取得処理
// ----------------------------------------
D3DXVECTOR2 CScene_TWO::GetSize(void)
{
	return m_size;
}

// ----------------------------------------
// 位置情報取得処理
// ----------------------------------------
D3DXVECTOR3 CScene_TWO::GetPosition(void)
{
	return m_pos;
}

// ----------------------------------------
// 長さ情報取得処理
// ----------------------------------------
float CScene_TWO::GetLengh(void)
{
	return m_fLengh;
}

// ----------------------------------------
// テクスチャー設定処理
// ----------------------------------------
void CScene_TWO::BindTexture(LPDIRECT3DTEXTURE9 p_Tex)
{
	m_pTexture = p_Tex;
}
