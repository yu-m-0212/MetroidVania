//-------------------------------------------------------------------
//エフェクト
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect.h"
using namespace ML;
namespace  Effect
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
		this->cntLimit = 0;
		this->animCnt = 0;
		this->moveCnt = 0;
		this->hitBase = Box2D(-96, -64, 192, 128);//表示矩形はここで操作する
		
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
		//★データ＆タスク解放
		//限界の時間を迎えたら消滅
		if (this->moveCnt >= this->cntLimit) {
			//消滅申請
			this->Kill();
			return;
		}
		//移動
		this->pos += this->moveVec;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo  di = this->Anim();
		di.draw.Offset(this->pos);
		//スクロール対応
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->effectImage, di.draw, di.src);
	}
	//-------------------------------------------------------------------
	BChara::DrawInfo Object::Anim()
	{
		//デフォルトカラーを宣言
		Color dc(1, 1, 1, 1);
		//各エフェクトをテーブルで用意する
		BChara::DrawInfo imageTable[]
		{
			{this->hitBase,Box2D(0,  0,this->hitBase.w,this->hitBase.h),dc},//ストンプ着地の衝撃1[0]
			{this->hitBase,Box2D(0,128,this->hitBase.w,this->hitBase.h),dc},//ストンプ着地の衝撃2[1]
			{this->hitBase,Box2D(0,256,this->hitBase.w,this->hitBase.h),dc} //ストンプ着地の衝撃3[2]
		};
		//返す変数を用意
		BChara::DrawInfo  rtv;
		//各アニメーションの際、この変数にanimCntを入れ計算を行う
		int animCnt = 0;
		//状態ごとの描画処理
		switch (this->state)
		{
		case Unnon:
			break;
		case StompLanding:
			//ストンプ着地の衝撃アニメーション
			//切り替わるフレーム数
			animCnt	= this->animCnt / 6;
			//パターン数
			animCnt %= 3;
			//animCntに対象の要素番号を足して返す
			rtv = imageTable[animCnt+0];
			break;
		}
		return rtv;
	}
	//呼び出す際に消滅までの時間を指定する
	//引数	：	（消滅までの時間）
	void Object::Set_Limit(const int& limit_)
	{
		this->cntLimit = limit_;
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