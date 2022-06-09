#pragma once

class LepLoader
{
public:
	static LPCTSTR LepLoader::DefultFilesProcessedByLepton();
	static LPCTSTR LepLoader::DefultLeptonToolName();
	static LPCTSTR LepLoader::DefultLeptonToolExtraArgs();

	static bool LeptonToolPresent();

	static bool Process(const CString& path, const CHandle& outFile);

	// Gets the path where the global INI file and the EXE is located
	static CString GetToolPath();

private:
	LepLoader();

	static bool WriteToPipe(const CHandle& in, const CHandle& out);
	static bool ReadFromPipe(const CHandle& in, const CHandle& out);
};

