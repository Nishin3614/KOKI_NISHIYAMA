// ------------------------------------------
//
// モデル処理 [model.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------
// インクルードファイル
// ------------------------------------------
#include "manager.h"
#include "model_info.h"
#include "motion.h"

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
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
		D3DXMATRIX & mtx
	);
	// 作成
	static CModel * Create(void);
	// 親情報の設定
	void SetParent(CModel * pModel);
	// モデルの情報渡し処理
	void BindModel(								
		MODEL_INFO &model_info,	// モデルの情報
		MODEL_OFFSET &offset	// モデルのオフセット
	);
	// 位置情報設定
	void SetPos(D3DXVECTOR3 &pos);
	// モーション設定
	void SetMotion(KEY &key,int nMaxFrame);
	// モデル情報の取得
	MODEL_INFO *GetModelInfo(void);
	// モデルの情報の設定
	void SetModelInfo(CONST MODEL_INFO &ModelInfo);
protected:
private:
	MODEL_INFO	m_model_info;			// モデル情報
	D3DXMATRIX	m_mtxWorld;				// ワールドマトリックス
	D3DXMATRIX	m_Falts;
	D3DMATERIAL9		m_matDef;						// マテリアルデータのポインタ

	D3DXVECTOR3	m_size;					// サイズ
	CModel		*m_Parent;				// 親パーツ
	int			m_nFrame;				// フレーム
};
#endif