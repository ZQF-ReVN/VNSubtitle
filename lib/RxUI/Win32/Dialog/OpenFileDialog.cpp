#include "OpenFileDialog.h"


namespace RxUI::Win32::Dialog
{
	OpenFile::OpenFile()
	{
		SetFlags();
		SetFilter();
	}

	void OpenFile::OpenDialog()
	{
		m_vecPath.clear();

		IFileOpenDialog* file_dialog = NULL;

		auto for_item = [this, &file_dialog]()
		{
			DWORD item_count = 0;
			PWSTR path_ptr = NULL;
			IShellItem* item_one = NULL;
			IShellItemArray* item_array = NULL;

			if (SUCCEEDED(file_dialog->GetResults(&item_array)))
			{
				item_array->GetCount(&item_count);
				for (DWORD ite_item = 0; ite_item < item_count; ite_item++)
				{
					if (SUCCEEDED(item_array->GetItemAt(ite_item, &item_one)))
					{
						if (SUCCEEDED(item_one->GetDisplayName(SIGDN_FILESYSPATH, &path_ptr)))
						{
							m_vecPath.emplace_back(path_ptr);
							CoTaskMemFree(path_ptr);
						}
						item_one->Release();
					}
				}
				item_array->Release();
			}

		};

		if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
		{
			if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&file_dialog))))
			{
				file_dialog->SetOptions(m_Flag);
				file_dialog->SetFileTypes(m_dwFilterCount, m_pFilter);
				if (SUCCEEDED(file_dialog->Show(NULL)))
				{
					for_item();
				}
				file_dialog->Release();
			}
			CoUninitialize();
		}
	}

	void OpenFile::SetFlags()
	{
		this->m_Flag = FOS_FORCEFILESYSTEM | FOS_FORCESHOWHIDDEN | FOS_ALLOWMULTISELECT;
		//FILEOPENDIALOGOPTIONS flag_folder = FOS_PICKFOLDERS | FOS_FORCESHOWHIDDEN | FOS_ALLOWMULTISELECT;
	}

	void OpenFile::SetFilter()
	{
		static COMDLG_FILTERSPEC filter[] =
		{
			{ L"All", L"*.*"},
			{ L"PNG", L"*.png"},
			{ L"WEBP", L"*.webp"},
			{ L"JPG", L"*.jpg;*.jpeg"},
			{ L"BMP", L"*.bmp"},
		};

		this->m_pFilter = filter;
		this->m_dwFilterCount = ARRAYSIZE(filter);
	}

	std::wstring OpenFile::GetFirstPath()
	{
		if (m_vecPath.empty())
		{
			return L"";
		}
		else
		{
			return m_vecPath[0];
		}
	}

	std::vector<std::wstring>& OpenFile::GetPathList()
	{
		return m_vecPath;
	}


	std::wstring GetOpenFilePath(const wchar_t* wpFilter)
	{
		wchar_t buffer[MAX_PATH] = { 0 };

		OPENFILENAMEW ofn = { 0 };
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFile = buffer;
		ofn.nMaxFile = sizeof(buffer);
		ofn.lpstrFilter = wpFilter;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		return GetOpenFileNameW(&ofn) ? buffer : L"";
	}
}