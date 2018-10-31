//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Tutorials.h"
#include  "Task_Player.h"

namespace  Tutorials
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "imageName";
		DG::Image_Create(this->imageName, "./data/image/UI.png");
		this->fontName = "tutorialsFont";
		DG::Font_Create(this->fontName, "�l�r �S�V�b�N", FONT_WIDTH,FONT_HIGHT);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Font_Erase(this->fontName);
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
		this->render2D_Priority[1] = 0.2f;						//�`�揇
		this->message = "���̃��b�Z�[�W�������̂͂���������";		//���b�Z�[�W
		this->recieveBase = ML::Box2D(-128, -64, 256, 128);		//��`
		this->limit = -1;										//���ł܂ł̎���
		
		//���^�X�N�̐���
		
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//���Ԑ����t���ŌĂ΂ꂽ�ꍇ�A��x�����|�[�Y��������
		if (this->limit != -1)
		{
			if (!ge->pause)
			{
				ge->pause = true;
			}
		}
		//�O������\�����Ԃ��w�肵���ꍇ�A���ł���
		if (this->limit != -1)
		{
			this->limit--;
			if (this->limit == 0)
			{
				//�|�[�Y����
				ge->pause = false;
				//����
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		if (ge->debugMode)
		{
			//�f�o�b�O���[�h�̎��A�����`��\��
			ML::Box2D draw = this->recieveBase;
			ML::Box2D  src(64, 0, 64, 64);
			draw.Offset(this->pos);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.5f, 0.0f, 0.0f, 1.0f));
		}
		//�v���C���ƐڐG�������ʉ��Ƀ��b�Z�[�W��\��
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		if (nullptr != pl)
		{
			//���\�[�X�̎w��
			ML::Box2D  src(64, 0, 64, 64);
			//�v���C���̔����`
			ML::Box2D you = pl->recieveBase;
			you.Offset(pl->pos);
			//���Ŏ��Ԗ������̏ꍇ
			if (this->limit == -1)
			{
				if (this->CheckHit(you))
				{
					//�����`�̉��ɕ\��
					ML::Box2D draw(int(this->pos.x - this->message.length()*FONT_WIDTH / 2),
						int(this->pos.y + this->recieveBase.h / 2),
						this->message.length()*FONT_WIDTH, FONT_HIGHT);
					draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
					//���b�Z�[�W�{�b�N�X��\��
					DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.7f, 0.0f, 0.0f, 0.0f));
					//���b�Z�[�W��\��
					DG::Font_Draw(this->res->fontName, draw, this->message, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			//���Ԑ����t���̏ꍇ�A��ɕ\��
			else
			{
				//�v���C���̏�ɕ\��
				ML::Box2D draw(int(pl->pos.x - this->message.length()*FONT_WIDTH / 2),
					int(pl->pos.y), this->message.length()*FONT_WIDTH, FONT_HIGHT);
				draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
				//���b�Z�[�W�{�b�N�X�̕\��
				DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.7f, 0.0f, 0.0f, 0.0f));
				//���b�Z�[�W��\��
				DG::Font_Draw(this->res->fontName, draw, this->message, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
	//�O�����琶������ہA��Ԃ��w�肷��
	void Object::Set_Message(const string& message_)
	{
		this->message = message_;
	}
	//���Ŏ��Ԃ��w�肷��
	//����	�F	�i���ł܂ł̎��ԁj
	void Object::Set_Limit(const int& limit_)
	{
		this->limit = limit_;
	}
	//���b�Z�[�W�𐶐�����
	//����	�F	�i���b�Z�[�W,���W,���Ŏ��ԁj
	void Object::Create_Message(const string& message_,const ML::Vec2& pos_, const int& limit_)
	{
		auto message = Tutorials::Object::Create(true);
		message->Set_Message(message_);
		message->pos = pos_;
		message->Set_Limit(limit_);
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