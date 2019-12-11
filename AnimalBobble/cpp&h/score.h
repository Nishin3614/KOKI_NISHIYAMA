// ----------------------------------------
//
// 試し背景処理の説明[score.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define MAX_SCORE (7)

class CNumber;
// ------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------
class CScore : public CScene
{
public:
	/* 関数 */
	CScore();
	~CScore();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetScore(int nPoint);
	int GetScore(void);
	D3DXVECTOR3 GetPos(void);
	static CScore * Create(CManager::PLAYER playerId,D3DXVECTOR3 pos);	// 作成

protected:

private:
	/* 関数 */
	/* 変数 */
	CNumber *m_pNumber[MAX_SCORE];	// スコア表示
	D3DXVECTOR3 m_pos;	// 位置情報
	CManager::PLAYER m_PlayerId;		// プレイヤー番号
	int m_nScore;	// 得点
};

#endif