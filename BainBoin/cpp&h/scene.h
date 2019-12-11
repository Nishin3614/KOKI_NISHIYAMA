// ----------------------------------------
//
// シーン処理の説明[scene.h]
// Author : Koki Nishiyama
//
// ----------------------------------------

#ifndef _SCENE_H_
#define _SCENE_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "manager.h"
#include "load.h"
#include "collision.h"

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
class CScene
{
public:
	/* 列挙型 */
	// 物の配置順番
	typedef enum
	{
		LAYER_3DOBJECT = 0,
		LAYER_3DPARTICLE,
		LAYER_UI,
		LAYER_PARTICLE,
		LAYER_MAX
	} LAYER;
	// 物の配置順番(仮)
	typedef enum
	{
		ACTOR_BG = 0,
		ACTOR_BLOCK,
		ACTOR_EFFECT,
		ACTOR_EXPLOSION,
		ACTOR_FLOOR,
		ACTRO_GAME_UI,
		ACTOR_GOAL,
		ACTOR_ITEM,
		ACTOR_LINE,
		ACTOR_MARK,
		ACTOR_MESHDOME,
		ACTOR_MESHSPHERE,
		ACTOR_MODEL,
		ACTOR_NUMBER,
		ACTOR_PAUSE_UI,
		ACTOR_PLAYER_ELEMENT_UI,
		ACTOR_PLAYER,
		ACTOR_POLYGON,
		ACTOR_RANK_UI,
		ACTOR_SCENE_THREE,
		ACTOR_SCENE_TWO,
		ACTOR_SCORE,
		ACTOR_X,
		ACTOR_TITLE_UI,
		ACTOR_TIME,
		ACTOR_TOPSCORE,
		ACTOR_TUTORIAL_UI,
		ACTOR_3DCOLLISION,
		ACTOR_3DLINE,
		ACTOR_3DPARTICLE,
		ACTOR_3DEFFECT,
		ACTOR_WAYPOINT,
		ACTOR_MAX
	} ACTOR;

	/* 関数 */
	CScene();
	CScene(ACTOR actor,LAYER layer = LAYER_UI);
	virtual ~CScene();
	virtual void Init(void) = 0;		// 初期化
	virtual void Uninit(void) = 0;		// 終了
	virtual void Update(void) = 0;		// 更新
	virtual void Draw(void) = 0;		// 描画
	static void ReleaseAll(void);		// シーン親子リリース
	static void UpdateAll(void);		// シーン親子更新
	static void DrawAll(void);			// シーン親子描画
	static CScene * GetActor(ACTOR actor, LAYER layer,int nCntScene);	// シーン情報取得
	static int GetMaxLayer(LAYER layer);								// レイヤーの最大数
	void Release(void);	// オブジェクトの破棄
protected:

private:
	/* 関数 */
	static void DeadFragAll(void);		// 死亡フラグが立ったものをリリース
	/* 変数 */
	static CScene * m_pTop[LAYER_MAX];	// 先頭オブジェクトへのポインタ
	static CScene * m_pCur[LAYER_MAX];	// 現在オブジェクトへのポインタ
	static int m_nMaxLayer[LAYER_MAX];	// レイヤー最大数
	CScene * m_pNext;					// 次のオブジェクトへのポインタ
	CScene * m_pPrey;					// 前のオブジェクトへのポインタ
	LAYER m_layer;						// レイヤー番号
	ACTOR m_nActor;						// オブジェクト番号
	int m_nID;							// 自分の番号
	bool m_bDeadFrag;					// 不ラブ
};

#endif