server {
    server_name example.com;
    root /var/www/example.com;

    access_log off;
    error_log off;

    location/ {
        index index.html index.htm;
    }

    location ~ \.php$ {
        include /etc/nginx/fastcgi_params;
        fastcgi_pass unix:/var/run/php-fastcgi/php-fastcgi.socket;
        fastcgi_index   index.php;
        fastcgi_param   SCRIPT_FILENAME $document_root/$fastcgi_script_name;
    }
}
