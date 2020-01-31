// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// シーン処理の説明[scene.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef _SCENE_H_
#define _SCENE_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "manager.h"
#include "load.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene
{
public:
	/* 列挙型 */
	// 物の配置順番
	typedef enum
	{
		LAYER_3DOBJECT = 0,
		LAYER_CHARACTER,
		LAYER_3DPARTICLE,
		LAYER_COLLISION,
		LAYER_UI,
		LAYER_PARTICLE,
		LAYER_MAX
	} LAYER;
	// 物の配置順番(仮)
	typedef enum
	{
		ACTOR_BG = 0,
		ACTOR_BALL,
		ACTOR_BGDOME,
		ACTOR_BLOCK,
		ACTOR_BOXCOLLIDER,
		ACTOR_CHARACTER,
		ACTOR_COLLIDER,
		ACTOR_COLLISION,
		ACTOR_EFFECT,
		ACTOR_ENEMY,
		ACTOR_EXPLOSION,
		ACTOR_FLOOR,
		ACTOR_FORCELINE,
		ACTRO_GAME_UI,
		ACTOR_3DLINE,
		ACTOR_ITEM,
		ACTOR_MARK,
		ACTOR_MESHDOME,
		ACTOR_MESHMANAGER,
		ACTOR_MESHOBIT,
		ACTOR_MESHSPHERE,
		ACTOR_MESHWALL,
		ACTOR_MODEL,
		ACTOR_NUMBER,
		ACTOR_PAUSE_UI,
		ACTOR_PLAYER_ELEMENT_UI,
		ACTOR_PLAYER,
		ACTOR_POLYGON,
		ACTOR_RANK_UI,
		ACTOR_ROTHUMAN,
		ACTOR_SCENE_THREE,
		ACTOR_SCENE_TWO,
		ACTOR_SCORE,
		ACTOR_STATEGAUGE,
		ACTOR_STATICOBJ,
		ACTOR_X,
		ACTOR_TIME,
		ACTOR_TITLE_UI,
		ACTOR_TUTORIAL_UI,
		ACTOR_2DGAUGE,
		ACTOR_3DCOLLISION,
		ACTOR_3DEFFECT,
		ACTOR_3DPARTICLE,
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
#ifdef _DEBUG
	virtual void Debug(void) = 0;		// デバッグ用
#endif // _DEBUG

	static void ReleaseAll(void);		// シーン親子リリース
	static void UpdateAll(void);		// シーン親子更新
	static void DrawAll(void);			// シーン親子描画
	static CScene * GetActor(ACTOR actor, LAYER layer,int nCntScene);	// オブジェクト情報取得
	static CScene * GetScene(LAYER layer, int nCntScene);				// シーン情報取得
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
	bool m_bDeadFrag;					// フラグ
};

#endif