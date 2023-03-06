# Creacion de certificados


### Comando para generar un certificado de las ESP32
Hay que configurar el archivo `san.cnf` con la IP que corresponda y la informacion deseada.
```bash
openssl req -newkey rsa:2048 -nodes -keyout prvtkey.pem -x509 -days 3650 -out cacert.pem -config san.cnf
```

### Comando para generar el certificado del NGINX

```bash
openssl req -newkey rsa:2048 -nodes -keyout nginx_prvtkey.pem -x509 -days 3650 -out nginx_cacert.pem -subj "/CN=localhost"
```

