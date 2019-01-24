//-------------------------------------------------------------------
//���g���C
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Retry.h"
#include  "Task_Game.h"
#include  "Task_Title.h"
#include  "Task_Corpse.h"
#include  "Task_Display_Effect.h"

using namespace ML;

namespace  Retry
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->back_retry = "back_retry";
		DG::Image_Create(this->back_retry, "./data/image/Retry.png");
		this->button_retry = "button_retry";
		DG::Image_Create(this->button_retry, "./data/image/ui.png");
		DG::Font_Create("fontRetry", "HG�ۺ޼��M-PRO", 8, 16);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->back_retry);
		DG::Image_Erase(this->button_retry);
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
		this->render2D_Priority[1] = 0.4f;							//�`�揇
		this->controllerName = "P1";								//�R���g���[���錾
		this->flag_transition = false;								//��ʑJ�ڗp�t���O
		this->cnt_transition = 0;									//�J�E���^�J�ڗp
		this->title_or_game = 0;									//���p���^�X�N�̑I���t���O(0=Game,1=Title)
		this->time_create_next_task = 100;							//���p���^�X�N�̐����^�C�~���O
		this->time_kill_game = 100;									//���g�����ł�����^�C�~���O
		this->cnt_create_bubble = 0;								//�G�t�F�N�g�̐����J�E���^
		this->cnt_available_controll = 0;							//������A������󂯕t����܂ł̃J�E���^
		this->time_available_controll = 150;						//������A������󂯕t����܂ł̎���
		this->cnt_anim_back = 0.0f;									//�w�i�A�j���J�E���^
		this->interval_anim_back = 25.0f;							//�w�i�A�j������
		this->posY = -360.0f;										//�w�iY�����W
		this->posY_std = -240.0f;									//�w�iY�����W��l
		this->height_anim_back = 30.0f;								//�w�i�A�j���h�ꕝ
		this->init_bubble_pos_y = float(ge->screenHeight + 96.0f);	//�A��Y�����W�����ʒu
		this->eff = new Task_Effect::Object();						//���\�b�h�Ăяo��
		ge->pause = true;											//�������A������󂯕t���Ȃ�

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		ge->KillAll_G("�G�t�F�N�g");
		//���f�[�^���^�X�N���
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->cnt_create_bubble++;
		this->cnt_anim_back+=1.0f;
		//��莞�ԁA������󂯕t���Ȃ�
		if (this->cnt_available_controll < this->time_available_controll)
		{
			this->cnt_available_controll++;
		}
		else
		{
			ge->pause = false;
		}

		auto in = DI::GPad_GetState(this->controllerName);

		//�G�t�F�N�g�̐���
		if (this->cnt_create_bubble % 30 == 0)
		{
			float initX = float(rand() % (ge->screenWidth - 96));
			this->eff->Create_Effect(5, ML::Vec2(initX, this->init_bubble_pos_y));
		}
		//�w�i�A�j���[�V����
		float y = this->posY_std + sinf(this->cnt_anim_back / this->interval_anim_back)*this->height_anim_back;
		this->posY = y;
		//������A�t�F�[�h�A�E�g���I����Ă��瑀���t��
		if (ge->pause) { return; }
		//��ʑJ��
		if (!this->flag_transition)
		{
			//�{�҂��ĊJ����ꍇ
			if (in.ST.down)
			{
				this->flag_transition = true;
				auto display_effect = ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
				if (nullptr == display_effect)
				{
					display_effect->Create_Display_Effect(0);
				}
				this->title_or_game = 0;
			}
			//�^�C�g���ɖ߂�ꍇ
			else if (in.SE.down)
			{
				this->flag_transition = true;
				auto display_effect = ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
				if (nullptr == display_effect)
				{
					display_effect->Create_Display_Effect(0);
				}
				this->title_or_game = 1;
			}
		}
		//���ŃJ�E���g�_�E��
		else
		{
			this->cnt_transition++;
			if (this->title_or_game == 0)
			{
				//��ʑJ�ڎ��ԂɂȂ�����{�Ґ���
				if (this->cnt_transition == this->time_create_next_task)
				{
					Game::Object::Create(true);
					//���g���C����ꍇ�͑O��̎��S�n�_�Ɉ�̂�ݒu
					auto corpse = Corpse::Object::Create(true);
					corpse->pos = this->pos_dead;
					corpse->angle_LR = this->angle_dead;
				}
			}
			//�^�C�g���ɖ߂�
			else if (this->title_or_game == 1)
			{
				//��ʑJ�ڎ��ԂɂȂ�����^�C�g������
				if (this->cnt_transition == this->time_create_next_task)
				{
					Title::Object::Create(true);
				}
			}
		}
		//��莞�Ԃŏ��ł���
		if (this->cnt_transition > this->time_kill_game)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		{
			ML::Box2D  draw(-640, -360, 3200, 1800);
			draw.Offset(ML::Vec2(0, this->posY));
			ML::Box2D   src(0, 0, 3200, 1800);
			DG::Image_Draw(this->res->back_retry, draw, src);
		}
		Box2D textBox(ge->screenWidth / 2 - 125, ge->screenHeight / 2, 250, 250);
		string	 textJp = "�X�^�[�g�{�^���ő��s\n�Z���N�g�{�^���Ń^�C�g���֖߂�";
		DG::Font_Draw("fontRetry", textBox, textJp, Color(1.0f, 0.0f, 0.0f, 0.0f));
	}
	//���S�������W��ۑ�����
	//����	�F	�iVec2)
	void Object::Set_Pos_Dead(const Vec2& dead_)
	{
		this->pos_dead = dead_;
	}
	//�O�񎀖S�������W��Ԃ�
	ML::Vec2 Object::Get_Pos_Dead()
	{
		return this->pos_dead;
	}
	//���S���̌������w�肷��
	//����	�F	�iBChara::Angle_LR)
	void Object::Set_Angle_Dead(const BChara::Angle_LR& angle_)
	{
		this->angle_dead = angle_;
	}
	//���S���̌������擾����
	BChara::Angle_LR Object::Get_Angle_Dead()
	{
		return this->angle_dead;
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