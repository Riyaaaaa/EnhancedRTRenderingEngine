
#ifndef IKD_COMPTR_H
#define IKD_COMPTR_H

// LICENSED FROM
// Author: IKD
// HomePage : http://www.h6.dion.ne.jp/~game296o/index.html

#include <windows.h>

// #define OUTPUT_INTERFACEUSAGE  // <- �����̃R�����g�A�E�g���O���ƎQ�ƃJ�E���g���t�@�C���o�͂���܂�

#ifdef OUTPUT_INTERFACEUSAGE
   #if _DEBUG
   // �f�o�b�O���Ƀt�@�C���o��
   #include <fstream>
   using namespace std;
   #endif
#endif

template <class T>
class ComPtr
{
private:
   T **m_ppInterface;   // �C���^�[�t�F�C�X�ւ̃_�u���|�C���^
   ULONG *m_pRef;        // COM�|�C���^�̎Q�ƃJ�E���^
   static T* m_NullPtr;    // NULL�|�C���^

protected:
   // �C���^�[�t�F�C�X�̎Q�ƃJ�E���^����
   void AddRef(T* pInterface){
        #if defined(_DEBUG) && defined(OUTPUT_INTERFACEUSAGE)
            // �f�o�b�O�łł͎Q�ƃJ�E���g���o�͂���
            ULONG c = pInterface->AddRef();
            ofstream ofs;
            ofs.open("DEBUG.txt", ios::app);
            ofs << pInterface << "\t[" << c << "]" << endl;
            ofs.close();
        #else
            pInterface->AddRef();
        #endif  // _DEBUG        
   }

    // �C���^�[�t�F�C�X�̎Q�ƃJ�E���^����
    void Release(T* pInterface){
        #if defined(_DEBUG) && defined(OUTPUT_INTERFACEUSAGE)
            ULONG c = GetRefCnt();
            ofstream ofs;
            ofs.open("DEBUG.txt", ios::app);
            ofs << pInterface << "\t[" << c-1 << "]" << endl;    // �폜��̐����o�͂���
            ofs.close();
            pInterface->Release();
        #else
            pInterface->Release();
        #endif
    }

    // COM�|�C���^�Q�ƃJ�E���^���f�N�������g
    void ReleaseComRef()
    {
        if( --(*m_pRef) == 0 ){
            delete m_ppInterface;
            delete m_pRef;
        }
    }


public:
   // �f�t�H���g�R���X�g���N�^
   // �R���X�g���N�^��explicit�錾���܂��B
   // �����func(Com_ptr<IHoge> v)�Ƃ����֐��ɑ΂��āA
   // func(pIHoge)�̂悤�ȈÖٌ^�ϊ�����͏o���܂���B
   // �ʏ�A�R���X�g���N�^�ɓn�����C���^�[�t�F�C�X��
   // �Q�ƃJ�E���^���������Ă���̂�AddRef�͍s���܂���B
    explicit ComPtr(T* pInterface = NULL, BOOL add=FALSE){
        #if defined(_DEBUG) && defined(OUTPUT_INTERFACEUSAGE)
            // �f�o�b�O�łł͎Q�ƃJ�E���g�̏����l���o�͂���
            ofstream ofs;
            ofs.open("DEBUG.txt", ios::app);
            ofs << pInterface << "\t[" << 1 << "]" << endl;
            ofs.close();
        #endif
        if(pInterface && add)
            pInterface->AddRef();
        m_ppInterface = new T*;    // �|�C���^�i�[�̈��V�K�m��
        m_pRef = new ULONG(1);        // COM�|�C���^�Q�ƃJ�E���^��V�K�m��
        if(pInterface)
            *m_ppInterface = pInterface;
        else
            *m_ppInterface = m_NullPtr;
    }

    // �Öٌ^�L���X�g�R�s�[�R���X�g���N�^
    template<class T2> ComPtr( ComPtr<T2>& src )
    {
        // �����ɗ���̂͊֐��̈������A�������̑���Ɍ����܂��B
        // �������g�͏���������Ă��܂���B
        // �ł�����A�ⓚ���p�ŃA�b�v�L���X�g���ď��L�����擾���܂��B

        m_pRef = src.GetMyRefPtr();                // COM�|�C���^�Q�ƃJ�E���^���L
        *m_pRef += 1;    // COM�|�C���^�Q�ƃJ�E���^���C���N�������g

        m_ppInterface = (T**)src.Ref();    // ���L
        *m_ppInterface = src.Get();            // �^�`�F�b�N�p

        // �Q�ƃJ�E���^����
        if(*m_ppInterface)
            AddRef( *m_ppInterface );
    }


    // ���^�R�s�[�R���X�g���N�^
    ComPtr( const ComPtr<T> &src)
    {
        m_pRef = src.GetMyRefPtr();                // COM�|�C���^�Q�ƃJ�E���^���L
        *m_pRef += 1;    // COM�|�C���^�Q�ƃJ�E���^���C���N�������g

        m_ppInterface = src.Ref();    // ���L

        // �Q�ƃJ�E���^����
        if(*m_ppInterface)
            AddRef( *m_ppInterface );
    }

