#pragma warning(disable:4996)
#pragma once
#include <windows.h>
#define DIRECTINPUT_VERSION	0x0800
#pragma comment(lib,"dxguid")
#pragma comment(lib,"dInput8")
#include <dInput.h>
#include "myLib.h"
#include <string>
#include <vector>

//����������������������������������������������������������������������������������������������
//�����͐���n�@�\�Q																		�@��
//����������������������������������������������������������������������������������������������
namespace DI
{
	enum But {	//�v���O������Ŏg�p�ł���L�[�̎��
		//��,
		B1,
		//�~
		B2,
		//��
		B3,
		//��
		B4,
		L1, R1, L2, R2,
		ST, SE, L3, R3,
		//��
		HL,		//�n�b�g�X�C�b�`
		//��
		HR,
		//��
		HU,
		//��
		HD,
		S0, S1, S2, S3, S4, S5, S6, S7, S8, S9,
	};
	enum GPB {//�Q�[���p�b�h�̃L�[��\��
		B01, B02, B03, B04, B05, B06, B07, B08,
		B09, B10, B11, B12, B13, B14, B15, B16,
		HSL, HSR, HSU, HSD,		//�n�b�g�X�C�b�` 
	};

	struct  KD_KB{
		DWORD           vKey;	//�L�[�{�[�h�̃o�[�`�����L�[
		But             pKey;	//�{�^���̎�ށi�o�f��ł̖��́j
	};
	//�L�[�ݒ�L�[�{�[�h�p
	typedef  vector<KD_KB>	KeyDatas_KB;
	//�A�i���O�X�e�B�b�N�ݒ�L�[�{�[�h�p
	struct  AnalogAxisKB{
		DWORD           vKeyL;	//�L�[�{�[�h�̃o�[�`�����L�[
		DWORD           vKeyR;	//�L�[�{�[�h�̃o�[�`�����L�[
		DWORD           vKeyU;	//�L�[�{�[�h�̃o�[�`�����L�[
		DWORD           vKeyD;	//�L�[�{�[�h�̃o�[�`�����L�[
	};
	struct  KD_GP{
		GPB				 gKey;	//�Q�[���p�b�h�̃{�^��
		But              pKey;	//�{�^���̎�ށi�o�f��ł̖��́j
	};
	//�L�[�ݒ�Q�[���p�b�h�p
	typedef  vector<KD_GP>	KeyDatas_GP;

	struct  ButtonInfo {
		bool  on = false;
		bool  off = true;
		bool  down = false;
		bool  up = false;
		//�{�^����ON�ɏ���������
		void  SetOn() {   on = 1; off = 0; down = 0; up = 0; }
		//�{�^����OFF�ɏ���������
		void  SetOff() {  on = 0; off = 1; down = 0; up = 0; }
		//�{�^����DOWN�ɏ���������
		void  SetDown() { on = 1; off = 0; down = 1; up = 0; }
		//�{�^����UP�ɏ���������
		void  SetUp() {   on = 0; off = 1; down = 0; up = 1; }
	};
	//�A�i���O�X�e�B�b�N���
	struct  StickInfo {
		float			angle = 0.0f;		//�A�i���O�X�e�B�b�N�̊p�x�iRT0�x�@DW90�x�̃��W�A���j
		float			volume = 0.0f;	//�A�i���O�X�e�B�b�N�̋���0.0f�`1.0f
		ML::Vec2		axis = ML::Vec2(0,0);		//�����̗�-1.0f�`+1.0f
		ButtonInfo      L;		//�{�^���Ɍ����Ă���ԁ@��
		ButtonInfo      R;		//�{�^���Ɍ����Ă���ԁ@�E
		ButtonInfo      U;		//�{�^���Ɍ����Ă���ԁ@��
		ButtonInfo      D;		//�{�^���Ɍ����Ă���ԁ@��
	};
	//���z�R���g���[���\����
	struct  VGamePad {
		StickInfo  LStick;
		StickInfo  RStick;
		ButtonInfo  B1;
		ButtonInfo  B2;
		ButtonInfo  B3;
		ButtonInfo  B4;
		ButtonInfo  L1;
		ButtonInfo  L2;
		ButtonInfo  L3;
		ButtonInfo  R1;
		ButtonInfo  R2;
		ButtonInfo  R3;
		ButtonInfo  ST;
		ButtonInfo  SE;
		ButtonInfo  HL;
		ButtonInfo  HR;
		ButtonInfo  HU;
		ButtonInfo  HD;
		ButtonInfo  S0;
		ButtonInfo  S1;
		ButtonInfo  S2;
		ButtonInfo  S3;
		ButtonInfo  S4;
		ButtonInfo  S5;
		ButtonInfo  S6;
		ButtonInfo  S7;
		ButtonInfo  S8;
		ButtonInfo  S9;
	};
	//�}�E�X�\����
	struct  Mouse {
		POINT  distance = { 0,0 };	//�ړ���
		POINT  cursorPos = { 0,0 };	//���݈ʒu
		int    wheel = 0;		//�z�C�[��
		ButtonInfo  LB;		//���{�^��
		ButtonInfo  RB;		//�E�{�^��
		ButtonInfo  CB;		//�����{�^���i�z�C�[��
		ButtonInfo  B1;		//�g���{�^��
		ButtonInfo  B2;		//�g���{�^��
		ButtonInfo  B3;		//�g���{�^��
		ButtonInfo  B4;		//�g���{�^��
		ButtonInfo  B5;		//�g���{�^��
	};

	//������
	bool  Initialize(HWND  hw_, float scaleX_ = 1.f, float scaleY_ = 1.f);
	//���
	void  Finalize();
	//�X�V
	void  UpDate();
	//���ݓo�^���Ă��邷�ׂẴf�o�C�X��j��
	void  GPad_Clear();
	//�w�肵���f�o�C�X��j��
	void  GPad_Erase(const string name_);



	//�L�[�{�[�h�Ńf�o�C�X����
	void  GPad_CreateKB(
		const string	name_,
		const AnalogAxisKB	axisL_,
		const AnalogAxisKB	axisR_,
		const KeyDatas_KB		button_);
	//�Q�[���p�b�h�Ńf�o�C�X����
	void  GPad_CreateGP(
		const string	name_,
		const DWORD			id_,
		const KeyDatas_GP		button_);
	//�L�[�{�[�h���Q�[���p�b�h�Ńf�o�C�X����
	void  GPad_CreateKBGP(
		const string	name_,
		const AnalogAxisKB	axisL_,
		const AnalogAxisKB	axisR_,
		const KeyDatas_KB		buttonK_,
		const DWORD			id_,
		const KeyDatas_GP		buttonG_);
	//	�{�^���̏�Ԃ��m�F
	VGamePad  GPad_GetState(const string  name_);

	//	�}�E�X�̏�Ԃ��m�F
	Mouse  Mouse_GetState( );
}