// kursovayaDlg.h: файл заголовка
//

#pragma once
#include "afxcmn.h"

// Диалоговое окно CkursovayaDlg
class CkursovayaDlg : public CDialogEx
{
    // Создание
public:
    CkursovayaDlg(CWnd* pParent = nullptr);    // стандартный конструктор
    virtual ~CkursovayaDlg();

    // Функции проверки для стандартов
    void CheckNISTRequirements(CString strPolicy);
    void CheckPCIDSSRequirements(CString strPolicy);
    CString ConvertToLower(CString strText);

    // Данные диалогового окна
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_KURSOVAYA_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

    // Переменные контролов 
    CEdit       m_EditInput;        // Поле ввода политики паролей
    CListBox    m_ListResults;      // Список результатов проверки
    CComboBox   m_ComboStandard;    // Выпадающий список для выбора стандарта

    HICON m_hIcon;

    // Созданные функции схемы сообщений
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    // Обработчики кнопок
    afx_msg void OnBnClickedButtonCheck();   // Проверить
    afx_msg void OnBnClickedButtonLoad();    // Загрузить из файла
    afx_msg void OnBnClickedButtonClear();   // Очистить
    afx_msg void OnBnClickedButtonExit();    // Выход
    afx_msg void OnCbnSelchangeComboStandard();

    DECLARE_MESSAGE_MAP()
};