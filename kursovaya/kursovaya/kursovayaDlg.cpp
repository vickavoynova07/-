// kursovayaDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "kursovaya.h"
#include "kursovayaDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <locale.h>   // для setlocale() и LC_CTYPE
#include <string.h>   // для _strdup()

// Диалоговое окно CAboutDlg используется для описания сведений о приложении
class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Диалоговое окно CkursovayaDlg
CkursovayaDlg::CkursovayaDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_KURSOVAYA_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CkursovayaDlg::~CkursovayaDlg()
{
}

void CkursovayaDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_EditInput);
    DDX_Control(pDX, IDC_LIST_RESULTS, m_ListResults);
    DDX_Control(pDX, IDC_COMBO_STANDARD, m_ComboStandard);
}

BEGIN_MESSAGE_MAP(CkursovayaDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_CHECK, &CkursovayaDlg::OnBnClickedButtonCheck)
    ON_BN_CLICKED(IDC_BUTTON_EXIT, &CkursovayaDlg::OnBnClickedButtonExit)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CkursovayaDlg::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON_LOAD, &CkursovayaDlg::OnBnClickedButtonLoad)
    ON_CBN_SELCHANGE(IDC_COMBO_STANDARD, &CkursovayaDlg::OnCbnSelchangeComboStandard)
END_MESSAGE_MAP()

// Инициализация диалога
BOOL CkursovayaDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Добавление пункта "О программе..." в системное меню
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Задает значок для этого диалогового окна
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // Заполняем выпадающий список стандартами
    m_ComboStandard.AddString(_T("NIST SP 800-63B"));
    m_ComboStandard.AddString(_T("PCI DSS v4.0"));
    m_ComboStandard.SetCurSel(0);

    return TRUE;
}

void CkursovayaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void CkursovayaDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CkursovayaDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// ==================== КНОПКА "ПРОВЕРИТЬ" ====================
void CkursovayaDlg::OnBnClickedButtonCheck()
{
    int nSelect = m_ComboStandard.GetCurSel();
    if (nSelect == CB_ERR)
    {
        AfxMessageBox(_T("Пожалуйста, выберите стандарт из списка!"), MB_ICONWARNING);
        return;
    }

    CString strStandard;
    m_ComboStandard.GetLBText(nSelect, strStandard);

    CString strPolicy;
    m_EditInput.GetWindowText(strPolicy);

    if (strPolicy.IsEmpty())
    {
        AfxMessageBox(_T("ОШИБКА: Поле политики паролей пустое!"), MB_ICONERROR);
        return;
    }

    m_ListResults.ResetContent();
    m_ListResults.AddString(_T("=== Проверка по стандарту: ") + strStandard + _T(" ==="));

    if (strStandard == _T("NIST SP 800-63B"))
    {
        CheckNISTRequirements(strPolicy);
    }
    else if (strStandard == _T("PCI DSS v4.0"))
    {
        CheckPCIDSSRequirements(strPolicy);
    }
    else
    {
        m_ListResults.AddString(_T("ОШИБКА: Неизвестный стандарт!"));
        return;
    }

    m_ListResults.AddString(_T("=== АНАЛИЗ ЗАВЕРШЕН ==="));
}

// ==================== КНОПКА "ВЫХОД" ====================
void CkursovayaDlg::OnBnClickedButtonExit()
{
    EndDialog(IDCANCEL);
}

// ==================== КНОПКА "ОЧИСТИТЬ" ====================
void CkursovayaDlg::OnBnClickedButtonClear()
{
    m_EditInput.SetWindowText(_T(""));
    m_EditInput.SetFocus();
}

