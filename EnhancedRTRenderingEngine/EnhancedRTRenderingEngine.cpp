

#include "stdafx.h"
#include "EnhancedRTRenderingEngine.h"

#include "Algorithms/SpaceOctree.h"
#include "RenderCore.D3D11/D3D11ImmediateCommands.h"
#include "Render/ForwardRenderer.h"
#include "Render/TextureHUDRenderer.h"
#include "Render/TextureEffects.h"
#include "Render/PostEffectRenderer.h"
#include "Render/SpaceOctreeVisualizer.h"
#include "Render/RayTracer.h"
#include "Utility/TextureUtils.h"
#include "Resource/ResourceLoader.h"
#include "WindowManager.h"
#include "dxgidebug.h"
#include "Common.h"

#include <DirectXMath.h>

#include "test.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst; 
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING]; 

ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ENHANCEDRTRENDERINGENGINE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    HWND hWnd;

    // アプリケーションの初期化を実行します:
    if (!(hWnd = InitInstance(hInstance, nCmdShow)))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ENHANCEDRTRENDERINGENGINE));
    MSG msg;

#ifdef HAS_UNIT_TEST
    test();
    return 0;
#else
    WindowManager::getInstance()->RegisterWindow(hWnd, Size(1980, 1000));

    auto view = std::make_shared<GIRenderView>();
    auto cmd = std::make_unique<D3D11ImmediateCommands>();

    ViewportParam param;
    param.cfg.height = 1980;
    param.cfg.width = 1000;
    param.cfg.maxDepth = 1.0f;
    param.cfg.minDepth = 0.0f;
    param.cfg.topLeftX = 0;
    param.cfg.topLeftY = 0;

    param.MSAAQuality = MSAAQualityType::RAW_QUALITY;
    param.runtimeWindowHandle = hWnd;

    view->Initialize(cmd.get(), param);

    Scene* scene = new Scene();
    scene->CreateTestScene();

    {
        ForwardRenderer renderer;

        auto size = WindowManager::getInstance()->GetWindowSize();
        auto quadSize = size.w / 6 - 50.0f;

        //SpaceOctree::LinerOctreeFactory factory(scene->GetSceneAABB(), 4);

        /*using FactoryType = SpaceOctree::HashedOctreeFactory;
        FactoryType factory(AABB(Vector3D(-16, -16, -16), Vector3D(16, 16, 16)), 8);

        for (auto && object: scene->GetViewObjects()) {
            factory.Register(&object);
        }

        SpaceOctreeVisualizer visualizer;
        visualizer.Initialize(&factory);*/

        //auto result = GetColliderMeshList(&factory, Vector3D(-1, 10, -10), Vector3D(0, -1, 0.9));

        while (GetMessage(&msg, nullptr, 0, 0))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            renderer.render(cmd.get(), view.get(), scene);
            //renderer2.Apply("PostEffects");
            /*for (int i = 0; i < 6; i++) {
                quadRenderer.render(Vector2D{ (size.w / 8 + 50) * i + 200, size.h / 8 }, Size{ size.w / 8 , size.h / 8}, renderer.GetSceneInfo().GetPointShadow(0), i);
            }*/
            //quadRenderer.render(Vector2D{ size.w / 4, size.h / 2 }, Size{ size.w / 2, size.h / 2 }, renderer.GetSceneInfo().GetDirectionalShadow(0));
            //quadRenderer.render(Vector2D{ size.w / 4, size.h / 2 }, Size{ size.w / 2, size.h }, tex);
            //quadRenderer.render(Vector2D{ size.w / 2 + size.w / 4, size.h / 2 }, Size{ size.w / 2, size.h }, dst);

            //visualizer.RenderOctreeBoxes(cmd.get(), view.get(), scene);
            view->Present();
            //Sleep(1000 / 30);
        }
    }

    delete scene;
    return (int)msg.wParam;
#endif
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_ENHANCEDRTRENDERINGENGINE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ENHANCEDRTRENDERINGENGINE);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return nullptr;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return hWnd;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウの描画
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 選択されたメニューの解析:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: HDC を使用する描画コードをここに追加してください...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        WindowManager::getInstance()->ProcessInput(uMsg, wParam, lParam);
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
