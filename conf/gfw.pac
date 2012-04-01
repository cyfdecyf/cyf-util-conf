var direct = 'DIRECT';
//var http_proxy = 'PROXY 10.131.201.111:2012; DIRECT';
var http_proxy = 'PROXY 10.131.250.152:2012; DIRECT';

var gfwed = {
	"akamai.net": true,
	"akamaihd.net": true,
	"amazon.com": true,
	"appspot.com": true,
	"bitly.com": true,
	"blogger.com": true,
	"blogspot.com": true,
	"cl.ly": true,
	"facebook.com": true,
	"fbcdn.net": true,
	"feedburner.com": true,
	"feedsportal.com": true,
	"gmail.com": true,
	"goo.gl": true,
	"google.com": true,
	"j.mp": true,
	"openvpn.net": true,
	"osfoora.com": true,
	"posterous.com": true,
	"t.co": true,
	"twimg.com": true,
	"twitpic.com": true,
	"twitter.com": true,
	"vimeo.com": true,
	"wordpress.com": true,
	"yfrog.com": true,
	"youtube.com": true,
	"ytimg.com": true,
	"zzzzzzz last item for easy sorting": false
};

function host2domain(host) {
	var dotpos = host.lastIndexOf(".");
	if (dotpos === -1)
		return host;
	// Find the second last dot
	dotpos = host.lastIndexOf(".", dotpos - 1);
	if (dotpos === -1)
		return host;
	return host.substring(dotpos + 1);
};

function FindProxyForURL(url, host) {
	return gfwed[host2domain(host)] ? http_proxy : direct;
};