    // �R�s�[�R���X�g���N�^(NULL����p)
    ComPtr(const int nullval)
    {
        m_ppInterface = new T*;    // �|�C���^�i�[�̈��V�K�m��
        *m_ppInterface = m_NullPtr;
        m_pRef = new ULONG(1);
    }


    // �f�X�g���N�^
    virtual ~ComPtr()
    {
        // �L���ȃC���^�[�t�F�C�X������΃����[�X����
        if(*m_ppInterface)
            Release(*m_ppInterface);

        // COM�Q�ƃJ�E���^����0�ɂȂ�����
        // m_ppInterface�̐���K�v�Ȃ��Ȃ�̂ŏ�������
        ReleaseComRef();
    }

    /////////////////////////////////////////////
    // ���Z�q
    /////////////////////
    // =���^������Z�q�i�����I�R�s�[�j
    ComPtr<T>& operator =(const ComPtr<T>& src)
    {
        // ����COM�|�C���^�Q�ƃO���[�v�ł���ꍇ�͉������Ȃ�
        if( m_pRef == src.GetMyRefPtr() )
            return *this;

        // �����̎��C���^�[�t�F�C�X�̎Q�ƃJ�E���^��1���炷
        if(*m_ppInterface)
            Release(*m_ppInterface);

        // �����͑��l�ɂȂ��Ă��܂��̂ŎQ�ƃJ�E���^���f�N�������g
        ReleaseComRef();

        // ����̃|�C���^���R�s�[
        m_ppInterface = src.m_ppInterface;
        m_pRef = src.m_pRef;

        // �J�E���^���C���N�������g���ċ��L
        if(*m_ppInterface)
            AddRef( *m_ppInterface );
        *m_pRef += 1;

        return *this;
    }

    // =�Öٌ^�ϊ�������Z�q�i�����I�R�s�[�j
    template<class T2> ComPtr<T>& operator =(ComPtr<T2>& src)
    {
        // ����COM�|�C���^�Q�ƃO���[�v�ł���ꍇ�͉������Ȃ�
        if( m_pRef == src.GetMyRefPtr() )
            return *this;

        // �����̎��C���^�[�t�F�C�X�̎Q�ƃJ�E���^��1���炷
        if(*m_ppInterface)
            Release(*m_ppInterface);

        // �����͑��l�ɂȂ��Ă��܂��̂Ŏ��g�̎Q�ƃJ�E���^���f�N�������g
        ReleaseComRef();

        // ����̃|�C���^���R�s�[
        m_ppInterface = (T**)src.Ref();
        *m_ppInterface = src.Get();        // �`�F�b�N�p���
        m_pRef = src.GetMyRefPtr();

        // �J�E���^���C���N�������g���ċ��L
        if(*m_ppInterface)
            AddRef( *m_ppInterface );
        *m_pRef += 1;

        return *this;
    }

    // =NULL����p���Z�q�i�����I�R�s�[�j
    ComPtr<T>& operator =(const int nullval)
    {
        // �����̎��C���^�[�t�F�C�X�̎Q�ƃJ�E���^��1���炷
        if(*m_ppInterface)
            Release(*m_ppInterface);

        // �����͋���ۂɂȂ��Ă��܂��̂Ŏ��g�̎Q�ƃJ�E���^���f�N�������g
        ReleaseComRef();

        // �|�C���^��������
        m_ppInterface = new T*;
        *m_ppInterface = m_NullPtr;
        m_pRef = new ULONG(1);

        return *this;
    }

    // =�C���^�[�t�F�C�X������Z�q�i�V�K�C���^�[�t�F�C�X�o�^�j
    template<class T2> void operator =(T2* pInterface)
    {
        // �����I�ɃC���^�[�t�F�C�X��V�K�o�^����ꍇ�ɗp����

        // �����̎��C���^�[�t�F�C�X�̎Q�ƃJ�E���^��1���炷
        if(*m_ppInterface)
            Release(*m_ppInterface);

        // �����͑��l�ɂȂ��Ă��܂��̂Ŏ��g�̎Q�ƃJ�E���^���f�N�������g
        ReleaseComRef();

        // �����͐V�����l�ɂȂ�̂ŁA
        // �V�����_�u���|�C���^���쐬
        m_ppInterface = new T*;
        m_pRef = new ULONG(1);

        // �V�K���
        if(pInterface)
            *m_ppInterface = pInterface;
        else
            *m_ppInterface = m_NullPtr;
    }

    // !=��r���Z�q
    bool operator !=(T* ptr){
        if(ptr != *m_ppInterface)
            return true;
        return false;
    }

    // !=��r���Z�q
    bool operator !=(ComPtr<T> &src){
        if(*m_ppInterface != *src.m_ppInterface)
            return true;
        return false;
    }

    // !=��r���Z�q
    bool operator !=(const int nullval){
        if(*m_ppInterface != NULL)
            return true;
        return false;
    }

