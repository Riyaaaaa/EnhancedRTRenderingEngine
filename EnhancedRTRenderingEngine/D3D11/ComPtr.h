
#ifndef IKD_COMPTR_H
#define IKD_COMPTR_H

// LICENSED FROM
// Author: IKD
// HomePage : http://www.h6.dion.ne.jp/~game296o/index.html

#include <windows.h>

// #define OUTPUT_INTERFACEUSAGE  // <- ここのコメントアウトを外すと参照カウントがファイル出力されます

#ifdef OUTPUT_INTERFACEUSAGE
   #if _DEBUG
   // デバッグ時にファイル出力
   #include <fstream>
   using namespace std;
   #endif
#endif

template <class T>
class ComPtr
{
private:
   T **m_ppInterface;   // インターフェイスへのダブルポインタ
   ULONG *m_pRef;        // COMポインタの参照カウンタ
   static T* m_NullPtr;    // NULLポインタ

protected:
   // インターフェイスの参照カウンタ増加
   void AddRef(T* pInterface){
        #if defined(_DEBUG) && defined(OUTPUT_INTERFACEUSAGE)
            // デバッグ版では参照カウントを出力する
            ULONG c = pInterface->AddRef();
            ofstream ofs;
            ofs.open("DEBUG.txt", ios::app);
            ofs << pInterface << "\t[" << c << "]" << endl;
            ofs.close();
        #else
            pInterface->AddRef();
        #endif  // _DEBUG        
   }

    // インターフェイスの参照カウンタ減少
    void Release(T* pInterface){
        #if defined(_DEBUG) && defined(OUTPUT_INTERFACEUSAGE)
            ULONG c = GetRefCnt();
            ofstream ofs;
            ofs.open("DEBUG.txt", ios::app);
            ofs << pInterface << "\t[" << c-1 << "]" << endl;    // 削除後の数を出力する
            ofs.close();
            pInterface->Release();
        #else
            pInterface->Release();
        #endif
    }

    // COMポインタ参照カウンタをデクリメント
    void ReleaseComRef()
    {
        if( --(*m_pRef) == 0 ){
            delete m_ppInterface;
            delete m_pRef;
        }
    }


public:
   // デフォルトコンストラクタ
   // コンストラクタはexplicit宣言します。
   // よってfunc(Com_ptr<IHoge> v)という関数に対して、
   // func(pIHoge)のような暗黙型変換代入は出来ません。
   // 通常、コンストラクタに渡されるインターフェイスは
   // 参照カウンタが増加しているのでAddRefは行いません。
    explicit ComPtr(T* pInterface = NULL, BOOL add=FALSE){
        #if defined(_DEBUG) && defined(OUTPUT_INTERFACEUSAGE)
            // デバッグ版では参照カウントの初期値を出力する
            ofstream ofs;
            ofs.open("DEBUG.txt", ios::app);
            ofs << pInterface << "\t[" << 1 << "]" << endl;
            ofs.close();
        #endif
        if(pInterface && add)
            pInterface->AddRef();
        m_ppInterface = new T*;    // ポインタ格納領域を新規確保
        m_pRef = new ULONG(1);        // COMポインタ参照カウンタを新規確保
        if(pInterface)
            *m_ppInterface = pInterface;
        else
            *m_ppInterface = m_NullPtr;
    }

    // 暗黙型キャストコピーコンストラクタ
    template<class T2> ComPtr( ComPtr<T2>& src )
    {
        // ここに来るのは関数の引数か、生成時の代入に限られます。
        // 自分自身は初期化されていません。
        // ですから、問答無用でアップキャストして所有権を取得します。

        m_pRef = src.GetMyRefPtr();                // COMポインタ参照カウンタ共有
        *m_pRef += 1;    // COMポインタ参照カウンタをインクリメント

        m_ppInterface = (T**)src.Ref();    // 共有
        *m_ppInterface = src.Get();            // 型チェック用

        // 参照カウンタ増加
        if(*m_ppInterface)
            AddRef( *m_ppInterface );
    }


    // 同型コピーコンストラクタ
    ComPtr( const ComPtr<T> &src)
    {
        m_pRef = src.GetMyRefPtr();                // COMポインタ参照カウンタ共有
        *m_pRef += 1;    // COMポインタ参照カウンタをインクリメント

        m_ppInterface = src.Ref();    // 共有

        // 参照カウンタ増加
        if(*m_ppInterface)
            AddRef( *m_ppInterface );
    }

