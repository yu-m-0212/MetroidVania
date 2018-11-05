//-------------------------------------------------------------------
//エフェクト
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Effect.h"
#include  "Task_Effect.h"
#include  "Task_Player.h"

using namespace ML;
namespace  Task_Effect
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->effectImage = "effectImage";
		DG::Image_Create(this->effectImage,"./data/image/effect.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->effectImage);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->state_effect = Non;			//状態管理
		this->limit_erase = 0;				//時間消滅まで
		this->dist = 0.0f;					//回転する際の中心からの距離
		this->angle = 0.0f;					//角度
		this->center = Vec2(0, 0);			//回転軸
		this->num_bubble = 0;				//泡の大きさ
		this->interval_bubble = 16;			//泡の揺れ周期
		this->wide_bubble = 5.0f;			//泡の揺れ幅
		this->limit_erase_hit_shot = 18;	//時間消滅までヒットショット
		this->limit_erase_barrier = 15;		//時間消滅までバリア
		this->limit_erase_defeat = 24;		//時間消滅まで撃破
		this->limit_erase_heal = 24;		//時間消滅まで回復
		this->limit_erase_bubble = 600;		//時間消滅まで泡
		this->limit_erase_appear = 180;		//時間消滅まで登場
		this->speed_surfacing = 3.0f;		//速度浮上
		this->render2D_Priority[1] = 0.3f;	//描画順
		this->choice_state = -1;			//外部から状態を指定する際、使用
		Effect* eff = new Effect();			//メソッド呼び出し

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//状態が未指定なら一度だけ指定する
		if (this->state_effect == Non)
		{
			switch (this->choice_state)
			{
			default:
			case -1:	//-1は初期化値
			case 0:		//未指定
				break;
			case 1:		//ヒットショット
				this->state_effect = Hit_Shot;
				this->limit_erase = this->limit_erase_hit_shot;
				//左右によって角度を修正する
				if (this->angle_LR == Left)
				{
					this->angle = this->angle - 135.0f;
				}
				break;
			case 2:		//バリア
				this->state_effect = Barrier;
				this->limit_erase = this->limit_erase_barrier;
				break;
			case 3:
				this->state_effect = Defeat;
				this->limit_erase = this->limit_erase_defeat;
				break;
			case 4:		//回復
				this->state_effect = Heal;
				this->limit_erase = this->limit_erase_heal;
				break;
			case 5:		//泡
				this->state_effect = Bubble;
				//三種類の中から指定する
				this->num_bubble = rand() % 3;
				//描画角度
				this->angle = float(rand() % 360);
				//消滅までの時間
				this->limit_erase = this->limit_erase_bubble;
				break;
			case 6:		//登場
				this->state_effect = Appear;
				this->limit_erase = this->limit_erase_appear;
				break;
			}
		}
		//ポーズ
		if (ge->pause) { return; }
		this->moveCnt++;
		this->animCnt++;
		//状態毎の行動
		this->Move();
		//移動
		this->pos += this->moveVec;
		//限界の時間を迎えたら消滅
		if (this->moveCnt >= this->limit_erase)
		{
			//消滅申請
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo  di = this->Anim();
		di.draw.Offset(this->pos);
		//スクロール対応
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Rotation(this->res->effectImage, this->angle, ML::Vec2(float(di.draw.w / 2), float(di.draw.h / 2)));
		DG::Image_Draw(this->res->effectImage, di.draw, di.src,di.color);
	}
	//-------------------------------------------------------------------
	BChara::DrawInfo Object::Anim()
	{
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		//デフォルトカラーを宣言
		Color dc(1, 1, 1, 1);
		//各エフェクトをテーブルで用意する
		BChara::DrawInfo imageTable[]
		{
			{Box2D(-96, -64, 192, 128),Box2D(   0,   0, 192, 128),dc},//ストンプ着地の衝撃1		[ 0]
			{Box2D(-96, -64, 192, 128),Box2D(   0, 128, 192, 128),dc},//ストンプ着地の衝撃2		[ 1]
			{Box2D(-96, -64, 192, 128),Box2D(   0, 256, 192, 128),dc},//ストンプ着地の衝撃3		[ 2]
			{Box2D(-96, -64, 192, 128),Box2D( 192,   0, 192, 128),dc},//パンチ風切り1				[ 3]
			{Box2D(-96, -64, 192, 128),Box2D( 192, 128, 192, 128),dc},//パンチ風切り2				[ 4]
			{Box2D(-96, -64, 192, 128),Box2D( 192, 256, 192, 128),dc},//パンチ風切り3				[ 5]
			{Box2D(-96, -64, 192, 128),Box2D( 384,   0, 192, 128),dc},//パンチの衝撃1				[ 6]
			{Box2D(-96, -64, 192, 128),Box2D( 384, 128, 192, 128),dc},//パンチの衝撃2				[ 7]
			{Box2D(-96, -64, 192, 128),Box2D( 384, 256, 192, 128),dc},//パンチの衝撃3				[ 8]
			{Box2D(-96, -64, 192, 128),Box2D( 576,   0, 192, 128),dc},//遺体から回復1				[ 9]
			{Box2D(-96, -64, 192, 128),Box2D( 576, 128, 192, 128),dc},//遺体から回復2				[10]
			{Box2D(-96, -64, 192, 128),Box2D( 576, 256, 192, 128),dc},//遺体から回復3				[11]
			{Box2D(-96, -96, 192, 192),Box2D( 768,   0, 192, 192),dc},//エネミー爆散1				[12]
			{Box2D(-96, -96, 192, 192),Box2D( 768, 192, 192, 192),dc},//エネミー爆散2				[13]
			{Box2D(-96, -96, 192, 192),Box2D( 768, 384, 192, 192),dc},//エネミー爆散3				[14]
			{Box2D(-96, -96, 192, 192),Box2D(1344,   0, 192, 192),dc},//衝撃波1					[15]
			{Box2D(-96, -96, 192, 192),Box2D(1344, 192, 192, 192),dc},//衝撃波2					[16]
			{Box2D(-96, -96, 192, 192),Box2D(1344, 384, 192, 192),dc},//衝撃波3					[17]
			{Box2D(-96, -96, 192, 192),Box2D(1344, 576, 192, 192),dc},//衝撃波4					[18]
			{Box2D(-96, -96, 192, 192),Box2D(1344, 768, 192, 192),dc},//衝撃波5					[19]
			{Box2D(-48, -48,  96,  96),Box2D(1536,   0,  96,  96),ML::Color(0.3f,1,1,1)},//泡1	[20]
			{Box2D(-48, -48,  96,  96),Box2D(1536,  96,  96,  96),ML::Color(0.3f,1,1,1)},//泡2	[21]
			{Box2D(-48, -48,  96,  96),Box2D(1536, 192,  96,  96),ML::Color(0.3f,1,1,1)},//泡3	[22]
		};
		//返す変数を用意
		BChara::DrawInfo  rtv;
		//各アニメーションの際、この変数にanimCntを入れ計算を行う
		int effectCnt = 0;
		//状態ごとの描画処理
		switch (this->state_effect)
		{
		//アニメーションを付ける場合
		//切り替わるフレーム数
		//パターン数
		//effectCntに対象の要素番号を足して返す
		case Unnon:
			break;
		case Hit_Shot:
			effectCnt = this->animCnt / 6;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 6];
			break;
		case Barrier:
			effectCnt	= this->animCnt / 3;
			effectCnt %= 5;
			rtv = imageTable[effectCnt+15];
			break;
		case Heal:
			effectCnt	= this->animCnt / 8;
			effectCnt %= 3;
			rtv = imageTable[effectCnt+9];
			break;
		case Defeat:
			effectCnt = this->animCnt / 8;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 12];
			break;
		case Bubble:
			rtv = imageTable[this->num_bubble + 20];
			break;
		case Appear:
			effectCnt = this->animCnt / 8;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 9];
			rtv.color = ML::Color(1, 0, 0, 1);
			break;
		}
		//	向きに応じて画像を左右反転する
		if (this->angle_LR == Left)
		{
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
		}
		return rtv;
	}
	//エフェクトを生成する（角度を指定しない）
	//引数	：	（状態,座標）
	void Object::Create_Effect(const int& choice_, const ML::Vec2&pos_)
	{
		auto eff = Task_Effect::Object::Create(true);
		eff->choice_state = choice_;
		eff->pos = pos_;
	}
	//エフェクトを生成する（角度を指定する）
	//引数	：	（状態,座標,角度,向き）
	void Object::Create_Effect(const int& choice_, const ML::Vec2& pos_, const float& angle_, const BChara::Angle_LR& angle_lr_)
	{
		//ポインタを宣言して確実に代入すること
		auto eff = Task_Effect::Object::Create(true);
		eff->choice_state = choice_;
		eff->pos = pos_;
		eff->angle = angle_;
		eff->angle_LR = angle_lr_;
	}
	//状態ごとに行動を指定する
	void Object::Move()
	{
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		switch (this->state_effect)
		{
		default:
			break;
		case Non:
			break;
		case Heal:
			if (nullptr == pl) { return; }
			this->moveVec = pl->moveVec;
			if (pl->CheckFront_LR())
			{
				this->moveVec.x = 0.0f;
			}
			break;
		case Bubble:
			this->moveVec = this->eff->Move_Bubble(this->moveCnt,this->interval_bubble,this->wide_bubble,this->speed_surfacing);
			break;
		}
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}