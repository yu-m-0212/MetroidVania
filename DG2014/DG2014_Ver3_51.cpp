#include "DG2014_Ver3_51.h"
#include <map>

namespace  DG
{
	DG_::DGObject::SP		obj;
	unordered_map<string, DG_::Image::SP>	images;
	bool images_NotObject = false;
	unordered_map<string, DG_::Font::SP>	fonts;
	bool fonts_NotObject = false;
	unordered_map<string, DG_::Mesh::SP>	meshs;
	bool meshs_NotObject = false;
	unordered_map<string, DG_::Texture::SP>	textures;
	bool textures_NotObject = false;

	void  Line_Initialize();
	void  Line_Finalize();

	D3D10_PRIMITIVE_TOPOLOGY	line_priTopo;		//�v���~�e�B�u�g�|���W�[
	DG_::InputLayout::SP		line_inputLayout;	//���_���̓��̓��C�A�E�g
	DWORD						line_tecNum;		//�e�N�j�b�N�ԍ�
	DG_::VertexBuffer::SP		line_vPos;


	DG_::DGObject&  Object(){return *obj;}
	ID3D10Device&	Device(){	return obj->Device();	}
	ID3D10EffectTechnique&	EffectTec(){ return obj->ETechnique(); }

	//-----------------------------------------------------------------------
	bool  Initialize(
		HWND		hw_,	//	�E�B���h�E�n���h��
		int			w_,		//	����
		int			h_,		//	�c��
		DWORD		ms_,	//	�}���`�T���v��
		bool		sm_,	//	�X�N���[�����[�h
		int			w2D_,	//	2D�`��𑜓x�i����
		int			h2D_)	//	2D�`��𑜓x�i����	
	{
		if (!obj){
			obj = DG_::DGObject::Create(hw_, w_, h_, ms_, sm_, w2D_, h2D_);
			//
			Line_Initialize();
		}
		return  obj != nullptr;
	}
	//-----------------------------------------------------------------------
	void  Finalize()
	{
		Line_Finalize();
		Image_Clear();
		Font_Clear();
		Mesh_Clear();
		Texture_Clear();
		obj.reset();
	}
	//-----------------------------------------------------------------------
	//	��ʕ`��i�J�n�j
	BOOL  Begin(ML::Color color_)
	{
		return obj->Begin(color_);
	}
	//-----------------------------------------------------------------------
	//	3D�`��̋��𓾂�
	void  Begin_3D()
	{
		obj->Begin3D();
	}
	//-----------------------------------------------------------------------
	//	3D�`��̂��I������
	void  End_3D()
	{
		obj->End3D();
	}
	//-----------------------------------------------------------------------
	//	2D�`��̋��𓾂�
	void  Begin_2D()
	{
		obj->Begin2D();
	}
	//-----------------------------------------------------------------------
	//	2D�`��̂��I������
	void  End_2D()
	{
		obj->End2D();
	}
	//-----------------------------------------------------------------------
	//	��ʕ`��i�I���j
	void  End()
	{
		obj->End();
	}
	//-----------------------------------------------------------------------
	DG_::EffectState&	EffectState(){ return  obj->EState( ); }
	ID3DX10Sprite&			Sprite(){ return  obj->Sprite( ); }

//---------------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void  Image_Create(
		const string name_,
		const string fileName_)
	{
		auto  w = DG_::Image::Create(fileName_);
		images[name_] = w;
	}
	//-----------------------------------------------------------------------
	void  Image_Draw(
		const string name_,
		ML::Box2D  draw_,
		ML::Box2D  src_,
		const ML::Color&  color_)
	{
		auto it = images.find(name_);
		if (it == images.end()) {
			if (images_NotObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("���݂��Ȃ��I�t�X�N���[���ɐ����v�����܂���"), MB_OK);
			}
			images_NotObject = true;
			return;
		}
		it->second->Draw(draw_, src_, color_);
	}
	//-----------------------------------------------------------------------
	void  Image_Rotation(
		const string name_,
		float  r_,	//	��]�ʁi���W�A���j
		const  ML::Vec2&  c_)	//	��]���idraw���΍��W�j
	{
		auto it = images.find(name_);
		if (it == images.end()) {
			if (images_NotObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("���݂��Ȃ��I�t�X�N���[���ɐ����v�����܂���"), MB_OK);
			}
			images_NotObject = true;
			return;
		}
		it->second->Rotation(r_, c_);
	}
	//-----------------------------------------------------------------------
	void  Image_Clear()
	{
		images.clear();
	}
	//-----------------------------------------------------------------------
	void  Image_Erase(const string name_)
	{
		images.erase(name_);
	}
	//-----------------------------------------------------------------------
	POINT Image_Size(const string name_)
	{
		return images[name_]->Size( );
	}

