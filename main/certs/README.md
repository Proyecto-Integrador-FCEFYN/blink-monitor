# Creacion de certificados

## Comando para generar un certificado de las ESP32

Hay que configurar el archivo `san.cnf` con la IP que corresponda y la informacion deseada.

```bash
openssl req -newkey rsa:2048 -nodes -keyout prvtkey.pem -x509 -days 3650 -out cacert.pem -config san.cnf
```

## Certificado de las APIs del CENTRAL

Hay que poner el certificado del RootCA de las APIs en el archivo localhost.pem
