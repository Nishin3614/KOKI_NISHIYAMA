//*************************************************************************************************************
//
// パーティクルの処理[particle.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "3Deffect.h"
#include "manager.h"
//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9	C3DEffect::m_pVtxBuff = NULL;
C3DEffect::TEXTURE_INFO	*C3DEffect::m_pTexInfo = {};
C3DEffect::EFFECT		C3DEffect::m_aEffect[EFFECT_MAX] = {};
int						C3DEffect::m_nNumTextureMax = 0;
const float				C3DEffect::m_cfBaseAngle = D3DX_PI*0.25f;

//-------------------------------------------------------------------------------------------------------------
// 引数ありのコンストラクタ
//-------------------------------------------------------------------------------------------------------------
C3DEffect::C3DEffect() :CScene(ACTOR_3DEFFECT,LAYER_3DPARTICLE)
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
C3DEffect::~C3DEffect()
{
}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
HRESULT C3DEffect::Load(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice;			// デバイスのポインタ
	const char			*sFileName;			// エフェクト情報のテキストファイル名
	PFILETAG			pTextFileName;		// テクスチャファイル名
	PFILETAG			pFilePass;			// ファイルパス

	// 変数の初期化
	pDevice		= CManager::GetRenderer()->GetDevice();		// デバイスの取得
	sFileName	= "data/LOAD/EffectInfo.txt";

	// テクスチャ数を取得
	m_nNumTextureMax = C3DEffect::LoadNumber(sFileName, "NUMBER");

	// テクスチャファイル名のメモリ確保
	pTextFileName = new FILETAG[m_nNumTextureMax];
	// テクスチャ情報のメモリ確保
	m_pTexInfo = new C3DEffect::TEXTURE_INFO[m_nNumTextureMax];

	// テクスチャファイル名の取得
	C3DEffect::LoadTextureFile(sFileName, pTextFileName, m_nNumTextureMax);

	// ファイルパスメモリ確保
	pFilePass = new FILETAG;

	// ファイルパスの取得
	C3DEffect::LoadPass(sFileName, pFilePass, "PASS");

	for (int nCntTexture = 0; nCntTexture < m_nNumTextureMax; nCntTexture++)
	{
		char FileName[64];
		// 文字列の結合
		sprintf(FileName, "%s%s", pFilePass->string, pTextFileName[nCntTexture].string);
		// テクスチャの読み込み
		if (D3DXCreateTextureFromFile(pDevice, FileName, &m_pTexInfo[nCntTexture].pTexture) != D3D_OK)
		{
			MessageBox(NULL, "項目が見つかりません", "警告！", MB_ICONWARNING);
			return E_FAIL;
		}
	}

	// ファイル名の開放処理
	delete[m_nNumTextureMax]pTextFileName;
	pTextFileName = NULL;
	// ファイルパスの開放処理
	delete pFilePass;
	pFilePass = NULL;

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < m_nNumTextureMax; nCntTexture++)
	{// テクスチャ情報の開放
		if (m_pTexInfo[nCntTexture].pTexture != NULL)
		{
			m_pTexInfo[nCntTexture].pTexture->Release();
			m_pTexInfo[nCntTexture].pTexture = NULL;
		}
	}
	// テクスチャ情報のポインタの開放
	delete[m_nNumTextureMax]m_pTexInfo;
	m_pTexInfo = NULL;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Init(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice;					// デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	// 値の初期化
	this->InitValues();
	// 頂点情報作成
	this->MakeVertex(pDevice);
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Update(void)
{
	// 変数定義
	VERTEX_3D *pVtx;				// 頂点情報のポインタ
	C3DEffect::EFFECT *pEffect;		// エフェクトのポインタ
	// ポインタの初期化
	pEffect = &C3DEffect::m_aEffect[0];

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 最大数分ループ
	for (int nCount = 0; nCount < EFFECT_MAX; nCount++, pEffect++, pVtx+=4)
	{
		// 使用フラグがオフの時
		if (pEffect->bUse == false)
		{
			continue;
		}
		// 使用中のやつの更新
		if (this->UpdateLife(pVtx, pEffect) == false)
		{
			continue;
		}

		// 頂点情報の更新
		if (this->UpdateVetex(pVtx, pEffect) == false)
		{
			continue;
		}
		// 移動の更新
		this->UpdateMove(pVtx, pEffect);

	}
	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice;					// デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans, mtxView;			// 計算用マトリックス
	C3DEffect::EFFECT *pEffect;						// エフェクトのポインタ

	pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	pEffect = &m_aEffect[0];						// ポインタの初期化

	// ライティングモード無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zバッファ　有効　無効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// 裏面(左回り)をカリングする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCount = 0; nCount < EFFECT_MAX; nCount++, pEffect++)
	{
		if (pEffect->bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pEffect->mtxWorld);

			// マトリックスビューの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//逆行列
			pEffect->mtxWorld._11 = mtxView._11;
			pEffect->mtxWorld._12 = mtxView._21;
			pEffect->mtxWorld._13 = mtxView._31;
			pEffect->mtxWorld._21 = mtxView._12;
			pEffect->mtxWorld._22 = mtxView._22;
			pEffect->mtxWorld._23 = mtxView._32;
			pEffect->mtxWorld._31 = mtxView._13;
			pEffect->mtxWorld._32 = mtxView._23;
			pEffect->mtxWorld._33 = mtxView._33;

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(
				&mtxRot,
				pEffect->rot.y,
				pEffect->rot.x,
				pEffect->rot.z);
			D3DXMatrixMultiply(
				&pEffect->mtxWorld,
				&pEffect->mtxWorld,
				&mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(
				&mtxTrans,
				pEffect->pos.x,
				pEffect->pos.y,
				pEffect->pos.z);
			D3DXMatrixMultiply(
				&pEffect->mtxWorld,
				&pEffect->mtxWorld,
				&mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pEffect->mtxWorld);

			// 頂点バッファをデータストリームにバインド
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, m_pTexInfo[pEffect->nTexType].pTexture);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}

	//アルファテスト戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//Zバッファ　有効　無効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	// ライティングモード有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// 裏面(左回り)をカリングする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// レンダーステート(通常ブレンド処理)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//-------------------------------------------------------------------------------------------------------------
// 値の初期化
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::InitValues(void)
{
	// 変数宣言
	C3DEffect::EFFECT *pEffect;		// エフェクトのポインタ
	// ポインタの初期化
	pEffect = &C3DEffect::m_aEffect[0];

	for (int nCount = 0; nCount < EFFECT_MAX; nCount++, pEffect++)
	{	// 最大数分ループ
		pEffect->pos			= D3DXVECTOR3(0.0f,0.0f,0.0f);			// 位置
		pEffect->rot			= D3DXVECTOR3(0.0f,0.0f,0.0f);			// 回転量
		pEffect->move			= D3DXVECTOR3(0.0f,0.0f,0.0f);			// 移動量
		pEffect->col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
		pEffect->fRadius		= 0.0f;									// 半径
		pEffect->fAngle			= 0.0f;									// 角度
		pEffect->fRadiusValue	= 0.0f;									// 半径の変化値
		pEffect->fAlphaValue	= 0.0f;									// アルファ値の変化値
		pEffect->nLife			= 0;									// 持ち時間
		pEffect->nTexType		= 0;									// テクスチャの種類
		pEffect->bUse			= false;								// 使用しているかどうか
		pEffect->EffectType		= C3DEffect::EFFECT_TYPE_NONE;			// エフェクトの種類
	}
}

//-------------------------------------------------------------------------------------------------------------
// 頂点の作成
//-------------------------------------------------------------------------------------------------------------
HRESULT C3DEffect::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	VERTEX_3D *pVtx;							// 頂点情報のポインタ

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EFFECT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pVtx += 4)
	{
		SetVartexSize(pVtx, NULL, &nCntEffect);

		// 法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// texture座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点サイズの設定
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::SetVartexSize(VERTEX_3D * pVtx, EFFECT * pEffect, const int * nIndex)
{
	if (nIndex != NULL)
	{
		// 変数宣言
		C3DEffect::EFFECT *pLocalEffect;					// エフェクトのポインタ
		pLocalEffect = &C3DEffect::m_aEffect[*nIndex];		// ポインタの初期化

		// 頂点の設定
		pVtx[0].pos.x = -sinf(m_cfBaseAngle + pLocalEffect->fAngle) * pLocalEffect->fRadius;
		pVtx[0].pos.y = -cosf(m_cfBaseAngle + pLocalEffect->fAngle) * pLocalEffect->fRadius;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = -sinf(m_cfBaseAngle - pLocalEffect->fAngle) * pLocalEffect->fRadius;
		pVtx[1].pos.y = cosf(m_cfBaseAngle - pLocalEffect->fAngle) * pLocalEffect->fRadius;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = sinf(m_cfBaseAngle - pLocalEffect->fAngle) * pLocalEffect->fRadius;
		pVtx[2].pos.y = -cosf(m_cfBaseAngle - pLocalEffect->fAngle) * pLocalEffect->fRadius;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = sinf(m_cfBaseAngle + pLocalEffect->fAngle) * pLocalEffect->fRadius;
		pVtx[3].pos.y = cosf(m_cfBaseAngle + pLocalEffect->fAngle) * pLocalEffect->fRadius;
		pVtx[3].pos.z = 0.0f;
	}
	if (pEffect != NULL)
	{
		// 頂点の設定
		pVtx[0].pos.x = -sinf(m_cfBaseAngle + pEffect->fAngle) * pEffect->fRadius;
		pVtx[0].pos.y = -cosf(m_cfBaseAngle + pEffect->fAngle) * pEffect->fRadius;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = -sinf(m_cfBaseAngle - pEffect->fAngle) * pEffect->fRadius;
		pVtx[1].pos.y = cosf(m_cfBaseAngle - pEffect->fAngle) * pEffect->fRadius;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = sinf(m_cfBaseAngle - pEffect->fAngle) * pEffect->fRadius;
		pVtx[2].pos.y = -cosf(m_cfBaseAngle - pEffect->fAngle) * pEffect->fRadius;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = sinf(m_cfBaseAngle + pEffect->fAngle) * pEffect->fRadius;
		pVtx[3].pos.y = cosf(m_cfBaseAngle + pEffect->fAngle) * pEffect->fRadius;
		pVtx[3].pos.z = 0.0f;
	}
}


//-------------------------------------------------------------------------------------------------------------
// 頂点カラーの設定
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::SetVetexColor(VERTEX_3D * pVtx, EFFECT * pEffect, const int * nIndex)
{
	if (nIndex != NULL)
	{
		// 変数宣言
		C3DEffect::EFFECT *pLocalEffect;					// エフェクトのポインタ
		pLocalEffect = &C3DEffect::m_aEffect[*nIndex];		// ポインタの初期化

		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = pLocalEffect->col;
	}
	if (pEffect != NULL)
	{
		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = pEffect->col;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
C3DEffect * C3DEffect::Create(void)
{
	C3DEffect *p3DEffect = new C3DEffect;
	p3DEffect->Init();
	return p3DEffect;
}

//-------------------------------------------------------------------------------------------------------------
// 削除
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Delete(int nCntIndex)
{
}

//-------------------------------------------------------------------------------------------------------------
// エフェクトの設定
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Set3DEffect(
	EFFECT_TYPE EffectType, int nTexType,
	D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fRadius, int nLife, float fRadiusValue)
{
	// 変数宣言
	C3DEffect::EFFECT *pEffect;			// エフェクトのポインタ
	pEffect = &C3DEffect::m_aEffect[0];	// ポインタの初期化

	// 最大数ループ
	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pEffect++)
	{
		// 使用フラグオフの個体の時
		if (pEffect->bUse == false)
		{
			// 変数定義
			VERTEX_3D *pVtx;		// 頂点ポインタ
			//頂点データの範囲をロックし、頂点バッファへのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点ポインタの更新
			pVtx += nCntEffect * 4;
			// ライフの設定
			pEffect->nLife = nLife;
			// テクスチャ種類の設定
			pEffect->nTexType = nTexType;
			// エフェクト種類の設定
			pEffect->EffectType = EffectType;
			// 移動量の設定
			pEffect->move = move;
			// 半径の設定
			pEffect->fRadius = fRadius;
			// 位置の設定
			pEffect->pos = pos;
			// 頂点位置の設定
			SetVartexSize(pVtx, pEffect);
			// 色の設定
			pEffect->col = col;
			// 頂点カラーの設定
			SetVetexColor(pVtx, pEffect);
			// アルファ変化値の設定
			pEffect->fAlphaValue = pEffect->col.a / pEffect->nLife;
			if (pEffect->nTexType == 1)
			{
				// 半径変化値の設定
				pEffect->fRadiusValue = -50.0f;
			}
			else
			{
				// 半径変化値の設定
				pEffect->fRadiusValue = pEffect->fRadius / pEffect->nLife;
			}
			// 使用フラグをオン
			pEffect->bUse = true;
			//頂点データをアンロック
			m_pVtxBuff->Unlock();
			break;
		}
	}

}

//-------------------------------------------------------------------------------------------------------------
// 番号の読み込み
//-------------------------------------------------------------------------------------------------------------
int C3DEffect::LoadNumber(const char * TxtName, const char * SetItem)
{
	// 変数宣言
	FILE *pFile = NULL;				// ファイルのポインタ
	char cRead[128];				// 読み込み用
	char cComp[128];				// 比較用
	char cEmpty[128];				// 要らないもの用
	int nNumber = -1;				// 個数
	int nCntError = 0;				// エラー用

	// ファイルが開かれていなかったら
	if ((pFile = fopen(TxtName, "r")) == NULL)
	{// メッセージの表示
#ifdef _DEBUG
		MessageBox(NULL, "テキストファイルが見つかりませんでした", "警告！", MB_ICONWARNING);
		return -1;
#endif // _DEBUG
	}
	// スクリプトが存在しているかチェック
	if (!C3DEffect::Existenceofscript(pFile))
	{
#ifdef _DEBUG
		MessageBox(NULL, "[SCRIP]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
		fclose(pFile);
		return -1;
#endif // _DEBUG
	}
	// END_SCRIPTが来るまで
	while (strcmp(cComp, "END_SCRIPT") != 0)
	{// END_SCRIPTまでループ
	 // 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > 1048576)
		{// エラー
			nCntError = 0;
#ifdef _DEBUG
			MessageBox(NULL, "[END_SCRIPT]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
			fclose(pFile);
			return -1;
#endif // _DEBUG

		}
		if (strcmp(cComp, SetItem) == 0)
		{
			sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &nNumber);
			cComp[0] = '\0';
		}
	}
	if (nNumber == -1)
	{
#ifdef _DEBUG
		MessageBox(NULL, "項目が見つかりません", "警告！", MB_ICONWARNING);
		fclose(pFile);
		return -1;
#endif // _DEBUG
	}
	// ファイルを閉じる
	fclose(pFile);

	return nNumber;

	return 0;
}

//-------------------------------------------------------------------------------------------------------------
// パスの読み込み
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::LoadPass(const char * TxtName, PFILETAG sFilePass , const char *ItemName)
{
	// 変数宣言
	FILE *pFile = NULL;				// ファイルのポインタ
	char cRead[128];				// 読み込み用
	char cComp[128];				// 比較用
	char cEmpty[128];				// 要らないもの用
	int nCntError = 0;				// エラー用

									// ファイルが開かれていなかったら
	if ((pFile = fopen(TxtName, "r")) == NULL)
	{// メッセージの表示
#ifdef _DEBUG
		MessageBox(NULL, "テキストファイルが見つかりませんでした", "警告！", MB_ICONWARNING);
		return;
#endif // _DEBUG
	}
	// スクリプトが存在しているかチェック
	if (!C3DEffect::Existenceofscript(pFile))
	{
#ifdef _DEBUG
		MessageBox(NULL, "[SCRIP]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
		fclose(pFile);
		return;
#endif // _DEBUG
	}
	// END_SCRIPTが来るまで
	while (strcmp(cComp, "END_SCRIPT") != 0)
	{
	 // 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > 1048576)
		{// エラー
			nCntError = 0;
#ifdef _DEBUG
			MessageBox(NULL, "[END_SCRIPT]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
			fclose(pFile);
			return;
#endif // _DEBUG

		}
		if (strcmp(cComp, ItemName) == 0)
		{
			sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &sFilePass->string);
			cComp[0] = '\0';
		}
	}

	// ファイルを閉じる
	fclose(pFile);
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャファイル名の読み込み
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::LoadTextureFile(const char * sFileName, PFILETAG TexName,const int &nNumMax)
{
	// 変数宣言
	FILE *pFile = NULL;				// ファイルのポインタ
	char cRead[128];				// 読み込み用
	char cComp[128];				// 比較用
	char cEmpty[128];				// 要らないもの用
	int nNumCount = 0;				// 個数のカウント
	int nCntError = 0;				// エラー用

	// ファイルが開かれていなかったら
	if ((pFile = fopen(sFileName, "r")) == NULL)
	{// メッセージの表示
#ifdef _DEBUG
		MessageBox(NULL, "テキストファイルが見つかりませんでした", "警告！", MB_ICONWARNING);
		return;
#endif // _DEBUG
	}
	// スクリプトが存在しているかチェック
	if (!C3DEffect::Existenceofscript(pFile))
	{
#ifdef _DEBUG
		MessageBox(NULL, "[SCRIP]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
		fclose(pFile);
		return;
#endif // _DEBUG
	}

	// END_SCRIPTが来るまで
	while (strcmp(cComp, "END_SCRIPT") != 0)
	{// END_SCRIPTまでループ
	 // 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > 1048576)
		{// エラー
			nCntError = 0;
#ifdef _DEBUG
			MessageBox(NULL, "[END_SCRIPT]が見つかりません\n終了してください", "警告！", MB_ICONWARNING);
			fclose(pFile);
			return;
#endif // _DEBUG

		}
		if (strcmp(cComp, "FILENAME") == 0)
		{
			if (nNumCount >= nNumMax)
			{
				MessageBox(NULL, "モデル数が合っていません", "警告！", MB_ICONWARNING);
				fclose(pFile);
				return;
			}
			sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &TexName[nNumCount].string);
			nNumCount++;
			cComp[0] = '\0';
		}
	}
	if (nNumCount < nNumMax)
	{
#ifdef _DEBUG
		MessageBox(NULL, "項目が見つかりません", "警告！", MB_ICONWARNING);
		fclose(pFile);
		return;
#endif // _DEBUG
	}
	// ファイルを閉じる
	fclose(pFile);
}

//-------------------------------------------------------------------------------------------------------------
// スクリプトが存在するか判定
//-------------------------------------------------------------------------------------------------------------
bool C3DEffect::Existenceofscript(FILE * pFile)
{
	// 変数宣言
	char cRead[128];	// 読み込み用
	char cComp[128];	// 比較用
	int nCntError = 0;	// エラー用
	while (strcmp(cComp, "SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > 1048576)
		{// エラー
			nCntError = 0;
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------------------------
// ライフの更新
//-------------------------------------------------------------------------------------------------------------
bool C3DEffect::UpdateLife(VERTEX_3D *pVtx, EFFECT * pEffect)
{
	if (pEffect->nLife < 0)
	{
		// ライフが0になったとき
		pEffect->bUse = false;
		return false;
	}
	else
	{
		// ライフを減らすw
		pEffect->nLife--;
		return true;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 頂点の更新
//-------------------------------------------------------------------------------------------------------------
bool C3DEffect::UpdateVetex(VERTEX_3D * pVtx, EFFECT * pEffect)
{
	// 半径を変化させる
	pEffect->fRadius -= pEffect->fRadiusValue;

	if (pEffect->fRadius <= 0.0f)
	{
		pEffect->fRadius = 0.0f;
		pEffect->bUse = false;
		return false;
	}
	else
	{
		SetVartexSize(pVtx, pEffect);
	}

	// アルファ値を変化させる
	pEffect->col.a -= pEffect->fAlphaValue;
	if (pEffect->col.a <= 0.0f)
	{
		pEffect->col.a = 0.0f;
		pEffect->bUse = false;
		return false;
	}
	else
	{
		SetVetexColor(pVtx, pEffect);
	}
	return true;
}

//-------------------------------------------------------------------------------------------------------------
// 移動の更新
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::UpdateMove(VERTEX_3D * pVtx, EFFECT * pEffect)
{
	switch (pEffect->EffectType)
	{
	case C3DEffect::EFFECT_TYPE::EFFECT_TYPE_EXPLOSION:
		// 慣性
		pEffect->move.x += (0 - pEffect->move.x)*0.2f;
		pEffect->move.y += (0 - pEffect->move.y)*0.2f;
		pEffect->move.z += (0 - pEffect->move.z)*0.2f;
		// 位置情報の更新
		pEffect->pos += pEffect->move;
		break;
	case C3DEffect::EFFECT_TYPE::EFFECT_TYPE_SPARK:
		// 位置情報の更新
		pEffect->pos += pEffect->move;
		break;
	default:
		break;
	}
}


