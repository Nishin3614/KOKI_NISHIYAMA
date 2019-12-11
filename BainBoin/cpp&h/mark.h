// ----------------------------------------
//
// 目印処理の説明[mark.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _MARK_H_
#define _MARK_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "main.h"
#include "scene_two.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define MAX_MARK (2)

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CMark : public CScene
{
public:
	/* 関数 */
	CMark();
	CMark(CScene::ACTOR actor);
	~CMark();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR2 size);			// 位置・サイズ設定
	void SetPosGoal(D3DXVECTOR3 pos, D3DXVECTOR2 size);		// 目標の位置・サイズ設定
	D3DXVECTOR3 GetPos(void);								// 位置情報取得
	D3DXVECTOR2 GetSize(void);								// サイズ情報取得
	static HRESULT Load(void);
	static void UnLoad(void);
	static CMark * Create(D3DXVECTOR3 pos,D3DXVECTOR2 size,CScene::ACTOR actor);	// 作成
protected:

private:
	/* 関数 */
	void Animation(int nCnt);				// アニメーション管理

	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex[MAX_MARK];
	D3DXVECTOR3 m_pos = {};			// 位置情報
	D3DXVECTOR3 m_posGoal = {};		// 目標の位置情報
	D3DXVECTOR2 m_size = {};			// サイズ情報
	D3DXVECTOR2 m_sizeGoal = {};		// 目標のサイズ情報
	int m_nHorizonAnim[MAX_MARK];		// 水平のアニメーションパターン
	int m_nVirticalAnim[MAX_MARK];		// 垂直のアニメーションパターン
	int m_nCntAnim[MAX_MARK];			// アニメーションカウント
	CScene_TWO *m_aScene_Two[MAX_MARK];
};

#endif