//---------------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void  Font_Create(
		const string	name_,
		const string	fontName_,
		WORD			width_,
		WORD			height_,
		WORD			weight_,
		BYTE			charSet_)
	{
		auto  w = DG_::Font::Create(fontName_, width_, height_, weight_, charSet_);
		fonts[name_] = w;
	}
	//-----------------------------------------------------------------------
	void  Font_Draw(const string name_,
		const ML::Box2D&	draw_,	//	�`���
		const string&		tex_,
		ML::Color			color_,
		UINT				uFormat_)
	{
		auto it = fonts.find(name_);
		if (it == fonts.end()) {
			if (fonts_NotObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("���݂��Ȃ��t�H���g�ɐ����v�����܂���"), MB_OK);
			}
			fonts_NotObject = true;
			return;
		}
		it->second->Draw(draw_, tex_, color_, uFormat_);
	}
	//-----------------------------------------------------------------------
	void  Font_DrawF(
		const string			name_,
		const ML::Box2D&		draw_,	//	�`���
		const string&			tex_,
		FRAME					mode_,
		ML::Color				color_,
		ML::Color				fcolor_,
		UINT					uFormat_)
	{
		auto it = fonts.find(name_);
		if (it == fonts.end()) {
			if (fonts_NotObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("���݂��Ȃ��t�H���g�ɐ����v�����܂���"), MB_OK);
			}
			fonts_NotObject = true;
			return;
		}
		it->second->DrawF(draw_, tex_, (DG_::Font::FRAME)mode_, color_, fcolor_, uFormat_);
	}
	//-----------------------------------------------------------------------
	//���ݐ������Ă���t�H���g�����ׂĔj��
	void  Font_Clear()
	{
		fonts.clear();
	}
	//-----------------------------------------------------------------------
	//�w�肵���t�H���g��j��
	void  Font_Erase(const string name_)
	{
		fonts.erase(name_);
	}
//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void  Mesh_CreateFromSOBFile(
		const string	name_,
		const  string&  fpath_)
	{
		auto  w = DG_::Mesh::Create_FromSOBFile(fpath_);
		meshs[name_] = w;
	}
	//-----------------------------------------------------------------------
	void Mesh_Draw(
		const string	name_)
	{
		auto it = meshs.find(name_);
		if (it == meshs.end()) {
			if (meshs_NotObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("���݂��Ȃ����b�V���ɐ����v�����܂���"), MB_OK);
			}
			meshs_NotObject = true;
			return;
		}
		it->second->Draw();
	}
	//-----------------------------------------------------------------------
	bool  Mesh_HitRay(
		const string	name_,
		const  ML::Vec3&  pos_,
		const  ML::Vec3&  dir_,
		float&  dist_,
		bool  qmode_)
	{
		auto it = meshs.find(name_);
		if (it == meshs.end()) {
			if (meshs_NotObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("���݂��Ȃ����b�V���ɐ����v�����܂���"), MB_OK);
			}
			meshs_NotObject = true;
			return false;
		}
		return it->second->Hit_Ray(pos_, dir_, dist_, qmode_);
	}
	//-----------------------------------------------------------------------
	bool  Mesh_HitRayBB(
		const string	name_,
		const  ML::Vec3&  pos_,
		const  ML::Vec3&  dir_,
		float&  dist_)
	{
		auto it = meshs.find(name_);
		if (it == meshs.end()) {
			if (meshs_NotObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("���݂��Ȃ����b�V���ɐ����v�����܂���"), MB_OK);
			}
			meshs_NotObject = true;
			return false;
		}
		return it->second->Hit_RayBB(pos_, dir_, dist_);
	}
	//-----------------------------------------------------------------------
	void  Mesh_Clear()
	{
		meshs.clear();
	}
	//-----------------------------------------------------------------------
	void  Mesh_Erase(const string name_)
	{
		meshs.erase(name_);
	}
//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void  Line_Draw(
		const ML::Vec3&	posS_,
		const ML::Vec3&	posE_)
	{
		//�o�[�e�b�N�X�o�b�t�@�̐���
		ML::Vec3	pos[] = { posS_, posE_ };
		auto  vPos = DG_::VertexBuffer::Create((BYTE*)pos, sizeof(ML::Vec3), _countof(pos));

		//�`��p�����[�^��萔�o�b�t�@�ɔ��f������
		obj->EState().Update_Params();

		//�e�N�j�b�N�I���i�Ăяo�����V�F�[�_�����肷��j
		obj->ETechnique().GetPassByIndex(line_tecNum)->Apply(0);
		//���_�C���v�b�g���C�A�E�g���Z�b�g
		line_inputLayout->IASet();
		//�v���~�e�B�u�E�g�|���W�[���Z�b�g
		obj->Device().IASetPrimitiveTopology(line_priTopo);

		//���_�o�b�t�@���e�X���b�g�ɐڑ��i�f�ޓ������I���j
		UINT  ofsPos = 0 * sizeof(ML::Vec3);
		obj->Device().IASetVertexBuffers(0, 1, &vPos->buf, &vPos->stride, &ofsPos);
		//�����_�����O�J�n
		obj->Device().Draw(vPos->num, 0);
	}
	void  Line_Initialize()
	{
		//�o�[�e�b�N�X�V�F�[�_���������_�C���v�b�g���C�A�E�g����
		//���_�C���v�b�g���C�A�E�g���`	
		D3D10_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		};
		//���_�C���v�b�g���C�A�E�g�̐���
		line_tecNum = 2;
		line_inputLayout = DG_::InputLayout::Create(line_tecNum, layout, _countof(layout));
		//�v���~�e�B�u�E�g�|���W�[��I��
		line_priTopo = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
	}
	void  Line_Finalize()
	{
	}
//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void  Texture_Create(
		const string	name_,
		const  string&  fpath_)
	{
		auto  w = DG_::Texture::Create(fpath_);
		textures[name_] = w;
	}
	void  Texture_PSSet(const string	name_, UINT  slotNum)
	{
		auto it = textures.find(name_);
		if (it == textures.end()) {
			if (textures_NotObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("���݂��Ȃ��e�N�X�`���ɐ����v�����܂���"), MB_OK);
			}
			textures_NotObject = true;
			return;
		}
		it->second->PSSet(slotNum);
	}
	void  Texture_Clear()
	{
		textures.clear();
	}
	void  Texture_Erase(const string name_)
	{
		textures.erase(name_);
	}
}