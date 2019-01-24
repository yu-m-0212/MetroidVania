#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�{�X�i�w�b�h�j
//-------------------------------------------------------------------
#include "BChara.h"
#include "Boss.h"
#include "Task_Shot01.h"
#include "Task_Map2D.h"
#include "Task_Effect.h"

namespace  Boss_Head
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�{�X");		//�O���[�v��
	const  string  defName("�{�X�i�w�b�h�j");		//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//�ύX������������������������������������������������������
			//���L����ϐ��͂����ɒǉ�����
		string name_image;					//���\�[�X��
		string base_file_path_sound;		//��b�t�@�C���p�X
		string name_sound_wait_under_00;	//�ҋ@��SE00
		string name_sound_wait_under_01;	//�ҋ@��SE01
		string name_sound_hiding_under;		//���s��SE
		string name_sound_hit_reflect;		//���˒e��e��
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		//�ύX������������������������������������������������������
		//�U�����
		int add_un_hit;						//�v���C���ɗ^���閳�G����
		//���h��
		float speed_shake;					//���������s�����h�ꑬ�x
		float speed_shake_def;				//�ʏ펞�̉��h�ꑬ�x
		float speed_shake_ref;				//���˔�e���̉��h�ꑬ�x
		float std_pos_x;					//���h���l
		float cnt_shake;					//�J�E���^���h��
		float interval_shake;				//�Ԋu���h��
		//�V���b�g����
		int num_shot;						//�e�̐�����
		float angle_create_shot;			//�e�𐶐�����p�x�̊Ԋu
		//�V���b�g���
		ML::Vec2 vec_shot;					//�ړ��ʃV���b�g
		ML::Box2D hit_shot;					//��`�V���b�g
		//��ԑJ�ڗp�ϐ�
		bool flag_alive_base;				//�y��̐���
		int interval_to_appear;				//�o�ꂷ��܂ł̎���
		int interval_create_effect;			//�G�t�F�N�g�̐����Ԋu
		int interval_shot;					//�V���b�g�Ɉڂ鎞��
		int interval_return;				//�V���b�g����߂�܂ł̎���
		int limit_move_vertically;			//�c�������̓o��ړ�����
		float speed_move_under;				//�c�������̓o��E�ޏꑬ�x
		float speed_chase_hiding;			//���s���v���C���ɐڋ߂��銄��
		ML::Box2D hit_vertically_long;		//�c���̎��̋�`�ihitBase�ɑ�����Ďg�p�j
		ML::Box2D hit_horizontally_long;	//�����̎��̋�`�ihitBase�ɑ�����Ďg�p�j
		//��ʗh��p�ϐ�
		int dist_quake_boss;				//�{�X�p��ʗh�ꕝ
		int limit_quake_boss;				//�{�X�p��ʗh�ꎞ��
		//�C���X�^���X
		Boss* boss;							//�{�X�N���X�|�C���^
		Shot01::Object* shot;				//�V���b�g�I�u�W�F�N�g�|�C���^
		Task_Effect::Object* eff;			//�G�t�F�N�g�I�u�W�F�N�g�|�C���^
	public:
		//�R���X�g���N�^
		Object();
		//�v�l&�󋵔��f(�X�e�[�^�X����)
		void Think();
		//���[�V�����ɑΉ���������
		void Move();
		//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
		//����	�F	�i�U�����̃|�C���^,�U�����,�^���G���ԁj
		void Recieved(BChara* from_, AttackInfo at_, const int&);
		//�A�j���[�V��������
		BChara::DrawInfo Anim();
	};
}