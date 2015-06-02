#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include "windows.h"
#include <windows.h>
#include <stdio.h>
#include <cstring>
#include <atlstr.h>
#include <vector>

#include <curl/curl.h>

using namespace std;
using boost::property_tree::ptree; 



//------------------------------------------------------------------------------------------------------------------
// Prototype(s)...
//------------------------------------------------------------------------------------------------------------------
CHAR cpuusage(void);

//-----------------------------------------------------
typedef BOOL ( __stdcall * pfnGetSystemTimes)( LPFILETIME lpIdleTime, LPFILETIME lpKernelTime, LPFILETIME lpUserTime );
static pfnGetSystemTimes s_pfnGetSystemTimes = NULL;

static HMODULE s_hKernel = NULL;
//-----------------------------------------------------
void GetSystemTimesAddress()
{
    if( s_hKernel == NULL )
    {   
        s_hKernel = LoadLibrary( L"Kernel32.dll" );
        if( s_hKernel != NULL )
        {
            s_pfnGetSystemTimes = (pfnGetSystemTimes)GetProcAddress( s_hKernel, "GetSystemTimes" );
            if( s_pfnGetSystemTimes == NULL )
            {
                FreeLibrary( s_hKernel ); s_hKernel = NULL;
            }
        }
    }
}
//----------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------
// cpuusage(void)
// ==============
// Return a CHAR value in the range 0 - 100 representing actual CPU usage in percent.
//----------------------------------------------------------------------------------------------------------------
CHAR cpuusage()
{
    FILETIME               ft_sys_idle;
    FILETIME               ft_sys_kernel;
    FILETIME               ft_sys_user;

    ULARGE_INTEGER         ul_sys_idle;
    ULARGE_INTEGER         ul_sys_kernel;
    ULARGE_INTEGER         ul_sys_user;

    static ULARGE_INTEGER    ul_sys_idle_old;
    static ULARGE_INTEGER  ul_sys_kernel_old;
    static ULARGE_INTEGER  ul_sys_user_old;

    CHAR  usage = 0;

    // we cannot directly use GetSystemTimes on C language
    /* add this line :: pfnGetSystemTimes */
    s_pfnGetSystemTimes(&ft_sys_idle,    /* System idle time */
        &ft_sys_kernel,  /* system kernel time */
        &ft_sys_user);   /* System user time */

    CopyMemory(&ul_sys_idle  , &ft_sys_idle  , sizeof(FILETIME)); // Could been optimized away...
    CopyMemory(&ul_sys_kernel, &ft_sys_kernel, sizeof(FILETIME)); // Could been optimized away...
    CopyMemory(&ul_sys_user  , &ft_sys_user  , sizeof(FILETIME)); // Could been optimized away...

    usage  =
        (
        (
        (
        (
        (ul_sys_kernel.QuadPart - ul_sys_kernel_old.QuadPart)+
        (ul_sys_user.QuadPart   - ul_sys_user_old.QuadPart)
        )
        -
        (ul_sys_idle.QuadPart-ul_sys_idle_old.QuadPart)
        )
        *
        (100)
        )
        /
        (
        (ul_sys_kernel.QuadPart - ul_sys_kernel_old.QuadPart)+
        (ul_sys_user.QuadPart   - ul_sys_user_old.QuadPart)
        )
        );

    ul_sys_idle_old.QuadPart   = ul_sys_idle.QuadPart;
    ul_sys_user_old.QuadPart   = ul_sys_user.QuadPart;
    ul_sys_kernel_old.QuadPart = ul_sys_kernel.QuadPart;

    return usage;
}

int main() {
	MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);    
	
	ptree pt;

	string url = "http://104.236.51.159:8080/CPUMonitor/input";

	char hostname[MAX_COMPUTERNAME_LENGTH + 2];
	DWORD nameBufSize;
	nameBufSize = sizeof hostname - 1;
	GetComputerNameA(hostname, &nameBufSize);
	string hostnameString = hostname;
	
	GetSystemTimesAddress();

	time_t now;
	tm* localtm;

	CURL *curl;
	CURLcode res;

	vector<string> vec;
	stringstream ss;
	string json;

	curl = curl_easy_init();
	for (;;){
		while (curl) {		
			int memoryUsage = 100* (memInfo.ullTotalPhys - memInfo.ullAvailPhys)/ (double) memInfo.ullTotalPhys;
			CHAR c = cpuusage();
			int a = (int) c;
			
			now = time(0);
			localtm = localtime(&now);

			pt.put("cpuUsage", a);
			pt.put("memoryUsage", memoryUsage);
			pt.put("date", asctime(localtm));
			pt.put("hostname", hostnameString);

			stringstream ss;
			write_json(ss, pt);
			json = ss.str();
			json = "json=" + json;

			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
			res = curl_easy_perform(curl);
			
			if (res == 0) {
				//cout << "enviado: " << json << endl;
				for (int i=vec.size(); i>0; i--){
					curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
					curl_easy_setopt(curl, CURLOPT_POSTFIELDS, vec[i-1].c_str());
					res = curl_easy_perform(curl);

					if (res == 0) {
						//cout << "enviado vetor " << i << endl;
						vec.erase(vec.begin()+i-1);
					}
				}
			}
			else {
				vec.push_back(json);
				//cout << "erro! tamanho do vetor: "<< vec.size() << endl;
			}

			Sleep(1000);
			curl = curl_easy_init();
		}
	}
	

	return 0;
}
