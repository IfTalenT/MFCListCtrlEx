::"%VS110COMNTOOLS%..\IDE\devenv.exe" MFCListCtrlEx.sln /Clean "Release|Win32"
"%VS110COMNTOOLS%..\IDE\devenv.exe" MFCListCtrlEx.sln /Build "Release|Win32"  /Out build.log
echo build result is %ERRORLEVEL%
pause