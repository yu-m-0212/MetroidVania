//-------------------------------------------------------------------
//��ʌ���
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Display_Effect.h"
#include  "Task_Game.h"
#include  "Task_Ending.h"
#include  "Task_Title.h"

namespace  Display_Effect
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "Display_Effect";
		DG::Image_Create(this->imageName, "./data/image/Display_Effect.png");
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
		this->render2D_Priority[1] = 0.1f;	//�`�揇
		this->transparency = 0.0f;			//�����x
		this->flag_in_out = 0;				//0=in 1=out
		this->cnt_transition = 0;			//��ʑJ�ڃJ�E���^
		this->time_create_next_task = 100;	//���̃^�X�N�𐶐�����^�C�~���O
		this->time_start_fade_out = 150;	//�t�F�[�h�A�E�g���n�߂�^�C�~���O
		
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->cnt_transition++;
		//�t�F�[�h�C��
		if (!this->flag_in_out)
		{
			this->transparency += 0.01f;
		}
		//��莞�ԂŃt���O�𔽓]
		if (this->cnt_transition == this->time_create_next_task)
		{
			this->flag_in_out = !this->flag_in_out;
		}
		//��莞�ԂŃt�F�[�h�A�E�g���n�߂�
		if (this->flag_in_out && this->cnt_transition>this->time_start_fade_out)
		{
			this->transparency -= 0.01f;
		}
		//�����ɂȂ����玩�g�����ł�����
		if (this->transparency < 0.0f)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 1920, 1080);
		ML::Box2D  src(0, 0, 1920, 1080);
		DG::Image_Draw(this->res->imageName, draw, src,ML::Color(this->transparency,1,1,1));
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