// ==================== КНОПКА "ЗАГРУЗИТЬ ИЗ ФАЙЛА" ====================
void CkursovayaDlg::OnBnClickedButtonLoad()
{
    CFileDialog fileDlg(TRUE, _T("txt"), NULL,
        OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
        _T("Текстовые файлы (*.txt)|*.txt|Все файлы (*.*)|*.*||"), this);

    if (fileDlg.DoModal() == IDOK)
    {
        CString filePath = fileDlg.GetPathName();

        char* pOldLocale = _strdup(setlocale(LC_CTYPE, NULL));
        setlocale(LC_CTYPE, ".65001");

        CStdioFile file;
        CString m_strFileContent;

        if (file.Open(filePath, CFile::modeRead))
        {
            CString strLine;
            while (file.ReadString(strLine))
            {
                m_strFileContent += strLine;
                m_strFileContent += _T("\r\n");
            }
            file.Close();

            m_EditInput.SetWindowText(m_strFileContent);
            m_ListResults.AddString(_T("Загружен файл: ") + fileDlg.GetFileName());
        }
        else
        {
            AfxMessageBox(_T("Не удалось открыть файл!"), MB_ICONERROR);
        }

        setlocale(LC_CTYPE, pOldLocale);
        free(pOldLocale);
    }
}

// ==================== ВЫПАДАЮЩИЙ СПИСОК ====================
void CkursovayaDlg::OnCbnSelchangeComboStandard()
{
    // Можно оставить пустым, либо добавить действие при смене стандарта
}

// ==================== ПРЕОБРАЗОВАНИЕ В НИЖНИЙ РЕГИСТР ====================
CString CkursovayaDlg::ConvertToLower(CString strText)
{
    CString strLower = strText;
    int nLen = strLower.GetLength();
    for (int i = 0; i < nLen; i++)
    {
        strLower.SetAt(i, towlower(strLower.GetAt(i)));
    }
    return strLower;
}

// ==================== ПРОВЕРКА ПО СТАНДАРТУ NIST SP 800-63B ====================
void CkursovayaDlg::CheckNISTRequirements(CString strPolicy)
{
    m_ListResults.AddString(_T(""));
    m_ListResults.AddString(_T("--- NIST SP 800-63B ---"));

    int nLength = strPolicy.GetLength();

    if (nLength >= 8)
        m_ListResults.AddString(_T("✓ Пароль НЕ МЕНЕЕ 8 символов: ПРОЙДЕНО"));
    else
    {
        CString strLength;
        strLength.Format(_T("%d"), nLength);
        m_ListResults.AddString(_T("✗ Пароль НЕ МЕНЕЕ 8 символов: НЕ ПРОЙДЕНО (текущая длина: ") +
            strLength + _T(" символов)"));
    }

    BOOL bOnlySpaces = TRUE;
    if (nLength == 0) bOnlySpaces = TRUE;
    else
    {
        bOnlySpaces = TRUE;
        for (int i = 0; i < nLength; i++)
        {
            if (strPolicy[i] != _T(' ') && strPolicy[i] != _T('\t') && strPolicy[i] != _T('\r') && strPolicy[i] != _T('\n'))
            {
                bOnlySpaces = FALSE;
                break;
            }
        }
    }

    if (!bOnlySpaces && nLength > 0)
        m_ListResults.AddString(_T("✓ Пароль не состоит только из пробелов: ПРОЙДЕНО"));
    else
        m_ListResults.AddString(_T("✗ Пароль не состоит только из пробелов: НЕ ПРОЙДЕНО"));

    m_ListResults.AddString(_T("! NIST НЕ ТРЕБУЕТ: принудительную смену пароля, сложные правила"));
    m_ListResults.AddString(_T("! Рекомендуется проверка по словарям компрометированных паролей"));

    m_ListResults.AddString(_T("----------"));
    if (nLength >= 8 && !bOnlySpaces && nLength > 0)
        m_ListResults.AddString(_T("ИТОГ: Политика СООТВЕТСТВУЕТ NIST SP 800-63B"));
    else
        m_ListResults.AddString(_T("ИТОГ: Политика НЕ СООТВЕТСТВУЕТ NIST SP 800-63B"));
}

