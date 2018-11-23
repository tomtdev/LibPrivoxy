Privoxy is a non-caching web proxy with advanced filtering capabilities for enhancing privacy, modifying web page data and HTTP headers, controlling access, and removing ads and other obnoxious Internet junk. Privoxy has a flexible configuration and can be customized to suit individual needs and tastes. It has application for both stand-alone systems and multi-user networks.<br/>
<br/>
Many people want to integrate Privoxy into their own projects, I made some changes to the Privoxy codes to compiled into a DLL or static library, so that you can better integrate it into their own projects.<br/>
<br/>
Base on work from:
<br/>
Privoxy: https://www.privoxy.org/ (based on its latest version 3.0.26)
<br/>
LibPrivoxy: https://github.com/tarolabs/LibPrivoxy
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
Notice: Important changes to original Privoxy:
<br/>
<br/>
1. Use custom DNS servers
<br />
You can set DNS servers in config file to use specified DNS servers (of example, 8.8.8.8 and/or 208.67.222.222) instead of system-wide DNS settings. You can add no more than 6 DNS servers. Like:
<br/>
<br/>
<code>
dns-servers 8.8.8.8;208.67.222.222
</code>
<br/>
<br/>
2. Choose different forward ways from listener's address
<br/>
You can let Privoxy listen to multiple ports, and forward client's requests to differnt proxies. Here is an example of config file:
<br/>
<br/>
<code>
listen-address 127.0.0.1:8080
listen-address 127.0.0.1:8081
listen-address 127.0.0.1:8082
forward-socks5 :8080 / 127.0.0.1:1080 .
forward :8081 / 192.168.0.1:3128
</code>
<br/>
<br/>
If a client set its http proxy to 127.0.0.1:8080, Privoxy will forward requests to parent SOCKS 5 proxy 127.0.0.1:1080;
<br/>
If a client set its http proxy to 127.0.0.1:8081, Privoxy will forward requests to parent HTTP proxy 192.168.0.1:3128;
<br/>
If a client set its http proxy to 127.0.0.1:8082, Privoxy will forward requests to Internet directly.
<br/>
<br/>
Sample code (C#) of using the dll:
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