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
		this->render2D_Priority[1] = 0.1f;					//�`�揇
		this->transparency = 0.0f;							//�����x
		this->flag_in_out = 0;								//0=in 1=out
		this->cnt_transition = 0;							//��ʑJ�ڃJ�E���^
		this->time_create_next_task = 100;					//���̃^�X�N�𐶐�����^�C�~���O
		this->time_start_fade_out = 150;					//�t�F�[�h�A�E�g���n�߂�^�C�~���O
		this->state_display = Non;							//��ԊǗ�
		this->num_state = -1;								//��ԊǗ��ԍ�
		this->flag_switching_slide = false;					//�X���C�h�C��/�A�E�g�t���O
		this->move_letter=0;								//���^�[�{�b�N�X�A�j���J�E���^
		this->add_move_letter = 2;							//���^�[�{�b�N�X�X���C�h���x
		this->limit_move_letter=192;						//���^�[�̈ړ��ʏ��
		this->cnt_switch_letter = 0;						//�X���C�h�C���A�E�g�̐؂�ւ��J�E���^
		this->time_switch_letter = 180;						//�X���C�h�C���A�E�g�̐؂�ւ��^�C�~���O

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
		//�O�������x������Ԃ��w�肵�A������
		if (this->state_display == Non)
		{
			switch (this->num_state)
			{
			default:
				break;
				//�t�F�[�h�C��/�A�E�g
			case 0:
				this->state_display = Fade;
				break;
				//���^�[�{�b�N�X
			case 1:
				this->state_display = Letter_Box;
				break;
				//�z���C�g�A�E�g
			case 2:
				this->state_display = White_Out;
				break;
			}
		}
		//�ėp�J�E���^
		this->cnt_transition++;
		//��Ԃ��Ƃ̏���
		switch (this->state_display)
		{
		default:
			break;
		case Fade:
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
			if (this->flag_in_out &&
				this->cnt_transition > this->time_start_fade_out)
			{
				this->transparency -= 0.01f;
			}
			//�����ɂȂ����玩�g�����ł�����
			if (this->transparency < 0.0f)
			{
				this->Kill();
			}
			break;
		case Letter_Box:
			//�X���C�h�C��
			if (!this->flag_switching_slide)
			{
				this->move_letter+=this->add_move_letter;
				//�K��ʈړ�������t���O���]
				if (this->move_letter == this->limit_move_letter)
				{
					this->flag_switching_slide=true;
				}
			}
			//�X���C�h�A�E�g
			if(this->flag_switching_slide)
			{
				this->cnt_switch_letter++;
				//�X���C�h�A�E�g�܂ł̊Ԋu
				if (this->cnt_switch_letter >= this->time_switch_letter)
				{
					this->move_letter-=this->add_move_letter;
					//�t���O���]��A�K��ʈړ��Ŏ��g�����ł�����
					if (this->move_letter == 0)
					{
						this->Kill();
					}
				}
			}
			break;
		case White_Out:
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
			if (this->flag_in_out &&
				this->cnt_transition > this->time_start_fade_out)
			{
				this->transparency -= 0.01f;
			}
			//�����ɂȂ����玩�g�����ł�����
			if (this->transparency < 0.0f)
			{
				this->Kill();
			}
			break;
		}	
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		switch (this->state_display)
		{
		default:
			break;
		case Fade:
		{
			ML::Box2D draw(0, 0, 1920, 1080);
			ML::Box2D src(0, 0, 1920, 1080);
			DG::Image_Draw(this->res->imageName, draw, src, ML::Color(this->transparency, 1, 1, 1));
			break;
		}
		case White_Out:
		{
			ML::Box2D draw(0, 0, 1920, 1080);
			ML::Box2D src(0, 1700, 1920, 1080);
			DG::Image_Draw(this->res->imageName, draw, src, ML::Color(this->transparency, 1, 1, 1));
			break;
		}
		//��ʊO���烌�^�[�{�b�N�X�������/��ʊO�ɏ�����
		case Letter_Box:
		{
			ML::Box2D  src(0, 1245, 1920, 192);
			//X�����S��錾
			float center_x = float(ge->screenWidth / 2);
			//�㕔��Y�����W
			float top_y = float(-src.h + this->move_letter);
			ML::Box2D draw_top(-960, 0, 1920, 192);
			draw_top.Offset(ML::Vec2(center_x, top_y));
			DG::Image_Draw(this->res->imageName, draw_top, src, ML::Color(1, 1, 1, 1));
			//������Y�����W
			float bottom_y = float(ge->screenHeight - this->move_letter);
			ML::Box2D draw_bottom(-960, 0, 1920, 192);
			draw_bottom.Offset(ML::Vec2(center_x, bottom_y));
			DG::Image_Draw(this->res->imageName, draw_bottom, src, ML::Color(1, 1, 1, 1));
			break;
		}
		}
	}
	//�O�������Ԃ��w�肷��
	//����	�F	�i�����j
	//0�F�t�F�[�h�C��/�A�E�g
	//1�F���^�[�{�b�N�X
	void Object::Set_Num_State(const int& num_)
	{
		this->num_state = num_;
	}
	//��ʑS�̂̌��ʂ𐶐����A��Ԃ��w�肷��
	//����	�F	�i��Ԕԍ��j
	//0�F�t�F�[�h�C��/�A�E�g
	//1�F���^�[�{�b�N�X
	//2: �z���C�g�A�E�g
	void Object::Create_Display_Effect(const int& num_)
	{
		auto display_effect =
			Display_Effect::Object::Create(true);
		display_effect->Set_Num_State(num_);
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