// ==================== ПРОВЕРКА ПО СТАНДАРТУ PCI DSS v4.0 ====================
void CkursovayaDlg::CheckPCIDSSRequirements(CString strPolicy)
{
    m_ListResults.AddString(_T(""));
    m_ListResults.AddString(_T("--- PCI DSS v4.0 ---"));

    int nLength = strPolicy.GetLength();

    if (nLength >= 12)
        m_ListResults.AddString(_T("✓ Пароль НЕ МЕНЕЕ 12 символов: ПРОЙДЕНО"));
    else
    {
        CString strLength;
        strLength.Format(_T("%d"), nLength);
        m_ListResults.AddString(_T("✗ Пароль НЕ МЕНЕЕ 12 символов: НЕ ПРОЙДЕНО (текущая длина: ") +
            strLength + _T(" символов)"));
    }

    BOOL bHasLower = FALSE;
    BOOL bHasUpper = FALSE;
    BOOL bHasDigit = FALSE;
    BOOL bHasSpecial = FALSE;

    CString specialChars = _T("!@#$%^&*()_+-=[]{};':\",./<>?\\|`~");

    for (int i = 0; i < nLength; i++)
    {
        TCHAR ch = strPolicy[i];
        if (ch >= _T('a') && ch <= _T('z')) bHasLower = TRUE;
        else if (ch >= _T('A') && ch <= _T('Z')) bHasUpper = TRUE;
        else if (ch >= _T('0') && ch <= _T('9')) bHasDigit = TRUE;
        else if (specialChars.Find(ch) != -1) bHasSpecial = TRUE;
    }

    if (bHasUpper && bHasLower)
        m_ListResults.AddString(_T("✓ Заглавные И строчные буквы: ПРОЙДЕНО"));
    else if (bHasUpper && !bHasLower)
        m_ListResults.AddString(_T("✗ Заглавные И строчные буквы: НЕ ПРОЙДЕНО (есть только заглавные)"));
    else if (!bHasUpper && bHasLower)
        m_ListResults.AddString(_T("✗ Заглавные И строчные буквы: НЕ ПРОЙДЕНО (есть только строчные)"));
    else
        m_ListResults.AddString(_T("✗ Заглавные И строчные буквы: НЕ ПРОЙДЕНО (нет букв)"));

    if (bHasDigit)
        m_ListResults.AddString(_T("✓ Цифры (0-9): ПРОЙДЕНО"));
    else
        m_ListResults.AddString(_T("✗ Цифры (0-9): НЕ ПРОЙДЕНО"));

    if (bHasSpecial)
        m_ListResults.AddString(_T("✓ Специальные символы: ПРОЙДЕНО"));
    else
        m_ListResults.AddString(_T("✗ Специальные символы: НЕ ПРОЙДЕНО (требуются символы типа !@#$%^&*)"));

    m_ListResults.AddString(_T("! PCI DSS v4.0 ТАКЖЕ ТРЕБУЕТ:"));
    m_ListResults.AddString(_T("  - Запрет общих/слабых паролей"));
    m_ListResults.AddString(_T("  - Смена паролей при компрометации"));

    m_ListResults.AddString(_T("----------"));
    if (nLength >= 12 && bHasUpper && bHasLower && bHasDigit && bHasSpecial)
        m_ListResults.AddString(_T("ИТОГ: Политика ПОЛНОСТЬЮ СООТВЕТСТВУЕТ PCI DSS v4.0"));
    else if (nLength >= 12 && (bHasUpper || bHasLower) && bHasDigit)
        m_ListResults.AddString(_T("ИТОГ: Политика ЧАСТИЧНО СООТВЕТСТВУЕТ PCI DSS v4.0"));
    else
        m_ListResults.AddString(_T("ИТОГ: Политика НЕ СООТВЕТСТВУЕТ PCI DSS v4.0"));
}