function FindProxyForURL(url, host)
{
	if( isPlainHostName(host) ||
		dnsDomainIs(host, ".mu.oz.au") ||
		dnsDomainIs(host, ".mu.OZ.AU") ||
		dnsDomainIs(host, ".unimelb.edu.au"))
		return "DIRECT";
	else
		return "PROXY wwwproxy.unimelb.edu.au:8000; DIRECT";
}
