server {
    server_name example.com;
    root /var/www/example.com;

    access_log off;
    error_log off;

    location/ {
        index index.html index.htm;
    }

    location ~ \.php$ {
        # For security reson, if a file does not exist, return 404 immediately
        # instead of passing it to fastcgi worker. This requires nginx and
        # fastcgi workers on the same server.
	try_files $uri =404;
        include /etc/nginx/fastcgi_params;
        # Don't pass files under the /images directory to be passed to fastcgi
        # worker.
	if ($uri !~ "^/images/") {
	    fastcgi_pass unix:/var/run/php-fastcgi/php-fastcgi.socket;
	}
        fastcgi_index index.php;
        fastcgi_param SCRIPT_FILENAME $document_root/$fastcgi_script_name;
    }
}
