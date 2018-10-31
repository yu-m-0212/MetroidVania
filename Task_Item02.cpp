//-------------------------------------------------------------------
//アイテム02(無敵ポーション)
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Item02.h"

namespace  Item02
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "Item02Img";
		DG::Image_Create(this->imageName, "./data/image/Item02.png");
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
		this->render2D_Priority[1] = 0.7f;
		this->hitBase = ML::Box2D(-16, -16, 32, 32);
		this->recieveBase = this->hitBase;
		//★タスクの生成
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


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
		this->animCnt++;
		if (this->time_un_hit > 0) { this->time_un_hit--; }

		State nm = this->state;
		switch (this->state) {
		case Stand:
			break;
		case Lose:
			this->pos.y -= 3;
			if (this->moveCnt > 20) {
				this->Kill();//一定時間経過後、消滅
			}
			break;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		//スクロール対応
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, di.draw, di.src, di.color);
	}
	//-------------------------------------------------------------------
	//接触時の応答処理（必ず受け身の処理として実装する）
	void Object::Received(BChara* from_, AttackInfo at_)
	{
		if (this->state != Stand) {
			return;
		}
		this->UpdateMotion(Lose);
		from_->time_un_hit = 300;//プレイヤが5秒間無敵になる
	}
	//-------------------------------------------------------------------
	//アニメーション制御
	BChara::DrawInfo Object::Anim()
	{
		BChara::DrawInfo imageTable[] = {
			//draw					src						color
			{ ML::Box2D(-16,-16,32,32),ML::Box2D(0,0,32,32),ML::Color(  1 ,1,1,1)},	//Stand
			{ ML::Box2D(-16,-16,32,32),ML::Box2D(0,0,32,32),ML::Color(0.3f,1,1,1) },//Lose
		};
		BChara::DrawInfo  rtv;
		switch (this->state) {
			//	停止----------------------------------------------------------------------------
		case  Stand:	rtv = imageTable[0];	break;
			//	消える・昇天---------------------------------------------------------------------
		case Lose:		rtv = imageTable[1];	break;
		}
		return rtv;
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