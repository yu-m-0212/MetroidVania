//-------------------------------------------------------------------
//プレイヤの出す弾
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Shot01.h"
#include  "Task_Player.h"
#include  "Task_Shot00.h"
#include  "Task_Effect.h"

namespace  Shot01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "Shot01Img";
		DG::Image_Create(this->imageName, "./data/image/Shot00.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
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
		this->render2D_Priority[1] = 0.4f;		//描画順
		this->recieveBase = this->hitBase;		//キャラクタとの接触矩形
		this->flag_Erase = true;				//接触時消えるか
		this->power = 0;						//攻撃力
		this->limit_Erase = 0;					//消滅するまでの時間
		this->add_un_hit = 60;					//プレイヤに与える無敵時間
		this->angle = 0.0f;						//角度
		this->eff = new Task_Effect::Object();	//メソッド呼び出し
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		if (!ge->QuitFlag() && this->nextTaskCreate) {
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
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		this->moveCnt++;
		//各状態ごとの処理
		this->Move();

		//移動
		this->pos += this->moveVec;
		//当たり判定
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("プレイヤ");
			if (nullptr == pl) { return; }
			for (auto it = targets->begin();
				it != targets->end();
				++it) {
				//相手に接触の有無を確認させる
				if ((*it)->CheckHit(me)) {
					//相手にダメージの処理を行わせる
					BChara::AttackInfo at = { this->power,0,0 };
					(*it)->Received(this, at,this->add_un_hit);
					//ショットのみ消滅
					//格闘は複数体にあたる
					if (this->flag_Erase)
					{
						//対応したヒット時のエフェクトを生成
						//現状、引数には対象の敵の座標をいれる
						this->Effect_Hit((*it)->pos);
						this->Kill();
					}
					else
					{
						//対応したヒット時のエフェクトを生成
						//現状、引数には対象の敵の座標をいれる
						//格闘は矩形が残る為、当たった瞬間のみエフェクトを生成する
						if ((*it)->moveCnt == 0)
						{
							this->Effect_Hit((*it)->pos);
						}
					}
					break;
				}
			}
		}
		//反射処理
		auto shot_pl = ge->GetTask_One_G<Shot00::Object>(Shot00::defGroupName);
		if (nullptr != shot_pl)
		{
			ML::Box2D  me = this->hitBase.OffsetCopy(this->pos);
			ML::Box2D you = shot_pl->hitBase.OffsetCopy(shot_pl->pos);
			if (you.Hit(me) && shot_pl->state == StompLanding)
			{
				this->moveVec = ML::Vec2(-this->moveVec.x, -this->moveVec.y);
			}
		}
		//射撃は壁に当たると消滅する
		if (this->flag_Erase)
		{
			if (auto map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName,Map2D::defName))
			{
				ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
				if (true == map->CheckHit(hit))
				{
					//対応したヒット時のエフェクトを生成し消滅する
					this->Effect_Hit(this->pos);
					this->Kill();
				}
			}
		}
		//限界の時間を迎えたら消滅
		if (this->moveCnt >= this->limit_Erase)
		{
			//消滅申請
			this->Kill();
			return;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase;
		draw.Offset(this->pos);
		ML::Box2D src(0, 0, 32, 32);
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.5f, 1.0f, 0.0f, 0.0f));
	}
	//呼び出したタスクから寿命を設定する
	void Object::Set_Limit(const int& cl_)
	{
		this->limit_Erase = cl_;
	}
	//壁や敵に衝突したとき、消えるか否かを指定する
	void Object::Set_Erase(const int& erase_)
	{
		this->flag_Erase = erase_;
	}
	//外部から生成する際、攻撃力を指定
	//引数	：	（整数値）
	void Object::Set_Power(const int& pow_)
	{
		this->power = pow_;
	}
	//角度を指定する
	//引数	：	（角度）
	void Object::Set_Angle(const float& angle_)
	{
		this->angle = angle_;
	}
	//状態ごとに行動を指定する
	void Object::Move()
	{
		switch (this->state)
		{
		default:
			break;
		case Shoot:
			//敵に衝突したとき消えるか否か
			this->flag_Erase = true;
			break;
		}
	}
	//消滅する際、状態に応じてエフェクトを生成
	//引数	：	（エフェクトを生成する座標）
	//caseの中で宣言したローカル変数のスコープがswitch文全体に及ぶ恐れがある為
	//意図的にブロック{}を設けること
	void Object::Effect_Hit(const ML::Vec2& pos_)
	{
		switch (this->state)
		{
		default:
			break;
		case Shoot:
			this->eff->Create_Effect(1, this->pos, this->angle,this->angle_LR);
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