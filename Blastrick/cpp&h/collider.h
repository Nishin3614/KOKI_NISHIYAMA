// ----------------------------------------
//
// 当たり判定処理の説明[collider.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _COLLIDER_H_
#define _COLLIDER_H_	 // ファイル名を基準を決める

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

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CCollider : public CScene
{
public:
	/* 列挙型 */
	// 管理者
	typedef enum
	{
		OWNER_MYSELF = 0,
		OWNER_YOU,
		OWNER_MAX
	} OWNER;
	/* 関数 */
	CCollider();
	CCollider(ACTOR actor, LAYER layer);
	~CCollider();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	// 設定
	void Set(	
		D3DXMATRIX	*mtx,				// 行列
		D3DXVECTOR3 const &offset,		// オフセット位置
		float		const &fRadius,		// 半径
		int			const &nStartFrame,	// スタート
		int			const &nEndFrame,	// エンド
		int			const &nAttack		// 攻撃力
	);
	// 使用状態
	void SetUse(bool const bUse);
	// 位置設定
	void SetPos(D3DXVECTOR3 const &pos);	
	// 位置取得
	D3DXVECTOR3 GetPos(void);					// 位置
	// キャラクター取得
	int GetCharacter(void);						// キャラクタータイプ
	// 自分の位置からの生成
	static CCollider * Create(
		D3DXVECTOR3	const &pos,					// 位置
		float		const &fRadius,				// 半径
		int			const &nAttack,				// 攻撃力
		int			const &nCharacter,			// キャラクター
		OWNER		const &owner = OWNER_YOU	// 管理者
	);
	// 行列からの生成
	static CCollider * Create(
		D3DXMATRIX	*mtx,						// 行列
		D3DXVECTOR3 const &offset,				// オフセット位置
		float		const &fRadius,				// 半径
		int			const &nStartFrame,			// スタート
		int			const &nEndFrame,			// エンド
		int			const &nAttack,				// 攻撃力
		int			const &nCharacter,			// キャラクター
		OWNER		const &owner = OWNER_MYSELF	// 管理者
	);

protected:
private:
	/* 関数 */
	void OwnerMyself(void);		// 管理者自分
	void OwnerYou(void);		// 管理者使用しているもの
	/* 変数 */
	D3DXMATRIX	*m_mtx;			// 行列
	D3DXVECTOR3 m_offset;		// オフセット位置
	D3DXVECTOR3 m_pos;			// 中心座標
	OWNER		m_owner;		// 管理者
	float		m_fRadius;		// 半径
	int			m_nStartFrame;	// スタート
	int			m_nEndFrame;	// エンド
	int			m_nCntFrame;	// フレームカウント
	int			m_nAttack;		// 攻撃力
	int			m_nCharacter;	// キャラクタータイプ
	bool		m_bUse;			// 有効かどうか
};

#endif