    // コピーコンストラクタ(NULL代入用)
    ComPtr(const int nullval)
    {
        m_ppInterface = new T*;    // ポインタ格納領域を新規確保
        *m_ppInterface = m_NullPtr;
        m_pRef = new ULONG(1);
    }


    // デストラクタ
    virtual ~ComPtr()
    {
        // 有効なインターフェイスがあればリリースする
        if(*m_ppInterface)
            Release(*m_ppInterface);

        // COM参照カウンタ数が0になったら
        // m_ppInterfaceの先も必要なくなるので消去する
        ReleaseComRef();
    }

    /////////////////////////////////////////////
    // 演算子
    /////////////////////
    // =同型代入演算子（明示的コピー）
    ComPtr<T>& operator =(const ComPtr<T>& src)
    {
        // 同じCOMポインタ参照グループである場合は何もしない
        if( m_pRef == src.GetMyRefPtr() )
            return *this;

        // 自分の持つインターフェイスの参照カウンタを1つ減らす
        if(*m_ppInterface)
            Release(*m_ppInterface);

        // 自分は他人になってしまうので参照カウンタをデクリメント
        ReleaseComRef();

        // 相手のポインタをコピー
        m_ppInterface = src.m_ppInterface;
        m_pRef = src.m_pRef;

        // カウンタをインクリメントして共有
        if(*m_ppInterface)
            AddRef( *m_ppInterface );
        *m_pRef += 1;

        return *this;
    }

    // =暗黙型変換代入演算子（明示的コピー）
    template<class T2> ComPtr<T>& operator =(ComPtr<T2>& src)
    {
        // 同じCOMポインタ参照グループである場合は何もしない
        if( m_pRef == src.GetMyRefPtr() )
            return *this;

        // 自分の持つインターフェイスの参照カウンタを1つ減らす
        if(*m_ppInterface)
            Release(*m_ppInterface);

        // 自分は他人になってしまうので自身の参照カウンタをデクリメント
        ReleaseComRef();

        // 相手のポインタをコピー
        m_ppInterface = (T**)src.Ref();
        *m_ppInterface = src.Get();        // チェック用代入
        m_pRef = src.GetMyRefPtr();

        // カウンタをインクリメントして共有
        if(*m_ppInterface)
            AddRef( *m_ppInterface );
        *m_pRef += 1;

        return *this;
    }

    // =NULL代入用演算子（明示的コピー）
    ComPtr<T>& operator =(const int nullval)
    {
        // 自分の持つインターフェイスの参照カウンタを1つ減らす
        if(*m_ppInterface)
            Release(*m_ppInterface);

        // 自分は空っぽになってしまうので自身の参照カウンタをデクリメント
        ReleaseComRef();

        // ポインタを初期化
        m_ppInterface = new T*;
        *m_ppInterface = m_NullPtr;
        m_pRef = new ULONG(1);

        return *this;
    }

    // =インターフェイス代入演算子（新規インターフェイス登録）
    template<class T2> void operator =(T2* pInterface)
    {
        // 明示的にインターフェイスを新規登録する場合に用いる

        // 自分の持つインターフェイスの参照カウンタを1つ減らす
        if(*m_ppInterface)
            Release(*m_ppInterface);

        // 自分は他人になってしまうので自身の参照カウンタをデクリメント
        ReleaseComRef();

        // 自分は新しい人になるので、
        // 新しいダブルポインタを作成
        m_ppInterface = new T*;
        m_pRef = new ULONG(1);

        // 新規代入
        if(pInterface)
            *m_ppInterface = pInterface;
        else
            *m_ppInterface = m_NullPtr;
    }

    // !=比較演算子
    bool operator !=(T* ptr){
        if(ptr != *m_ppInterface)
            return true;
        return false;
    }

    // !=比較演算子
    bool operator !=(ComPtr<T> &src){
        if(*m_ppInterface != *src.m_ppInterface)
            return true;
        return false;
    }

    // !=比較演算子
    bool operator !=(const int nullval){
        if(*m_ppInterface != NULL)
            return true;
        return false;
    }

    // ==比較演算子
    bool operator ==(T* ptr){
        if(ptr == *m_ppInterface)
            return true;
        return false;
    }