    // ==��r���Z�q
    bool operator ==(T* ptr){
        if(ptr == *m_ppInterface)
            return true;
        return false;
    }

    // ==��r���Z�q
    bool operator ==(ComPtr<T> &src){
        if(*m_ppInterface == *src.m_ppInterface)
            return true;
        return false;
    }

    // !�P�����Z�q
    bool operator !(){
        if( *m_ppInterface == NULL )
            return true;
        return false;
    }

    // ->�����o�I�����Z�q
    T* operator ->(){ return *m_ppInterface; }


/////////////////
// �����o�֐�
////////////////
    // COM�|�C���^�̎Q�ƃJ�E���^�|�C���^���擾
    ULONG *GetMyRefPtr() const { return m_pRef;}

    // �C���^�[�t�F�C�X�̎Q�ƃJ�E���^�����擾
    ULONG GetRefCnt()
    {
        if(*m_ppInterface){
            ULONG c = (*m_ppInterface)->AddRef()-1;
            (*m_ppInterface)->Release();
            return c;
        }
        return 0;
    }

    // �A�b�v�L���X�g�R�s�[
    template<class T1> void UpcastCopy(ComPtr<T1> & src ){
        // �����̃C���^�[�t�F�C�X�̎Q�ƃJ�E���^��1���炷
        if(*m_ppInterface)
            Release(*m_ppInterface);

        // �����͑��l�ɂȂ��Ă��܂��̂Ŏ��g�̎Q�ƃJ�E���^���f�N�������g
        ReleaseComRef();

        // ������R�s�[
        m_ppInterface = src.Ref();    // ���L
        *m_ppInterface = src.Get();        // �`�F�b�N�p���
        m_pRef = src.GetMyRefPtr();            // COM�|�C���^�Q�ƃJ�E���^���L

        // �Q�ƃJ�E���^���C���N�������g
        if(*m_ppInterface)
            AddRef( *m_ppInterface );
        *m_pRef += 1;
    }

    // �|�C���^�擾
    T* Get(){return *m_ppInterface;}

    // �_�u���|�C���^���擾
    T** Ref() const {return m_ppInterface;}

    // �C���^�[�t�F�C�X�����֐��֓n����p�֐�
    T** ToCreator(){
        // �����̃C���^�[�t�F�C�X�̎Q�ƃJ�E���^��1���炷
        if(*m_ppInterface)
            Release(*m_ppInterface);

        // �����͑��l�ɂȂ��Ă��܂��̂Ŏ��g�̎Q�ƃJ�E���^���f�N�������g
        ReleaseComRef();

        // �|�C���^��������
        m_ppInterface = new T*;
        *m_ppInterface = m_NullPtr;        // �ꉞ������Ă����܂�
        m_pRef = new ULONG(1);

        return m_ppInterface;
    }

    // COM�X���b�v
    bool Swap( ComPtr<T> &src )
    {
        // ������COM�|�C���^���ێ�����C���^�[�t�F�C�X�Ǝ��g�̂Ƃ����ւ���

        // �o����COM�|�C���^�Q�ƃJ�E���^���̃`�F�b�N
        ULONG SrcComRef = *src.GetMyRefPtr();
        ULONG MyComRef = *m_pRef;

        // �o����COM�|�C���^�Q�ƃJ�E���^���̍����Z�o
        bool SrcDecriment = false;    // �����̎Q�ƃJ�E���^������������ꍇtrue�ɂȂ�
        ULONG DefComRef = MyComRef - SrcComRef;
        if( SrcComRef > MyComRef )
        {
            // �����̎Q�Ɛ��̕��������̂�SrcDecriment��true��
            DefComRef = SrcComRef - MyComRef;
            SrcDecriment = true;
        }

        // �Q�ƃJ�E���^�̑������ƌ��������m��
        T *pReleaseObj, *pAddObj;
        if(SrcDecriment){
            pReleaseObj = src.Get();        // �����̂�����
            pAddObj = *m_ppInterface;
        }
        else{
            pReleaseObj = *m_ppInterface;
            pAddObj = src.Get();            // �����̂𑝉�
        }

        // �݂��̎Q�ƃJ�E���g��������
        ULONG i;
        if(pReleaseObj && pAddObj)        // �o�����L���ȃC���^�[�t�F�C�X
        {
            for(i=0; i<DefComRef; i++){
                pReleaseObj->Release();
                pAddObj->AddRef();
            }
        }
        else if(pReleaseObj && (pAddObj==NULL))    // �������������L��
        {
            for(i=0; i<DefComRef; i++)
                pReleaseObj->Release();
        }
        else if((pReleaseObj==NULL) && pAddObj)    // �������������L��
        {
            for(i=0; i<DefComRef; i++)
                pAddObj->AddRef();
        }

        // COM�|�C���^���̃C���^�[�t�F�C�X�|�C���^������
        T* pTmp = *src.m_ppInterface;
        *src.m_ppInterface = *m_ppInterface;
        *m_ppInterface = pTmp;

        return true;
    }
};


template <class T>
T* ComPtr<T>::m_NullPtr = NULL;

#endif