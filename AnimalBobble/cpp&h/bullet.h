// ------------------------------------------------------------------------------------------
//
// 弾処理の説明[bullet.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_	 // ファイル名を基準を決める

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------
#include "scene_two.h"
#include "array.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------
#define BULLETSIZE (50.0f)	// 弾のサイズ

// ------------------------------------------------------------------------------------------
//
// 前置クラス
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------
class CBullet : public CScene_TWO
{
public:
	/* 列挙型 */
	// タイプ
	typedef enum
	{
		TYPE_BEE = 0,	// ハチ
		TYPE_CAT,		// 猫
		TYPE_FISH,		// 魚
		TYPE_HAWK,		// 鷹
		TYPE_SNAKE,		// 蛇
		TYPE_MAX
	} TYPE;
	// 方向
	typedef enum
	{
		DIRECTION_NORTHWEST = 0,// 北西
		DIRECTION_NORTHEAST,	// 北東
		DIRECTION_EAST,			// 東
		DIRECTION_SOUTHEAST,	// 南東
		DIRECTION_SOUTHWEST,	// 南西
		DIRECTION_WEST,			// 西
		DIRECTION_MAX
	} DIRECTION;
	// 状態
	typedef enum
	{
		STATE_BOX = 0,	// 箱
		STATE_PLAYER,	// プレイヤー
		STATE_OUT,		// 放出
		STATE_END,		// 消された
		STATE_MAX
	} STATE;
	
	/* 関数 */
	CBullet();
	~CBullet();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos);			// 位置
	void SetPosGoal(D3DXVECTOR3 posgoal);	// 目的の位置まで
	void SetRot(float frot);				// 回転情報
	void SetState(STATE state);				// 状態
	CManager::PLAYER GetPlayerId(void);		// プレイヤー番号
	STATE GetState(void);					// 状態
	D3DXVECTOR3 GetPos(void);				// 位置
	static CBullet * Create(				// 作成
		CManager::PLAYER PlayerId,
		D3DXVECTOR3 pos,
		TYPE nType);	
	static CBullet * LoadCreate(			// 読み込んだものを作成
		CManager::PLAYER PlayerId,
		int nMode, 
		int nStage);	
	static HRESULT Load(void);				// 読み込み
	static void LoadStage(void);			// ステージ読み込み
	static void UnLoad(void);				// 読み込んだ情報を破棄
	static int GetAllBullet(void);			// 弾の総数
protected:

private:
	/* 関数 */
	void Box(void);								// ボックス待機中
	void Player(void);							// プレイヤー待機中
	void Goout(void);							// すでに打った時の関数
	void DownAnim(void);						// 落ちるアニメーション
	void Collision(void);						// 当たり判定
	void Collision_After(void);					// 当たった後の処理
	void DownOne(void);							// 落ちる処理
	void ConectDelete(CBullet * bulletOri);		// 連鎖削除
	void ConectDown(CBullet * bulletCom);		// 連鎖落ち
	void Delete(void);							// 削除処理
	void Dead(void);							// ゲームオーバー
	void BoxToPlayer(void);						// プレイヤーの位置へ
	bool Existence(								// 存在しているかどうか
		CBullet * bulletOri,		// 弾の比較元
		CBullet*bulletCom,			// 弾の比較先
		DIRECTION direction);		// 方向
	bool BulletJudg(							// 条件判定
		CBullet * pBulletOrigin,	// 弾の比較元
		CBullet * pBulletComp,		// 弾の比較先
		CManager::PLAYER plyerid,	// プレイヤー番号
		CBullet::STATE state,		// 弾の状態
		bool bJubg					// 判定
	);
	
	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex[TYPE_MAX];	// テクスチャー情報
	static int m_nCntJudg;						// つながっている数
	static int m_nCntIdMax;						// ナンバー総数
	/* 一人用 */
	static char m_cFileName[ONEPLAYER_STAGE][128];	// ファイル名
	static int m_nLineLoad[ONEPLAYER_STAGE][MAX_ARRAY_LINE * MAX_ARRAY_COLUMN];		// 行番号保存
	static int m_nColumnLoad[ONEPLAYER_STAGE][MAX_ARRAY_LINE * MAX_ARRAY_COLUMN];	// 列番号保存
	static int m_nType[ONEPLAYER_STAGE][MAX_ARRAY_LINE * MAX_ARRAY_COLUMN];			// 種類番号保存
	static int m_nAllLoad[ONEPLAYER_STAGE];	// 読み込む個数
	static int m_nDownCntLoad[ONEPLAYER_STAGE];						// 落ちるカウント
	static int m_nDownCnt[CManager::PLAYER_MAX];					// 落ちるカウント
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posgoal;						// 目標地点
	D3DXVECTOR3 m_posdif;						// 目標地点と現在地の距離
	D3DXVECTOR3 m_move;							// 移動量
	TYPE	m_type;								// タイプ
	STATE	m_state;							// 状態
	CManager::PLAYER m_PlayerId;				// プレイヤー番号
	float	m_fRot;								// 回転量
	int		m_nLife;							// 体力
	int		m_nLine;							// 行番号				
	int		m_nColumn;							// 列番号
	int		m_nCntMove;							// 移動カウント
	bool	m_bJudg;							// 判定されたかどうか
	bool	m_bDown;							// 落ちるものかどうか
	bool	m_bMove;							// 弾が動いているかどうか
	bool m_bShake;											// 揺れるカウント
};
#endif

