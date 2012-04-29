var direct = 'DIRECT';
//var http_proxy = 'SOCKS5 127.0.0.1:1080; SOCKS 127.0.0.1:1080; DIRECT';
var http_proxy = 'PROXY 127.0.0.1:8123; DIRECT';

var gfwed_list = [
	"akamai.net",
	"akamaihd.net",
	"amazon.com",
	"appspot.com",
	"archive.org",
	"bitly.com",
	"blogger.com",
	"blogspot.com",
	"cl.ly",
	"facebook.com",
	"fbcdn.net",
	"feedburner.com",
	"feedsportal.com",
	"gmail.com",
	"goo.gl",
	"google.com",
	"j.mp",
	"mediafire.com",
	"openvpn.net",
	"osfoora.com",
	"posterous.com",
	"rapidshare.com",
	"t.co",
	"twimg.com",
	"twitpic.com",
	"twitter.com",
	"vimeo.com",
	"wordpress.com",
	"yfrog.com",
	"youtube.com",
	"ytimg.com"
];

var gfwed = {};
for (var i = 0; i < gfwed_list.length; i += 1) {
	gfwed[gfwed_list[i]] = true;
}

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
