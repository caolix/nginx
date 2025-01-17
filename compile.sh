#!/bin/bash

yum install -y openssl openssl-devel pcre pcre-devel zlib zlib-devel openssl openssl-devel libxml2 libxml2-dev libxslt-devel
yum install -y gd-devel perl-devel perl-ExtUtils-Embed GeoIP GeoIP-devel GeoIP-data
yum install -y pcre2 pcre2-devel gperftools rpm-build

./configure \
    --prefix=/usr/share/nginx \
	--sbin-path=/usr/sbin/nginx \
	--modules-path=/usr/lib64/nginx/modules \
	--conf-path=/etc/nginx/nginx.conf \
	--error-log-path=/var/log/nginx/error.log \
	--http-log-path=/var/log/nginx/access.log \
	--http-client-body-temp-path=/var/lib/nginx/tmp/client_body \
	--http-proxy-temp-path=/var/lib/nginx/tmp/proxy \
	--http-fastcgi-temp-path=/var/lib/nginx/tmp/fastcgi \
	--http-uwsgi-temp-path=/var/lib/nginx/tmp/uwsgi \
	--http-scgi-temp-path=/var/lib/nginx/tmp/scgi \
	--pid-path=/run/nginx.pid \
	--lock-path=/run/lock/subsys/nginx \
	--user=nginx \
	--group=nginx \
	--with-compat \
	--with-debug \
	--with-file-aio \
	--with-google_perftools_module \
	--with-http_addition_module \
	--with-http_auth_request_module \
	--with-http_dav_module \
	--with-http_degradation_module \
	--with-http_flv_module \
	--with-http_gunzip_module \
	--with-http_gzip_static_module \
	--with-http_image_filter_module=dynamic \
	--with-http_mp4_module \
	--with-http_perl_module=dynamic \
	--with-http_random_index_module \
	--with-http_realip_module \
	--with-http_secure_link_module \
	--with-http_slice_module \
	--with-http_ssl_module \
	--with-http_stub_status_module \
	--with-http_sub_module \
	--with-http_v2_module \
	--with-http_xslt_module=dynamic \
	--with-mail=dynamic \
	--with-mail_ssl_module \
	--with-pcre \
	--with-pcre-jit \
	--with-stream=dynamic \
	--with-stream_ssl_module \
	--with-stream_ssl_preread_module \
	--with-threads \
	--with-cc-opt='-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector-strong --param=ssp-buffer-size=4 -grecord-gcc-switches -specs=/usr/lib/rpm/redhat/redhat-hardened-cc1 -m64 -mtune=generic' \
    --with-ld-opt='-Wl,-z,relro -specs=/usr/lib/rpm/redhat/redhat-hardened-ld -Wl,-E'