#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//UI�\��
//-------------------------------------------------------------------
#include "BChara.h"
#include "Effect.h"

namespace  UI
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("UI");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��
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
		//���L����ϐ��͂����ɒǉ�����
		string hpImageName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
		int cnt;					//�A�j���[�V�����J�E���^
		
		string controllerName;		//�R���g���[���[�錾
		
		ML::Vec2 pos_hp_boss;		//�{�X��HP�o�[���W
		bool flag_highlight_bar;	//�{�XHP�o�[�����\���t���O
		int max_range_hp_bar;		//�{�XHP�̕\���ő啝
		int cnt_highllight_bar;		//�{�XHP�o�[�\�����ԃJ�E���^
		int limit_highlight_bar;	//�{�XHP�o�[�����\������
		float cnt_gradation_bar;	//�{�XHP�o�[�O���f�[�V�����J�E���^

		int max_dist_quake_x_bar;		//HP�o�[X���h��ő�l
		int max_dist_quake_y_bar;		//HP�o�[y���h��ő�l
		int interval_quake_bar;			//HP�o�[�h��Ԋu

		Effect* eff;				//�G�t�F�N�g�N���X�C���X�^���X
	public:
		//�A�N�Z�T

		//HP�o�[�̋����\�����O������w�肷��
		void Set_Highlight_Bar();
	};
}