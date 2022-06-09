#include "stdafx.h"
#include "LepLoader.h"
#include "SettingsProvider.h"
#include <atlfile.h>

LPCTSTR LepLoader::DefultFilesProcessedByLepton()
{
	return _T("*.lep");
}

LPCTSTR LepLoader::DefultLeptonToolName()
{
	return _T("lepton-avx.exe");
}

LPCTSTR LepLoader::DefultLeptonToolExtraArgs()
{
	return _T("-allowprogressive -memory=1024M -threadmemory=128M");
}

bool LepLoader::LeptonToolPresent()
{
	return (::GetFileAttributes(GetToolPath()) != INVALID_FILE_ATTRIBUTES);
}

bool LepLoader::Process(const CString& path, const CHandle& outFile)
{
	static const CString space(_T(" "));

	if (outFile == INVALID_HANDLE_VALUE) {
		return false;
	}

	// Open the input Lepton file for reading
	CAtlFile in;
	if (FAILED(in.Create(path, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING)))
		return false;

	// Set the bInheritHandle flag so pipe handles are inherited. 
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDIN. 
	CHandle inPipeRead;
	CHandle inPipeWrite;

	// Query the size of input file first
	ULONGLONG size;
	in.GetSize(size);

	if (!::CreatePipe(&inPipeRead.m_h, &inPipeWrite.m_h, &sa, DWORD(size)))
		return false;

	// Ensure the write handle to the pipe for STDIN is not inherited. 
	if (!::SetHandleInformation(inPipeWrite, HANDLE_FLAG_INHERIT, 0))
		return false;

	// Create a pipe for the child process's STDOUT. 
	CHandle outPipeRead;
	CHandle outPipeWrite;

	if (!::CreatePipe(&outPipeRead.m_h, &outPipeWrite.m_h, &sa, 0))
		return false;

	// Ensure the write handle to the pipe for STDIN is not inherited. 
	if (!::SetHandleInformation(outPipeRead, HANDLE_FLAG_INHERIT, 0))
		return false;

	CString outFileName = _T("-");
	CString sCommandLine = CSettingsProvider::This().LeptonToolExtraArgs();
	if (!sCommandLine.IsEmpty())
		sCommandLine += space;

	sCommandLine += space + outFileName;

	HANDLE console;

	AllocConsole();
	DWORD acces = GENERIC_READ;
	acces |= GENERIC_WRITE;
	DWORD Flags = CONSOLE_TEXTMODE_BUFFER;
	console = CreateConsoleScreenBuffer(
		acces,
		0,
		NULL,
		Flags,
		NULL
	);


	STARTUPINFO si = { 0 };
	si.cb = sizeof(STARTUPINFO);
	si.hStdInput = inPipeRead;
	si.hStdOutput = console; // outPipeWrite;
	si.hStdError = console; // outPipeWrite;
	si.dwFlags = /*STARTF_UNTRUSTEDSOURCE | */ STARTF_USESTDHANDLES;
	PROCESS_INFORMATION pi = { 0 };
	if (!::CreateProcess(LepLoader::GetToolPath(), sCommandLine.GetBuffer(),
		NULL, NULL, FALSE, /*CREATE_UNICODE_ENVIRONMENT | CREATE_NO_WINDOW*/0, NULL, NULL, &si, &pi)) {
		sCommandLine.ReleaseBuffer();
		return false;
	}

	sCommandLine.ReleaseBuffer();

	if (!WriteToPipe(in, inPipeWrite))
		return false;

	// Close handles to the stdin and stdout pipes no longer needed by the child process.
	// If they are not explicitly closed, there is no way to recognize that the child process has ended.
//	inPipeRead.Close();

	// Close the pipe handle so the child process stops reading.
//	inPipeWrite.Close();

	if (!ReadFromPipe(outPipeRead, outFile))
		return false;

	::WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD exitCode = 0;
	::GetExitCodeProcess(pi.hProcess, &exitCode);

	::CloseHandle(pi.hProcess);
	::CloseHandle(pi.hThread);

	return exitCode == 0;
}

CString LepLoader::GetToolPath()
{
	return CString(CSettingsProvider::This().GetEXEPath()) + CSettingsProvider::This().LeptonToolName();
}

// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data. 
bool LepLoader::WriteToPipe(const CHandle& in, const CHandle& out)
{
	CAtlFileMapping<BYTE> mmf;
	if (FAILED(mmf.MapFile(in)))
		return false;
		
	DWORD dwWritten;
	// Guess, there is no such a huge LEP file in the world!
	BOOL bRet = ::WriteFile(out, mmf, (DWORD)mmf.GetMappingSize(), &dwWritten, NULL);
	return true;
}

// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT. 
// Stop when there is no more data. 
bool LepLoader::ReadFromPipe(const CHandle& in, const CHandle& out)
{
#define BUFSIZE 4096 

	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;

	for (;;)
	{
		bSuccess = ::ReadFile(in, chBuf, BUFSIZE, &dwRead, NULL);
		if (!bSuccess || dwRead == 0) break;

		bSuccess = ::WriteFile(out, chBuf,
			dwRead, &dwWritten, NULL);
		if (!bSuccess) break;
	}

	return true;
}