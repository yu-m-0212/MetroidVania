//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_Effect.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "TitleLogoImg";
		DG::Image_Create(this->imageName, "./data/image/Title.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render2D_Priority[1] = 1.0f;	//�`�揇
		this->cnt_create_bubble = 0;		//�G�t�F�N�g�̐����J�E���^
		this->cnt_anim_back = 0;			//�w�i�A�j���J�E���^
		this->interval_anim_back = 25;		//�w�i�A�j������
		this->posY = -360.0f;				//�w�iY�����W
		this->posY_std = -240.0f;			//�w�iY�����W��l
		this->height_anim_back = 25.0f;		//�w�i�A�j���h�ꕝ

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�G�t�F�N�g");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->cnt_create_bubble++;
		this->cnt_anim_back++;

		auto in = DI::GPad_GetState("P1");
		//�f�o�b�O�؂�ւ�
		if (in.SE.down) { ge->debugMode = !ge->debugMode; }

		//�G�t�F�N�g�̐���
		if (this->cnt_create_bubble % 30 == 0)
		{
			float initX = float(rand() % (ge->screenWidth - 96));
			int num = rand() % 3;
			float ang = float(rand() % 360);
			eff->Create_Bubble(num, ML::Vec2(initX, float(ge->screenHeight + 96)), 16, 5.0f, 3.0f, ang, 600);
		}
		//���g�ɏ��ŗv��
		if (in.ST.down)
		{
			this->Kill();
		}
		//�w�i�A�j���[�V����
		float y = this->posY_std + float(sin(this->cnt_anim_back / this->interval_anim_back)*this->height_anim_back);
		this->posY = y;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D  draw(-640, -360, 3200, 1800);
		draw.Offset(ML::Vec2(0, this->posY));
		ML::Box2D   src(   0,    0, 3200, 1800);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
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
	//���\�[�X�N���X�̐���
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