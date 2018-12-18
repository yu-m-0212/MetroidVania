//-------------------------------------------------------------------
//ボス（ヘッド）
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Boss_Head.h"
#include  "Task_Player.h"

namespace  Boss_Head
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->name_image = "head";
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
		this->state = Appear_Under;									//状態管理
		this->render2D_Priority[1] = 0.6f;							//描画順
		this->hitBase = ML::Box2D(-92, -46, 184, 92);				//マップとの判定矩形
		this->recieveBase = this->hitBase;							//キャラクタとの判定矩形
		this->std_pos_x = 700.0f;									//横揺れ基準値
		this->speed_shake = 64.0f;									//速度横揺れ
		this->cnt_shake = 0.0f;										//カウンタ横揺れ
		this->interval_shake = 12.0f;								//間隔横揺れ
		this->shot = new Shot01::Object();							//ショットオブジェクトインスタンス
		this->boss = new Boss();									//ボスクラスインスタンス
		this->vec_shot = ML::Vec2(SPEED_SHOT, 0.0f);				//移動量ショット
		this->hit_shot = ML::Box2D(-8, -8, 16, 16);					//矩形ショット
		this->interval_shot = 360;									//生成時間ショット
		this->hp = HP_BOSS;											//HPボス
		this->add_un_hit = 60;										//プレイヤに与える無敵時間
		this->num_shot = 5;											//弾の生成数
		this->angle_create_shot = 18.0f;							//弾を生成する間隔
		this->flag_alive_base = true;								//土台の生死
		this->interval_to_appear = 180;								//登場するまでの時間
		this->interval_create_effect = 30;							//エフェクトの生成間隔
		this->interval_return = 180;								//ショットから戻るまでの時間
		this->limit_move_vertically = 120;							//縦向き時の登場、退場移動時間
		this->speed_move_under = 5.0f;								//縦向き時の登場、退場速度
		this->speed_chase_hiding = 0.05f;							//潜行中プレイヤに接近する割合
		this->hit_vertically_long = ML::Box2D(-92, -46, 184, 92);	//縦長の時の矩形（hitBaseに代入して使用）
		this->hit_horizontally_long = ML::Box2D(-46, -92, 92, 184);	//横長の時の矩形（hitBaseに代入して使用）

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
		//カウンタ
		this->cnt_shake += 1.0f;
		this->moveCnt++;
		this->animCnt++;
		//無敵時間の減少
		if (this->time_un_hit > 0) { this->time_un_hit--; }
		//思考・状況判断
		this->Think();
		//現モーションに対応した制御
		this->Move();
		//移動処理
		this->pos += this->moveVec;
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
	//思考&状況判断(ステータス決定)
	void Object::Think()
	{
		//とりあえず今の状態を指定
		BChara::State nm = this->state; 
		//思考（入力）や状況に応じてモーションを変更することを目的としている。
		//モーションの変更以外の処理は行わない
		switch (nm)
		{
		case Appear_Under:
			if (this->moveCnt > this->limit_move_vertically) { nm = Wait_Under; }
			break;
		case Wait_Under:
			if (this->moveCnt > this->interval_shot) { nm = Shot_Under; }
			break;
		case Shot_Under:
			if (this->moveCnt > this->interval_return) { nm = Return_Under; }
			break;
		case Return_Under:
			if (this->moveCnt > this->limit_move_vertically) { nm = Choice_Vertically_Or_Horizontally; }
			break;
		case Hiding_Under:
			if (this->moveCnt > this->interval_to_appear) { nm = Appear_Under; }
			break;
		case Choice_Vertically_Or_Horizontally:
			//仮に縦向きのみを選択する
			nm = Hiding_Under;
			break;
		case Lose:		//死亡
			break;
		}
		//死亡処理
		if (this->hp <= 0) { nm = Lose; }
		//モーション更新
		this->UpdateMotion(nm);
	}
	//モーションに対応した処理
	void Object::Move()
	{
		//複数の状態で共通して行う処理はここに記述する
		//例外がある場合はswitch文の下方に記述する

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
		//左右に揺れる
		//このタスクの動きを基準とする
		switch (this->state)
		{
		default:
			break;
		case Wait_Under:
			this->pos.x = this->std_pos_x + sinf(this->cnt_shake / this->interval_shake)*this->speed_shake;
			break;
		case Hiding_Under:
			auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
			//潜行中はプレイヤの座標にやんわり合わせる
			float to_vec_x= pl->pos.x - this->pos.x;
			this->pos.x += to_vec_x * this->speed_chase_hiding;
			//状態遷移の直前に揺れの基準点を現在の座標に補正する
			if (this->moveCnt == this->interval_to_appear)
			{
				this->std_pos_x = this->pos.x;
			}
		}
		//-------------------------------------------------------------------
		//モーション毎に固有の処理
		switch (this->state)
		{
		default:
			break;
		case Hiding_Under:
			break;
		case Appear_Under:
			//定位置まで移動する
			if (this->moveCnt < this->limit_move_vertically)
			{
				this->moveVec.y = -this->speed_move_under;
			}
			else
			{
				this->moveVec.y = 0.0f;
			}
			break;
		case Shot_Under:
			if (this->moveCnt == 0)
			{
				auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
				if (nullptr == pl) { return; }
				//目標座標を出してから方向のみを抽出する
				ML::Vec2 to_vec = pl->pos - this->pos;
				to_vec = to_vec.Normalize();
				//角度の増加量
				float increase = ML::ToRadian(this->angle_create_shot);
				//弾の生成
				for (int i = -this->num_shot / 2; i <= this->num_shot / 2; i++)
				{
					//角度の基準値に増加量を生成順に与える
					float ang = acos(to_vec.x) + increase * i;
					ML::Vec2 vec = ML::Vec2(cos(ang), sin(ang));
					this->shot->Create_Shot(this->pos, vec*SPEED_SHOT, this->hit_shot, LIMIT_SHOT, POWER_SHOT, true);
				}
			}
			break;
		case Return_Under:
			if (this->moveCnt < this->limit_move_vertically)
			{
				this->moveVec.y = +this->speed_move_under;
			}
			else
			{
				this->moveVec.y = 0.0f;
			}
			break;
		}
	}
	//接触時の応答処理（必ず受け身の処理として実装する）
	//引数	：	（攻撃側のポインタ,攻撃情報,与無敵時間）
	void Object::Received(BChara* from_, AttackInfo at_, const int& un_hit_)
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
		case Lose:
			rtv = imageTable[2];
			break;
		}
		//ヒット表示
		if (this->time_un_hit > 0)
		{
			rtv.src.y += rtv.src.h;
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