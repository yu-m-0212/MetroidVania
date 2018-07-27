#include "DG2014_Ver3_51.h"

namespace DG_
{
	unordered_map<string, Texture::WP>	 Texture::rep;

	Texture::Texture( )
	:tex_srv(nullptr)
	{
	}
	//-----------------------------------------------------------------------
	bool  Texture::Initialize(const  string&  fpath_)
	{
		auto  dgi = DG_::DGObject::GetInst( );	if(dgi == 0){	return false; }

		//�e�N�X�`���[�쐬
		HRESULT rtv  =  D3DX10CreateShaderResourceViewFromFile( 
			&dgi->Device( ), fpath_.c_str( ), NULL, NULL, &tex_srv, NULL );
		if(FAILED(rtv)){
			MessageBox(nullptr, "�t�@�C�����J���܂���B", fpath_.c_str( ), MB_OK);
			return false;
		}
		//�e�N�X�`���̏��𓾂�
		ID3D10Resource* wtr;
		tex_srv->GetResource(&wtr);
		ID3D10Texture2D*  wt = (ID3D10Texture2D*)wtr;
		wt->GetDesc(&desc);
		ML::SafeRelease(&wtr);
		return true;
	}
	//-----------------------------------------------------------------------
	Texture::~Texture( )
	{
		ML::SafeRelease(&tex_srv);
	}
	//-----------------------------------------------------------------------
	//��������
	Texture::SP Texture::Create(const string&  fpath_)
	{
		auto it = rep.find( fpath_ );
		//���ɓǍ��ς݂̃f�[�^������ꍇ
		if(it != rep.end( )){
			if(Texture::SP sp = (*it).second.lock( )){
				return sp;
			}
		}
		//�V�K�Ǎ����K�v�ȏꍇ
		if( Texture::SP sp = CreateRep(fpath_) ){
			rep[fpath_] = sp;
			return sp;
		}
		return nullptr;
	}
	//�d������摜��ʓr�ǂݍ���
	Texture::SP Texture::CreateRep(const string&  fpath_)
	{
		if( Texture::SP  ob = Texture::SP(new Texture( )) ){
			if( ob->Initialize(fpath_) ){	return ob;	}
		}
		return nullptr;
	}
	//-----------------------------------------------------------------------
	//�s�N�Z���V�F�[�_�ɓo�^����
	void  Texture::PSSet(UINT  slotNum_)
	{
		auto  dgi = DG_::DGObject::GetInst( );	if(dgi == 0){	return; }
		ID3D10EffectShaderResourceVariable*  srv = nullptr;
		char texNum[5] = "tex0";
		texNum[3] += slotNum_;
		srv = dgi->Effect( ).GetVariableByName(texNum)->AsShaderResource();
		if(srv){
			srv->SetResource(tex_srv);
		}
	}
	void  Texture::Empty_PSSet(UINT  slotNum_)
	{
		auto  dgi = DG_::DGObject::GetInst( );	if(dgi == 0){	return; }
		ID3D10EffectShaderResourceVariable*  srv = nullptr;
		char texNum[5] = "tex0";
		texNum[3] += slotNum_;
		srv = dgi->Effect( ).GetVariableByName(texNum)->AsShaderResource();
		if(srv){
			srv->SetResource(nullptr);
		}
	}
	//-----------------------------------------------------------------------
	const  D3D10_TEXTURE2D_DESC&  Texture::Desc( )
	{
		return desc;
	}

}
