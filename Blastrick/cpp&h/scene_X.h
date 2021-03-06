// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Xシーン処理の説明[scene_X.h]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _SCENE_X_H_
#define _SCENE_X_H_	 // ファイル名を基準を決める

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "model_info.h"


class CCollision;

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_X : public CScene
{
public:
	/* 関数 */
	CScene_X();
	CScene_X(ACTOR actor);
	~CScene_X();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	void BindModel(XMODEL const &XModel);	// モデル
	static CScene_X * Create();				// 作成
	// 設定
	void SetPos(D3DXVECTOR3 &pos);			// 位置
	void SetRot(D3DXVECTOR3 &rot);			// 回転
	// 取得
	XMODEL &GetXModel(void);				// Xモデル
	D3DXVECTOR3 &GetPos(void);				// 位置
	D3DXVECTOR3 &GetRot(void);				// 回転
protected:
private:
	XMODEL			m_pXmodel;			// モデル情報
	D3DXMATRIX		m_mtxWorld;			// ワールドマトリックス
	CCollision		*m_pCollision;		// 当たり判定
};

#endif