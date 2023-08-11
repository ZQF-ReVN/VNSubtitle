#pragma once
#include <string>
#include <vector>
#include <atlbase.h>
#include <shobjidl_core.h>


namespace RxUI::Win32::Dialog
{
	class OpenFile
	{
	private:
		DWORD m_dwFilterCount;
		COMDLG_FILTERSPEC* m_pFilter;
		FILEOPENDIALOGOPTIONS m_Flag;
		std::vector<std::wstring> m_vecPath;

	protected:
		virtual void SetFlags();
		virtual void SetFilter();

	public:
		OpenFile();

		void OpenDialog();
		std::wstring GetFirstPath();
		std::vector<std::wstring>& GetPathList();

	};

	std::wstring GetOpenFilePath(const wchar_t* wpFilter = L"ALL\0*.*\0CPP\0*.cpp");
}