    // ==比較演算子
    bool operator ==(ComPtr<T> &src){
        if(*m_ppInterface == *src.m_ppInterface)
            return true;
        return false;
    }

    // !単項演算子
    bool operator !(){
        if( *m_ppInterface == NULL )
            return true;
        return false;
    }

    // ->メンバ選択演算子
    T* operator ->(){ return *m_ppInterface; }


/////////////////
// メンバ関数
////////////////
    // COMポインタの参照カウンタポインタを取得
    ULONG *GetMyRefPtr() const { return m_pRef;}

    // インターフェイスの参照カウンタ数を取得
    ULONG GetRefCnt()
    {
        if(*m_ppInterface){
            ULONG c = (*m_ppInterface)->AddRef()-1;
            (*m_ppInterface)->Release();
            return c;
        }
        return 0;
    }

    // アップキャストコピー
    template<class T1> void UpcastCopy(ComPtr<T1> & src ){
        // 自分のインターフェイスの参照カウンタを1つ減らす
        if(*m_ppInterface)
            Release(*m_ppInterface);

        // 自分は他人になってしまうので自身の参照カウンタをデクリメント
        ReleaseComRef();

        // 相手をコピー
        m_ppInterface = src.Ref();    // 共有
        *m_ppInterface = src.Get();        // チェック用代入
        m_pRef = src.GetMyRefPtr();            // COMポインタ参照カウンタ共有

        // 参照カウンタをインクリメント
        if(*m_ppInterface)
            AddRef( *m_ppInterface );
        *m_pRef += 1;
    }

    // ポインタ取得
    T* Get(){return *m_ppInterface;}

    // ダブルポインタを取得
    T** Ref() const {return m_ppInterface;}

    // インターフェイス生成関数へ渡す専用関数
    T** ToCreator(){
        // 自分のインターフェイスの参照カウンタを1つ減らす
        if(*m_ppInterface)
            Release(*m_ppInterface);

        // 自分は他人になってしまうので自身の参照カウンタをデクリメント
        ReleaseComRef();

        // ポインタを初期化
        m_ppInterface = new T*;
        *m_ppInterface = m_NullPtr;        // 一応代入しておきます
        m_pRef = new ULONG(1);

        return m_ppInterface;
    }

    // COMスワップ
    bool Swap( ComPtr<T> &src )
    {
        // 引数のCOMポインタが保持するインターフェイスと自身のとを入れ替える

        // 双方のCOMポインタ参照カウンタ数のチェック
        ULONG SrcComRef = *src.GetMyRefPtr();
        ULONG MyComRef = *m_pRef;

        // 双方のCOMポインタ参照カウンタ数の差を算出
        bool SrcDecriment = false;    // 引数の参照カウンタを減少させる場合trueになる
        ULONG DefComRef = MyComRef - SrcComRef;
        if( SrcComRef > MyComRef )
        {
            // 引数の参照数の方が多いのでSrcDecrimentをtrueに
            DefComRef = SrcComRef - MyComRef;
            SrcDecriment = true;
        }

        // 参照カウンタの増加側と減少側を確定
        T *pReleaseObj, *pAddObj;
        if(SrcDecriment){
            pReleaseObj = src.Get();        // 引数のを減少
            pAddObj = *m_ppInterface;
        }
        else{
            pReleaseObj = *m_ppInterface;
            pAddObj = src.Get();            // 引数のを増加
        }

        // 互いの参照カウント数を交換
        ULONG i;
        if(pReleaseObj && pAddObj)        // 双方が有効なインターフェイス
        {
            for(i=0; i<DefComRef; i++){
                pReleaseObj->Release();
                pAddObj->AddRef();
            }
        }
        else if(pReleaseObj && (pAddObj==NULL))    // 減少側だけが有効
        {
            for(i=0; i<DefComRef; i++)
                pReleaseObj->Release();
        }
        else if((pReleaseObj==NULL) && pAddObj)    // 増加側だけが有効
        {
            for(i=0; i<DefComRef; i++)
                pAddObj->AddRef();
        }

        // COMポインタ内のインターフェイスポインタを交換
        T* pTmp = *src.m_ppInterface;
        *src.m_ppInterface = *m_ppInterface;
        *m_ppInterface = pTmp;

        return true;
    }
};


template <class T>
T* ComPtr<T>::m_NullPtr = NULL;

#endif