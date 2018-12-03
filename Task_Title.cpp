//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_Effect.h"
#include  "Task_Display_Effect.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{

		this->back_title = "back";
		DG::Image_Create(this->back_title, "./data/image/Title.png");

		this->button_title = "button";
		DG::Image_Create(this->button_title, "./data/image/ui.png");

		this->base_file_path = "./data/sound/wav/";

		this->name_sound_bubble = "sound_bubble";
		DM::Sound_CreateStream(this->name_sound_bubble, this->base_file_path + "bubbling.wav");
		DM::Sound_Volume(this->name_sound_bubble, 1000);

		this->name_environmental_title = "environmental_title";
		DM::Sound_CreateStream(this->name_environmental_title, this->base_file_path + "environmental_title.wav");
		DM::Sound_Volume(this->name_environmental_title, 1000);

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->back_title);
		DG::Image_Erase(this->button_title);
		DM::Sound_Erase(this->name_sound_bubble);
		DM::Sound_Erase(this->name_environmental_title);
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
		this->render2D_Priority[1] = 1.0f;								//�`�揇
		this->flag_transition = false;									//��ʑJ�ڃt���O
		this->cnt_transition = 0;										//���ŃJ�E���^
		this->limit_transparency = 100;									//��ʑJ�ڎ��̏��Ŏ���
		this->cnt_create_bubble = 0;									//�G�t�F�N�g�̐����J�E���^
		this->time_avalable_controll = 250;								//������󂯕t����܂ł̎���
		this->cnt_anim_button = 0.0f;									//�{�^���A�j���J�E���^
		this->interval_button = 2.0f;									//�{�^���A�j������
		this->cnt_anim_back = 0.0f;										//�w�i�A�j���J�E���^
		this->interval_anim_back = 25.0f;								//�w�i�A�j������
		this->posY = -360.0f;											//�w�iY�����W
		this->posY_std = -240.0f;										//�w�iY�����W��l
		this->height_anim_back = 30.0f;									//�w�i�A�j���h�ꕝ
		this->init_bubble_pos_y = float(ge->screenHeight + 96.0f);		//�A��Y�����W�����ʒu
		this->pos_button = ML::Vec2(float(ge->screenWidth / 2), 800);	//���W�{�^��
		this->controllerName = "P1";									//�R���g���[���[�錾
		this->eff = new Task_Effect::Object();							//���\�b�h�Ăяo��
		ge->pause = true;												//�������͑�����󂯕t���Ȃ�
		
		//���^�X�N�̐���

		//BGM�̍Đ�
		DM::Sound_Play(this->res->name_sound_bubble, true);
		DM::Sound_Play(this->res->name_environmental_title, true);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�G�t�F�N�g");
		//�T�E���h�̒�~
		DM::Sound_Stop(this->res->name_sound_bubble);
		DM::Sound_Stop(this->res->name_environmental_title);
		//�������p���^�X�N�̐���
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			Game::Object::Create(true);
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->cnt_create_bubble++;
		this->cnt_anim_back += 1.0f;
		this->cnt_anim_button += 0.1f;
		this->cnt_available_controll++;

		auto in = DI::GPad_GetState(this->controllerName);
		//�f�o�b�O�؂�ւ�
		if (in.SE.down) { ge->debugMode = !ge->debugMode; }

		//�G�t�F�N�g�̐���
		if (this->cnt_create_bubble % 30 == 0)
		{
			float initX = float(rand() % (ge->screenWidth - 96));
			this->eff->Create_Effect(5, ML::Vec2(initX, this->init_bubble_pos_y));
		}
		//�w�i�A�j���[�V����
		float y = this->posY_std + sinf(this->cnt_anim_back / this->interval_anim_back)*this->height_anim_back;
		this->posY = y;
		//��莞�Ԍo�ߌ�A������󂯕t����
		if (this->cnt_available_controll < this->time_avalable_controll)
		{
			this->cnt_available_controll++;
		}
		else
		{
			ge->pause = false;
		}
		//��莞�ԑ�����󂯕t���Ȃ�
		if (ge->pause) { return; }
		//���g�ɏ��ŗv��
		if (in.ST.down)
		{
			this->flag_transition = true;
			//�A�łɂ�鑽�d�����h�~
			auto display_effect = ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
			if (nullptr == display_effect)
			{
				Display_Effect::Object::Create(true);
			};
			//�T�E���h�̃t�F�[�h�A�E�g
			DM::Sound_FadeOut(this->res->name_sound_bubble);
			DM::Sound_FadeOut(this->res->name_environmental_title);
		}
		//���ŃJ�E���g�_�E��
		if (this->flag_transition)
		{
			this->cnt_transition++;
		}
		//��莞�Ԃŏ��ł���
		if (this->cnt_transition > this->limit_transparency)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i�̕`��
		{
			ML::Box2D  draw(-640, -360, 3200, 1800);
			draw.Offset(ML::Vec2(0, this->posY));
			ML::Box2D   src(0, 0, 3200, 1800);
			DG::Image_Draw(this->res->back_title, draw, src);
		}
		//�{�^���̕`��
		{
			ML::Box2D draw(-425, -125, 850, 250);
			draw.Offset(this->pos_button);
			ML::Box2D  src(0, 250, 850, 250);
			DG::Image_Draw(this->res->button_title, draw, src,ML::Color(float(sin(this->cnt_anim_button/this->interval_button)),1,1,1));
		}
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