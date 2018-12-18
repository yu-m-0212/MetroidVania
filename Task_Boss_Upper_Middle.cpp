//-------------------------------------------------------------------
//ボス（アッパーミドル）
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Boss_Upper_Middle.h"
#include  "Task_Boss_Upper.h"

namespace  Boss_Upper_Middle
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->name_image = "upper_middle";
		DG::Image_Create(this->name_image, "./data/image/Boss01.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->name_image);
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
		this->state = Stand;								//状態管理
		this->render2D_Priority[1] = 0.6f;					//描画順
		this->hitBase = ML::Box2D(-92, -46, 184, 92);		//マップとの判定矩形
		this->recieveBase = this->hitBase;					//キャラクタとの判定矩形
		this->speed_chase = 0.075f;							//速度追従
		this->shot = new Shot01::Object();					//メソッド呼び出し
		this->boss = new Boss();							//メソッド呼び出し
		this->vec_shot = ML::Vec2(-SPEED_SHOT, 0.0f);		//移動量ショット
		this->hit_shot = ML::Box2D(-8, -8, 16, 16);			//矩形ショット
		this->cnt_move = 0;									//カウンタ行動
		this->interval_shot = -1;							//生成時間ショット
		this->hp = HP_BOSS;									//HPボス
		this->add_un_hit = 60;								//プレイヤに与える無敵時間
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		delete this->shot;
		delete this->boss;
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
		//ポーズ
		if (ge->pause) { return; }
		this->moveCnt++;
		this->animCnt++;
		//無敵時間の減少
		if (this->time_un_hit > 0) { this->time_un_hit--; }
		//思考・状況判断
		this->Think();
		//現モーションに対応した制御
		this->Move();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		//スクロール対応
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->name_image, di.draw, di.src, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
	}
	//接触時の応答処理（必ず受け身の処理として実装する）
	//引数	：	(攻撃側,攻撃情報,与無敵時間)
	void Object::Received(BChara* from_, BChara::AttackInfo at_, const int& un_hit_)
	{
		//無敵時間中はダメージを受けない
		if (this->time_un_hit > 0)
		{
			return;
		}
		this->hp -= at_.power;
		//無敵時間
		this->time_un_hit = un_hit_;
		//状態遷移
		this->UpdateMotion(Damage);
	}
	//-------------------------------------------------------------------
	//思考&状況判断　モーション決定
	void Object::Think()
	{
		BChara::State nm = this->state; //とりあえず今の状態を指定
										//思考（入力）や状況に応じてモーションを変更することを目的としている。
										//モーションの変更以外の処理は行わない
		switch (nm)
		{
		case Stand:		//立っている
			break;
		case Damage:	//被弾
			if (this->time_un_hit == 0) { nm = Stand; }
			break;
		case Lose:		//死亡
			break;
		}
		//死亡処理
		if (this->hp <= 0) { nm = Lose; }
		//モーション更新
		this->UpdateMotion(nm);
	}
	//-------------------------------------------------------------------
	//モーションに対応した処理
	//(モーションは変更しない)
	void Object::Move()
	{
		//複数の状態で共通して行う処理はここに記述する
		//例外がある場合はswitch文の下方に記述する

		//基準となるタスクに追従する
		auto upper = 
			ge->GetTask_One_GN<Boss_Upper::Object>(Boss_Upper::defGroupName,Boss_Upper::defName);
		//存在するか確認
		if (nullptr == upper) { return; }
		//目標に向かって移動する
		this->pos.x += this->boss->Chase_Target(this->pos.x, upper->pos.x, this->speed_chase);
		this->pos.y = upper->pos.y + float(this->hitBase.h);
		//状態を受け取る
		this->state = upper->state;

		//当たり判定
		ML::Box2D me = this->recieveBase.OffsetCopy(this->pos);
		auto targets = ge->GetTask_Group_G<BChara>("プレイヤ");
		if (nullptr == targets) { return; }
		for (auto it = targets->begin();
			it != targets->end();
			++it) {
			//相手に接触の有無を確認させる
			if ((*it)->CheckHit(me)) {
				//相手にダメージの処理を行わせる
				BChara::AttackInfo at = { 1,0,0 };
				(*it)->Received(this, at, this->add_un_hit);
				break;
			}
		}

		//基準となるタスクの状態に応じて攻撃する
		switch (this->state)
		{
		default:
			break;
		case Wait_Under:
			//ショット生成用カウンタを進める
			this->cnt_move++;
			//ショットの生成時間が初期値なら値を入れる
			if (this->interval_shot == -1)
			{
				this->interval_shot = rand() % MAX_INTERVAL_SHOT;
			}
			//生成時間になったらショットを生成する
			if (this->cnt_move == this->interval_shot)
			{
				this->shot->Create_Shot(this->pos, this->vec_shot, this->hit_shot, LIMIT_SHOT, POWER_SHOT, true);
				//カウンタと生成時間をリセットする
				this->cnt_move = 0;
				this->interval_shot = -1;
			}
			break;
		}
		//-------------------------------------------------------------------
		//モーション毎に固有の処理

	}
	//-----------------------------------------------------------------------------
	//アニメーション制御
	BChara::DrawInfo Object::Anim()
	{
		//デフォルトの値を宣言
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] =
		{
			//draw		   src			 color
			{ this->hitBase,ML::Box2D(0,this->hitBase.h * 0,this->hitBase.w,this->hitBase.h),dc },//default	[0]
			{ this->hitBase,ML::Box2D(0,this->hitBase.h * 1,this->hitBase.w,this->hitBase.h),dc },//Damage	[1]
			{ this->hitBase,ML::Box2D(0,this->hitBase.h * 2,this->hitBase.w,this->hitBase.h),dc } //Lose	[2]
		};
		BChara::DrawInfo rtv;
		//アニメーション用変数

		switch (this->state)
		{
		default:
			rtv = imageTable[0];
			break;
		case Stand:
			rtv = imageTable[0];
			break;
		case Damage:
			rtv = imageTable[1];
			break;
		case Lose:
			rtv = imageTable[2];
			break;
		}
		return rtv;
		/*return imageTable[int(this->state)];*/
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