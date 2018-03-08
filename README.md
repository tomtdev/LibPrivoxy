Privoxy is a non-caching web proxy with advanced filtering capabilities for enhancing privacy, modifying web page data and HTTP headers, controlling access, and removing ads and other obnoxious Internet junk. Privoxy has a flexible configuration and can be customized to suit individual needs and tastes. It has application for both stand-alone systems and multi-user networks.<br/>
<br/>
Many people want to integrate Privoxy into their own projects, I made some changes to the Privoxy codes to compiled into a DLL or static library, so that you can better integrate it into their own projects.<br/>
<br/>
Base on work from:
<br/>
LibPrivoxy: https://github.com/tarolabs/LibPrivoxy
<br/>
Privoxy: https://www.privoxy.org/ (based on its latest version 3.0.26)
<br/>
DNS query: https://www.binarytides.com/dns-query-code-in-c-with-winsock/
<br/>
<br/>
Develop environment: Visual Studio 2017<br/>
<br/>
License: GNU GENERAL PUBLIC LICENSE Version 2
<br/>
<br/>
Author: Shiyu Tang <shiyutang#gmail.com>
<br/>
<br/>
You can set DNS servers in config file to use specified DNS servers (of example, 8.8.8.8 and/or 208.67.222.222) instead of system-wide DNS settings. You can add no more than 6 DNS servers. Like:
<br/>
<code>
dns-servers 8.8.8.8;208.67.222.222
</code>
<br/>
<br/>
Sample code (C#):
<br/>
<br/>
<code>
public class PrivoxyWrapper
{

    [DllImport(@"LibPrivoxy.dll")]
    public static extern int StartPrivoxy([MarshalAs(UnmanagedType.LPStr)] string configFileFullPath);

    [DllImport(@"LibPrivoxy.dll")]
    public static extern void StopPrivoxy();

	[DllImport(@"LibPrivoxy.dll")]
    public static extern int IsRunning();
}
</code>