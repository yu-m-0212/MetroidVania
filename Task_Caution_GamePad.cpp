//-------------------------------------------------------------------
//�Q�[���p�b�h����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Caution_GamePad.h"
#include  "Task_Title.h"

namespace  Caution
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->name_image_caution = "image_caution";
		DG::Image_Create(this->name_image_caution, "./data/image/caution_game_pad.png");
		this->name_font = "font_skip";
		DG::Font_Create(this->name_font, "HG�ۺ޼��M - PRO", 12, 24);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->name_image_caution);
		DG::Font_Erase(this->name_font);
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
		this->transition_scene = false;
		this->pos = ge->center;						//�`����W
		this->rate_close_up = 1;					//�`��g�嗦
		this->speed_rate_close_up = 2;				//�`��g�呬�x
		this->add_rate_close_up = 1;				//�`��g�嗦������
		this->time_flag = 300;						//�^�C���t���O�𔽓]�����鎞��
		this->time_erase = 90;						//�t���O���]��A���ł܂ł̎���
		this->name_pad = "P1";						//�p�b�h�錾
		this->text_skip = "START�{�^���ŃX�L�b�v";	//�X�L�b�v�\��
		this->pos_font = ML::Vec2(1750, 1040);		//�t�H���g���W
		this->color_font = ML::Color(1, 0, 0, 0);	//�t�H���g�F

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
			Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//�J�E���^�C���N�������g
		this->moveCnt++;
		//�t���O�Ǘ�
		this->Transition_Scene();
		//�������ŉ�ʑJ��
		if (this->transition_scene)
		{
			//�g�呬�x
			if (this->moveCnt%this->speed_rate_close_up == 0)
			{
				this->rate_close_up += this->add_rate_close_up;
			}
			//���ł܂ł̎���
			if (this->moveCnt > this->time_erase)
			{
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		{
			//���\�[�X�̃T�C�Y���擾
			POINT size =
				DG::Image_Size(this->res->name_image_caution);
			ML::Box2D draw
			(
				-size.x / 2,
				-size.y / 2,
				size.x,
				size.y
			);
			//�g�嗦�𔽉f����
			draw.x *= this->rate_close_up;
			draw.y *= this->rate_close_up;
			draw.w *= this->rate_close_up;
			draw.h *= this->rate_close_up;
			draw.Offset(this->pos);
			ML::Box2D src(0, 0, size.x, size.y);
			DG::Image_Draw(this->res->name_image_caution, draw, src);
		}
		//�t�H���g�\��
		{
			//�J�ڃt���O����
			if (!this->transition_scene)
			{
				ML::Box2D text_box(-256,-64,512,128);
				text_box.Offset(this->pos_font);
				DG::Font_Draw(this->res->name_font, text_box, this->text_skip, this->color_font);
			}
		}
	}
	//��ʐ؂�ւ��t���O�Ǘ�
	bool Object::Transition_Scene()
	{
		//���Ԍo�߂Ő���
		if (this->moveCnt > this->time_flag)
		{
			this->transition_scene = true;
			this->moveCnt = 0;
			return true;
		}
		//START�{�^���^�b�v�Ő���
		auto in =
			DI::GPad_GetState(this->name_pad);
		if (in.ST.down)
		{
			this->transition_scene = true;
			this->moveCnt = 0;
			return true;
		}
		return false;
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