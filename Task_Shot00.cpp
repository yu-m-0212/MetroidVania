//-------------------------------------------------------------------
//プレイヤの出す弾
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
//#include  "Task_EnemyTest.h"
//#include  "Task_Effect00.h"
#include  "Task_Shot00.h"

namespace  Shot00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "Shot00Img";
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
		this->render2D_Priority[1] = 0.4f;
		this->eraseFlag = true;
		this->hp = 0;				//hp初期値
		this->cntLimit = 0;			//消滅するまでの時間
		this->hitBase = ML::Box2D(-32,-32,64,64);
		
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
		this->moveCnt++;
		//★データ＆タスク解放
		//限界の時間を迎えたら消滅
		if (this->moveCnt >= this->cntLimit) {
			//消滅申請
			this->Kill();
			return;
		}
		//移動
		this->pos += this->moveVec;

		//当たり判定
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("敵");
			for (auto it = targets->begin();
				it != targets->end();
				++it) {
				//相手に接触の有無を確認させる
				if ((*it)->CheckHit(me)) {
					//相手にダメージの処理を行わせる
					BChara::AttackInfo at = { this->hp,0,0 };
					(*it)->Received(this, at);
					//ショットのみ消滅
					//格闘は複数体にあたる
					if (this->eraseFlag)
					{
						this->Kill();
					}
					break;
				}
			}
		}
		//移動先で障害物に接触したら消滅
		//マップが存在するか調べてからアクセス
		//shotとして呼ばれた時のみ、判定
		if (this->eraseFlag)
		{
			if (auto map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ")) {
				ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
				if (true == map->CheckHit(hit)) {
					//消滅申請
					this->Kill();

					//とりあえず星はばらまくよ
					/*for (int c = 0; c < 4; ++c) {
						auto eff = Effect00::Object::Create(true);
						eff->pos = this->pos;
					}*/
					return;
				}
			}
		}

		//敵対象と衝突判定&ダメージを与える処理
		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);//shot
		//全ての敵に対してあたり判定を行う
		//for(auto it =ge->qa_Enemys->begin();
		//	it != ge->qa_Enemys->end();
		//	++it)
		//{
		//	if ((*it)->CheckState() == BTask::eKill) { continue; }//消滅予定の敵は判定しない

		//	//敵キャラクタのあたり判定矩形を用意
		//	ML::Box2D you = (*it)->hitBase.OffsetCopy((*it)->pos);
		//	//重なりを判定
		//	if (true == you.Hit(me)) {
		//		(*it)->Kill();
		//		this->Kill();

		//		//とりあえず星をばら撒くよ
		//		for (int c = 0; c < 4; ++c) {
		//			auto eff = Effect00::Object::Create(true);
		//			eff->pos = this->pos;
		//		}
		//		break;
		//	}
		//}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw=this->hitBase;
		draw.Offset(this->pos);
		ML::Box2D src(0, 0, 32, 32);
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//呼び出したタスクから寿命を設定する
	void Object::Set_Limit(const int& cl_)
	{
		this->cntLimit = cl_;
	}
	//壁や敵に衝突したとき、消えるか否かを指定する
	void Object::Set_Erase(const int& erase_)
	{
		this->eraseFlag = erase_;
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