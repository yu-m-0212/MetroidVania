//-------------------------------------------------------------------
//���g���C
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Retry.h"
#include  "Task_Game.h"
#include  "Task_Title.h"
#include  "Task_Corpse.h"
using namespace ML;

namespace  Retry
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "retryImage";
		DG::Font_Create("fontRetry", "HG�ۺ޼��M-PRO", 8, 16);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Font_Erase("fontRetry");
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
		this->controllerName = "P1";
		
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		auto in = DI::GPad_GetState(this->controllerName);
		//���f�[�^���^�X�N���
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//�������p���^�X�N�̐���
			//���g���C
			if (in.ST.down)
			{
				auto nextTask = Game::Object::Create(true);
				//���g���C����ꍇ�͑O��̎��S�n�_�Ɉ�̂�ݒu
				auto corpse = Corpse::Object::Create(true);
				corpse->pos = this->deadPos;
			}
			//�^�C�g���ɖ߂�
			else if (in.SE.down)
			{
				auto nextTask = Title::Object::Create(true);
			}
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState(this->controllerName);
		//�X�^�[�g�Z���N�g�{�^���Ŏ��g������
		if (in.ST.down || in.SE.down)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		Box2D textBox(ge->screenWidth / 2 - 125, ge->screenHeight / 2, 250, 250);
		string	 textJp = "�X�^�[�g�{�^���ő��s\n�Z���N�g�{�^���Ń^�C�g���֖߂�";
		DG::Font_Draw("fontRetry", textBox, textJp, Color(1.0f, 0.0f, 0.0f, 0.0f));
	}
	//���S�������W��ۑ�����
	//����	�F	�iVec2)
	void Object::Set_DeadPos(const Vec2& dead_)
	{
		this->deadPos = dead_;
	}
	//�O�񎀖S�������W��Ԃ�
	ML::Vec2 Object::Get_DeadPos()
	{
		return this->deadPos;
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