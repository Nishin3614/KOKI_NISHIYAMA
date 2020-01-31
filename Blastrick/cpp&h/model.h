// ----------------------------------------------------------------------------------------------------
//
// モデル処理 [model.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------
#include "manager.h"
#include "model_info.h"

// ----------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------
class CModel
{
public:
	/* 関数 */
	CModel();
	~CModel();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(
		D3DXMATRIX & mtx,
		float & fAlpha
	);
	// 作成
	static CModel * Create(void);											
	// モデルの情報渡し処理
	void BindModel(								
		XMODEL const &xModel,				// モデルの情報
		MODEL_OFFSET const &offset			// モデルのオフセット
	);
	// 設定
	void SetParent(CModel * pModel);		// 親情報
	void SetPos(D3DXVECTOR3 &pos);			// 位置情報
	void SetMotion(KEY &key,int nMaxFrame);	// モーション設定
	// 取得
	XMODEL &GetXModel(void);				// モデル情報
	D3DXMATRIX	&GetMtx(void);				// 行列情報
	D3DXVECTOR3 &GetPos(void);				// 位置情報
	D3DXVECTOR3	&GetRot(void);				// 回転情報
protected:
private:
	XMODEL				m_Xmodel;			// xモデル情報
	D3DXMATRIX			m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3			m_size;				// サイズ
	int					m_nFrame;			// フレーム
	CModel				*m_Parent;			// 親パーツ
};
#endif